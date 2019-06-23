/*!
 * @file iic_motor.c
 * @date 05.01.2015
 * @author Niklas May-Johann
 *
 * @brief	This lib abstract the access to the motor controller
 */

#include "motor_lib.h"

#include "bram_lib.h"

/*!
 * instanz of an iic
 */
XIic iic_Motor;

static const uint8_t glb_moto_adr[] = {(0x52>>1), (0x54>>1), (0x56>>1), (0x58>>1)}; /*!< The 7-bit Addresses of the 4 Motor drivers. The Value from the motor_id_e matched with the address in the array */

static motor_info_s motor_info[4]; /*!< the status for each motor driver */
static uint32_t motor_setpoints[4] = {0, 0, 0, 0};

/*!
 * Initialize the IIC bus with 400 kHz. Then set the rotation direction of the four motors
 *
 * @param		u16	IicDeviceId		The ID of the IIC device for the motor control (from xparamter.h)
 *
 * @return		err_motor_e			MOTOR_NO_ERROR 				if okay
 * @return		eer_motor_e			MOTOR_ERR_INITIALISATION 	if no IIC AXI_IP Core is found
 *
 * @info		As ID take the defines from the xparameter.h like XPAR_<AXI_IP_CORE>_<Nr>_DEVICE_ID
 *
 */
err_motor_e motor_init(void)
{
	uint16_t IicDeviceId = XPAR_SENSOR_ACTOR_AXI_IIC_MOTORS_DEVICE_ID;
	motor_info_s motor_info = {0, 0, 0};
	err_motor_e result = MOTOR_NO_ERROR;
	uint32_t i = 0;
	// init the iic ip core

	if(XIic_Initialize(&iic_Motor, IicDeviceId) == XST_FAILURE)
	{
		result = MOTOR_ERR_INITIALISATION;
	}

	// doing a selftest for the core
	if(XIic_SelfTest(&iic_Motor) == XST_FAILURE)
	{
		result = MOTOR_ERR_INITIALISATION;
	}
	
	/* alles null */
	motor_set_value(MOTOR_REAR,  0, &motor_info);
	for(i = 0; i< 1000; i++) // wait
	{}
	motor_set_value(MOTOR_FRONT, 0, &motor_info);
	for(i = 0; i< 1000; i++) // wait
	{}
	motor_set_value(MOTOR_LEFT,  0, &motor_info);
	for(i = 0; i< 1000; i++) // wait
	{}
	motor_set_value(MOTOR_RIGHT, 0, &motor_info);
	
	/* set rotation */
	for(i = 0; i< 10000; i++) // wait
	{}

	motor_set_rotation_direction(MOTOR_REAR, MOTOR_DIR_RIGHT);
	for(i = 0; i< 1000; i++) // wait
	{}
	motor_set_rotation_direction(MOTOR_FRONT, MOTOR_DIR_RIGHT);
	for(i = 0; i< 1000; i++) // wait
	{}
	motor_set_rotation_direction(MOTOR_LEFT, MOTOR_DIR_LEFT);
	for(i = 0; i< 1000; i++) // wait
	{}
	motor_set_rotation_direction(MOTOR_RIGHT, MOTOR_DIR_LEFT);

	for(i = 0; i< 10000; i++) // wait
	{}
	// if the init or the selftest fails, an error will be returned otherwise a NO_ERROR
	return result;
}

/*!
 * Send a 16bit control value to one motor.
 *
 * @param		motor_id_e 			motorNr	The ID of one motor
 * @param		u16					speed	the speed of the motor (SPEED_MIN - SPEED_MAX)
 * @param		motor_info_s*	 	info	the feedback information of the motor
 *
 * @return		err_motor_e			MOTOR_NO_ERROR 				if okay
 * @return		err_motor_e			MOTOR_ERR_NO_ANSWER		 	if no ACK received from the motor driver
 * @return		err_motor_e			MOTOR_UNKNOWN				if an not known motor id was send
 *
 * @info		If an speed value over or under the range is set, the value will be set to the nearest min/max value
 *
 */
