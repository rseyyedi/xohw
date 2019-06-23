/*
 * sensors.c
 *
 *  Created on: May 4, 2017
 *      Author: sschreiner
 */
#include "sensors.h"

#include "platform/bram_lib.h"

static sensor_data_s data;
static sensor_float_data_s offset;
static sensor_float_data_s final;

static uint32_t max_temperature_zynq = 0;
static uint32_t min_lipo_cell_voltage = 0;
static uint32_t min_voltage_logic = 0;
static uint32_t min_voltage_motor = 0;

void sensors_from_bram_init(void)
{
	bram_pl_data_read_uint(TEMPERATURE_MAX_ZYNQ_ADDR, &max_temperature_zynq);
	bram_pl_data_read_uint(VOLTAGE_MIN_CELL_ADDR, &min_lipo_cell_voltage);
	bram_pl_data_read_uint(VOLTAGE_MIN_LOGIC_ADDR, &min_voltage_logic);
	bram_pl_data_read_uint(VOLTAGE_MIN_MOTOR_ADDR, &min_voltage_motor);
}

void sensors_from_bram(void)
{
	bram_pl_data_read_int(RAW_ACC_X_ADDR,			&data.mpu9150_data.acc_x);
	bram_pl_data_read_int(RAW_ACC_Y_ADDR,			&data.mpu9150_data.acc_y);
	bram_pl_data_read_int(RAW_ACC_Z_ADDR,			&data.mpu9150_data.acc_z);
	bram_pl_data_read_int(RAW_GYRO_X_ADDR,			&data.mpu9150_data.gyro_x);
	bram_pl_data_read_int(RAW_GYRO_Y_ADDR,			&data.mpu9150_data.gyro_y);
	bram_pl_data_read_int(RAW_GYRO_Z_ADDR,			&data.mpu9150_data.gyro_z);
	bram_pl_data_read_int(RAW_MAG_X_ADDR,			&data.mpu9150_data.mag_x);
	bram_pl_data_read_int(RAW_MAG_Y_ADDR,			&data.mpu9150_data.mag_y);
	bram_pl_data_read_int(RAW_MAG_Z_ADDR,			&data.mpu9150_data.mag_z);
	bram_pl_data_read_float(RAW_HEIGHT_ADDR,		&data.mpl3115a2_data.height);
	bram_pl_data_read_int(TEMPERATURE_MPU_ADDR, 	&data.mpu9150_data.temperature);
	bram_pl_data_read_float(TEMPERATURE_MPL_ADDR, 	&data.mpl3115a2_data.temperature);
	bram_pl_data_read_float(TEMPERATURE_ZYNQ_ADDR,	&data.temperature_zynq);
	bram_pl_data_read_float(VOLTAGE_LOGIC_ADDR,		&data.voltage_logic);
	bram_pl_data_read_float(VOLTAGE_MOTOR_ADDR,		&data.voltage_motor);
	bram_pl_data_read_uint(IIC_ERRORS_ADDR, 		&data.iic_errors);
	// copy time stamp of data from PL bram
	uint32_t time_stamp = 0;
	bram_pl_data_read_uint(SENSOR_TIME_STAMP_ADDR,	&time_stamp);

	if (time_stamp != data.time_stamp)
		data.data_valid = 1;
	else
		data.data_valid = 0;
}

void sensors_init_gyro_offsets(const uint16_t max_init_samples, const uint16_t init_counter)
{
	static int32_t gyro_x_offset_sum = 0;
	static int32_t gyro_y_offset_sum = 0;
	static int32_t gyro_z_offset_sum = 0;

	gyro_x_offset_sum += data.mpu9150_data.gyro_x;
	gyro_y_offset_sum += data.mpu9150_data.gyro_y;
	gyro_z_offset_sum += data.mpu9150_data.gyro_z;

	if (init_counter >= max_init_samples)
	{
		// Initialization complete, calculate Offsets
		offset.mpu9150_data.gyro_x = (float)gyro_x_offset_sum / (float)max_init_samples;
		offset.mpu9150_data.gyro_y = (float)gyro_y_offset_sum / (float)max_init_samples;
		offset.mpu9150_data.gyro_z = (float)gyro_z_offset_sum / (float)max_init_samples;
	}
}

