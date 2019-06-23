/*
 * gpio_lib.c
 *
 *  Created on: Aug 5, 2015
 *      Author: sschreiner
 */

#include "gpio_lib.h"

#include <xil_io.h>
#include <xstatus.h>
#include <xgpio.h>
#include <xparameters.h>

#define CHANNEL	0x01	/*!< the channel of the GPIO IP Core */

static uint16_t led_device_id = XPAR_SENSOR_ACTOR_AXI_GPIO_DEBUG_LEDS_SA_DEVICE_ID;

static XGpio	led;			/*!< instance of the led ip core */

// glb GPIO Masks
static uint32_t glb_mask_gpio[SIZEOF_GPIO_NR] = {0x00, 0x01}; /*!< allocation between the bits and the gpio pin */

// current GPIO/led status
static uint32_t glb_led_mask = 0;		/*!< masks to set the led */

/*!
 * These function initialize gpio and the led ip core
 *
 * @param[in]		uint16_t	gpioDeviceId	The device Id from the GPIO IP Core. Get the value form xparameter.h
 * @param[in]		uint16_t	ledDeviceId		The device Id from the LED IP Core. Get the value form xparameter.h
 * @param[in]		uint16_t periphDeviceId	The device Id from the PERIPHERAL IP Core. Get the value form xparameter.h
 *
 * @return		gpio_error_t	GPIO_NO_ERROR 				if everything ok
 * @return		gpio_error_t	GPIO_ERR_INITIALISATION		if an error while initalization occurs
 *
 * @info		nothing
 */
gpio_error_e gpio_init(void)
{
	gpio_error_e status = GPIO_NO_ERROR;
	int state;

	// Init GPIO Ip Core
	state = XGpio_Initialize(&led, led_device_id);
	if(state == XST_FAILURE)
	{
		status = GPIO_INIT_ERROR;
	}

	//selftest LED
	state = XGpio_SelfTest(&led);
	if(state == XST_FAILURE)
	{
		status = GPIO_INIT_ERROR;
	}

	// set data direction ... 0 all OUTPUT
	XGpio_SetDataDirection(&led,CHANNEL, 0);

	XGpio_DiscreteWrite(&led,CHANNEL,glb_led_mask);
	XGpio_DiscreteWrite(&led,CHANNEL,glb_led_mask);

	return status;
}

/*!
 * These function set the gpio and or the LEDs to HIGH(ON) or LOW(OFF)
 *
 * @param[in]		gpio_num_t	gpioName	The to set GPIO
 * @param[in]		uint8_t			toggle		The value of the gpio HIGH(ON) or LOW(OFF)
 *
 * @return		gpio_error_t	GPIO_NO_ERROR 		if everything ok
 * @return		gpio_error_t	GPIO_ERR_SET		if the set failed
 *
 * @info		nothing
 */
gpio_error_e gpio_set(gpio_num_e gpio_name, uint8_t toggle)
{
	gpio_error_e status = GPIO_NO_ERROR;

	// check if an led or an gpio or an Peripheral has to set
	if(gpio_name == LED_1 || gpio_name == LED_2)
	{
		// LED
		// check ON	OFF
		if(toggle == ON)
		{
			glb_led_mask |= (ON << glb_mask_gpio[gpio_name]);
		}
		else
		{
			glb_led_mask &= ~(ON<<glb_mask_gpio[gpio_name]);
		}
		XGpio_DiscreteWrite(&led, CHANNEL, glb_led_mask);
	}
	else
	{
		// No GPIO or LED known
		status = GPIO_SET_ERROR;
	}

	return status;
}
