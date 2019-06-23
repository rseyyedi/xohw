/*
 * iic_mpu9150_lib.c
 *
 *  Created on: Jul 17, 2015
 *      Author: sschreiner
 */

#include "mpu9150_lib.h"
#include "iic_lib.h"
#include "timer_lib.h"
#include "xparameters.h"
#include <math.h>

#include "bram_lib.h"

#define M_PIF		3.1415926F
#define ROTATION	45

static const uint32_t mpu9150_device_id = IIC_INTERFACE_ID_1;
static uint32_t mpu9150_errors = 0;

static void remap_acc_gyro_values(volatile mpu9150_data_s *data);
static void remap_mag_values(volatile mpu9150_data_s *data);
static void rotate_sensor_values(volatile mpu9150_data_s *data);

mpu9150_error_e mpu9150_init(void)
{
	iic_error_e iic_status = IIC_NO_ERROR;

	iic_status = iic_interface_init(mpu9150_device_id);

	if (iic_status == IIC_NO_ERROR) // Interface init was successful
	{
		uint8_t send_buffer[2];
		//enable power mode
		send_buffer[0]=0x6B;
		send_buffer[1]=0b00000000;


		if(iic_send(mpu9150_device_id, MPU9150_ADDRESS, send_buffer, 2)!=2)
		{
			mpu9150_errors++;
			return MPU9150_SEND_ERROR;
		}

		timer_wait_for(10); //timer_wait_for(1000);

		//enable bypass-mode
		send_buffer[0]=0x37;
		send_buffer[1]=0b00000010;

		if(iic_send(mpu9150_device_id, MPU9150_ADDRESS, send_buffer, 2)!=2)
		{
			mpu9150_errors++;
			return MPU9150_SEND_ERROR;
		}

		timer_wait_for(10); //timer_wait_for(1000);

	}
	else
	{
		mpu9150_errors++;
		return MPU9150_INIT_ERROR;
	}

	return MPU9150_NO_ERROR;
}

/*!
 * This funcition reads the sensordata from the MPU-9150
 *
 *@param		sensorData_type* tmpSensordata Pointer to the struct, where the Sensorvalues are stored
 *
 *@return		IicError_type	Iic_No_Error 			if everything ok
 * 								Iic_Send_Error	        if an error occurs while sending information to the IIC-Device
 * 								Iic_Recv_Error	        if an error occurs while receiving information to the IIC-Device
 *
 * @info		nothing
 */
mpu9150_error_e mpu9150_get_data(volatile mpu9150_data_s *data)
{
	uint8_t send_buffer[2];
	uint8_t recv_buffer[14];

	uint8_t i = 0;
	for(i = 0; i<sizeof(recv_buffer)/sizeof(uint8_t); i++)
	{
		recv_buffer[i] = 0;
	}

	//read sensor data
	send_buffer[0]=0x3B;
	if(iic_send(mpu9150_device_id, MPU9150_ADDRESS, send_buffer, 1)!=1)
	{
		mpu9150_errors++;
		return MPU9150_SEND_ERROR;
	}
	if(iic_recv(mpu9150_device_id, MPU9150_ADDRESS, recv_buffer, 14)!=14)
	{
		mpu9150_errors++;
		return MPU9150_RECV_ERROR;
	}

	data->acc_x=(recv_buffer[0]<<8);
	data->acc_x+=recv_buffer[1];
	data->acc_y=(recv_buffer[2]<<8);
	data->acc_y+=recv_buffer[3];
	data->acc_z=(recv_buffer[4]<<8);
	data->acc_z+=recv_buffer[5];
	data->temperature=(recv_buffer[6]<<8);
	data->temperature+=recv_buffer[7];
	data->temperature=data->temperature/340.0F+35;

	data->gyro_x=(recv_buffer[8]<<8);
	data->gyro_x+=recv_buffer[9];
	data->gyro_y=(recv_buffer[10]<<8);
	data->gyro_y+=recv_buffer[11];
	data->gyro_z=(recv_buffer[12]<<8);
	data->gyro_z+=recv_buffer[13];

	remap_acc_gyro_values(data);
//	rotate_sensor_values(data);

	data->mpu9150_errors = mpu9150_errors;

	return MPU9150_NO_ERROR;
}

//----------------------------AK8975--------------------------//
/*!
 * This funcition starts the magnetometer measurement on the AK8975 (which is placed on the MPU-9150-Board)
 *
 *
 *@return		IicError_type	Iic_No_Error 			if everything ok
 * 								Iic_Send_Error	        if an error occurs while sending information to the IIC-Device
 * 								Iic_Recv_Error	        if an error occurs while receiving information to the IIC-Device
 *
 * @info		nothing
 */
