/*
 * ppm_rc.c
 *
 *  Created on: Aug 7, 2015
 *      Author: sschreiner
 */

#include "ppm_rc.h"
#include <ppm_interpreter.h>
#include <xparameters.h>
#include <xil_io.h>

// some defines for the two state switch
#define LOW_THRESHOLD_TWO_STATE_SW	100

// some defines for the three state switch
#define LOW_THRESHOLD_THREE_STATE_SW	70
#define HIGH_THRESHOLD_THREE_STATE_SW	120

#define MIN_PPM_VALUE			58
#define ZERO_POSTION			100
#define OFFSET_ZERO_POSITION	2
#define MAX_PPM_VALUE			141

// array --> contained the informations over all ppm channels
static ppm_channel_s	glb_ppm_channel[NR_OF_PPM_CHANNELS];

// the baseaddress of the ppm_interpreter ip-core
static const uint32_t ppm_interpreter_baseaddress = XPAR_SENSOR_ACTOR_PPM_INTERPRETER_REMOTE_CONTROL_S00_AXI_BASEADDR;

/***********************************
 * prototyp function
 */
static uint8_t value_to_percent_thrust(uint8_t value);
static uint8_t value_to_percent_left(uint8_t value);
static uint8_t value_to_percent_right(uint8_t value);

/*!
 * Initialize the PPM Lib. The function just set some default values into the channel structure and allocate the channel nr to the functions
 *
 * @param[in]	u32		baseaddress		the baseaddress of the ppm_interpreter IP-Core
 *
 * @return	void
 */
void ppm_init(void)
{
	uint8_t i = 0;

	// set direction and values
	for(i = 0; i < NR_OF_PPM_CHANNELS; i++)
	{
		glb_ppm_channel[i].value = 0;
		glb_ppm_channel[i].direction = PPM_CHANNEL_DIR_UNDEF;
	}

	/****** ppm_nr form 0-7 *********/

	// the Thrust-Channel is ppm channel 2
	glb_ppm_channel[PPM_CHANNEL_THRUST].type = LEAVER;
	glb_ppm_channel[PPM_CHANNEL_THRUST].ppm_nr = 2;

	// the Gier-Channel is ppm channel 1
	glb_ppm_channel[PPM_CHANNEL_YAW].type = LEAVER;
	glb_ppm_channel[PPM_CHANNEL_YAW].ppm_nr = 1;

	// the Nick-Channel is ppm channel 0
	glb_ppm_channel[PPM_CHANNEL_NICK].type = LEAVER;
	glb_ppm_channel[PPM_CHANNEL_NICK].ppm_nr = 0;

	// the Roll-Channel is ppm_channel 3

	glb_ppm_channel[PPM_CHANNEL_ROLL].type = LEAVER;
	glb_ppm_channel[PPM_CHANNEL_ROLL].ppm_nr = 3;

	// the ppm-channel 4 HOLD ALTITUDE SW9
	glb_ppm_channel[PPM_CHANNEL_SW_HOLD_ALTITUDE].type = TWO_STATE_SW;
	glb_ppm_channel[PPM_CHANNEL_SW_HOLD_ALTITUDE].ppm_nr = 4;

	// the ppm-channel 5 KAMERA AND OBJECT DETECTION SW5/6
	glb_ppm_channel[PPM_CHANNEL_SW_CAMERA].type = THREE_STATE_SW;
	glb_ppm_channel[PPM_CHANNEL_SW_CAMERA].ppm_nr = 5;

	// the ppm-channel 5 SW5/6
	// has the same ppm channel nr than the Kamera channel, cause of the three state sw.... just the evaluation of the value will be different
	glb_ppm_channel[PPM_CHANNEL_SW_OBJECT_DETECTION].type = THREE_STATE_SW;
	glb_ppm_channel[PPM_CHANNEL_SW_OBJECT_DETECTION].ppm_nr = 5;

	// the ppm-channel 6 SW1 Motor on off
	glb_ppm_channel[PPM_CHANNEL_SW_MOTOR].type = TWO_STATE_SW;
	glb_ppm_channel[PPM_CHANNEL_SW_MOTOR].ppm_nr = 6;
}

/*!
 * These function returns the information and the value of the channel.
 *
 * @param[in]	ppm_channel_names_e		the name of the requested channel
 *
 * @return 	ppm_channel_s			the information of the channel
 *
 */
