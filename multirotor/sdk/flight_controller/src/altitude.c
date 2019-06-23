/*
 * altitude.c
 *
 *  Created on: Aug 12, 2015
 *      Author: sschreiner
 */

#include "altitude.h"
#include "attitude.h"
#include "sensors.h"
#include <math.h>
#include "platform/bram_lib.h"

#define TIME_PER_STEP			0.002F
#define TIMER_CYCLES_PER_SEC	500.0F
#define GRAVITY					9.81F // [m per square sec]

static height_s acc_data;
static float baro_height;
static height_s remote_data;

static void calculate_acc_altitude(void);
static void calculate_baro_altitude(void);
static void calculate_kalman_filter_height(void);
static void kalman_filter_height(float *ref, float *value, float *out, float influence);

void altitude_calculation(void)
{
	calculate_acc_altitude();
	calculate_baro_altitude();
	calculate_kalman_filter_height();
}

height_s altitude_get_data(void)
{
	return remote_data;
}

void altitude_to_bram(void)
{
	bram_pl_data_write_float(ALTITUDE_ADDR, remote_data.height);
	bram_pl_data_write_float(CLIMBRATE_ADDR, remote_data.speed);
	bram_pl_data_write_float(ACC_GEO_Z_ADDR, remote_data.acceleration);
	bram_pl_data_write_uint(ALTITUDE_VALID_ADDR, 1);
}

void calculate_acc_altitude(void)
{
	float tmp_acc_x = sensors_get_data().mpu9150_data.acc_x;
	float tmp_acc_y = sensors_get_data().mpu9150_data.acc_y;
	float tmp_acc_z = sensors_get_data().mpu9150_data.acc_z;

	//****Calculate height with acc****
	// Calculate absolute acceleration in geographic z-axis
	float tmp_acc_geo_z = tmp_acc_x * attitude_get_sin_cos_values().theta_sin + (tmp_acc_y * attitude_get_sin_cos_values().phi_sin + tmp_acc_z * attitude_get_sin_cos_values().phi_cos) * attitude_get_sin_cos_values().theta_cos;

	acc_data.acceleration = (tmp_acc_geo_z * GRAVITY) - GRAVITY;
	// Calculate height and vertical speed with acc
	acc_data.speed = (acc_data.acceleration * TIME_PER_STEP) + remote_data.speed;
	acc_data.height = (acc_data.speed * TIME_PER_STEP) + remote_data.height;
}

void calculate_baro_altitude(void)
{
	float tmp_baro_height = sensors_get_data().mpl3115a2_data.height;// / 100.0F * 8.43F;
	baro_height = tmp_baro_height;
}

void calculate_kalman_filter_height(void)
{
	//****Kalman-Filter height****
	kalman_filter_height(&baro_height, &acc_data.height, &remote_data.height, 0.3f);
	static float height_value_old = 0.0f;
    float heightDifference = remote_data.height - height_value_old;
    height_value_old = remote_data.height;
    float tmp_height_speed = heightDifference * TIMER_CYCLES_PER_SEC;
    //****Kalman-Filter speed****
    kalman_filter_height(&tmp_height_speed, &acc_data.speed, &remote_data.speed, 0.01f);

    remote_data.acceleration = acc_data.acceleration;
}

void kalman_filter_height(float *ref, float *value, float *out, float influence)
{
  float difference = *ref - *value;
  difference = difference * influence;
  *out = difference + *value;
}
