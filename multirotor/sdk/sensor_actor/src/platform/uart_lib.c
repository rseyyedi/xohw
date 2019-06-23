/*
 * uart_lib.c
 *
 *  Created on: Jul 22, 2015
 *      Author: sschreiner
 */

#include "uart_lib.h"
#include "xuartlite.h"
#include "xuartlite_l.h"
#include "xparameters.h"

static XUartLite gam_device_object;	/*!< instance from the uart lite ip core */

uart_error_e uart_interface_init(void)
{
	uart_error_e status = UART_NO_ERROR;

	// Init UART
	if (XUartLite_Initialize(&gam_device_object, XPAR_UARTLITE_0_DEVICE_ID) != XST_SUCCESS)
	{
		status = UART_INIT_ERROR;
		return status;
	}

	if (XUartLite_SelfTest(&gam_device_object) != XST_SUCCESS)
	{
		status = UART_INIT_ERROR;
		return status;
	}

	return status;
}

void uart_send(uint8_t data)
{
	XUartLite_SendByte(gam_device_object.RegBaseAddress, data);
}

uint8_t uart_is_receive_empty(void)
{
	return XUartLite_IsReceiveEmpty(gam_device_object.RegBaseAddress);
}

uint16_t uart_recv(uint8_t *buffer, uint16_t byte_count)
{
	return XUartLite_Recv(&gam_device_object, buffer, sizeof(buffer)/sizeof(uint8_t));
}

void uart_reset_fifos(void)
{
	XUartLite_ResetFifos(&gam_device_object);
}
