/*
 * remote.c
 *
 *  Created on: Jul 10, 2015
 *      Author: sschreiner
 */

#include "remote.h"
#include "platform/ppm_rc.h"
#include "platform/bram_lib.h"

//Maximum Values for calculation of RemoteControl
#define MAX_V_PHI 25.0f				// maximale Giergeschwindigkeit in degrees/s
#define MAX_ANGLE_THETA_PHI 25.0f	// maximaler Nick- / Rollwinkel in degree
#define MIN_THRUST 200.0f			// minimaler Schub
#define MAX_THRUST 1885.0f			// maximaler Schub

static remote_data_s remote_data;
static remote_events_s events;

void remote_from_bram(void)
{
	static ppm_channel_s ppm_channel[PPM_CHANNEL_SIZE_OF];			/*!< the status of all ppm channels*/

	bram_pl_data_read_ppm_channel(RAW_RC_THRUST_ADDR,					&ppm_channel[PPM_CHANNEL_THRUST]);
	bram_pl_data_read_ppm_channel(RAW_RC_YAW_ADDR,						&ppm_channel[PPM_CHANNEL_YAW]);
	bram_pl_data_read_ppm_channel(RAW_RC_ROLL_ADDR,						&ppm_channel[PPM_CHANNEL_NICK]);
	bram_pl_data_read_ppm_channel(RAW_RC_NICK_ADDR,						&ppm_channel[PPM_CHANNEL_ROLL]);
	bram_pl_data_read_ppm_channel(RAW_RC_BUTTON_MOTOR_ON_ADDR,			&ppm_channel[PPM_CHANNEL_SW_MOTOR]);
	bram_pl_data_read_ppm_channel(RAW_RC_BUTTON_HOLD_ALTITUDE_ADDR,		&ppm_channel[PPM_CHANNEL_SW_HOLD_ALTITUDE]);
	bram_pl_data_read_ppm_channel(RAW_RC_BUTTON_CAM_ADDR,				&ppm_channel[PPM_CHANNEL_SW_CAMERA]);
	bram_pl_data_read_ppm_channel(RAW_RC_BUTTON_OBJECTDETECTION_ADDR,	&ppm_channel[PPM_CHANNEL_SW_OBJECT_DETECTION]);

	// copy time stamp of data from PL bram
	uint32_t time_stamp = 0;
	bram_pl_data_read_uint(RAW_RC_TIME_STAMP_ADDR,	&time_stamp);

	// Encode Thrust
	remote_data.thrust = MIN_THRUST + (((MAX_THRUST - MIN_THRUST) * (float)ppm_channel[PPM_CHANNEL_THRUST].value) / 100.0f);

	// Encode Yaw
	if (ppm_channel[PPM_CHANNEL_YAW].direction == PPM_CHANNEL_DIR_LEFT)
		remote_data.yaw = -(((float)ppm_channel[PPM_CHANNEL_YAW].value * MAX_V_PHI) / 100.0f) ;
	else if (ppm_channel[PPM_CHANNEL_YAW].direction == PPM_CHANNEL_DIR_RIGHT)
		remote_data.yaw = ((float)ppm_channel[PPM_CHANNEL_YAW].value * MAX_V_PHI) / 100.0f;
	else
		remote_data.yaw = 0.0f;

	// Encode Nick
	if (ppm_channel[PPM_CHANNEL_NICK].direction == PPM_CHANNEL_DIR_FRONT)
		remote_data.nick = -(((float)ppm_channel[PPM_CHANNEL_NICK].value * MAX_ANGLE_THETA_PHI) / 100.0f);
	else if (ppm_channel[PPM_CHANNEL_NICK].direction == PPM_CHANNEL_DIR_REAR)
		remote_data.nick = ((float)ppm_channel[PPM_CHANNEL_NICK].value * MAX_ANGLE_THETA_PHI) / 100.0f;
	else
		remote_data.nick = 0.0f;

	// Encode Roll
	if (ppm_channel[PPM_CHANNEL_ROLL].direction == PPM_CHANNEL_DIR_LEFT)
		remote_data.roll = -(((float)ppm_channel[PPM_CHANNEL_ROLL].value * MAX_ANGLE_THETA_PHI) / 100.0f);
	else if (ppm_channel[PPM_CHANNEL_ROLL].direction == PPM_CHANNEL_DIR_RIGHT)
		remote_data.roll = ((float)ppm_channel[PPM_CHANNEL_ROLL].value * MAX_ANGLE_THETA_PHI) / 100.0f;
	else
		remote_data.roll = 0.0f;

	// Encode Motor Switch
	if(ppm_channel[PPM_CHANNEL_SW_MOTOR].value == 1)
		remote_data.motor_sw = 1;
	else
		remote_data.motor_sw = 0;

	// Encode Altitude Switch
	if(ppm_channel[PPM_CHANNEL_SW_HOLD_ALTITUDE].value == 1)
		remote_data.altitude_sw = 1;
	else
		remote_data.altitude_sw = 0;

	// Encode Altitude Switch
	if(ppm_channel[PPM_CHANNEL_SW_CAMERA].value == 1)
		remote_data.camera_sw = 1;
	else
		remote_data.camera_sw = 0;

	// Encode Altitude Switch
	if(ppm_channel[PPM_CHANNEL_SW_OBJECT_DETECTION].value == 1)
		remote_data.obj_detect_sw = 1;
	else
		remote_data.obj_detect_sw = 0;

	bram_pl_data_write_float(FINAL_RC_THRUST_ADDR, 	remote_data.thrust);
	bram_pl_data_write_float(FINAL_RC_YAW_ADDR, 	remote_data.yaw);
	bram_pl_data_write_float(FINAL_RC_NICK_ADDR, 	remote_data.nick);
	bram_pl_data_write_float(FINAL_RC_ROLL_ADDR, 	remote_data.roll);
	bram_pl_data_write_uint(FINAL_RC_BUTTON_MOTOR_ON_ADDR, 		remote_data.motor_sw);
	bram_pl_data_write_uint(FINAL_RC_BUTTON_HOLD_ALTITUDE_ADDR, remote_data.altitude_sw);
	bram_pl_data_write_uint(FINAL_RC_BUTTON_CAM_ADDR, 			remote_data.camera_sw);
	bram_pl_data_write_uint(FINAL_RC_BUTTON_OBJECTDETECTION_ADDR, remote_data.obj_detect_sw);
}

void remote_check_events(void)
{
	// Do nothing if thrust stick is in lowest position
	if (remote_data.thrust <= MIN_THRUST)
	{
		remote_data.yaw = 0.0f;
		remote_data.nick = 0.0f;
		remote_data.roll = 0.0f;
	}

	// Motors On event
	if (remote_data.thrust <= MIN_THRUST && remote_data.motor_sw == 1)
		events.motors_on = 1;
	else if(remote_data.motor_sw == 0)
		events.motors_on = 0;

	// Hold altitude
	if (remote_data.altitude_sw == 1 && events.motors_on == 1)
		events.altitude_control = 1;
	else
		events.altitude_control = 0;

	// Camera On event
	if (remote_data.camera_sw == 1)
		events.camera_on = 1;
	else
		events.camera_on = 0;

	// Object detection event
	if (remote_data.obj_detect_sw == 1 && events.camera_on == 1)
		events.object_detection_on = 1;
	else
		events.object_detection_on = 0;
}

remote_data_s remote_get_data(void)
{
	return remote_data;
}

remote_events_s remote_get_events(void)
{
	return events;
}
