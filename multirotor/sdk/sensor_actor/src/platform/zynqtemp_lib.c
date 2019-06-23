/*
 * zynqtemp_lib.c
 *
 *  Created on: Jul 21, 2015
 *      Author: sschreiner
 */

#include "zynqtemp_lib.h"
#include "xparameters.h"
#include "xsysmon.h"

static const uint32_t zynqtemp_device_id = XPAR_SYSMON_0_DEVICE_ID;
static uint32_t zynqtemp_errors = 0;
static float zynqtemp_filter_sum = 0.0f;
static uint16_t zynqtemp_filter_length = 1;

static XSysMon sys_mon;

zynqtemp_error_e zynqtemp_init(void)
{
	zynqtemp_error_e status = ZYNQTEMP_NO_ERROR;

    XSysMon_Config *config_ptr;
    XSysMon *sys_mon_ptr = &sys_mon;

    config_ptr = XSysMon_LookupConfig(zynqtemp_device_id);
    if (config_ptr == NULL)
    {
    	zynqtemp_errors++;
    	status = ZYNQTEMP_INIT_ERROR;
    	return status;
    }

    XSysMon_CfgInitialize(sys_mon_ptr, config_ptr, config_ptr->BaseAddress);

    /*
	 * Self Test the System Monitor/ADC device
	 */
	if (XSysMon_SelfTest(sys_mon_ptr) != XST_SUCCESS) {
		zynqtemp_errors++;
		status = ZYNQTEMP_INIT_ERROR;
		return status;
	}

	/*
	 * set average calculation for the adc, which read the temp value
	 */
	XSysMon_SetSeqAvgEnables(sys_mon_ptr, XSM_SEQ_CH_TEMP);

	return status;
}

void zynqtemp_init_filter(const uint16_t filter_length)
{
	uint16_t temp_raw_data;
	float temp_data;
	zynqtemp_filter_sum = 0.0f;

	zynqtemp_filter_length = filter_length;
	temp_raw_data = XSysMon_GetAdcData(&sys_mon, XSM_CH_TEMP);
	temp_data = XSysMon_RawToTemperature(temp_raw_data);
	zynqtemp_filter_sum = temp_data * (float)zynqtemp_filter_length;
}

void zynqtemp_get_data(float *value)
{
	uint16_t temp_raw_data;

	temp_raw_data = XSysMon_GetAdcData(&sys_mon, XSM_CH_TEMP);
	*value = XSysMon_RawToTemperature(temp_raw_data);
}

void zynqtemp_get_filtered_data(float *value)
{
	uint16_t temp_raw_data;

	zynqtemp_filter_sum -= (zynqtemp_filter_sum / (float)zynqtemp_filter_length);
	temp_raw_data = XSysMon_GetAdcData(&sys_mon, XSM_CH_TEMP);
	zynqtemp_filter_sum += XSysMon_RawToTemperature(temp_raw_data);

	if (zynqtemp_filter_length==0){
		return;
	}

	*value = zynqtemp_filter_sum / (float)zynqtemp_filter_length;
}
