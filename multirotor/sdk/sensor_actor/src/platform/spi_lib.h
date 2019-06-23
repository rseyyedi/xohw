/*
 * spi.h
 *
 *  Created on: Jul 20, 2015
 *      Author: sschreiner
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

#define SPI_INTERFACE_ID_0 0

/*
 * struct to describe the spi_error type
 */
typedef enum{
	SPI_NO_ERROR,   /*!< no SPI-Error */
	SPI_RECV_ERROR, /*!< an error occurred while receiving data */
	SPI_SEND_ERROR, /*!< an error occurred while sending data */
	SPI_INIT_ERROR,   /*!< an error occurred, while initializing the spi-ports */
	SPI_SLAVE_SELECT_ERROR,
	SPI_DATA_ERROR    /*!< the received data from an spi-sensor is logical invalid */
} spi_error_e;  /*!< Enum_type to describe the occurred spi-error */

spi_error_e spi_interface_init(const uint32_t interface_id);
spi_error_e spi_recv(const uint8_t slave_id, uint8_t *recv_buffer, uint32_t byte_count);

#endif /* SPI_H_ */
