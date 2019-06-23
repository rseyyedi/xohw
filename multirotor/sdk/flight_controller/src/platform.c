/*
 * platform.c
 *
 *  Created on: Jul 17, 2015
 *      Author: sschreiner
 */

#include "platform.h"

#include "platform/timer_lib.h"
#include "platform/bram_lib.h"
#include "platform/ppm_rc.h"
#include "platform/gpio_lib.h"
#include "sensors.h"
#include "remote.h"
#include "altitude.h"
#include "attitude.h"
#include "controller.h"

#define UPDATE_SENSOR_DATA_TIME_MS	40
#define UPDATE_WARNINGS_TIME_MS		250
#define INIT_SAMPLES 				50//5000

platform_error_e platform_init(void)
{
	//////////////////////////////////////////////////////////////////////////////
	// Initialize Peripherals
	//////////////////////////////////////////////////////////////////////////////
	// Initialize GPIO
	if (gpio_init() != GPIO_NO_ERROR)
		return PLATFORM_GPIO_INIT_ERROR;
	// Initialize Timer
	if (timer_init() != TIMER_NO_ERROR)
		return PLATFORM_TIMER_INIT_ERROR;
	// Initialize BRAMs
	if (bram_pl_data_init() != BRAM_NO_ERROR)
		return PLATFORM_BRAM_INIT_ERROR;
	//////////////////////////////////////////////////////////////////////////////
	// Initialization complete
	//////////////////////////////////////////////////////////////////////////////

	// Temporary variables
	uint32_t main_update_time = 0;

	uint16_t init_counter = 0;
	uint8_t  init_phase = 1;

	sensors_from_bram_init();
	timer_wait_for(10);

	while (init_phase)
	{
		// Check 2ms
		if (timer_check_delay(main_update_time))
		{
			// Update 2ms timer
			main_update_time = timer_set_delay(UPDATE_SENSOR_DATA_TIME_MS);

			// Read remote data
			remote_from_bram();
			// Read sensor data
			sensors_from_bram();
			// Sensor offset calculation
			sensors_init_gyro_offsets(INIT_SAMPLES, init_counter);
			sensors_init_acc_offsets(INIT_SAMPLES, init_counter);
			sensors_init_height_offset(INIT_SAMPLES, init_counter);

			// Check if initialization is complete
			init_counter++;
			if ( init_counter >= INIT_SAMPLES )
				init_phase = 0;
		}
	}

	bram_pl_data_write_uint(MB_PC_INIT_VALID_ADDR, 1);

	remote_check_events();
	sensors_pre_calculations();

	attitude_calculation();
	attitude_to_bram();

	altitude_calculation();
	altitude_to_bram();

	return PLATFORM_NO_ERROR;
}

void execute_function(void)
{
	// Get remote control values from BRAM
	remote_from_bram();
	remote_check_events();
	// Get sensor values
	sensors_from_bram();
	sensors_pre_calculations();
	// Calculate attitude
	attitude_calculation();
	attitude_to_bram();
	// Calculate altitude
	altitude_calculation();
	altitude_to_bram();
	// Controllers
	controller_attitude();
	controller_altitude();
	// Calculate motor values and copy them to BRAM
	calculate_motor_values();
}

platform_error_e platform_execute(void)
{
	static uint32_t main_update_time = 0;
	static uint32_t warning_timer = 0;

	// Read sensor data
	if (timer_check_delay(main_update_time))
	{
		main_update_time = timer_set_delay(UPDATE_SENSOR_DATA_TIME_MS);

		execute_function();
	}

	if (timer_check_delay(warning_timer))
	{
		warning_timer = timer_set_delay(UPDATE_WARNINGS_TIME_MS);

		static uint8_t led_state = 0;
		if (led_state) led_state = 0;
		else led_state = 1;
		gpio_set(LED_3, led_state);
	}

	return PLATFORM_NO_ERROR;
}
