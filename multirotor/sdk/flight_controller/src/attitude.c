/*
 * attitude.c
 *
 *  Created on: Aug 11, 2015
 *      Author: sschreiner
 */

#include "attitude.h"
#include "sensors.h"
#include <math.h>
#include "platform/bram_lib.h"
//#include "compass.h"

#define TIME_PER_STEP		0.002F
#define M_PIF				3.1415926F

static sin_cos_s sin_cos_values;

static acc_angles_s acc_angles; // angles calculated via the acceleration sensor

static angles_s angle_steps; // discrete changes of angles for every calculation step
static angles_s angles; // angles in degrees
static angles_s velocities; // angle velocities in degrees per second

static void calculate_sin_cos_angle_values(void);
static void calculate_acc_angles(void);
static void calculate_discrete_gyro_changes(void);
static void calculate_euler_transformation(void);
static void calculate_angle_integration(void);
static void calculate_kalman_filter(void);

static void limit_val(float *val, float min, float max);
static void limit_angle(float *angle, float range);
static void kalman_filter_angles(float *ref, float *value, float influence, float range);

void attitude_calculation(void)
{
	calculate_sin_cos_angle_values();
	calculate_acc_angles();
	calculate_discrete_gyro_changes();
	calculate_euler_transformation();
	calculate_angle_integration();
	calculate_kalman_filter();
}

angles_s attitude_get_angles(void)
{
	return angles;
}

angles_s attitude_get_angle_velocities(void)
{
	return velocities;
}

sin_cos_s attitude_get_sin_cos_values(void)
{
	return sin_cos_values;
}

void attitude_to_bram(void)
{
	bram_pl_data_write_float(ANGLE_THETA_ADDR, angles.theta);
	bram_pl_data_write_float(ANGLE_PHI_ADDR, angles.phi);
	bram_pl_data_write_float(ANGLE_PSI_ADDR, angles.psi);
	bram_pl_data_write_float(ANGLE_VEL_THETA_ADDR, velocities.theta);
	bram_pl_data_write_float(ANGLE_VEL_PHI_ADDR, velocities.phi);
	bram_pl_data_write_float(ANGLE_VEL_PSI_ADDR, velocities.psi);
	bram_pl_data_write_float(ANGLE_ACC_THETA_ADDR, acc_angles.theta);
	bram_pl_data_write_float(ANGLE_ACC_PHI_ADDR, acc_angles.phi);
	bram_pl_data_write_uint(ATTITUDE_VALID_ADDR, 1);
//	bram_pl_data_write_float(COMPASS_BEARING_ADDR, compass_get_data());
}

void calculate_sin_cos_angle_values(void)
{
	sin_cos_values.theta_sin = sinf((M_PIF / 180.0f) * angles.theta);
	sin_cos_values.theta_cos = cosf((M_PIF / 180.0f) * angles.theta);

	sin_cos_values.phi_sin = sinf((M_PIF / 180.0f) * angles.phi);
	sin_cos_values.phi_cos = cosf((M_PIF / 180.0f) * angles.phi);

	sin_cos_values.psi_sin = sinf((M_PIF / 180.0f) * angles.psi);
	sin_cos_values.psi_cos = cosf((M_PIF / 180.0f) * angles.psi);
}

void calculate_acc_angles(void)
{
	float acc_x = sensors_get_data().mpu9150_data.acc_x;
	float acc_y = sensors_get_data().mpu9150_data.acc_y;
	float acc_z = sensors_get_data().mpu9150_data.acc_z;

	limit_val(&acc_x, -1.0f, 1.0f);
	limit_val(&acc_y, -1.0f, 1.0f);
	limit_val(&acc_z, -1.0f, 1.0f);

	// Angle theta
	float tmp_theta = asinf(acc_x);
	acc_angles.theta = tmp_theta * (180.0f / M_PIF);

	// Angle phi
	float tmp = cosf(tmp_theta);
	if (tmp != 0.0f)
	{
		tmp = acc_y / tmp;
	}
	else
	{
		if (acc_y > 0) 			tmp = 1;
		else if (acc_y <= 0) 	tmp = -1;
	}
	limit_val(&tmp, -1.0f, 1.0f);
	tmp = asinf(tmp) * (180.0f / M_PIF);

	if (acc_z < 0.0f) // TODO: check if correct!
	{
		if (tmp > 0.0f)
		{
			acc_angles.phi = 180.0f - tmp; // TODO: check if correct!
		}
		else
		{
			acc_angles.phi = -180.0f - tmp; // TODO: check if correct!
		}
	}
	else
	{
		acc_angles.phi = tmp;
	}
}

void calculate_discrete_gyro_changes(void)
{
	float gyro_x = sensors_get_data().mpu9150_data.gyro_x;
	float gyro_y = sensors_get_data().mpu9150_data.gyro_y;
	float gyro_z = sensors_get_data().mpu9150_data.gyro_z;

	velocities.theta = gyro_x;
	velocities.phi = gyro_y;
	velocities.psi = gyro_z;

	angle_steps.theta = gyro_x * TIME_PER_STEP;
	angle_steps.phi = gyro_y * TIME_PER_STEP;
	angle_steps.psi = gyro_z * TIME_PER_STEP;
}

void calculate_euler_transformation(void)
{
	float tmp_angle_step_theta = angle_steps.theta;
	float tmp_angle_step_phi   = angle_steps.phi;
	float tmp_angle_step_psi   = angle_steps.psi;

	float tmp = tmp_angle_step_theta * sin_cos_values.phi_sin + tmp_angle_step_psi * sin_cos_values.phi_cos;

	if (sin_cos_values.theta_cos != 0)
		angle_steps.phi = tmp_angle_step_phi + tmp * sin_cos_values.theta_sin / sin_cos_values.theta_cos;

	angle_steps.theta = tmp_angle_step_theta * sin_cos_values.phi_cos - tmp_angle_step_psi * sin_cos_values.phi_sin;

	if (sin_cos_values.theta_cos != 0)
		angle_steps.psi = tmp / sin_cos_values.theta_cos;
}

void calculate_angle_integration(void)
{
	angles.theta += angle_steps.theta;
	angles.phi += angle_steps.phi;
	angles.psi += angle_steps.psi;

	limit_angle(&angles.theta, 90.0f);
	limit_angle(&angles.phi, 180.0f);
	limit_angle(&angles.psi, 180.0f);
}

void calculate_kalman_filter(void)
{   // Theta
    float influence_theta = 0.001f - fabsf(angles.theta) * 0.00001f;
    kalman_filter_angles(&acc_angles.theta, &angles.theta, influence_theta, 90.0f);
    // Phi
    float influence_phi = 0.001f - fabsf(angles.phi) * 0.00001f;
    kalman_filter_angles(&acc_angles.phi, &angles.phi, influence_phi, 180.0f);
    // Psi
    float influence_psi = 0.002f - (fabsf(angles.theta) + fabsf(angles.phi)) * 0.00001f;
//    kalman_filter_angles(&sensorData.compass, &angles.psi, influence_psi, 180.0f); // TODO: add compass value here!
}

void limit_val(float *val, float min, float max)
{
	if (*val < min) 		*val = min;
	else if (*val > max) 	*val = max;
}

void limit_angle(float *angle, float range)
{
  if(*angle >= range)      *angle -= 2*range;
  else if(*angle < -range) *angle += 2*range;
}

void kalman_filter_angles(float *ref, float *value, float influence, float range)
{
  float difference = *ref - *value;
  limit_angle(&difference, range);
  difference = difference * influence;
  *value = difference + *value;
}
