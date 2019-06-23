/*
 * bram_lib.c
 *
 *  Created on: Aug 5, 2015
 *      Author: sschreiner
 */

#include "bram_lib.h"
#include "xparameters.h"
#include "xbram.h"

#define INIT_BRAM_VALUE	(unsigned long)0x00000000 			/*!< the initial value for each bram address */

static XBram bram_pl_data; /*!< The Instance of the BRAM Driver DataMiner */
static XBram bram_pl_to_ps_data; /*!< The Instance of the BRAM Driver ASG */

static XBram_Config *config_ptr_pl_data = NULL; /*!< the config pointer of the bram... needed in each function and is depent on the device id */
static XBram_Config *config_ptr_pl_to_ps_data = NULL; /*!< the config pointer of the bram... needed in each function and is depent on the device id */

static uint16_t device_id_pl_data = XPAR_BRAM_0_DEVICE_ID;
static uint16_t device_id_pl_to_ps_data = 0xFFFF;

/*!
 * These function initialize the PL data bram and set some default values to the data registers
 *
 * @return		bram_status_e	BRAM_NO_ERROR 				if everything ok
 * @return		bram_status_e	BRAM_ERR_INITIALISATION		if an error while initialization occurs
 *
 */
bram_error_e bram_pl_data_init(void)
{
	bram_error_e status = BRAM_NO_ERROR;
	uint32_t address = 0;
	uint16_t i = 0;
	uint32_t data = 0;
	int16_t state;

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this
	 * driver.
	 */

	config_ptr_pl_data = XBram_LookupConfig(device_id_pl_data);
	if (config_ptr_pl_data == (XBram_Config *) NULL)
	{
		status = BRAM_ERR_INITIALIZATION;
	}

	state = XBram_CfgInitialize(&bram_pl_data, config_ptr_pl_data, config_ptr_pl_data->CtrlBaseAddress);
	if (state != XST_SUCCESS)
	{
		status = BRAM_ERR_INITIALIZATION;
	}

	/*
	 * Init ECC
	 */
	if (config_ptr_pl_data->EccPresent
		&& config_ptr_pl_data->EccOnOffRegister
		&& config_ptr_pl_data->EccOnOffResetValue == 0
		&& config_ptr_pl_data->WriteAccess != 0)
	{
		for (address = config_ptr_pl_data->MemBaseAddress; address < config_ptr_pl_data->MemHighAddress; address += 4)
		{
			data = XBram_In32(address);
			XBram_Out32(address, data);
		}
		XBram_WriteReg(config_ptr_pl_data->CtrlBaseAddress, XBRAM_ECC_ON_OFF_OFFSET, 1);
	}

	/*
	 * Selftest
	 */
	state = XBram_SelfTest(&bram_pl_data, 0);
	if (state != XST_SUCCESS)
	{
		status = BRAM_ERR_INITIALIZATION;
	}

	/*
	 * Initialize the bram values to 0xFFFFFFFF
	 */
	for (i = 0; i < (config_ptr_pl_data->MemHighAddress - config_ptr_pl_data->MemBaseAddress); i+=4)
	{
		XBram_WriteReg(config_ptr_pl_data->MemBaseAddress, i, INIT_BRAM_VALUE);
	}

	return status;
}

/*!
 * These function initialize the PL to PS bram and set some default values to the data registers
 *
 * @return		bram_status_e	BRAM_NO_ERROR 				if everything ok
 * @return		bram_status_e	BRAM_ERR_INITIALISATION		if an error while initialization occurs
 *
 * @info		If device_id_asg = 0xFFFF BRAM of asg will be ignored
 */