err_motor_e motor_set_value(motor_id_e motor_nr, uint32_t setpoint, motor_info_s *info)
{
	err_motor_e result = MOTOR_NO_ERROR;
	uint16_t iic_bus_status = 0;
	uint8_t sendBuffer[2] = {0,0};
	uint8_t recvBuffer[3] = {0,0,0};
	uint8_t byteCount = 0;

	uint16_t speed = (uint16_t)setpoint;

	// check if the speed parameter is in range
	if(speed >= MOTOR_SPEED_MIN)
	{
		if(speed <= MOTOR_SPEED_MAX)
		{
			// do nothing, in range!
		}
		else
		{
			speed = MOTOR_SPEED_MAX;
		}
	}
	else
	{
		speed = MOTOR_SPEED_MIN;
	}

	// check the motorNr
	if(motor_nr > sizeof(glb_moto_adr)-1)
	{
		//wrong motorNr
		result = MOTOR_UNKNOWN;
	}
	else
	{
		/*
		 * first databyte contain the 8 MSBs of the speed parameter and the
		 * second databyte the last 3 bits of the speed parameter and five configuration bits.
		 * In normal mode these are 0.
		 */
		sendBuffer[0] = (uint8_t)(speed>>3);
		sendBuffer[1] = (uint8_t)(speed & 0x0007);

		// read bus stats
		iic_bus_status = XIic_ReadReg(iic_Motor.BaseAddress, XIIC_SR_REG_OFFSET);

		// if BUS is Busy dont send data
		if(!(iic_bus_status & XIIC_SR_BUS_BUSY_MASK))
		{
			// Send the data
			XIic_Send(iic_Motor.BaseAddress, glb_moto_adr[motor_nr], sendBuffer, sizeof(sendBuffer), XIIC_STOP);//glb_moto_adr[motorNr] sizeof(sendBuffer), XIIC_STOP);

//			int i;
//			for(i = 0; i< 1000; i++) // wait
//			{}

			// read data from the motor driver
			byteCount = XIic_Recv(iic_Motor.BaseAddress, glb_moto_adr[motor_nr], recvBuffer, sizeof(recvBuffer), XIIC_STOP);
			if(byteCount == sizeof(recvBuffer))
			{
				// all data received
				// first  byte (unsigned): the actual current in 0.1A Steps
				// second byte (unsigned): the MaxPWM if these value lower than 255 the BL driver is in current limit mode
				// third  byte (signed)  : the temperature of the BL driver

				motor_info[motor_nr].current 	= recvBuffer[0];
				motor_info[motor_nr].maxPWM 		= recvBuffer[1];
				motor_info[motor_nr].temperature = recvBuffer[2];

				*info = motor_info[motor_nr];
			}
			else
			{
				// error
				result = MOTOR_ERR_NO_ANSWER;
			}
		}
		else
		{
			// IIC Bus is busy
			result = MOTOR_IIC_BUS_BUSY;
		}
	}

	return result;
}

/*!
 *
 * Get the current setpoints processed by controller processing chain
 * for the motors from BRAM
 *
 */
void motor_values_from_bram(void)
{
	int i = MOTOR_REAR_VALUE_ADDR;
	bram_pl_data_read_uint(MOTOR_REAR_VALUE_ADDR,  &motor_setpoints[MOTOR_REAR]);
	bram_pl_data_read_uint(MOTOR_FRONT_VALUE_ADDR, &motor_setpoints[MOTOR_FRONT]);
	bram_pl_data_read_uint(MOTOR_LEFT_VALUE_ADDR,  &motor_setpoints[MOTOR_LEFT]);
	bram_pl_data_read_uint(MOTOR_RIGHT_VALUE_ADDR, &motor_setpoints[MOTOR_RIGHT]);
}

/*!
 *
 * Transmit current setpoints to the motor drivers
 *
 */
