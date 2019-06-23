/*
 * remote.c
 *
 *  Created on: Aug 10, 2015
 *      Author: sschreiner
 */

#include "remote.h"
#include "platform/gam_lib.h"
#include "platform/ppm_rc.h"
#include "platform/bram_lib.h"
#include "platform/timer_lib.h"
#include "sensors.h"

void remote_transmit_data(uint8_t calibrating)
{
	// Display data
	uint16_t tmp_voltage_logic = (uint16_t)(sensors_get_data().voltage_logic * 10);
	uint16_t tmp_voltage_motor = (uint16_t)(sensors_get_data().voltage_motor * 10);
	gam_update_voltage(tmp_voltage_logic, LOGIC_BATT);
	gam_update_voltage(tmp_voltage_motor, MOTOR_BATT);

	uint8_t tmp_temperature_zynq = (uint8_t)(sensors_get_data().temperature_zynq);
	uint8_t tmp_temperature_ambient = (uint8_t)(sensors_get_data().mpu9150_data.temperature);
	gam_update_temperature(tmp_temperature_zynq, ZYNQ_TEMP);
	gam_update_temperature(tmp_temperature_ambient, AMBIENT_TEMP);

	static const uint8_t min_cell_voltage = (MIN_LIPO_CELL_VOLTAGE >> 1)*10;
	gam_min_cell_voltage(min_cell_voltage);

//	gam_update_altitude((uint16_t)altitude_get_data().height);
//	gam_update_climbrate((int16_t)(altitude_get_data().speed * 100));

	// Warnings
	if (tmp_voltage_logic < MIN_VOLTAGE_LOGIC || tmp_voltage_motor < MIN_VOLTAGE_MOTOR)
		gam_warning_beep(UNDERVOLTAGE);
	else if (tmp_temperature_zynq > MAX_TEMPERATURE_ZYNQ)
		gam_warning_beep(MAX_TEMPERATUR);
	else if (calibrating)
		gam_warning_beep(CALIBRATION);
	else
		gam_warning_beep(NO_ALARM);

	gam_send_frame();
}

void remote_to_bram(void)
{
	static ppm_channel_s ppm_channel[PPM_CHANNEL_SIZE_OF];			/*!< the status of all ppm channels*/

	ppm_channel[PPM_CHANNEL_THRUST] 				= ppm_get_data(PPM_CHANNEL_THRUST);
	ppm_channel[PPM_CHANNEL_YAW] 					= ppm_get_data(PPM_CHANNEL_YAW);
	ppm_channel[PPM_CHANNEL_NICK] 					= ppm_get_data(PPM_CHANNEL_NICK);
	ppm_channel[PPM_CHANNEL_ROLL] 					= ppm_get_data(PPM_CHANNEL_ROLL);
	ppm_channel[PPM_CHANNEL_SW_HOLD_ALTITUDE] 		= ppm_get_data(PPM_CHANNEL_SW_HOLD_ALTITUDE);
	ppm_channel[PPM_CHANNEL_SW_CAMERA] 				= ppm_get_data(PPM_CHANNEL_SW_CAMERA);
	ppm_channel[PPM_CHANNEL_SW_OBJECT_DETECTION] 	= ppm_get_data(PPM_CHANNEL_SW_OBJECT_DETECTION);
	ppm_channel[PPM_CHANNEL_SW_MOTOR] 				= ppm_get_data(PPM_CHANNEL_SW_MOTOR);

	bram_pl_data_write_ppm_channel(RAW_RC_THRUST_ADDR, &ppm_channel[PPM_CHANNEL_THRUST]);
	bram_pl_data_write_ppm_channel(RAW_RC_YAW_ADDR, &ppm_channel[PPM_CHANNEL_YAW]);
	bram_pl_data_write_ppm_channel(RAW_RC_ROLL_ADDR, &ppm_channel[PPM_CHANNEL_NICK]);
	bram_pl_data_write_ppm_channel(RAW_RC_NICK_ADDR, &ppm_channel[PPM_CHANNEL_ROLL]);
	bram_pl_data_write_ppm_channel(RAW_RC_BUTTON_HOLD_ALTITUDE_ADDR, &ppm_channel[PPM_CHANNEL_SW_HOLD_ALTITUDE]);
	bram_pl_data_write_ppm_channel(RAW_RC_BUTTON_CAM_ADDR, &ppm_channel[PPM_CHANNEL_SW_CAMERA]);
	bram_pl_data_write_ppm_channel(RAW_RC_BUTTON_OBJECTDETECTION_ADDR, &ppm_channel[PPM_CHANNEL_SW_OBJECT_DETECTION]);
	bram_pl_data_write_ppm_channel(RAW_RC_BUTTON_MOTOR_ON_ADDR, &ppm_channel[PPM_CHANNEL_SW_MOTOR]);
	// copy time stamp of data to PL bram
	bram_pl_data_write_uint(RAW_RC_TIME_STAMP_ADDR, timer_get_current_system_time_MS());
}