void sensors_init_acc_offsets(const uint16_t max_init_samples, const uint16_t init_counter)
{
	static int32_t acc_x_offset_sum = 0;
	static int32_t acc_y_offset_sum = 0;
	static int32_t acc_z_offset_sum = 0;

	acc_x_offset_sum += data.mpu9150_data.acc_x;
	acc_y_offset_sum += data.mpu9150_data.acc_y;
	acc_z_offset_sum += (data.mpu9150_data.acc_z - 16384); // we want to have 1g gravity

	if (init_counter >= max_init_samples)
	{
		// Initialization complete, calculate Offsets
		offset.mpu9150_data.acc_x = (float)acc_x_offset_sum / (float)max_init_samples;
		offset.mpu9150_data.acc_y = (float)acc_y_offset_sum / (float)max_init_samples;
		offset.mpu9150_data.acc_z = (float)acc_z_offset_sum / (float)max_init_samples;
	}
}

void sensors_init_height_offset(const uint16_t max_init_samples, const uint16_t init_counter)
{
	static float height_offset_sum = 0;

	height_offset_sum += data.mpl3115a2_data.height;

	if (init_counter >= max_init_samples)
	{
		// Initialization complete, calculate Offset
		offset.mpl3115a2_data.height = height_offset_sum / (float)max_init_samples;
	}
}

void sensors_pre_calculations(void)
{
	// Apply offsets to sensor values
	float tmp_acc_x  = (float)data.mpu9150_data.acc_x  - offset.mpu9150_data.acc_x;
	float tmp_acc_y  = (float)data.mpu9150_data.acc_y  - offset.mpu9150_data.acc_y;
	float tmp_acc_z  = (float)data.mpu9150_data.acc_z  - offset.mpu9150_data.acc_z;
	float tmp_gyro_x = (float)data.mpu9150_data.gyro_x - offset.mpu9150_data.gyro_x;
	float tmp_gyro_y = (float)data.mpu9150_data.gyro_y - offset.mpu9150_data.gyro_y;
	float tmp_gyro_z = (float)data.mpu9150_data.gyro_z - offset.mpu9150_data.gyro_z;
	float tmp_mag_x  = (float)data.mpu9150_data.mag_x;
	float tmp_mag_y  = (float)data.mpu9150_data.mag_y;
	float tmp_mag_z  = (float)data.mpu9150_data.mag_z;
	float tmp_height = data.mpl3115a2_data.height - offset.mpl3115a2_data.height;
	// Apply units to sensor values
	final.mpu9150_data.acc_x    = tmp_acc_x / 16384.0f; // to g
	final.mpu9150_data.acc_y    = tmp_acc_y / 16384.0f; // to g
	final.mpu9150_data.acc_z    = tmp_acc_z / 16384.0f; // to g

	final.mpu9150_data.gyro_x   = tmp_gyro_x / 131.0f; // to degree/sec
	final.mpu9150_data.gyro_y   = tmp_gyro_y / 131.0f; // to degree/sec
	final.mpu9150_data.gyro_z   = tmp_gyro_z / 131.0f; // to degree/sec

	final.mpu9150_data.mag_x    = tmp_mag_x * 3.0f; // to uT
	final.mpu9150_data.mag_y    = tmp_mag_y * 3.0f; // to uT
	final.mpu9150_data.mag_z    = tmp_mag_z * 3.0f; // to uT

	final.mpl3115a2_data.height = tmp_height; // to hPa over starting point
	// Other data
	final.mpu9150_data.temperature = (float)data.mpu9150_data.temperature;
	final.mpl3115a2_data.temperature = data.mpl3115a2_data.temperature;
}

sensor_float_data_s sensors_get_data(void)
{
	return final;
}
