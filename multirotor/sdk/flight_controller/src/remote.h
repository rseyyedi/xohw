/*
 * remote.h
 *
 *  Created on: Jul 10, 2015
 *      Author: sschreiner
 */

#ifndef REMOTE_H_
#define REMOTE_H_

#include <stdint.h>

typedef struct {
	float thrust;
	float yaw;
	float nick;
	float roll;
	uint8_t motor_sw;
	uint8_t altitude_sw;
	uint8_t camera_sw;
	uint8_t obj_detect_sw;
} remote_data_s;

typedef struct {
	uint8_t motors_on;
	uint8_t altitude_control;
	uint8_t camera_on;
	uint8_t object_detection_on;
} remote_events_s;

void remote_from_bram(void);
void remote_check_events(void);
remote_data_s remote_get_data(void);
remote_events_s remote_get_events(void);

#endif /* REMOTE_H_ */