bram_error_e bram_pl_to_ps_data_init(void)
{
	bram_error_e status = BRAM_NO_ERROR;
	uint32_t address = 0;
	uint16_t i = 0;
	uint32_t data = 0;
	int16_t state;

	if (device_id_pl_to_ps_data != 0xFFFF)
	{
		/*
		 * Lookup configuration data in the device configuration table.
		 * Use this configuration info down below when initializing this
		 * driver.
		 */
		config_ptr_pl_to_ps_data = XBram_LookupConfig(device_id_pl_to_ps_data);
		if (config_ptr_pl_to_ps_data == (XBram_Config *) NULL)
		{
			status = BRAM_ERR_INITIALIZATION;
		}

		state = XBram_CfgInitialize(&bram_pl_to_ps_data, config_ptr_pl_to_ps_data, config_ptr_pl_to_ps_data->CtrlBaseAddress);
		if (state != XST_SUCCESS)
		{
			status = BRAM_ERR_INITIALIZATION;
		}

		/*
		 * Init ECC
		 */
		if (config_ptr_pl_to_ps_data->EccPresent
			&& config_ptr_pl_to_ps_data->EccOnOffRegister
			&& config_ptr_pl_to_ps_data->EccOnOffResetValue == 0
			&& config_ptr_pl_to_ps_data->WriteAccess != 0)
		{
			for (address = config_ptr_pl_to_ps_data->MemBaseAddress; address < config_ptr_pl_to_ps_data->MemHighAddress; address += 4)
			{
				data = XBram_In32(address);
				XBram_Out32(address, data);
			}
			XBram_WriteReg(config_ptr_pl_to_ps_data->CtrlBaseAddress,
					XBRAM_ECC_ON_OFF_OFFSET, 1);
		}

		/*
		 * Selftest
		 */
		state = XBram_SelfTest(&bram_pl_to_ps_data, 0);
		if (state != XST_SUCCESS)
		{
			status = BRAM_ERR_INITIALIZATION;
		}

		/*
		 * Initialize the bram values to 0xFFFFFFFF
		 */
		for (i = 0; i < (config_ptr_pl_to_ps_data->MemHighAddress - config_ptr_pl_to_ps_data->MemBaseAddress); i++)
		{
			XBram_WriteReg(config_ptr_pl_to_ps_data->MemBaseAddress, i, INIT_BRAM_VALUE);
		}
	}
	else
	{
		status = BRAM_ERR_INITIALIZATION;
	}

	return status;
}

/*!
 * This function copies the whole content of the PL data BRAM into the PL to PS BRAM
 *
 * @param		void
 *
 * @return		bram_error_t		BRAM_NO_ERROR		if data copied successfully
 * @return		bram_error_t		BRAM_ERR_WRITE		if data can#t be copied
 *
 * @info		An BRAM_ERR_WRITE occurs, if no ASG BRAM was instantiated.
 */
bram_error_e bram_duplicate(void)
{
	bram_error_e status = BRAM_NO_ERROR;
	uint16_t i = 0;

	if (config_ptr_pl_to_ps_data != NULL) {
		for (i = 0; i < BRAM_ADDRESS_MAP_SIZE; i = i + SINGLE_STEP_SIZE)
		{
			XBram_WriteReg(config_ptr_pl_to_ps_data->MemBaseAddress, i, (XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, i)));
		}
	}
	else
	{
		status = BRAM_ERR_WRITE;
	}

	return status;
}

/*!
 * These function write the given data into the PL data bram
 *
 * @param[in]		uint32_t		address 	The address of the data
 * @param[in]		uint32_t		data		the data to write
 *
 * @return		bram_error_t		BRAM_NO_ERROR		if data written
 * @return		bram_error_t		BRAM_ERR_WRITE		if data can#t written
 *
 * @info		An BRAM_ERR_WRITE occurs, if the address is higher than the bram address space or
 * 										  if the readback data contained not the same data which was written
 * 										  if the device hasn't write access
 */
bram_error_e bram_pl_data_write_uint(uint32_t address, uint32_t data)
{
	bram_error_e status = BRAM_NO_ERROR;
	uint32_t data_readback = 0;

	/* check if the address is inside the bram address space */
	if ((config_ptr_pl_data->MemBaseAddress + address) > config_ptr_pl_data->MemHighAddress)
	{
		status = BRAM_ERR_WRITE;
	}
	else if (address >= BRAM_ADDRESS_MAP_SIZE)
	{
		status = BRAM_ERR_WRITE;
	}
	else
	{
		XBram_WriteReg(config_ptr_pl_data->MemBaseAddress, address, data);

		data_readback = XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, address);

		/* check if the written data is the same than the read data */
		if (data != data_readback)
		{
			status = BRAM_ERR_WRITE;
		}
	}

	return status;
}

bram_error_e bram_pl_data_write_int(uint32_t address, int32_t data)
{
	union bram_type_u temp;
	temp.type_int32 = data;
	return bram_pl_data_write_uint(address, temp.type_uint);
}

bram_error_e bram_pl_data_write_float(uint32_t address, float data)
{
	union bram_type_u temp;
	temp.type_float = data;
	return bram_pl_data_write_uint(address, temp.type_uint);
}

