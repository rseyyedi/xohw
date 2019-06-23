/*
 * gam_lib.c
 *
 *  Created on: Jul 22, 2015
 *      Author: sschreiner
 */

#include "gam_lib.h"

#include "uart_lib.h"
#include <string.h>

/*
 * some defines for easier usage to set the const initial frame values
 */
#define GAM_SENSOR_ID	0x8D	/*!< the id of the sensor */
#define GAM_START_BYTE	0x7C	/*!< the start byte of the gam frame */
#define GAM_SENSOR_ID_2	0xD0	/*!< the second byte of the id */
#define GAM_STOP_BYTE	0x7D	/*!< the last byte of the game frame */

/*
 * The ID of the GAM sensor sends from the RC
 */
#define GAM_REQUEST_BYTE 	GAM_SENSOR_ID	/*!< the id of the sensor, call periodical from the receiver */

static gam_data_s gam_data; /*!< current data structure */
static gam_data_s gam_data_to_send;	/*!< If a frame send, it will be copy to gam_send... so the send data from one frame are values from the same point of time */
static gam_help_s gam_data_help; /*!< critical values like minVoltage or maxTemperature */

// uartStatus is the internal state, glb_TimerNr is the Number of the timer from the AXI_IP Timer core
static gam_status_e gam_status = WAITING_FOR_DATA; /*!< the internal state of the statemaschine */

/*!
 * Initialize the GAM. All values sets to zero and the UART communication starts.
 *
 * @return		gam_error_t			GAM_NO_ERROR 			if okay
 *
 */
gam_error_e gam_init(void)
{
	gam_error_e result = GAM_NO_ERROR;

	if (uart_interface_init() != UART_NO_ERROR)
	{
		result = GAM_INIT_ERROR;
	}

	// fill inital gam values
	gam_data.gam_sensor_id = GAM_SENSOR_ID;
	gam_data.sensor_id = GAM_SENSOR_ID_2;
	gam_data.start_byte = GAM_START_BYTE;
	gam_data.stop_byte = GAM_STOP_BYTE;
	gam_data.warning_beeps = NO_ALARM;

	/* cap of the flight accu */
	gam_data.batt_cap_L = (660 & 0xFF);
	gam_data.batt_cap_H = (660 >> 8) & 0xFF;

	gam_data_help.maxTemp1 = 80;
	gam_data_help.maxTemp2 = 80;
	gam_data_help.minVoltage1 = 100;
	gam_data_help.minVoltage2 = 100;
	gam_data_help.voltage1 = 110;
	gam_data_help.voltage2 = 110;


	gam_status = WAITING_FOR_DATA;

	return result;
}

/*!
 * Update the current voltage for one battery in the GAM frame.
 * This frame will be send to the RC
 *
 * @param[in]				uint16_t voltage			The Battery voltage in mV
 * @param[in]				accu_e battery		The chosen battery
 * 												LOGIC_BATT
 * 												ENERGY_BATT
 *
 * @return				gam_error_t			GAM_NO_ERROR if okay
 * @return				gam_error_t			GAM_ERR_WRONG_BATTERY if an unknown battery chosen
 */
gam_error_e gam_update_voltage(uint16_t voltage, gam_battery_e battery)
{
	uint8_t voltageLow = 0;
	uint8_t voltageHigh = 0;
	gam_error_e status = GAM_NO_ERROR;

	// split voltage in two u8 values
	voltageLow 	= (uint8_t)(voltage & 0xFF);
	voltageHigh = (uint8_t)(((voltage)>>8) & 0xFF);

	// set voltage to GAM frame
	// battery 1 is LOGIC and battery 2 is ENERGY
	if(battery == LOGIC_BATT)
	{
		gam_data.batt1_L = voltageLow;
		gam_data.batt1_H = voltageHigh;
		gam_data_help.voltage1 = voltage;
	}
	else if(battery == MOTOR_BATT)
	{
		gam_data.batt2_L = voltageLow;
		gam_data.batt2_H = voltageHigh;
		gam_data.main_voltage_L = voltageLow;
		gam_data.main_voltage_H = voltageHigh;
		gam_data_help.voltage2 = voltage;
	}
	else
	{
		status = GAM_WRONG_BATTERY_ERROR;
	}

	return status;
}

/*!
 * Update the altitude of the Device. The GAM sends the altitude to the RC.
 *
 * @param[in]				uint16_t altitude		The altitude in m
 *
 * @return				void
 */
void gam_update_altitude(uint16_t altitude)
{
	// offset in the gam frame is 500m
	altitude += 500;
	// set the altitude to the gam frame
	gam_data.altitude_L = (uint8_t)(altitude & 0xFF);
	gam_data.altitude_H = (uint8_t)((altitude>>8) & 0xFF);
}

/*!
 * Update the temperature in the GAM frame. The GAM can send two temperatures to the RC.
 *
 * @param[in]				uint8_t temperature			The temperature in gradC
 * @param[in]				temperature_e device	The chosen device
 *
 * @return				gam_error_t				GAM_NO_ERROR if okay
 * @return				gam_error_t				GAM_ERR_WRONG_TEMPSENSOR if an unknown temperature sensor chosen
 */
gam_error_e gam_update_temperature(uint8_t temperature, gam_temperature_e device)
{
	gam_error_e status = GAM_NO_ERROR;
	// tempoffset is 20, a value of 20 = 0Ã‚Â°C
	temperature += 20;

	// set temperature to GAM frame
	// temperature 1 is ZYNQ_TEMP and temperature 2 is TEMPSENSOR_2
	if(device == ZYNQ_TEMP)
	{
		gam_data.temperature1 = temperature;
	}
	else if(device == AMBIENT_TEMP)
	{
		gam_data.temperature2 = temperature;
	}
	else
	{
		status = GAM_WRONG_TEMPSENSOR_ERROR;
	}

	return status;
}

