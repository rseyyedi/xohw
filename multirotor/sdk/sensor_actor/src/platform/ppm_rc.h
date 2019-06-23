/*
 * ppm_rc.h
 *
 *  Created on: Aug 7, 2015
 *      Author: sschreiner
 */

#ifndef PPM_RC_H_
#define PPM_RC_H_

#include <stdint.h>

/*!
 * The association between the ppm channel names and the number in the PPM signal
 */
typedef enum {
	PPM_CHANNEL_NICK = 0,					/*<! RC left leaver up down */
	PPM_CHANNEL_YAW = 1,					/*<! RC right leaver left right */
	PPM_CHANNEL_ROLL = 2,					/*<! RC left leaver left right */
	PPM_CHANNEL_SW_HOLD_ALTITUDE = 3,		/*<! RC SW9 */
	PPM_CHANNEL_THRUST = 4,					/*<! RC right leaver up down */
	PPM_CHANNEL_SW_CAMERA = 5,				/*<! RC SW5/6 */
	PPM_CHANNEL_SW_MOTOR = 6,	/*<! RC SW5/6 */
	PPM_CHANNEL_SW_OBJECT_DETECTION = 7,				/*<! RC SW1 */
	PPM_CHANNEL_SIZE_OF
} ppm_channel_names_e;

/*!
 * PPM Channel types
 */
typedef enum {
	TWO_STATE_SW,
	THREE_STATE_SW,
	LEAVER
} ppm_channel_type_e;

/*!
 * PPM Channel directions
 */
typedef enum {
	PPM_CHANNEL_DIR_LEFT,
	PPM_CHANNEL_DIR_RIGHT,
	PPM_CHANNEL_DIR_FRONT,
	PPM_CHANNEL_DIR_REAR,
	PPM_CHANNEL_DIR_UNDEF
} ppm_channel_dir_e;

/*!
 * data structure for each ppm channel.
 */
typedef struct {
	ppm_channel_type_e type; 		/*!< the type of the channel */
	uint8_t value;						/*!< the value of the channel in percent (leaver) or TRRUE FALSE (SW) */
	uint8_t ppm_nr;						/*!< the nr of the channel in the ppm signal 0 - 7 */
	ppm_channel_dir_e direction;	/*!< the direction of the channel. Just used by the leaver Channels otherwise PPM_CHANNEL_DIR_UNDEF */
} ppm_channel_s;


#define NR_OF_PPM_CHANNELS PPM_CHANNEL_SIZE_OF	/*!< Numbers of channels on the ppm signal */

void ppm_init(void);
ppm_channel_s ppm_get_data(ppm_channel_names_e channel);

#endif /* PPM_RC_H_ */
