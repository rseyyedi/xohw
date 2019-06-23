/*
 * @file: 	iic_motor.h
 * @author:	Niklas May-Johann
 * @date: 	05.01.2015
 *
 * @brief	This module initialize the iic communication and send the motor control values over
 * 			the iic bus. The bus will be initialize with 400 kHz
 */

#ifndef IIC_MOTOR_H_
#define IIC_MOTOR_H_

#include <stdio.h>

#include "xil_types.h"
#include "xparameters.h"
#include "xiic.h"
#include "xiic_l.h"

/*! some BL config defines */
#define BL_READMODE_STATUS  			0
#define BL_READMODE_CONFIG      		16
#define BLCONFIG_REVISION 				0x02
#define MASK_SET_PWM_SCALING            0x01
#define MASK_SET_CURRENT_LIMIT          0x02
#define MASK_SET_TEMP_LIMIT             0x04
#define MASK_SET_CURRENT_SCALING        0x08
#define MASK_SET_BITCONFIG              0x10
#define MASK_SET_STARTPWM               0x20
#define MASK_SET_DEFAULT_PARAMS         0x40
#define MASK_SET_SAVE_EEPROM            0x80
#define BITCONF_REVERSE_ROTATION 		0x01
#define BITCONF_STARTGAS1 				0x02
#define BITCONF_STARTGAS2 				0x04
#define BITCONF_STARTGAS3 				0x08
#define BITCONF_RES4 					0x10
#define BITCONF_RES5 					0x20
#define BITCONF_RES6 					0x40
#define BITCONF_RES7 					0x80

/*
 * the range of the control value of the motor driver
 */
#define MOTOR_SPEED_MIN	0 		/*!< minimum value for the motor speed */
#define MOTOR_SPEED_MAX	2047	/*!< maximum value of the motor speed */

/*!
 * different error codes
 */
typedef enum{
	MOTOR_NO_ERROR = 0, /*!< no error */
	MOTOR_ERR_NO_ANSWER, /*!< error while sending to motor controller */
	MOTOR_UNKNOWN,		/*!< not used */
	MOTOR_IIC_BUS_BUSY,	/*!< error, can't start sending... bus is busy */
	MOTOR_ERR_INITIALISATION	/*!< error while initialisation */
} err_motor_e;

/*!
 * The motor rotating direction
 */
typedef enum{
	MOTOR_DIR_LEFT = 0,	/*!< rotate left */
	MOTOR_DIR_RIGHT	/*!< rotate right */
} motor_direction_e;

/*!
 * Enums for easier usage
 *
 * The motor enums are set as follows
 *
 * 				^ Fly direction 
 * 				:
 * 				x MOTOR_FRONT
 * 				|
 * MOTOR_LEFT x---x	MOTOR_RIGHT
 * 				|
 * 				x MOTOR_REAR
 *
 */
typedef enum {
	MOTOR_FRONT = 0,	/*!< fly direction ... motor on the blue boom */
	MOTOR_LEFT = 3,		/*!< motor on the left boom */
	MOTOR_RIGHT = 2,	/*!< motor on the right boom */
	MOTOR_REAR = 1		/*!< motor on the rear boom */
} motor_id_e;

/*!
 * structure of the read back informations from the BL motor driver
 */
typedef struct {
	uint8_t current; /*!< the current in 0.1A Steps */
	uint8_t maxPWM;  /*!< if these value lower than 255, the BL driver is in current limit mode */
	int8_t temperature; /*!< the temperature of the BL driver in grad C */
} motor_info_s;

err_motor_e motor_init(void);
err_motor_e motor_set_value(motor_id_e motor_nr, uint32_t setpoint, motor_info_s* info);
void motor_values_from_bram(void);
void motor_send_values(uint8_t enable_motors);
motor_info_s motor_get_status(motor_id_e motor_nr);
void motor_set_rotation_direction(motor_id_e motor_nr, motor_direction_e dir);

#endif /* IIC_MOTOR_H_ */
