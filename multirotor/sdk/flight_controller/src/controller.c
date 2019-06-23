//**********************************************************************************
//*
//*   Autor: Soeren Schreiner
//* 
//*   controller.c
//*
//**********************************************************************************

#include "controller.h"
#include "remote.h"
#include "attitude.h"
#include "altitude.h"

#include "platform/bram_lib.h"

// Defines for Controller Values
#define P_THETA_PHI 6.5f
#define I_THETA_PHI 0.04f
#define D_THETA_PHI 0.0f
#define S_THETA_PHI 1.3f

#define P_PSI 6.25f
#define I_PSI 0.0f
#define D_PSI 0.0f
#define S_PSI 2.0f

#define I_MAX 100.0f
#define I_MIN -100.0f

#define P_ALT 30.0f
#define I_ALT 0.002f
#define D_ALT 0.0f
#define S_ALT 5.0f

#define I_MAX_ALT 1885.0f
#define I_MIN_ALT 200.0f

//**********************************************************************************

// Defines for Motorvalues
#define MIN_ALT_GAS 200.0
#define MAX_ALT_GAS 1885.0

#define MIN_ATT_GAS -1200.0
#define MAX_ATT_GAS 1200.0

#define MIN_GAS 200
#define MAX_GAS 2047

//**********************************************************************************

#define RCSENSITIVITY_ALT 200
#define RCNORMALIZING_ALT 280.0f
#define RCMAX_PR 25.0f // Maximum set point for Pitch and Roll
#define RCMAX_Y 25.0f // Maximum set point for Yaw
#define RCMAX_T 1885.0f // Maximum set point for Thrust
#define RCMIN_T 200.0f // Minimum set point for Thrust

#define STOP_YAW 250.0f // Stop yawing under this thrust
#define STOP_I 250.0f // Stop I part of attitude controller under this thrust
#define LC_ALTITUDE_SETPOINT -0.5f // If losing connection altitude controller activates with this sink speed

//**********************************************************************************

static controller_t ctrl_values;

static float deviation_theta = 0.0f;
static float deviation_phi = 0.0f;
static float deviation_psi = 0.0f;

static float psi_setpoint = 0.0f;

static uint8_t active_altitude_ctrl = 0;
static float thrust_setpoint = 0.0f;
static float climbing_rate_setpoint = 0.0f;
static float altitude_setpoint = 0.0f;

static float deviation_altitude = 0.0f;

// Temp for integral term of altitude controller
static float altitude_setpoint_old = 0.0f;

//**********************************************************************************
/*
static int16_t motor_front_intern = 0;
static int16_t motor_back_intern = 0;
static int16_t motor_right_intern = 0;
static int16_t motor_left_intern = 0;
*/

static int16_t motor_front = 0; // cw
static int16_t motor_back = 0; // cw
static int16_t motor_right = 0; // ccw
static int16_t motor_left = 0; // ccw

//**********************************************************************************