void motor_send_values(uint8_t enable_motors)
{
	if (enable_motors)
	{
		// send motor values  --- motor_setpoints[MOTOR_REAR]
		motor_set_value(MOTOR_REAR, motor_setpoints[MOTOR_REAR]  ,  &motor_info[MOTOR_REAR]);
		int i;
		for(i = 0; i< 100; i++) // wait
		{}
		// ---motor_setpoints[MOTOR_FRONT]
		motor_set_value(MOTOR_FRONT, motor_setpoints[MOTOR_FRONT] , &motor_info[MOTOR_FRONT]);

		for(i = 0; i< 100; i++) // wait
		{}
		//motor_setpoints[MOTOR_LEFT]
		motor_set_value(MOTOR_LEFT,motor_setpoints[MOTOR_LEFT],  &motor_info[MOTOR_LEFT]);

		for(i = 0; i< 100; i++) // wait
		{}

		//motor_setpoints[MOTOR_RIGHT]
		motor_set_value(MOTOR_RIGHT, motor_setpoints[MOTOR_RIGHT] , &motor_info[MOTOR_RIGHT]);

	}
	else
	{
		int i;
		// send 0 as all motor values
		motor_set_value(MOTOR_REAR,  0,  &motor_info[MOTOR_REAR]);
		for(i = 0; i< 100; i++) // wait
		{}

		motor_set_value(MOTOR_FRONT, 0, &motor_info[MOTOR_FRONT]);
		for(i = 0; i< 100; i++) // wait
		{}

		motor_set_value(MOTOR_LEFT,  0,  &motor_info[MOTOR_LEFT]);
		for(i = 0; i< 100; i++) // wait
		{}

		motor_set_value(MOTOR_RIGHT, 0, &motor_info[MOTOR_RIGHT]);

	}
}

/*!
 * Get the current status of the motor
 *
 * @param		motor_id_e 	motorNr	The ID of one motor
 *
 * @return		motor_info_s		The status of the motor
 * 									current			0.1A steps
 * 									maxPWM
 * 									temperature		grad C
 *
 * @info		If an unknown motor selected, all return data will be 0xFF
 */
motor_info_s motor_get_status(motor_id_e motor_nr)
{
	motor_info_s error = {0xFF, 0xFF, 0xFF};

	if( motor_nr < sizeof(glb_moto_adr)-1 )
	{
		return motor_info[motor_nr];
	}
	else
	{
		return error;
	}
}

/*!
 * Set the Motor rotating direction
 *
 * @param		motor_id_e 	motorNr	The ID of one motor
 * @param		motor_direction_e dir	The rotation direction of the motor
 *
 * @return		void
 *
 */
void motor_set_rotation_direction(motor_id_e motor_nr, motor_direction_e dir)
{
	uint8_t sendBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
	uint8_t i = 0;
	uint16_t iic_bus_status = 0;

	if( motor_nr > sizeof(glb_moto_adr)-1 )
	{
		// wrong motorid.... nothing to do
	}
	else
	{
		// an known motor id set.... setting the rotate direction

		sendBuffer[0] = 0x00; // speed ... zero
		sendBuffer[1] = 0x00 | BL_READMODE_CONFIG; // config mode
		sendBuffer[2] = BLCONFIG_REVISION; // id
		sendBuffer[3] = MASK_SET_BITCONFIG | MASK_SET_SAVE_EEPROM ; // which config byte will be set... just take direction byte 8
		sendBuffer[4] = 0x00; // max. speed value.... unused
		sendBuffer[5] = 0x00; // max. current.... unused
		sendBuffer[6] = 0x00; // max. temperature ... unused
		sendBuffer[7] = 0x00; // scale current measure ... unused
		if(dir == MOTOR_DIR_RIGHT)
		{
			sendBuffer[8] = BITCONF_REVERSE_ROTATION;
		}
		else
		{
			// right
			sendBuffer[8] = 0x00;
		}
		sendBuffer[9] = 0xAA; // crc
		for(i = 2; i<9; i++)
		{
			// calc checksum
			sendBuffer[9] += sendBuffer[i];
		}

		// read bus stats
		iic_bus_status = XIic_ReadReg(iic_Motor.BaseAddress, XIIC_SR_REG_OFFSET);
		// if BUS is Busy dont send data
		if(!(iic_bus_status & XIIC_SR_BUS_BUSY_MASK))
		{
			// Send the data
			XIic_Send(iic_Motor.BaseAddress, glb_moto_adr[motor_nr], sendBuffer, sizeof(sendBuffer), XIIC_STOP);
		}

	}
}
