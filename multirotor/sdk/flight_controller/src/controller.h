//**********************************************************************************
//*
//*   Autor: Soeren Schreiner
//* 
//*   flight.h
//*
//**********************************************************************************

#ifndef _FLIGHT_H
#define _FLIGHT_H

//**********************************************************************************

#include <stdint.h>

typedef struct {
  float p;
  float i;
  float d;
  float s;
  float sum;
} controllerparts_t;

typedef struct {
  controllerparts_t theta;
  controllerparts_t phi;
  controllerparts_t psi;
  controllerparts_t altitude;
} controller_t;

//**********************************************************************************

/*!
 * Enums for the motor state maschine
 */
typedef enum{
	M_OFF_TH_MIN_ST = 0,	/*!< Motors off and rc thrust is min value in the startup phase */
	M_ON_TH_MIN_ST,			/*!< Motors on and rc thrust is min value in the startup phase */
	M_OFF_TH_NORM,			/*!< Motors off and rc thrust is in normal area */
	M_ON_TH_NORM,			/*!< Motors on and rc thrust is in normal area */
	M_ON_TH_MIN,			/*!< Motors on and rc thrust is min value */
	M_OFF_TH_MIN,			/*!< Motors off and rc thrust is min value */
	UNDEFINED
} motor_states_e;

//**********************************************************************************

void controller_attitude(void);
void controller_altitude(void);
void calculate_motor_values(void);

//**********************************************************************************

void min_max_value(float *input, float *output, float min, float max);
void limit_angle(float *input, float *output, float range);


//**********************************************************************************

void propotional_term(float* input, float influence, float* output);
void integral_term(float *input, uint8_t motors_on, float influence, float *output);
void derivative_term(float *input, float *input_old, float influence, float *output);
void damping_term(float input, float influence, float *output);

//**********************************************************************************

void rcAdjustingFloat(float *input, float *output, float sensitivity);
void get_offset_thrust_position(float thrust, uint8_t *active, float *setpoint_thrust, float *setpoint_climbing_rate);
void get_altitude_setpoint(float *setpoint_climbing_rate, float height, uint8_t *active, float *setpoint_altitude);
void integral_term_alt(float *input, float thrust, float *setpoint_altitude, float *setpoint_altitude_old, uint8_t *active, float influence, float *output);

//**********************************************************************************

void motorLimitation(int16_t *input, int16_t *output, int16_t min, int16_t max);

//**********************************************************************************

#endif