mpu9150_error_e ak8975_start_magneto_measurement(void)
{
	uint8_t send_buffer[2];
	send_buffer[0]=0x0a;
	send_buffer[1]=0x01;

	if(iic_send(mpu9150_device_id, AK8975_ADDRESS, send_buffer, 2)!=2)
	{
		mpu9150_errors++;
		return MPU9150_SEND_ERROR;
	}

	return MPU9150_NO_ERROR;
}

/*!
 * This funcition reads the sensordata from the AK8975
 *
 *@param		sensorData_type* tmpSensordata Pointer to the struct, where the Sensorvalues are stored
 *
 *@return		IicError_type	Iic_No_Error 			if everything ok
 * 								Iic_Send_Error	        if an error occurs while sending information to the IIC-Device
 * 								Iic_Recv_Error	        if an error occurs while receiving information to the IIC-Device
 *
 * @info		nothing
 */
mpu9150_error_e ak8975_get_magneto_measurement(volatile mpu9150_data_s *data)
{
	uint8_t send_buffer[1];
	uint8_t recv_buffer[6];

	//read sensor data
	send_buffer[0]=0x03;

	 if(iic_send(mpu9150_device_id, AK8975_ADDRESS, send_buffer, 1)!=1)
	 {
		 mpu9150_errors++;
		return MPU9150_SEND_ERROR;
	 }

	 if(iic_recv(mpu9150_device_id, AK8975_ADDRESS, recv_buffer, 6)!=6)
	 {
		 mpu9150_errors++;
	  	return MPU9150_RECV_ERROR;
	 }

	data->mag_x=(recv_buffer[0]);
	data->mag_x+=(recv_buffer[1]<<8);
	data->mag_y=(recv_buffer[2]);
	data->mag_y+=(recv_buffer[3]<<8);
	data->mag_z=(recv_buffer[4]);
	data->mag_z+=(recv_buffer[5]<<8);

	remap_mag_values(data);

	data->mpu9150_errors = mpu9150_errors;

	return MPU9150_NO_ERROR;
}

/*!
 * This function makes a first re-mapping of the sensor values to the right axes and directions
 *
 * @param		mpu9150_data_s *data Pointer to the struct, where the sensor values are stored
 * @info		nothing
 */
void remap_acc_gyro_values(volatile mpu9150_data_s *data)
{
	// Accelerations
	int16_t tmp_acc_x = data->acc_y;
	int16_t tmp_acc_y = data->acc_x;
	int16_t tmp_acc_z = -data->acc_z; // -?
	data->acc_x=tmp_acc_x;
	data->acc_y=tmp_acc_y;
	data->acc_z=tmp_acc_z;
	// Angular velocities
	int16_t tmp_gyro_x = data->gyro_x;
	int16_t tmp_gyro_y = data->gyro_y;
	int16_t tmp_gyro_z = -data->gyro_z;
	data->gyro_x = tmp_gyro_x;
	data->gyro_y = tmp_gyro_y;
	data->gyro_z = tmp_gyro_z;
}

void remap_mag_values(volatile mpu9150_data_s *data)
{
	// Magnetometer
	int16_t tmp_mag_x = data->mag_y;// TODO: muss hier wirklich getauscht werden?
	int16_t tmp_mag_y = data->mag_x;
	int16_t tmp_mag_z = -data->mag_z;
	data->mag_x = tmp_mag_x;
	data->mag_y = tmp_mag_y;
	data->mag_z = tmp_mag_z;
}

/*!
 * This function processes a 2D rotation of the sensor values, to compensate the initial orientation of the sensor board towards the flight orientation
 *
 * @param		mpu9150_data_s *data Pointer to the struct, where the sensor values are stored
 * @info		nothing
 */
void rotate_sensor_values(volatile mpu9150_data_s *data)
{
	// Accelerations
	float tmp_acc_x = data->acc_x*cosf(ROTATION*M_PIF/180) - data->acc_y*sinf(ROTATION*M_PIF/180);
	float tmp_acc_y = data->acc_y*cosf(ROTATION*M_PIF/180) + data->acc_x*sinf(ROTATION*M_PIF/180);
	data->acc_x = (int16_t)tmp_acc_x;
	data->acc_y = (int16_t)tmp_acc_y;
	// Angular velocities
	float tmp_gyro_x = data->gyro_x*cosf(ROTATION*M_PIF/180) - data->gyro_y*sinf(ROTATION*M_PIF/180);
	float tmp_gyro_y = data->gyro_y*cosf(ROTATION*M_PIF/180) + data->gyro_x*sinf(ROTATION*M_PIF/180);
	data->gyro_x = (int16_t)tmp_gyro_x;
	data->gyro_y = (int16_t)tmp_gyro_y;
}
