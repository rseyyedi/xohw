/*
 * iic_mpl3115A2_lib.c
 *
 *  Created on: Dec 08, 2015
 *      Author: dgrundt
 */

#include "mpl3115a2_lib.h"

#include "xparameters.h"
#include "iic_lib.h"

static const uint32_t mpl3115a2_device_id = IIC_INTERFACE_ID_0;
uint32_t mpl3115a2_errors = 0;

mpl3115a2_error_e mpl3115a2_init(void)
{

	iic_error_e iic_status = IIC_NO_ERROR;
	iic_status = iic_interface_init(mpl3115a2_device_id);

	if (iic_status == IIC_NO_ERROR) // Interface init was successful
	{
		//Set Sensor to Altimeter mode
		//Set new data to every 100Hz(10ms);
		uint8_t send_buffer[2];
		send_buffer[0] = CTRL_REG1;
		send_buffer[1] = 0x89;

		if(iic_send(mpl3115a2_device_id, MPL3115A2_ADDRESS, send_buffer, 2) != 2)
		{
			mpl3115a2_errors++;
			return MPL3115A2_INIT_ERROR;
		}

		// Set Event flag activce;
		//Generate data ready event flag on new Pressure/Altitude or Temperature data
		send_buffer[0] = PT_DATA_CFG;
		send_buffer[1] = 0x07;

		if(iic_send(mpl3115a2_device_id, MPL3115A2_ADDRESS, send_buffer, 2) != 2)
		{
			mpl3115a2_errors++;
			return MPL3115A2_INIT_ERROR;

		}
	}
	else
	{
		mpl3115a2_errors++;
		return MPL3115A2_INIT_ERROR;
	}
	return MPL3115A2_NO_ERROR;
}

mpl3115a2_error_e mpl3115a2_get_data(mpl3115a2_data_s *data)
{

	uint8_t recv_buffer[6] = {0,0,0,0,0,0};
	uint8_t status_reg = 0;
	uint8_t amsb = 0;
	uint8_t acsb = 0;
	uint8_t alsb = 0;
	uint8_t tmsb = 0;
	uint8_t tlsb = 0;

	if(iic_recv(mpl3115a2_device_id, MPL3115A2_ADDRESS, recv_buffer, 6) != 6)
	{
		mpl3115a2_errors++;
		data->mpl3115a2_errors = mpl3115a2_errors;
	    return MPL3115A2_RECV_ERROR;
	}

	status_reg = recv_buffer[0];

	amsb = recv_buffer[1]; // msb of altitude
	acsb = recv_buffer[2]; // csb of altitude
	alsb = recv_buffer[3]; // lsb of altitude

	tmsb = recv_buffer[4]; // msb of temperature
	tlsb = recv_buffer[5]; // lsb of temperature

//	if((status_reg & 0x08) == 1)
//	{
		//Druckausgabe
		float templsb = (float)(alsb)/16.0f;
		float altitude = (float)( (amsb << 8) | acsb ) + templsb;

		//Tempausgabe
		templsb = (float)(tlsb)/16.0f;
		float temperature = (float)(tmsb + templsb);

		data->height = altitude;
		data->temperature = temperature;
//	}

	return MPL3115A2_NO_ERROR;


}
