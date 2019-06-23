/*
 * gpio_lib.h
 *
 *  Created on: Aug 5, 2015
 *      Author: sschreiner
 */

#ifndef GPIO_LIB_H_
#define GPIO_LIB_H_

#include <stdint.h>

/*
 * defines for better readability
 */
#define OFF		0x00	/*!< to disable the LED */
#define ON 		0x01	/*!< to enable the LED */

#define LOW		0x00	/*!< to set an GPIO LOW */
#define HIGH	0x01	/*!< to set an GPIO HIGH */

/*!
 * Lib. error status
 */
typedef enum{
	GPIO_NO_ERROR,
	GPIO_INIT_ERROR,
	GPIO_SET_ERROR
} gpio_error_e;

/*!
 * some enums for easier usage of the GPIOs
 */
typedef enum{

	LED_1 = 0,		 /*!< debug led 1 */
	LED_2,
	SIZEOF_GPIO_NR	 /*!< Sum of the GPIOs */
} gpio_num_e;

gpio_error_e gpio_init(void);
gpio_error_e gpio_set(gpio_num_e gpio_name, uint8_t toggle);

#endif /* GPIO_LIB_H_ */
