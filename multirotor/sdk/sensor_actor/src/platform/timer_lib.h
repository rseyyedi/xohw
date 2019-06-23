/*
 * timer_lib.h
 *
 *  Created on: Jul 17, 2015
 *      Author: sschreiner
 */

#ifndef TIMER_LIB_H_
#define TIMER_LIB_H_

#include <stdint.h>

typedef enum {
	TIMER_NO_ERROR,
	TIMER_INIT_ERROR,
	TIMER_SELFTEST_ERROR,
	TIMER_INTERRUPT_ERROR
} timer_error_e;

timer_error_e timer_init(void);
uint32_t timer_get_current_system_time_MS(void);
uint32_t timer_set_delay(uint32_t delay);
uint8_t timer_check_delay(uint32_t delay);
void timer_wait_for(uint32_t delay);

#endif /* TIMER_LIB_H_ */