/*!
 * Set the warning beep, that the QC is calibrating. The RC call this warning periodic and the pilot know, that the QC
 * is calibrating
 *
 * @param[in]				u8 on					TRUE if calibrating, FALSE if calibrating is done
 *
 * @return				void
 */
void gam_calibrating_beep(uint8_t state)
{
	if(state == 1)
	{
		gam_data.warning_beeps = CALIBRATION;
	}
	else
	{
		gam_data.warning_beeps = NO_ALARM;
	}
}

/*!
 * Send an variable warning beep
 *
 * @param[in]				gam_beeps_t		warningbeep		The chosen warningbeep
 *
 * @return				void
 */
void gam_warning_beep(gam_beeps_e warningbeep)
{
	gam_data.warning_beeps = warningbeep;
}

/*!
 * Send the climbrate.
 * It calculate the climbrate over m/s and m/3s. Therefore the function needs to call periodical and
 * the updateRate has to known
 *
 * @param[in]				int16_t		climbrate	The climbrate in 0.01 m/s per bit
 *
 * @return				void
 */
void gam_update_climbrate(int16_t climbrate)
{
	static uint16_t call_counter = 0;
	static int32_t climbrate_sum = 0;

	climbrate_sum += climbrate;

	// add climbrate offset of the RC
	climbrate = climbrate + 30000;

	gam_data.climbrate_L = climbrate & 0xFF;
	gam_data.climbrate_H = (climbrate >> 8) & 0xFF;

	// called once inc counter
	call_counter++;
	// check if 3s passed
	if( call_counter == 1500 )
	{
		// reset counter ... it was run 3s
		call_counter = 0;
		// calc mean value
		climbrate_sum = (climbrate_sum / (1500 * 3 * 100) ) + 120 ;// 120 offset ... zero m/3s climbrate is 120
		gam_data.climbrate3s = (int8_t)climbrate_sum;
		// reset 3s climbrate
		climbrate_sum = 0;
	}
}

/*!
 * Send the minimum cell voltage
 *
 * @param[in]				uint8_t		cellVoltage		voltage in 20mV per bit... 124 -> 2,48 V
 *
 * @return				void
 */
void gam_min_cell_voltage(uint8_t cell_voltage)
{
	gam_data.min_cell_volt = cell_voltage;
}

/*!
 *  function which has to call periodical, when data has to send
 *
 *  @param void
 *  @return void
 */
void gam_frame_handler(void)
{
	static uint8_t i = 0;
	static uint8_t crc = 0;

	// all data send?
	if(i == (sizeof(gam_data_to_send)) )
	{
		// now send the last byte (crc)
		uart_send(crc);
		i = 0;
		crc = 0;
		gam_status = ALL_DATA_SEND;
	}
	else
	{
		// send one byte
		uart_send(((uint8_t*)&gam_data_to_send)[i]);
		// calc crc
		crc += ((uint8_t*)&gam_data_to_send)[i];
		// set a new reset value
		gam_status = IS_SENDING;
		i++;
	}
}

/*!
 * Send the frame to the GAM. This function needs to call periodically. The RC call the GAM each ~200ms.
 *
 * gam_sendFrame checks each call, if an request from the RC exist.
 * If it exist, the GAM frame will be send.
 *
 * @param				void
 *
 * @return				gam_error_t				GAM_IS_SENDING				if the UART currently sending data
 * @return				gam_error_t				GAM_NO_REQUEST				if there was no request
 */
gam_error_e gam_send_frame(void)
{
	gam_error_e status = GAM_NO_REQUEST;
	uint8_t bytes_recv = 0;
	uint8_t i = 0;
	uint8_t recv_buffer[1]; // UartLite FiFo max  = 16;

	// init recvBuffer
	for(i = 0; i<sizeof(recv_buffer)/sizeof(uint8_t); i++)
	{
		recv_buffer[i] = 0;
	}


	switch (gam_status) {
		case WAITING_FOR_DATA:
			// check if data received
			if(!uart_is_receive_empty())
			{
				bytes_recv = uart_recv(recv_buffer, sizeof(recv_buffer)/sizeof(uint8_t));
				for(i = 0; i< bytes_recv; i++)
				{
					// check if incoming data are a request for the GAM
					if(recv_buffer[i] == GAM_REQUEST_BYTE)// && recvBuffer[i] != GAM_REQUEST_BYTE_2)
					{

						// copy the data to the send structure
						memcpy((void*)&gam_data_to_send,(void*)&gam_data,sizeof(gam_data_s));
						uart_reset_fifos();
						// gam request received -> set new state
						gam_status = DATA_REQUESET_RECEIVED;
						status = GAM_IS_SENDING;
						break;
					}
					else
					{
						// no gam request -> hold state
						gam_status = WAITING_FOR_DATA;
						status = GAM_NO_REQUEST;
					}
				}
			}
			else
			{
				// do nothing
				status = GAM_NO_REQUEST;
			}
			break;
		case DATA_REQUESET_RECEIVED:
			// next state has to be send data
			// start sending gam data next iteration to give the receiver chance to free the bus
			// no gam request -> hold state
			gam_status = IS_SENDING;
			status = GAM_IS_SENDING;
			break;
		case IS_SENDING:
			// gam is sending
			gam_frame_handler();
			status = GAM_IS_SENDING;
			break;
		case ALL_DATA_SEND:
			// all data send, rest fifos
			uart_reset_fifos();
			gam_status = WAITING_FOR_DATA;
			status = GAM_NO_REQUEST;
			break;
		default:
			break;
	}

	return status;
}
