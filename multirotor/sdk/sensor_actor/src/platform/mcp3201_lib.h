/*
 * mcp3201_lib.h
 *
 *  Created on: Jul 20, 2015
 *      Author: sschreiner
 */

#ifndef MCP3201_LIB_H_
#define MCP3201_LIB_H_

#include <stdint.h>

#define LOGIC_VOLTAGE		1   /**< Slave Select Signal to address logic battery guard */
#define MOTOR_VOLTAGE		2   /**< Slave Select Signal to address motor battery guard */

/*
 * struct to describe the mcp3201_error type
 */
typedef enum{
	MCP3201_NO_ERROR,   /*!< no mcp3201-Error */
	MCP3201_RECV_ERROR, /*!< an error occurred while receiving data */
	MCP3201_SEND_ERROR, /*!< an error occurred while sending data */
	MCP3201_INIT_ERROR,   /*!< an error occurred, while initializing the mcp3201-ports */
	MCP3201_SLAVE_SELECT_ERROR,
	MCP3201_DATA_ERROR    /*!< the received data from an mcp3201-sensor is logical invalid */
} mcp3201_error_e;  /*!< Enum_type to describe the occurred mcp3201-error */

mcp3201_error_e mcp3201_init(void);
mcp3201_error_e mcp3201_init_filter(const uint8_t slave_id, const uint16_t filter_length);
mcp3201_error_e mcp3201_get_data(const uint8_t slave_id, float *value);
mcp3201_error_e mcp3201_get_filtered_data(const uint8_t slave_id, float *value);

#endif /* MCP3201_LIB_H_ */
