/*
 * remote.h
 *
 *  Created on: Aug 10, 2015
 *      Author: sschreiner
 */

#ifndef REMOTE_H_
#define REMOTE_H_

#include <stdint.h>

void remote_transmit_data(uint8_t calibrating);
void remote_to_bram(void);

#endif /* REMOTE_H_ */
