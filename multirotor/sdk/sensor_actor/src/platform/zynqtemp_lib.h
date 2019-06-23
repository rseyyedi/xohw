/*
 * zynqtemp_lib.h
 *
 *  Created on: Jul 21, 2015
 *      Author: sschreiner
 */

#ifndef ZYNQTEMP_LIB_H_
#define ZYNQTEMP_LIB_H_

#include <stdint.h>

typedef enum{
	ZYNQTEMP_NO_ERROR,				/*!< no temperature error */
	ZYNQTEMP_INIT_ERROR		/*!< error while initialisation */
} zynqtemp_error_e;

zynqtemp_error_e zynqtemp_init(void);
void zynqtemp_init_filter(const uint16_t filter_length);
void zynqtemp_get_data(float *value);
void zynqtemp_get_filtered_data(float *value);

#endif /* ZYNQTEMP_LIB_H_ */
