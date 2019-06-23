/*
 * platform.h
 *
 *  Created on: Jul 17, 2015
 *      Author: sschreiner
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <stdint.h>

/*
 * Platform Error Enum
 */
typedef enum {
	PLATFORM_NO_ERROR,
	PLATFORM_TIMER_INIT_ERROR,
	PLATFORM_BRAM_INIT_ERROR,
	PLATFORM_MPU9150_INIT_ERROR,
	PLATFORM_MPL3115A2_INIT_ERROR,
	PLATFORM_GPIO_INIT_ERROR,
	PLATFORM_MCP3201_INIT_ERROR,
	PLATFORM_ZYNQTEMP_INIT_ERROR,
	PLATFORM_GAM_INIT_ERROR,
	PLATFORM_MOTOR_INIT_ERROR
} platform_error_e;

platform_error_e platform_init(void);
platform_error_e platform_execute(void);


#endif /* PLATFORM_H_ */