/*!
 * These function read the given data from the PL data bram address
 *
 * @param[in]		uint32_t		address 	The address of the data
 * @param[out] 		uint32_t		*data
 *
 * @return		bram_error_t		BRAM_NO_ERROR		if data read
 * @return		bram_error_t		BRAM_ERR_READ		if data can't read
 *
 * @info		An BRAM_ERR_READ occurs, if the address is higher than the bram address space
 */
bram_error_e bram_pl_data_read_uint(uint32_t address, uint32_t *data)
{
	bram_error_e status = BRAM_NO_ERROR;

	/* check if the address is inside the bram address space */
	if (config_ptr_pl_data->MemBaseAddress + address > config_ptr_pl_data->MemHighAddress)
	{
		status = BRAM_ERR_READ;
	}
	else if (address >= BRAM_ADDRESS_MAP_SIZE)
	{
		status = BRAM_ERR_READ;
	}
	else
	{
		*data = XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, address);
	}

	return status;
}

bram_error_e bram_pl_data_read_int(uint32_t address, int32_t *data)
{
	union bram_type_u temp;
	bram_error_e status = bram_pl_data_read_uint(address, &temp.type_uint);
	*data = temp.type_int32;
	return status;
}

bram_error_e bram_pl_data_read_float(uint32_t address, float *data)
{
	union bram_type_u temp;
	bram_error_e status = bram_pl_data_read_uint(address, &temp.type_uint);
	*data = temp.type_float;
	return status;
}

/*!
 * These function write the ppm channel data into the PL data bram
 *
 * @param[in]		u32					address 	The address of the data
 * @param[in]		ppm_channel_s*		data		the data to write
 *
 * @return		bram_status_e		BRAM_NO_ERROR		if data written
 * @return		bram_status_e		BRAM_ERR_WRITE		if data can#t written
 *
 * @info		An BRAM_ERR_WRITE occurs, if the address is higher than the bram address space or
 * 										  if the readback data contained not the same data which was written
 * 										  if the device hasn't write access
 */
bram_error_e bram_pl_data_write_ppm_channel(uint32_t address, ppm_channel_s *data)
{
	bram_error_e status = BRAM_NO_ERROR;
	uint32_t data_readback = 0;

	/* check if the address is inside the bram addressspace */
	if (config_ptr_pl_data->MemBaseAddress + (address + PPM_CHANNEL_STEP_SIZE) > config_ptr_pl_data->MemHighAddress)
	{
		status = BRAM_ERR_WRITE;
	}/* check if device has write access */
	else if (address >= BRAM_ADDRESS_MAP_SIZE)
	{
		status = BRAM_ERR_WRITE;
	}
	else
	{
		XBram_WriteReg(config_ptr_pl_data->MemBaseAddress, address, (uint32_t)((data->direction)));
		XBram_WriteReg(config_ptr_pl_data->MemBaseAddress, address + SINGLE_STEP_SIZE * 1, (uint32_t)((data->ppm_nr)));
		XBram_WriteReg(config_ptr_pl_data->MemBaseAddress, address + SINGLE_STEP_SIZE * 2, (uint32_t)((data->type)));
		XBram_WriteReg(config_ptr_pl_data->MemBaseAddress, address + SINGLE_STEP_SIZE * 3, (uint32_t)((data->value)));

		data_readback = XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, address);
		/* check if the written data is the same than the read data */
		if ((uint32_t) (data->direction) != data_readback)
		{
			status = BRAM_ERR_WRITE;
		}

		data_readback = XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, address + (SINGLE_STEP_SIZE * 1));
		/* check if the written data is the same than the read data */
		if ((uint32_t) (data->ppm_nr) != data_readback)
		{
			status = BRAM_ERR_WRITE;
		}

		data_readback = XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, address + (SINGLE_STEP_SIZE * 2));
		/* check if the written data is the same than the read data */
		if ((uint32_t) (data->type) != data_readback)
		{
			status = BRAM_ERR_WRITE;
		}

		data_readback = XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, address + (SINGLE_STEP_SIZE * 3));
		/* check if the written data is the same than the read data */
		if ((uint32_t) (data->value) != data_readback)
		{
			status = BRAM_ERR_WRITE;
		}
	}

	return status;
}

/*!
 * These function read the ppm channel data from the address in PL data bram
 *
 * @param[in]		u32				address 	The address of the data
 * @param[out]		ppm_channel_s*	data		contained direction, value and so on
 *
 * @return		bram_status_e		BRAM_NO_ERROR		if data read
 * @return		bram_status_e		BRAM_ERR_READ		if data can't read
 *
 * @info		An BRAM_ERR_READ occurs, if the address is higher than the bram address space
 */