void controller_attitude(void) {
	// Get setpoint for psi
	static float temp_psi = 0.0f;

	if (remote_get_data().thrust < STOP_YAW) temp_psi = 0.0f;
	else                           			 temp_psi = remote_get_data().yaw;

	if (remote_get_events().motors_on && temp_psi == 0.0f) psi_setpoint = psi_setpoint;
	else										 		   psi_setpoint = temp_psi + attitude_get_angles().psi;

	limit_angle(&psi_setpoint, &psi_setpoint, 180.0f);

	// Calculate new differences between set points and actual values
	deviation_theta = remote_get_data().nick - attitude_get_angles().theta; // attitude_get_angles().theta - remote_get_data().nick;
	deviation_phi = remote_get_data().roll - attitude_get_angles().phi; // attitude_get_angles().phi - remote_get_data().roll;
	deviation_psi = psi_setpoint - attitude_get_angles().psi;

	// Limit differences
	limit_angle(&deviation_theta, &deviation_theta, 90.0f);
	limit_angle(&deviation_phi, &deviation_phi, 180.0f);
	limit_angle(&deviation_psi, &deviation_psi, 180.0f);

	bram_pl_data_write_float(DEVIATION_THETA_ATTITUDE, deviation_theta);
	bram_pl_data_write_float(DEVIATION_PHI_ATTITUDE, deviation_phi);
	bram_pl_data_write_float(DEVIATION_PSI_ATTITUDE, deviation_psi);
	//**********************************************************************************
	// For Theta
	propotional_term(&deviation_theta, P_THETA_PHI, &ctrl_values.theta.p);
	integral_term(&deviation_theta, remote_get_events().motors_on, I_THETA_PHI, &ctrl_values.theta.i);

	if (remote_get_data().thrust < STOP_I)
		ctrl_values.theta.i = 0.0f;

	min_max_value(&ctrl_values.theta.i, &ctrl_values.theta.i, I_MIN, I_MAX);
	damping_term(attitude_get_angle_velocities().theta, S_THETA_PHI, &ctrl_values.theta.s);
	ctrl_values.theta.sum = ctrl_values.theta.p + ctrl_values.theta.i + ctrl_values.theta.d - ctrl_values.theta.s;
	//**********************************************************************************
	// For Phi
	propotional_term(&deviation_phi, P_THETA_PHI, &ctrl_values.phi.p);
	integral_term(&deviation_phi, remote_get_events().motors_on, I_THETA_PHI, &ctrl_values.phi.i);

	if (remote_get_data().thrust < STOP_I)
		ctrl_values.phi.i = 0.0f;

	min_max_value(&ctrl_values.phi.i, &ctrl_values.phi.i, I_MIN, I_MAX);
	damping_term(attitude_get_angle_velocities().phi, S_THETA_PHI, &ctrl_values.phi.s);
	ctrl_values.phi.sum = ctrl_values.phi.p + ctrl_values.phi.i + ctrl_values.phi.d - ctrl_values.phi.s;
	//**********************************************************************************
	// For Psi
	propotional_term(&deviation_psi, P_PSI, &ctrl_values.psi.p);
	//integralTerm(&deviationPsi, &rcEventList.motorsOn, I_PSI, &ctrlValues.psi.i);
	//minMaxValue(&ctrlValues.psi.i, &ctrlValues.psi.i, I_MIN, I_MAX);
	damping_term(attitude_get_angle_velocities().psi, S_PSI, &ctrl_values.psi.s);
	ctrl_values.psi.sum = ctrl_values.psi.p + ctrl_values.psi.i + ctrl_values.psi.d - ctrl_values.psi.s;

	bram_pl_data_write_float(CV_P_THETA, ctrl_values.theta.p);
	bram_pl_data_write_float(CV_I_THETA, ctrl_values.theta.i);
	bram_pl_data_write_float(CV_S_THETA, ctrl_values.theta.s);
	bram_pl_data_write_float(CV_SUM_THETA, ctrl_values.theta.sum);

	bram_pl_data_write_float(CV_P_PHI, ctrl_values.phi.p);
	bram_pl_data_write_float(CV_I_PHI, ctrl_values.phi.i);
	bram_pl_data_write_float(CV_S_PHI, ctrl_values.phi.s);
	bram_pl_data_write_float(CV_SUM_PHI, ctrl_values.phi.sum);

	bram_pl_data_write_float(CV_P_PSI, ctrl_values.psi.p);
	bram_pl_data_write_float(CV_I_PSI, ctrl_values.psi.i);
	bram_pl_data_write_float(CV_S_PSI, ctrl_values.psi.s);
	bram_pl_data_write_float(CV_SUM_PSI, ctrl_values.psi.sum);
}

//**********************************************************************************

