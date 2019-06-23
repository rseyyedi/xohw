/*
 * spi_lib.c
 *
 *  Created on: Jul 20, 2015
 *      Author: sschreiner
 */

#include "spi_lib.h"

#include "xspi.h"
#include "xparameters.h"

static XSpi mcp3201_device_object;   /**< Instance of the SPI device */

spi_error_e spi_interface_init(const uint32_t interface_id)
{
	spi_error_e status = SPI_NO_ERROR;

	XSpi_Config *ConfigPtr;

	//Initialize the SPI driver so that it is  ready to use
	ConfigPtr = XSpi_LookupConfig(interface_id);
	if (ConfigPtr == NULL)
	{
		status = SPI_INIT_ERROR;
		return status;
	}

	if (XSpi_Initialize(&mcp3201_device_object, interface_id) != XST_SUCCESS)
	{
		status = SPI_INIT_ERROR;
		return status;
	}

	//Perform a self-test to ensure that the hardware was built correctly
	if (XSpi_SelfTest(&mcp3201_device_object) != XST_SUCCESS) {
		status = SPI_INIT_ERROR;
		return status;
	}

	//Set the Spi device as a master and let the user manage the slave select signal
	if (XSpi_SetOptions(&mcp3201_device_object, XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION) != XST_SUCCESS) {
		status = SPI_INIT_ERROR;
		return status;
	}

	//Start the SPI driver so that the device is enabled
	XSpi_Start(&mcp3201_device_object);

	//Disable global interrupts for the device
	XSpi_IntrGlobalDisable(&mcp3201_device_object);

	return status;
}

spi_error_e spi_recv(const uint8_t slave_id, uint8_t *recv_buffer, uint32_t byte_count)
{
	spi_error_e status = SPI_NO_ERROR;

	if (XSpi_SetSlaveSelect(&mcp3201_device_object, slave_id) != XST_SUCCESS)
	{
		status = SPI_SLAVE_SELECT_ERROR;
		return status;
	}

	if (XSpi_Transfer(&mcp3201_device_object, recv_buffer, recv_buffer, 2) != XST_SUCCESS)
	{
		status = SPI_RECV_ERROR;
		return status;
	}

	if (XSpi_SetSlaveSelect(&mcp3201_device_object, 0) != XST_SUCCESS)
	{
		status = SPI_SLAVE_SELECT_ERROR;
		return status;
	}

	return status;
}