bram_error_e bram_pl_data_read_ppm_channel(uint32_t address, ppm_channel_s *data)
{
	bram_error_e status = BRAM_NO_ERROR;

	/* check if the address is inside the bram addressspace */
	if (config_ptr_pl_data->MemBaseAddress + (address + PPM_CHANNEL_STEP_SIZE) > config_ptr_pl_data->MemHighAddress)
	{
		status = BRAM_ERR_READ;
	}
	else if (address >= BRAM_ADDRESS_MAP_SIZE)
	{
		status = BRAM_ERR_READ;
	}
	else
	{
		data->direction = (ppm_channel_dir_e) (XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, address));
		data->ppm_nr = (uint8_t) (XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, address + SINGLE_STEP_SIZE));
		data->type = (ppm_channel_type_e) (XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, address + SINGLE_STEP_SIZE * 2));
		data->value = (uint8_t) (XBram_ReadReg(config_ptr_pl_data->MemBaseAddress, address + SINGLE_STEP_SIZE * 3));
	}

	return status;
}

/*
 * These function write the given data into the PL to PS bram
 *
 * @param[in]		uint32_t		address 	The address of the data
 * @param[in]		uint32_t		data		the data to write
 *
 * @return		bram_error_t		BRAM_NO_ERROR		if data written
 * @return		bram_error_t		BRAM_ERR_WRITE		if data can#t written
 *
 * @info		An BRAM_ERR_WRITE occurs, if the address is higher than the bram address space or
 * 										  if the readback data contained not the same data which was written
 * 										  if the device hasn't write access
 */
bram_error_e bram_pl_to_ps_data_write(uint32_t address, uint32_t data)
{
	bram_error_e status = BRAM_NO_ERROR;
	uint32_t data_readback = 0;

	/* check if the address is inside the bram address space */
	if ((config_ptr_pl_to_ps_data->MemBaseAddress + address) > config_ptr_pl_to_ps_data->MemHighAddress)
	{
		status = BRAM_ERR_WRITE;
	}
	else if (address >= BRAM_ADDRESS_MAP_SIZE)
	{
		status = BRAM_ERR_WRITE;
	}
	else
	{
		XBram_WriteReg(config_ptr_pl_to_ps_data->MemBaseAddress, address, data);

		data_readback = XBram_ReadReg(config_ptr_pl_to_ps_data->MemBaseAddress, address);

		/* check if the written data is the same than the read data */
		if (data != data_readback)
		{
			status = BRAM_ERR_WRITE;
		}
	}

	return status;
}

bram_error_e bram_pl_to_ps_data_write_float(uint32_t address, float data)
{
	union bram_type_u temp;
	temp.type_float = data;
	return bram_pl_to_ps_data_write(address, temp.type_uint);
}

/*!
 * These function read the given data from the PL to PS bram address
 *
 * @param[in]		uint32_t		address 	The address of the data
 * @param[out] 		uint32_t		*data
 *
 * @return		bram_error_t		BRAM_NO_ERROR		if data read
 * @return		bram_error_t		BRAM_ERR_READ		if data can't read
 *
 * @info		An BRAM_ERR_READ occurs, if the address is higher than the bram address space
 */
bram_error_e bram_pl_to_ps_data_read(uint32_t address, uint32_t *data)
{
	bram_error_e status = BRAM_NO_ERROR;

	/* check if the address is inside the bram address space */
	if (config_ptr_pl_to_ps_data->MemBaseAddress + address > config_ptr_pl_to_ps_data->MemHighAddress)
	{
		status = BRAM_ERR_READ;
	}
	else if (address >= BRAM_ADDRESS_MAP_SIZE)
	{
		status = BRAM_ERR_READ;
	}
	else
	{
		*data = XBram_ReadReg(config_ptr_pl_to_ps_data->MemBaseAddress, address);
	}

	return status;
}

bram_error_e bram_pl_to_ps_data_read_float(uint32_t address, float *data)
{
	union bram_type_u temp;
	bram_error_e status = bram_pl_to_ps_data_read(address, &temp.type_uint);
	*data = temp.type_float;
	return status;
}

/*!
 * These function write the ppm channel data into the PL to PS bram
 *
 * @param[in]		u32					address 	The address of the data
 * @param[in]		ppm_channel_s*		data		the data to write
 *
 * @return		bram_status_e		BRAM_NO_ERROR		if data written
 * @return		bram_status_e		BRAM_ERR_WRITE		if data can#t written
 *
 * @info		An BRAM_ERR_WRITE occurs, if the address is higher than the bram address space or
 * 										  if the readback data contained not the same data which was written
 * 										  if the device hasn't write access
 */