void controller_altitude(void) {
	float tmp_thrust = remote_get_data().thrust;
	// Requirements for activating altitude controller
	active_altitude_ctrl = remote_get_events().altitude_control;
	// Get offset for thrust stick position
	get_offset_thrust_position(tmp_thrust, &active_altitude_ctrl, &thrust_setpoint, &climbing_rate_setpoint);
	// Get setpoint for altitude
	get_altitude_setpoint(&climbing_rate_setpoint, altitude_get_data().height, &active_altitude_ctrl, &altitude_setpoint);

	// Calculate new differences between set points and actual values
	deviation_altitude = altitude_setpoint - altitude_get_data().height;

	bram_pl_data_write_float(DEVIATION_ALTITUDE, deviation_altitude);

	// For Altitude
	propotional_term(&deviation_altitude, P_ALT, &ctrl_values.altitude.p);
	integral_term_alt(&deviation_altitude, tmp_thrust, &altitude_setpoint, &altitude_setpoint_old, &active_altitude_ctrl, I_ALT, &ctrl_values.altitude.i);
	min_max_value(&ctrl_values.altitude.i, &ctrl_values.altitude.i, I_MIN_ALT, I_MAX_ALT);
	//derivativeTerm(&deviationAltitude, &deviationAltitudeOld, D_ALT, &ctrlValues.altitude.d);
	damping_term(altitude_get_data().speed, S_ALT, &ctrl_values.altitude.s);

	if (active_altitude_ctrl)
	{
		ctrl_values.altitude.sum = ctrl_values.altitude.p + ctrl_values.altitude.i + ctrl_values.altitude.d - ctrl_values.altitude.s;
	}
	else
	{
		ctrl_values.altitude.sum = remote_get_data().thrust;
	}

	bram_pl_data_write_float(CV_P_ALTITUDE, ctrl_values.altitude.p);
	bram_pl_data_write_float(CV_I_ALTITUDE, ctrl_values.altitude.i);
	bram_pl_data_write_float(CV_S_ALTITUDE, ctrl_values.altitude.s);
	bram_pl_data_write_float(CV_SUM_ALTITUDE, ctrl_values.altitude.sum);
}

//**********************************************************************************

void calculate_motor_values(void) {
	static uint16_t motor_values[4];
	static motor_states_e state = UNDEFINED;

	float thrust = ctrl_values.altitude.sum;
	min_max_value(&thrust, &thrust, MIN_ALT_GAS, MAX_ALT_GAS);

	float psi = ctrl_values.psi.sum;
	min_max_value(&psi, &psi, MIN_ATT_GAS, MAX_ATT_GAS);

	float theta = ctrl_values.theta.sum;
	min_max_value(&theta, &theta, MIN_ATT_GAS, MAX_ATT_GAS);

	float phi = ctrl_values.phi.sum;
	min_max_value(&phi, &phi, MIN_ATT_GAS, MAX_ATT_GAS);

	motor_front = (int16_t) (thrust + theta - psi);
	motorLimitation(&motor_front, &motor_front, MIN_GAS, MAX_GAS);
	motor_back = (int16_t) (thrust - theta - psi);
	motorLimitation(&motor_back, &motor_back, MIN_GAS, MAX_GAS);
	motor_right = (int16_t) (thrust - phi + psi);
	motorLimitation(&motor_right, &motor_right, MIN_GAS, MAX_GAS);
	motor_left = (int16_t) (thrust + phi + psi);
	motorLimitation(&motor_left, &motor_left, MIN_GAS, MAX_GAS);

	/* check rc button state --> flight mode */
	switch (state) {
	case UNDEFINED:
		// all motors zero
		motor_values[1] = 0.0f;
		motor_values[0] = 0.0f;
		motor_values[3] = 0.0f;
		motor_values[2] = 0.0f;
		// wait for startup position of the rc
		if (!remote_get_events().motors_on && remote_get_data().thrust <= MIN_GAS)
		{ // toDo:  <-- defines
			// all motors zero
			state = M_OFF_TH_MIN_ST;
		}
		break;

	case M_OFF_TH_MIN_ST:

		// all motors zero
		motor_values[1] = 0.0f;
		motor_values[0] = 0.0f;
		motor_values[3] = 0.0f;
		motor_values[2] = 0.0f;
		// Motors are on and thrust is there switch state
		if (remote_get_events().motors_on && remote_get_data().thrust <= MIN_GAS)
		{
			// set min motor values
			state = M_ON_TH_MIN_ST;
		}

		break;
	case M_ON_TH_MIN_ST:

		// all motors min values
		motor_values[1] = MIN_GAS;
		motor_values[0] = MIN_GAS;
		motor_values[3] = MIN_GAS;
		motor_values[2] = MIN_GAS;
		// motors set off.... switch to M_OFF_TH_MIN_ST
		if (!remote_get_events().motors_on) {
			// all motor values set to zero
			state = M_OFF_TH_MIN_ST;
		}	// thrust rise switch to M_ON_TH_NORM
		else if (remote_get_data().thrust > MIN_GAS) {
			// calc motor values
			state = M_ON_TH_NORM;
		}
		break;

	case M_ON_TH_NORM:
		// normal motor values
		motor_values[1] = motor_front;
		motor_values[0] = motor_back;
		motor_values[3] = motor_right;
		motor_values[2] = motor_left;
		// if motors set to off and thrust is less then the minimum.... switch to M_OFF_TH_MIN_ST
		if (!remote_get_events().motors_on && remote_get_data().thrust <= MIN_GAS)
		{
			// all motor values set to zero
			state = M_OFF_TH_MIN_ST;
		} // of motors off and thrust < MIN_THRUST... switch to M_ON_TH_MIN_ST
		else if (remote_get_events().motors_on && remote_get_data().thrust <= MIN_GAS)
		{
			// doing this to disable the gier-move while staying on the ground
			// state = M_ON_TH_MIN_ST;
		}
		break;
	default:
		break;
		//
	}
	// copy motor values to BRAMs
	int i = MOTOR_REAR_VALUE_ADDR;
	bram_pl_data_write_uint(MOTOR_RIGHT_VALUE_ADDR, (uint32_t)motor_values[3]);
	bram_pl_data_write_uint(MOTOR_REAR_VALUE_ADDR, (uint32_t)motor_values[0]);
	bram_pl_data_write_uint(MOTOR_FRONT_VALUE_ADDR, (uint32_t)motor_values[1]);
	bram_pl_data_write_uint(MOTOR_LEFT_VALUE_ADDR, (uint32_t)motor_values[2]);
}

