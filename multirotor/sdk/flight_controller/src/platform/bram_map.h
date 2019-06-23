/*
 * bram_map.h
 *
 *  Created on: Aug 5, 2015
 *      Author: sschreiner
 */

#ifndef BRAM_MAP_H_
#define BRAM_MAP_H_

/* stepsize defines for address calculation */
#define SINGLE_STEP_SIZE				(unsigned long)0x04				/*!< stepsize from one address to the next in the BRAM  */
#define PPM_CHANNEL_STEP_SIZE			4*SINGLE_STEP_SIZE				/*!< stepsize from one PPM channel address to the next in the BRAM  */

/* addresses calculate as follows: the address offset, where the last data written + size of the data to write */
/* Sensor values */
#define RAW_ACC_X_ADDR						(unsigned long)0x00000000										/*!< baseaddress of the current acc x value */
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
/* Remote control values */
#define RAW_RC_THRUST_ADDR					(SENSOR_TIME_STAMP_ADDR				+ SINGLE_STEP_SIZE) 		/*!< baseaddress of the RC thrust leaver  */
#define RAW_RC_YAW_ADDR						(RAW_RC_THRUST_ADDR					+ PPM_CHANNEL_STEP_SIZE) 	/*!< baseaddress of the RC gier leaver  */
#define RAW_RC_ROLL_ADDR					(RAW_RC_YAW_ADDR					+ PPM_CHANNEL_STEP_SIZE) 	/*!< baseaddress of the RC roll leaver  */
#define RAW_RC_NICK_ADDR					(RAW_RC_ROLL_ADDR					+ PPM_CHANNEL_STEP_SIZE) 	/*!< baseaddress of the RC nick leaver  */
#define RAW_RC_BUTTON_MOTOR_ON_ADDR			(RAW_RC_NICK_ADDR					+ PPM_CHANNEL_STEP_SIZE) 	/*!< baseaddress of the RC button motor on  */
#define RAW_RC_BUTTON_HOLD_ALTITUDE_ADDR	(RAW_RC_BUTTON_MOTOR_ON_ADDR		+ PPM_CHANNEL_STEP_SIZE) 	/*!< baseaddress of the RC button hold altitude */
#define RAW_RC_BUTTON_CAM_ADDR				(RAW_RC_BUTTON_HOLD_ALTITUDE_ADDR	+ PPM_CHANNEL_STEP_SIZE)	/*!< baseaddress of the RC button CAM */
#define RAW_RC_BUTTON_OBJECTDETECTION_ADDR	(RAW_RC_BUTTON_CAM_ADDR				+ PPM_CHANNEL_STEP_SIZE)	/*!< baseaddress of the RC buton obj. detection */
#define RAW_RC_TIME_STAMP_ADDR				(RAW_RC_BUTTON_OBJECTDETECTION_ADDR	+ PPM_CHANNEL_STEP_SIZE)	/*!< baseaddress of the pitch angle value in [degree] */
/* Attitude */
#define ANGLE_THETA_ADDR					(RAW_RC_TIME_STAMP_ADDR				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the pitch angle value in [degree] */
#define ANGLE_PHI_ADDR						(ANGLE_THETA_ADDR					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the roll angle value [degree] */
#define ANGLE_PSI_ADDR						(ANGLE_PHI_ADDR						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the yaw angle value [degree] */
#define COMPASS_BEARING_ADDR				(ANGLE_PSI_ADDR						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the current compass bearing in [degree] */
#define ANGLE_VEL_THETA_ADDR				(COMPASS_BEARING_ADDR				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the pitch angle velocity value in [degrees/s] */
#define ANGLE_VEL_PHI_ADDR					(ANGLE_VEL_THETA_ADDR				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the roll angle velocity value in [degrees/s] */
#define ANGLE_VEL_PSI_ADDR					(ANGLE_VEL_PHI_ADDR					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the yaw angle velocity value in [degrees/s] */
/* Altitude */
#define ALTITUDE_ADDR						(ANGLE_VEL_PSI_ADDR					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the height value in [m] */
#define CLIMBRATE_ADDR						(ALTITUDE_ADDR						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the z-Speed value in [m/s] */
#define ACC_GEO_Z_ADDR						(CLIMBRATE_ADDR						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the z-acceleration value in [m/s^2] */
/* motor set points */
#define MOTOR_REAR_VALUE_ADDR				(ACC_GEO_Z_ADDR						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the rear motor control value */
#define MOTOR_FRONT_VALUE_ADDR				(MOTOR_REAR_VALUE_ADDR 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the front motor control value */
#define MOTOR_LEFT_VALUE_ADDR				(MOTOR_FRONT_VALUE_ADDR 			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the left motor control value */
#define MOTOR_RIGHT_VALUE_ADDR				(MOTOR_LEFT_VALUE_ADDR 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the right motor control value */
/* Main errors */
#define MAIN_ERRORS_ADDR					(MOTOR_RIGHT_VALUE_ADDR 			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the main errors */
/* Valid signals */
#define MB_PC_PRE_INIT_VALID_ADDR			(MAIN_ERRORS_ADDR					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the MB sensor processing and control pre init valid flag */
#define MB_SAIO_PRE_INIT_VALID_ADDR			(MB_PC_PRE_INIT_VALID_ADDR			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the MB sensor actor IO pre init valid flag */
#define MB_PC_INIT_VALID_ADDR				(MB_SAIO_PRE_INIT_VALID_ADDR		+ SINGLE_STEP_SIZE)			/*!< baseaddress of the MB sensor processing and control init valid flag */
#define MB_SAIO_INIT_VALID_ADDR				(MB_PC_INIT_VALID_ADDR				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the MB sensor actor IO init valid flag */
#define ATTITUDE_VALID_ADDR					(MB_SAIO_INIT_VALID_ADDR			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the attitude valid flag */
#define ALTITUDE_VALID_ADDR					(ATTITUDE_VALID_ADDR				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the altitude valid flag */
/* Remote final control values */
#define FINAL_RC_THRUST_ADDR				(ALTITUDE_VALID_ADDR				+ SINGLE_STEP_SIZE) 		/*!< baseaddress of the RC thrust leaver  */
#define FINAL_RC_YAW_ADDR					(FINAL_RC_THRUST_ADDR				+ SINGLE_STEP_SIZE) 		/*!< baseaddress of the RC gier leaver  */
#define FINAL_RC_ROLL_ADDR					(FINAL_RC_YAW_ADDR					+ SINGLE_STEP_SIZE) 		/*!< baseaddress of the RC roll leaver  */
#define FINAL_RC_NICK_ADDR					(FINAL_RC_ROLL_ADDR					+ SINGLE_STEP_SIZE) 		/*!< baseaddress of the RC nick leaver  */
#define FINAL_RC_BUTTON_MOTOR_ON_ADDR		(FINAL_RC_NICK_ADDR					+ SINGLE_STEP_SIZE) 		/*!< baseaddress of the RC button motor on  */
#define FINAL_RC_BUTTON_HOLD_ALTITUDE_ADDR	(FINAL_RC_BUTTON_MOTOR_ON_ADDR		+ SINGLE_STEP_SIZE) 		/*!< baseaddress of the RC button hold altitude */
#define FINAL_RC_BUTTON_CAM_ADDR			(FINAL_RC_BUTTON_HOLD_ALTITUDE_ADDR	+ SINGLE_STEP_SIZE)			/*!< baseaddress of the RC button CAM */
#define FINAL_RC_BUTTON_OBJECTDETECTION_ADDR (FINAL_RC_BUTTON_CAM_ADDR			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the RC buton obj. detection */
/* Controller Debug */
#define DEVIATION_THETA_ATTITUDE			(FINAL_RC_BUTTON_OBJECTDETECTION_ADDR + SINGLE_STEP_SIZE)		/*!< baseaddress of the deviation of the theta controller */
#define DEVIATION_PHI_ATTITUDE				(DEVIATION_THETA_ATTITUDE 			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the deviation of the phi controller */
#define DEVIATION_PSI_ATTITUDE				(DEVIATION_PHI_ATTITUDE 			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the deviation of the psi controller */
#define DEVIATION_ALTITUDE					(DEVIATION_PSI_ATTITUDE 			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the deviation of the altitude controller */

