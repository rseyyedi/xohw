/*
 * platform.c
 *
 *  Created on: Jul 17, 2015
 *      Author: sschreiner
 */

#include "platform.h"

#include "platform/timer_lib.h"
#include "platform/bram_lib.h"
#include "platform/mpu9150_lib.h"
#include "platform/mcp3201_lib.h"
#include "platform/zynqtemp_lib.h"
#include "platform/ppm_rc.h"
#include "platform/gam_lib.h"
#include "platform/gpio_lib.h"
#include "platform/motor_lib.h"
#include "platform/mpl3115a2_lib.h"
#include "sensors.h"
#include "remote.h"

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
	//////////////////////////////////////////////////
//	gpio_set(LED_1, 1);
////	timer_wait_for(20);//200); This is... we will call it... not so intelligent...
	// Initialize Timer
	if (timer_init() != TIMER_NO_ERROR)
		return PLATFORM_TIMER_INIT_ERROR;
//	//////////////////////////////////////////////////
//	gpio_set(LED_1, 0);
//	timer_wait_for(20);//200);
//	// Initialize BRAM
	if (bram_pl_data_init() != BRAM_NO_ERROR)
		return PLATFORM_BRAM_INIT_ERROR;
	else
		sensors_to_bram_init();
//	//////////////////////////////////////////////////
//	gpio_set(LED_1, 1);
	timer_wait_for(20);//200);
//	// Initialize PPM RC
	ppm_init();
//	//////////////////////////////////////////////////
//	gpio_set(LED_1, 0);
	timer_wait_for(20);//200);
//	// Initialize MPU9150
#ifdef IMU
	if (mpu9150_init() != MPU9150_NO_ERROR)
		return PLATFORM_MPU9150_INIT_ERROR;
#endif
	//////////////////////////////////////////////////
//	gpio_set(LED_1, 1);
//	timer_wait_for(20);//200);
	// Initialize MPL3115A2
#ifdef BU
	if (mpl3115a2_init() != MPL3115A2_NO_ERROR)
		return PLATFORM_MPL3115A2_INIT_ERROR;
#endif
	//////////////////////////////////////////////////
//	gpio_set(LED_1, 0);
//	timer_wait_for(20);//200);
//	// Initialize MCP3201
//	if (mcp3201_init() != MCP3201_NO_ERROR)
//		return PLATFORM_MCP3201_INIT_ERROR;
//	else
//	{
//		mcp3201_init_filter(LOGIC_VOLTAGE,100);
//		mcp3201_init_filter(MOTOR_VOLTAGE,100);
//	}
	//////////////////////////////////////////////////
//	gpio_set(LED_1, 1);
//	timer_wait_for(20);//200);
//	// Initialize Temperature Sensor
//	if (zynqtemp_init() != ZYNQTEMP_NO_ERROR)
//		return PLATFORM_ZYNQTEMP_INIT_ERROR;
//	else
//		zynqtemp_init_filter(100);
	//////////////////////////////////////////////////
//	gpio_set(LED_1, 0);
	timer_wait_for(20);//200);
//	// Initialize Motors
	if (motor_init() != MOTOR_NO_ERROR)
		return PLATFORM_MOTOR_INIT_ERROR;
	//////////////////////////////////////////////////////////////////////////////
	// Initialization complete
	//////////////////////////////////////////////////////////////////////////////

	// Temporary variables
	uint32_t main_update_time = 0;

	uint16_t init_counter = 0;
	uint8_t  init_phase = 1;
	uint32_t init_mb_pc = 0;

	while (init_phase)
	{
		// Check 2ms
		if (timer_check_delay(main_update_time))
		{

			// Update 2ms timer
			main_update_time = timer_set_delay(UPDATE_SENSOR_DATA_TIME_MS);
			// Read sensor data
			sensors_read_data();
			sensors_to_bram();
			// Transfer values to remote control display
			// with "calibrating" flag
			remote_transmit_data(1);
			// Copy remote control values to BRAM
			remote_to_bram();
			// Get motor setpoints, but send always 0 to all motors in init phase
			motor_values_from_bram();
			motor_send_values(0);
			// Check if initialization is complete
			init_counter++;
			bram_pl_data_read_uint(MB_PC_INIT_VALID_ADDR, &init_mb_pc);
			if (	init_counter >= INIT_SAMPLES &&	init_mb_pc )
				init_phase = 0;
		}
	}

	bram_pl_data_write_uint(MB_SAIO_INIT_VALID_ADDR, 1);

	return PLATFORM_NO_ERROR;
}

void toggle(){
	{
		//warning_timer = timer_set_delay(UPDATE_WARNINGS_TIME_MS);

		static uint8_t led_state = 0;
		if (led_state) led_state = 0;
		else led_state = 1;
		gpio_set(LED_2, led_state);
	}
}

void execute_function(void)
{
	static uint32_t attitude_valid_flag = 0;
	static uint32_t altitude_valid_flag = 0;

	static int j = 0;
	j++;
	if(j>100){
		toggle();
		j=0;
	}
//
	// Check for valid data
	bram_pl_data_read_uint(ATTITUDE_VALID_ADDR, &attitude_valid_flag);
	bram_pl_data_read_uint(ALTITUDE_VALID_ADDR, &altitude_valid_flag);
	sensors_read_data();
	sensors_to_bram();
	remote_transmit_data(0);
	remote_to_bram();
	motor_values_from_bram();
	motor_send_values(1); // TODO: read and check timestamp from BRAM if data is new
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

	//if (timer_check_delay(warning_timer))

//	{
//		warning_timer = timer_set_delay(UPDATE_WARNINGS_TIME_MS);
//
//		static uint8_t led_state = 0;
//		if (led_state) led_state = 0;
//		else led_state = 1;
//		gpio_set(LED_2, led_state);
//	}

	return PLATFORM_NO_ERROR;
}