ppm_channel_s ppm_get_data(ppm_channel_names_e channel)
{
	// get the value for the ppm channel
	glb_ppm_channel[channel].value = (uint16_t)((Xil_In32(ppm_interpreter_baseaddress + (glb_ppm_channel[channel].ppm_nr*0x08)))/10000);// + (glb_ppm_channel[channel].ppm_nr*0x08))/100)); // value in 10us per bit

	// interpret the value information for the requested channel
	// the value will be changed depended on the channel type
	switch(glb_ppm_channel[channel].type)
	{
	case LEAVER:
		// if the channel is the thrust, nothing to do otherwise set the direction and change the value
		if( channel != PPM_CHANNEL_THRUST )
		{
			// check direction and set the value
			if(glb_ppm_channel[channel].value > ZERO_POSTION + OFFSET_ZERO_POSITION)
			{
				// direction is LEFR or FRONT
				if(channel == PPM_CHANNEL_NICK)
					glb_ppm_channel[channel].direction = PPM_CHANNEL_DIR_FRONT;
				else
					glb_ppm_channel[channel].direction = PPM_CHANNEL_DIR_LEFT;

				// set the percent value
				glb_ppm_channel[channel].value = value_to_percent_left(glb_ppm_channel[channel].value);

			}
			else if(glb_ppm_channel[channel].value < ZERO_POSTION - OFFSET_ZERO_POSITION)
			{
				// direction is RIGHT or REAR
				if(channel == PPM_CHANNEL_NICK)
					glb_ppm_channel[channel].direction = PPM_CHANNEL_DIR_REAR;
				else
					glb_ppm_channel[channel].direction = PPM_CHANNEL_DIR_RIGHT;

				// set the percent value
				glb_ppm_channel[channel].value = value_to_percent_right(glb_ppm_channel[channel].value);
			}
			else
			{
				// value is in OFFSET range, so....
				// set the direction to undef
				glb_ppm_channel[channel].direction = PPM_CHANNEL_DIR_UNDEF;
				// and set the value to zero
				glb_ppm_channel[channel].value = 0;
			}

		}
		else
		{
			// convert to percent
			// thrust channel
			glb_ppm_channel[channel].value = value_to_percent_thrust(glb_ppm_channel[channel].value);
		}

		break;
	case TWO_STATE_SW:
		// set TRUE or FALSE to the value
		if(glb_ppm_channel[channel].value <= LOW_THRESHOLD_TWO_STATE_SW)
		{
				glb_ppm_channel[channel].value = FALSE;
		}
		else
		{
			glb_ppm_channel[channel].value = TRUE;
		}

		break;
	case THREE_STATE_SW:
		// check value
		if(glb_ppm_channel[channel].value <= LOW_THRESHOLD_THREE_STATE_SW)
		{
			// SW5/6 is in initial position so kamera and object detection off
			glb_ppm_channel[PPM_CHANNEL_SW_CAMERA].value = FALSE;
			glb_ppm_channel[PPM_CHANNEL_SW_OBJECT_DETECTION].value = FALSE;
		}
		else if(glb_ppm_channel[channel].value <= HIGH_THRESHOLD_THREE_STATE_SW)
		{
			// SW5/6 is on postion 1... middle postition
			glb_ppm_channel[PPM_CHANNEL_SW_CAMERA].value = TRUE;
			glb_ppm_channel[PPM_CHANNEL_SW_OBJECT_DETECTION].value = FALSE;
		}
		else
		{
			// SW5/6 is position 2... the switch looks to the pilot
			glb_ppm_channel[PPM_CHANNEL_SW_CAMERA].value = TRUE;
			glb_ppm_channel[PPM_CHANNEL_SW_OBJECT_DETECTION].value = TRUE;
		}

		break;
	default:
		//wrong input, nothing to do
		break;
	}

	return glb_ppm_channel[channel];
}

/*!
 * switch the given uint8_t  value to 0 - 100%
 * Just for thurst value form 58 up to 141
 *
 *	@param[in] uint8_t value the measured thurst form 58 - 141
 *
 *	@return uint8_t the thrust in percent
 */
uint8_t value_to_percent_thrust(uint8_t value)
{
	uint8_t result = 0;

	// check if the value is lower than MIN_PPM_VALUE
	if(value < MIN_PPM_VALUE)
	{
		// the min value is MIN_PPM_VALUE
		value = MIN_PPM_VALUE;
	}// check if the value is higher than MAX_PPM_VALUE
	else if(value > MAX_PPM_VALUE)
	{
		// the max value is MIN_PPM_VALUE
		value = MAX_PPM_VALUE;
	}// value is in range
	else
	{
		// nothing to do
	}

	result = (uint8_t)(((uint16_t)(value - MIN_PPM_VALUE) * 100)/(MAX_PPM_VALUE-MIN_PPM_VALUE));

	return result;
}

/*!
 * switch the given uint8_t value to 0 - 100%
 * Just for value form 100 down to 58
 * 58 is 100% and the value 100 is 0%
 *
 *	@param	uint8_t	value	values from the gier and roll leaver from 100 - 58
 *
 *	@return uint8_t	the amplitude in percent
 */
uint8_t value_to_percent_right(uint8_t value)
{
	uint8_t result = 57;

	// check if the value is lower than MIN_PPM_VALUE
	if(value < MIN_PPM_VALUE)
	{
		// the min value is MIN_PPM_VALUE
		value = MIN_PPM_VALUE;
	}// check if the value is higher than ZERO_POSTION
	else if(value > ZERO_POSTION)
	{
		// the max value is ZERO_POSTION
		value = ZERO_POSTION;
	}// value is in range
	else
	{
		// nothing to do
	}

	value = value + OFFSET_ZERO_POSITION;
	result = (uint8_t)(ZERO_POSTION -((((uint16_t)value - MIN_PPM_VALUE) * 100)/(ZERO_POSTION-MIN_PPM_VALUE)));

	return result;
}

/*!
 * switch the given uint8_t value to 0 - 100%
 * Just for value form 100 up to 141
 * 141 is 100% and the value 100 is 0%
 *
 *	@param	uint8_t	value	values from the gier and roll leaver from 100 - 141
 *
 *	@return uint8_t	the amplitude in percent
 */
uint8_t value_to_percent_left(uint8_t value)
{
	uint8_t result = 0;

	// check if the value is lower than ZERO_POSTION
	if(value < ZERO_POSTION)
	{
		// the min value is ZERO_POSTION
		value = ZERO_POSTION;
	}// check if the value is higher than MAX_PPM_VALUE
	else if(value > MAX_PPM_VALUE)
	{
		// the max value is MAX_PPM_VALUE
		value = MAX_PPM_VALUE;
	}// value is in range
	else
	{
		// nothing to do
	}

	result = (uint8_t)(((uint16_t)(value - ZERO_POSTION - OFFSET_ZERO_POSITION) * 100)/(MAX_PPM_VALUE-ZERO_POSTION));

	return result;
}