#define CV_P_THETA							(DEVIATION_ALTITUDE 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the P part of the theta attitude controller */
#define CV_I_THETA							(CV_P_THETA 						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the I part of the theta attitude controller */
#define CV_S_THETA							(CV_I_THETA 						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the S part of the theta attitude controller */
#define CV_SUM_THETA						(CV_S_THETA 						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the sum of the theta attitude controller */

#define CV_P_PHI							(CV_SUM_THETA			 			+ SINGLE_STEP_SIZE)			/*!< baseaddress of the P part of the phi attitude controller */
#define CV_I_PHI							(CV_P_PHI 							+ SINGLE_STEP_SIZE)			/*!< baseaddress of the I part of the phi attitude controller */
#define CV_S_PHI							(CV_I_PHI 							+ SINGLE_STEP_SIZE)			/*!< baseaddress of the S part of the phi attitude controller */
#define CV_SUM_PHI							(CV_S_PHI 							+ SINGLE_STEP_SIZE)			/*!< baseaddress of the sum of the phi attitude controller */

#define CV_P_PSI							(CV_SUM_PHI			 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the P part of the psi attitude controller */
#define CV_I_PSI							(CV_P_PSI 							+ SINGLE_STEP_SIZE)			/*!< baseaddress of the I part of the psi attitude controller */
#define CV_S_PSI							(CV_I_PSI 							+ SINGLE_STEP_SIZE)			/*!< baseaddress of the S part of the psi attitude controller */
#define CV_SUM_PSI							(CV_S_PSI 							+ SINGLE_STEP_SIZE)			/*!< baseaddress of the sum of the psi attitude controller */

#define CV_P_ALTITUDE						(CV_SUM_PSI			 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the P part of the altitude controller */
#define CV_I_ALTITUDE						(CV_P_ALTITUDE 						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the I part of the altitude controller */
#define CV_S_ALTITUDE						(CV_I_ALTITUDE 						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the S part of the altitude controller */
#define CV_SUM_ALTITUDE						(CV_S_ALTITUDE 						+ SINGLE_STEP_SIZE)			/*!< baseaddress of the sum of the altitude controller */
/* ACC angles */
#define ANGLE_ACC_THETA_ADDR				(CV_SUM_ALTITUDE 					+ SINGLE_STEP_SIZE)			/*!< baseaddress of the acc pitch angle value in [degree] */
#define ANGLE_ACC_PHI_ADDR					(ANGLE_ACC_THETA_ADDR 				+ SINGLE_STEP_SIZE)			/*!< baseaddress of the acc roll angle value in [degree] */
/* end of bram address map */
#define BRAM_ADDRESS_MAP_SIZE				(ANGLE_ACC_PHI_ADDR					+ SINGLE_STEP_SIZE)			/*!< the address, where the next data can start */

#endif /* BRAM_MAP_H_ */