bram_error_e bram_pl_to_ps_data_write_ppm_channel(uint32_t address, ppm_channel_s *data)
{
	bram_error_e status = BRAM_NO_ERROR;
	uint32_t data_readback = 0;

	/* check if the address is inside the bram addressspace */
	if (config_ptr_pl_to_ps_data->MemBaseAddress + (address + PPM_CHANNEL_STEP_SIZE) > config_ptr_pl_to_ps_data->MemHighAddress)
	{
		status = BRAM_ERR_WRITE;
	}/* check if device has write access */
	else if (address >= BRAM_ADDRESS_MAP_SIZE)
	{
		status = BRAM_ERR_WRITE;
	}
	else
	{
		XBram_WriteReg(config_ptr_pl_to_ps_data->MemBaseAddress, address, (uint32_t)((data->direction)));
		XBram_WriteReg(config_ptr_pl_to_ps_data->MemBaseAddress, address + SINGLE_STEP_SIZE * 1, (uint32_t)((data->ppm_nr)));
		XBram_WriteReg(config_ptr_pl_to_ps_data->MemBaseAddress, address + SINGLE_STEP_SIZE * 2, (uint32_t)((data->type)));
		XBram_WriteReg(config_ptr_pl_to_ps_data->MemBaseAddress, address + SINGLE_STEP_SIZE * 3, (uint32_t)((data->value)));

		data_readback = XBram_ReadReg(config_ptr_pl_to_ps_data->MemBaseAddress, address);
		/* check if the written data is the same than the read data */
		if ((uint32_t) (data->direction) != data_readback)
		{
			status = BRAM_ERR_WRITE;
		}

		data_readback = XBram_ReadReg(config_ptr_pl_to_ps_data->MemBaseAddress, address + (SINGLE_STEP_SIZE * 1));
		/* check if the written data is the same than the read data */
		if ((uint32_t) (data->ppm_nr) != data_readback)
		{
			status = BRAM_ERR_WRITE;
		}

		data_readback = XBram_ReadReg(config_ptr_pl_to_ps_data->MemBaseAddress, address + (SINGLE_STEP_SIZE * 2));
		/* check if the written data is the same than the read data */
		if ((uint32_t) (data->type) != data_readback)
		{
			status = BRAM_ERR_WRITE;
		}

		data_readback = XBram_ReadReg(config_ptr_pl_to_ps_data->MemBaseAddress, address + (SINGLE_STEP_SIZE * 3));
		/* check if the written data is the same than the read data */
		if ((uint32_t) (data->value) != data_readback)
		{
			status = BRAM_ERR_WRITE;
		}
	}

	return status;
}

/*!
 * These function read the ppm channel data from the address in PL to PS bram
 *
 * @param[in]		u32				address 	The address of the data
 * @param[out]		ppm_channel_s*	data		contained direction, value and so on
 *
 * @return		bram_status_e		BRAM_NO_ERROR		if data read
 * @return		bram_status_e		BRAM_ERR_READ		if data can't read
 *
 * @info		An BRAM_ERR_READ occurs, if the address is higher than the bram address space
 */
bram_error_e bram_pl_to_ps_data_read_ppm_channel(uint32_t address, ppm_channel_s *data)
{
	bram_error_e status = BRAM_NO_ERROR;

	/* check if the address is inside the bram addressspace */
	if (config_ptr_pl_to_ps_data->MemBaseAddress + (address + PPM_CHANNEL_STEP_SIZE) > config_ptr_pl_to_ps_data->MemHighAddress)
	{
		status = BRAM_ERR_READ;
	}
	else if (address >= BRAM_ADDRESS_MAP_SIZE)
	{
		status = BRAM_ERR_READ;
	}
	else
	{
		data->direction = (ppm_channel_dir_e) (XBram_ReadReg(config_ptr_pl_to_ps_data->MemBaseAddress, address));
		data->ppm_nr = (uint8_t) (XBram_ReadReg(config_ptr_pl_to_ps_data->MemBaseAddress, address + SINGLE_STEP_SIZE));
		data->type = (ppm_channel_type_e) (XBram_ReadReg(config_ptr_pl_to_ps_data->MemBaseAddress, address + SINGLE_STEP_SIZE * 2));
		data->value = (uint8_t) (XBram_ReadReg(config_ptr_pl_to_ps_data->MemBaseAddress, address + SINGLE_STEP_SIZE * 3));
	}

	return status;
}