//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************

void limit_angle(float *input, float *output, float range) {
	if (*input >= range)
		*output = *input - 2 * range;
	else if (*input < -range)
		*output = *input + 2 * range;
	else
		*output = *input;
}

//**********************************************************************************

void min_max_value(float *input, float *output, float min, float max)
{
	if (*input > max)
		*output = max;
	else if (*input < min)
		*output = min;
	else
		*output = *input;
}

//**********************************************************************************

void propotional_term(float *input, float influence, float *output)
{
	*output = *input * influence;
}

//**********************************************************************************

void integral_term(float *input, uint8_t motors_on, float influence, float *output)
{
	float temp;
	if (motors_on)
		temp = (*input * influence) + *output;
	else
		temp = 0.0f;

	*output = temp;
}

//**********************************************************************************

void derivative_term(float *input, float *inputOld, float influence, float *output)
{
	*output = (*input - *inputOld) * influence;
}

//**********************************************************************************

void damping_term(float input, float influence, float *output)
{
	*output = input * influence;
}

//**********************************************************************************

void rcAdjustingFloat(float* input, float* output, float sensitivity) {
	if (*input > sensitivity)
		*output = *input - sensitivity;
	else if (*input < -sensitivity)
		*output = *input + sensitivity;
	else
		*output = 0;
}

//**********************************************************************************

void get_offset_thrust_position(float thrust, uint8_t *active, float *setpoint_thrust, float *setpoint_climbing_rate)
{
	float temp;
	if (*active)
	{
		temp = thrust - *setpoint_thrust;
		rcAdjustingFloat(&temp, &temp, RCSENSITIVITY_ALT);
		temp = temp / RCNORMALIZING_ALT;
	}
	else
	{
		temp = 0.0;
		*setpoint_thrust = thrust;
	}
	*setpoint_climbing_rate = temp;
}

//**********************************************************************************

void get_altitude_setpoint(float *setpoint_climbing_rate, float height, uint8_t *active, float *setpoint_altitude)
{
	if (*setpoint_climbing_rate == 0.0f && *active)
		*setpoint_altitude = *setpoint_altitude;
	else
		*setpoint_altitude = height + *setpoint_climbing_rate;
}

//**********************************************************************************

void integral_term_alt(float *input, float thrust, float *setpoint_altitude, float *setpoint_altitude_old, uint8_t *active, float influence, float *output)
{
	float temp;
	if (*active)
	{
		if (*setpoint_altitude != *setpoint_altitude_old)
			temp = *output;
		else
			temp = (*input * influence) + *output;
	}
	else
	{
		temp = thrust;
	}
	*output = temp;
	*setpoint_altitude_old = *setpoint_altitude;
}

//**********************************************************************************

void motorLimitation(int16_t* input, int16_t* output, int16_t min, int16_t max) {
	if (*input > max)
		*output = max;
	else if (*input < min)
		*output = min;
	else
		*output = *input;
}

//**********************************************************************************
