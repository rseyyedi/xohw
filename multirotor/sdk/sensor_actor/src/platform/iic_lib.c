/*
 * iic_lib.c
 *
 *  Created on: Jul 17, 2015
 *      Author: sschreiner
 */

#include "iic_lib.h"

#include "xiic.h"
#include "xparameters.h"

static XIic mpl3115a2_device_object;
static XIic mpu9150_device_object;

iic_error_e iic_interface_init(const uint32_t interface_id)
{
	iic_error_e status = IIC_NO_ERROR;

	XIic *iic_temp_object;

	switch(interface_id)
	{
	case XPAR_SENSOR_ACTOR_AXI_IIC_BU_DEVICE_ID:
		iic_temp_object = &mpl3115a2_device_object;
		break;
	case XPAR_SENSOR_ACTOR_AXI_IIC_IMU_DEVICE_ID:
		iic_temp_object = &mpu9150_device_object;
		break;
	default:
		iic_temp_object = NULL;
	}

	if(XIic_Initialize(iic_temp_object, interface_id) == XST_FAILURE)
	{
		status = IIC_INIT_ERROR;
	}

	return status;
}

uint32_t iic_send(const uint32_t interface_id, const uint8_t iic_address, uint8_t *send_buffer, uint32_t byte_count)
{
	XIic *iic_temp_object;

	switch(interface_id)
	{
	case XPAR_SENSOR_ACTOR_AXI_IIC_BU_DEVICE_ID:
		iic_temp_object = &mpl3115a2_device_object;
		break;
	case XPAR_SENSOR_ACTOR_AXI_IIC_IMU_DEVICE_ID:
		iic_temp_object = &mpu9150_device_object;
		break;
	default:
		iic_temp_object = NULL;
	}

	return XIic_Send(iic_temp_object->BaseAddress, iic_address, send_buffer, byte_count, XIIC_STOP);
}

uint32_t iic_recv(const uint32_t interface_id, const uint8_t iic_address, uint8_t *recv_buffer, uint32_t byte_count)
{
	XIic *iic_temp_object;

	switch(interface_id)
	{
	case XPAR_SENSOR_ACTOR_AXI_IIC_BU_DEVICE_ID:
		iic_temp_object = &mpl3115a2_device_object;
		break;
	case XPAR_SENSOR_ACTOR_AXI_IIC_IMU_DEVICE_ID:
		iic_temp_object = &mpu9150_device_object;
		break;
	default:
		iic_temp_object = NULL;
	}

	return XIic_Recv(iic_temp_object->BaseAddress, iic_address, recv_buffer, byte_count, XIIC_STOP);
}

