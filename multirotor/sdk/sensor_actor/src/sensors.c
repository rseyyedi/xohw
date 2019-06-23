/*
 * attitude_altitude.c
 *
 *  Created on: Aug 7, 2015
 *      Author: sschreiner
 */

#include "sensors.h"

#include "platform/gpio_lib.h"
#include "platform/zynqtemp_lib.h"
#include "platform/mcp3201_lib.h"
#include "platform/bram_lib.h"
#include "platform/timer_lib.h"

static sensor_data_s data;
static counter = 0;


void sensors_read_data(void)
{
	static sensor_read_state_e sensor_read_state = p1;	//first state is p1



	// Acc and Gyro data are read every time
#ifdef IMU
	mpu9150_get_data(&data.mpu9150_data);
#endif

	switch(sensor_read_state)
	{
	case p1:
		//zynqtemp_get_filtered_data(&data.temperature_zynq);
#ifdef IMU
		ak8975_start_magneto_measurement();
#endif
		sensor_read_state = p2;
		break;
	case p2:
		//mcp3201_get_filtered_data(LOGIC_VOLTAGE, &data.voltage_logic);
		sensor_read_state = p3;
		break;
	case p3:
		//mcp3201_get_filtered_data(MOTOR_VOLTAGE, &data.voltage_motor);
		sensor_read_state = p4;
		break;
	case p4:
		sensor_read_state = p5;
		break;
	case p5:
#ifdef BU
		mpl3115a2_get_data(&data.mpl3115a2_data);
#endif
		sensor_read_state = p6;
		break;
	case p6:
		//zynqtemp_get_filtered_data(&data.temperature_zynq);
		sensor_read_state = p7;
		break;
	case p7:
#ifdef IMU
		ak8975_get_magneto_measurement(&data.mpu9150_data);
#endif

		sensor_read_state = p8;
		break;
	case p8:
#ifdef IMU
		ak8975_start_magneto_measurement();
#endif

		sensor_read_state = p9;
		break;
	case p9:
		sensor_read_state = p10;
		break;
	case p10:
#ifdef BU
		mpl3115a2_get_data(&data.mpl3115a2_data);
#endif
		sensor_read_state = p11;
		break;
	case p11:
		//zynqtemp_get_filtered_data(&data.temperature_zynq);
		sensor_read_state = p12;
		break;
	case p12:
		sensor_read_state = p13;
		break;
	case p13:
		sensor_read_state = p14;
		break;
	case p14:
#ifdef IMU
		ak8975_get_magneto_measurement(&data.mpu9150_data);
#endif
		sensor_read_state = p15;
		break;
	case p15:
#ifdef BU
		mpl3115a2_get_data(&data.mpl3115a2_data);
#endif
		sensor_read_state = p1;

		static uint8_t led_state = 0;
		if (led_state) led_state = 0;
		else led_state = 1;
		gpio_set(LED_1, led_state);
		break;
	default:
		sensor_read_state = p1;
		break;
	}
}

sensor_data_s sensors_get_data(void)
{
	return data;
}

void sensors_to_bram_init(void)
{
	bram_pl_data_write_uint(TEMPERATURE_MAX_ZYNQ_ADDR, MAX_TEMPERATURE_ZYNQ);
	bram_pl_data_write_uint(VOLTAGE_MIN_CELL_ADDR, MIN_LIPO_CELL_VOLTAGE);
	bram_pl_data_write_uint(VOLTAGE_MIN_LOGIC_ADDR, MIN_VOLTAGE_LOGIC);
	bram_pl_data_write_uint(VOLTAGE_MIN_MOTOR_ADDR, MIN_VOLTAGE_MOTOR);
}

void sensors_to_bram(void)
{
	bram_pl_data_write_int(RAW_ACC_X_ADDR,			(int32_t)data.mpu9150_data.acc_x);
	bram_pl_data_write_int(RAW_ACC_Y_ADDR,			(int32_t)data.mpu9150_data.acc_y);
	bram_pl_data_write_int(RAW_ACC_Z_ADDR,			(int32_t)data.mpu9150_data.acc_z);
	bram_pl_data_write_int(RAW_GYRO_X_ADDR,			(int32_t)data.mpu9150_data.gyro_x);
	bram_pl_data_write_int(RAW_GYRO_Y_ADDR,			(int32_t)data.mpu9150_data.gyro_y);
	bram_pl_data_write_int(RAW_GYRO_Z_ADDR,			(int32_t)data.mpu9150_data.gyro_z);
	bram_pl_data_write_int(RAW_MAG_X_ADDR,			(int32_t)data.mpu9150_data.mag_x);
	bram_pl_data_write_int(RAW_MAG_Y_ADDR,			(int32_t)data.mpu9150_data.mag_y);
	bram_pl_data_write_int(RAW_MAG_Z_ADDR,			(int32_t)data.mpu9150_data.mag_z);
	bram_pl_data_write_float(RAW_HEIGHT_ADDR,		data.mpl3115a2_data.height);
	bram_pl_data_write_int(TEMPERATURE_MPU_ADDR, 	(int32_t)data.mpu9150_data.temperature);
	bram_pl_data_write_float(TEMPERATURE_MPL_ADDR, 	data.mpl3115a2_data.temperature);
	bram_pl_data_write_float(TEMPERATURE_ZYNQ_ADDR, data.temperature_zynq);
	bram_pl_data_write_float(VOLTAGE_LOGIC_ADDR,	data.voltage_logic);
	bram_pl_data_write_float(VOLTAGE_MOTOR_ADDR,	data.voltage_motor);
	bram_pl_data_write_uint(IIC_ERRORS_ADDR, 		(data.mpu9150_data.mpu9150_errors+data.mpl3115a2_data.mpl3115a2_errors));
	// copy time stamp of data to PL bram
	bram_pl_data_write_uint(SENSOR_TIME_STAMP_ADDR, timer_get_current_system_time_MS());
}
