/*
 * uart_lib.h
 *
 *  Created on: Jul 22, 2015
 *      Author: sschreiner
 */

#ifndef UART_LIB_H_
#define UART_LIB_H_

#include <stdint.h>

#define UART_INTERFACE_ID_0 0

/*
 * struct to describe the uart_error type
 */
typedef enum{
	UART_NO_ERROR,   /*!< no uart-Error */
	UART_RECV_ERROR, /*!< an error occurred while receiving data */
	UART_SEND_ERROR, /*!< an error occurred while sending data */
	UART_INIT_ERROR,   /*!< an error occurred, while initializing the uart-ports */
	UART_DATA_ERROR    /*!< the received data from an uart-sensor is logical invalid */
} uart_error_e;  /*!< Enum_type to describe the occurred uart-error */

uart_error_e uart_interface_init(void);
void uart_send(uint8_t data);
uint8_t uart_is_receive_empty(void);
uint16_t uart_recv(uint8_t *buffer, uint16_t byte_count);
void uart_reset_fifos(void);

#endif /* UART_LIB_H_ */
