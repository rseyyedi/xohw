/*
 * altitude.h
 *
 *  Created on: Aug 12, 2015
 *      Author: sschreiner
 */

#ifndef ALTITUDE_H_
#define ALTITUDE_H_

typedef struct {
	float height;
	float speed;
	float acceleration;
} height_s;

void altitude_calculation(void);
height_s altitude_get_data(void);
void altitude_to_bram(void);

#endif /* ALTITUDE_H_ */
