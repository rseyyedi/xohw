#ifndef __MAIN_H_
#define __MAIN_H_

/* stepsize defines for address calculation */
#define SINGLE_STEP_SIZE				(unsigned long)0x04				/*!< stepsize from one address to the next in the BRAM  */
//#define PPM_CHANNEL_STEP_SIZE			4*SINGLE_STEP_SIZE				/*!< stepsize from one PPM channel address to the next in the BRAM  */

/* addresses calculate as follows: the address offset, where the last data written + size of the data to write */
#define RAW_ACC_X_ADDR						(unsigned long)0xC0000000										/*!< baseaddress of the current acc x value */
#define RAW_ACC_Y_ADDR						(RAW_ACC_X_ADDR 					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current acc y value */
#define RAW_ACC_Z_ADDR						(RAW_ACC_Y_ADDR						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current acc z value */
#define RAW_GYRO_X_ADDR						(RAW_ACC_Z_ADDR						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current gyro x value */
#define RAW_GYRO_Y_ADDR						(RAW_GYRO_X_ADDR					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current gyro y value */
#define RAW_GYRO_Z_ADDR						(RAW_GYRO_Y_ADDR					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current gyro z value */
#define RAW_MAG_X_ADDR						(RAW_GYRO_Z_ADDR					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current mag x value */
#define RAW_MAG_Y_ADDR						(RAW_MAG_X_ADDR						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current mag y value */
#define RAW_MAG_Z_ADDR						(RAW_MAG_Y_ADDR						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current mag z value */
#define RAW_HEIGHT_ADDR						(RAW_MAG_Z_ADDR						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current pressure */
#define TEMPERATURE_MPU_ADDR				(RAW_HEIGHT_ADDR 					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current mpu9150 temperature in [degree C] */
#define TEMPERATURE_MPL_ADDR				(TEMPERATURE_MPU_ADDR 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current mpl3115a2 temperature in [degree C] */
#define TEMPERATURE_ZYNQ_ADDR				(TEMPERATURE_MPL_ADDR 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current zynq temperature in [degree C] */
#define TEMPERATURE_MAX_ZYNQ_ADDR			(TEMPERATURE_ZYNQ_ADDR 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the max zynq temperature in [degree C] */
#define VOLTAGE_LOGIC_ADDR					(TEMPERATURE_MAX_ZYNQ_ADDR 			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current logic voltage in [V] */
#define VOLTAGE_MOTOR_ADDR					(VOLTAGE_LOGIC_ADDR 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current motor voltage in [V] */
#define VOLTAGE_MIN_CELL_ADDR				(VOLTAGE_MOTOR_ADDR 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the minimum cell voltage in [V] */
#define VOLTAGE_MIN_LOGIC_ADDR				(VOLTAGE_MIN_CELL_ADDR 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the minimum logic voltage in [V] */
#define VOLTAGE_MIN_MOTOR_ADDR				(VOLTAGE_MIN_LOGIC_ADDR 			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the minimum motor voltage in [V] */
#define IIC_ERRORS_ADDR						(VOLTAGE_MIN_MOTOR_ADDR 			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the IIC sensor errors (MPU, BMP, AK Sensors) */
#define SENSOR_TIME_STAMP_ADDR				(IIC_ERRORS_ADDR 					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the time stamp for the sensor data */

#endif
