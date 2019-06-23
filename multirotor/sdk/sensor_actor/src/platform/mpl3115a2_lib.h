/*
 * mpl3115a2_lib.h
 *
 *  Created on: Dec 08, 2015
 *      Author: dgrundt
 */

#ifndef MPL3115A2_LIB_H_
#define MPL3115A2_LIB_H_

#include <stdint.h>

#define MPL3115A2_ADDRESS 0x60 // Unshifted 7-bit I2C address for sensor

#define STATUS     0x00					// Sensor Status
#define OUT_P_MSB  0x01					// Pressure Data Out MSB
#define OUT_P_CSB  0x02					// "        "    "   CSB
#define OUT_P_LSB  0x03					// "        "    "   LSB
#define OUT_T_MSB  0x04
#define OUT_T_LSB  0x05
#define DR_STATUS  0x06					// "Data ready" information
#define OUT_P_DELTA_MSB  0x07			// Pressure change data MSB
#define OUT_P_DELTA_CSB  0x08           //  "		"		"   CSB
#define OUT_P_DELTA_LSB  0x09			//  " 		"       "   LSB
#define OUT_T_DELTA_MSB  0x0A
#define OUT_T_DELTA_LSB  0x0B
#define WHO_AM_I   0x0C					// Fixed Device ID Number
#define F_STATUS   0x0D					// FIFO Status
#define F_DATA     0x0E					// FIFO data access
#define F_SETUP    0x0F					// FIFO setup
#define TIME_DLY   0x10					// Time since FIFO overflow
#define SYSMOD     0x11					// Current system mode
#define INT_SOURCE 0x12					// interrupt status
#define PT_DATA_CFG 0x13				// Data event flag configuration
#define BAR_IN_MSB 0x14					// Barometic input for Altitude calculation bits 8-15
#define BAR_IN_LSB 0x15					// "			"	"	"			"		bits 0-7
#define P_TGT_MSB  0x16					// Pressure/Altitude target value bis 8-15
#define P_TGT_LSB  0x17					// Pressure/Altitude target value bits 0-7
#define T_TGT      0x18
#define P_WND_MSB  0x19					// Pressure/Altitude window value bits 8-15
#define P_WND_LSB  0x1A					// Pressure/Altitude window value bits 0-7
#define T_WND      0x1B
#define P_MIN_MSB  0x1C					// Minimum Pressure/Altitude bits 12-19
#define P_MIN_CSB  0x1D					// Minimum Pressure/Altitude bits 4-11
#define P_MIN_LSB  0x1E					// "		"		"		 bits 0-3
#define T_MIN_MSB  0x1F
#define T_MIN_LSB  0x20
#define P_MAX_MSB  0x21					// Maximum Pressure/Altitude bits 12-19
#define P_MAX_CSB  0x22					// "		"		"		 bits 4-11
#define P_MAX_LSB  0x23					// "		"		"		 bits 0-3
#define T_MAX_MSB  0x24
#define T_MAX_LSB  0x25
#define CTRL_REG1  0x26					// Modes, oversampling
#define CTRL_REG2  0x27					// Acquisition time step
#define CTRL_REG3  0x28					// Interrupt pin config.
#define CTRL_REG4  0x29					// Interrupt enables
#define CTRL_REG5  0x2A					// Interrupt output pin assignment
#define OFF_P      0x2B					// Pressure data offset
#define OFF_T      0x2C
#define OFF_H      0x2D					// Altitude data offset

typedef struct {
	float temperature;   /*!< Temperature [deg C]*/
	float height;  /*!< height [m]*/
	uint32_t mpl3115a2_errors; /*!< number of IIC-Errors occurred since initialization*/
} mpl3115a2_data_s;	//Struct, in der alle Sensorwerte gespeichert werden

typedef enum {
	MPL3115A2_NO_ERROR,   /*!< no bmp085-Error */
	MPL3115A2_RECV_ERROR, /*!< an error occurred while receiving data */
	MPL3115A2_SEND_ERROR, /*!< an error occurred while sending data */
	MPL3115A2_INIT_ERROR,   /*!< an error occurred, while initializing the bmp085-ports */
	MPL3115A2_DATA_ERROR    /*!< the received data from an bmp085-sensor is logical invalid */
} mpl3115a2_error_e;  /*!< Enum_type to describe the occurred bmp085-error */

mpl3115a2_error_e mpl3115a2_init(void);
mpl3115a2_error_e mpl3115a2_get_data(mpl3115a2_data_s *data);

#endif /* MPL3115A2_LIB_H_ */
