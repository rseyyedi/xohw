/*
 * bram_lib.h
 *
 *  Created on: Aug 5, 2015
 *      Author: sschreiner
 */

#ifndef BRAM_LIB_H_
#define BRAM_LIB_H_

#include <stdint.h>
#include "bram_map.h"
#include "ppm_rc.h"

/*!
 * some status enums for easier usage and error feedback
 */
typedef enum {
	BRAM_NO_ERROR = 0,			/*!< no bram error */
	BRAM_ERR_INITIALIZATION,	/*!< error while initialization*/
	BRAM_ERR_WRITE,				/*!< error while writing into the bram*/
	BRAM_ERR_READ				/*!< error while reading from the bram*/
} bram_error_e;

union bram_type_u {
	uint32_t type_uint;
	int32_t	type_int32;
	float type_float;
};

bram_error_e bram_pl_data_init(void);
bram_error_e bram_pl_to_ps_data_init(void);

bram_error_e bram_duplicate(void);

bram_error_e bram_pl_data_write_uint(uint32_t address, uint32_t data);
bram_error_e bram_pl_data_write_int(uint32_t address, int32_t data);
bram_error_e bram_pl_data_write_float(uint32_t address, float data);
bram_error_e bram_pl_data_read_uint(uint32_t address, uint32_t *data);
bram_error_e bram_pl_data_read_int(uint32_t address, int32_t *data);
bram_error_e bram_pl_data_read_float(uint32_t address, float *data);
bram_error_e bram_pl_data_write_ppm_channel(uint32_t address, ppm_channel_s *data);
bram_error_e bram_pl_data_read_ppm_channel(uint32_t address, ppm_channel_s *data);

bram_error_e bram_pl_to_ps_data_write(uint32_t address, uint32_t data);
bram_error_e bram_pl_to_ps_data_write_float(uint32_t address, float data);
bram_error_e bram_pl_to_ps_data_read(uint32_t address, uint32_t *data);
bram_error_e bram_pl_to_ps_data_read_float(uint32_t address, float *data);
bram_error_e bram_pl_to_ps_data_write_ppm_channel(uint32_t address, ppm_channel_s *data);
bram_error_e bram_pl_to_ps_data_read_ppm_channel(uint32_t address, ppm_channel_s *data);

#endif /* BRAM_LIB_H_ */
