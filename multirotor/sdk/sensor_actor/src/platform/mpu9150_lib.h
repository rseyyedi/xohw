/*
 * iic_mpu9150_lib.h
 *
 *  Created on: Jul 17, 2015
 *      Author: sschreiner
 */

#ifndef IIC_MPU9150_LIB_H_
#define IIC_MPU9150_LIB_H_

#include <stdint.h>

#define MPU9150_ADDRESS 0x68
#define AK8975_ADDRESS (0x18>>1)

/*
 * struct to describe the mpu9150_error type
 */
typedef enum{
	MPU9150_NO_ERROR,   /*!< no mpu9150-Error */
	MPU9150_RECV_ERROR, /*!< an error occurred while receiving data */
	MPU9150_SEND_ERROR, /*!< an error occurred while sending data */
	MPU9150_INIT_ERROR,   /*!< an error occurred, while initializing the mpu9150-ports */
	MPU9150_DATA_ERROR    /*!< the received data from an mpu9150-sensor is logical invalid */
} mpu9150_error_e;  /*!< Enum_type to describe the occurred mpu9150-error */

/*
 * Struct to store the sensordata
 */
typedef struct{
	int16_t acc_x; /*!< Acceleration in x-axis [16384 LSB/mg]*/
	int16_t acc_y; /*!< Acceleration in y-axis [16384 LSB/mg]*/
	int16_t acc_z; /*!< Acceleration in z-axis [16384 LSB/mg]*/
	int16_t temperature;   /*!< Temperature [deg C]*/
	int16_t gyro_x;  /*!< angular-velocity round x-axis [131 LSB/deg/s]*/
	int16_t gyro_y;  /*!< angular-velocity round y-axis [131 LSB/deg/s]*/
	int16_t gyro_z;  /*!< angular-velocity round z-axis [131 LSB/deg/s]*/
	int16_t mag_x;   /*!< Magnetometer measurement in X-direction*/
	int16_t mag_y;    /*!< Magnetometer measurement in Y-direction*/
	int16_t mag_z;   /*!< Magnetometer measurement in Z-direction*/
	uint32_t mpu9150_errors; /*!< number of IIC-Errors occurred since initialization*/
} mpu9150_data_s;	//Struct, in der alle Sensorwerte gespeichert werden

mpu9150_error_e mpu9150_init(void);
mpu9150_error_e mpu9150_get_data(volatile mpu9150_data_s *data);
mpu9150_error_e ak8975_start_magneto_measurement(void);
mpu9150_error_e ak8975_get_magneto_measurement(volatile mpu9150_data_s *data);

#endif /* IIC_MPU9150_LIB_H_ */
