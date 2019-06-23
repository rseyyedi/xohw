/*
 * iic_lib.h
 *
 *  Created on: Jul 17, 2015
 *      Author: sschreiner
 */

#ifndef IIC_LIB_H_
#define IIC_LIB_H_

#include <stdint.h>

#define IIC_INTERFACE_ID_0 XPAR_SENSOR_ACTOR_AXI_IIC_BU_DEVICE_ID
#define IIC_INTERFACE_ID_1 XPAR_SENSOR_ACTOR_AXI_IIC_IMU_DEVICE_ID

/*
 * struct to describe the iic_error type
 */
typedef enum{
	IIC_NO_ERROR,   /*!< no IIC-Error */
	IIC_RECV_ERROR, /*!< an error occurred while receiving data */
	IIC_SEND_ERROR, /*!< an error occurred while sending data */
	IIC_INIT_ERROR,   /*!< an error occurred, while initializing the iic-ports */
	IIC_DATA_ERROR    /*!< the received data from an iic-sensor is logical invalid */
} iic_error_e;  /*!< Enum_type to describe the occurred iic-error */

iic_error_e iic_interface_init(const uint32_t interface_id);
uint32_t iic_send(const uint32_t interface_id, const uint8_t iic_address, uint8_t *send_buffer, uint32_t byte_count);
uint32_t iic_recv(const uint32_t interface_id, const uint8_t iic_address, uint8_t *recv_buffer, uint32_t byte_count);

#endif /* IIC_LIB_H_ */
