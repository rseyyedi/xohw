/*
 * sensors.h
 *
 *  Created on: May 4, 2017
 *      Author: sschreiner
 */

#ifndef SRC_SENSORS_H_
#define SRC_SENSORS_H_

#include <stdint.h>

/*
 * Structs to store the sensordata
 */
typedef struct{
	int32_t acc_x; /*!< Acceleration in x-axis [16384 LSB/mg]*/
	int32_t acc_y; /*!< Acceleration in y-axis [16384 LSB/mg]*/
	int32_t acc_z; /*!< Acceleration in z-axis [16384 LSB/mg]*/
	int32_t temperature;   /*!< Temperature [deg C]*/
	int32_t gyro_x;  /*!< angular-velocity round x-axis [131 LSB/deg/s]*/
	int32_t gyro_y;  /*!< angular-velocity round y-axis [131 LSB/deg/s]*/
	int32_t gyro_z;  /*!< angular-velocity round z-axis [131 LSB/deg/s]*/
	int32_t mag_x;   /*!< Magnetometer measurement in X-direction*/
	int32_t mag_y;    /*!< Magnetometer measurement in Y-direction*/
	int32_t mag_z;   /*!< Magnetometer measurement in Z-direction*/
} mpu9150_data_s;	//Struct, in der alle Sensorwerte gespeichert werden

typedef struct{
	float acc_x; /*!< Acceleration in x-axis [16384 LSB/mg]*/
	float acc_y; /*!< Acceleration in y-axis [16384 LSB/mg]*/
	float acc_z; /*!< Acceleration in z-axis [16384 LSB/mg]*/
	float temperature;   /*!< Temperature [deg C]*/
	float gyro_x;  /*!< angular-velocity round x-axis [131 LSB/deg/s]*/
	float gyro_y;  /*!< angular-velocity round y-axis [131 LSB/deg/s]*/
	float gyro_z;  /*!< angular-velocity round z-axis [131 LSB/deg/s]*/
	float mag_x;   /*!< Magnetometer measurement in X-direction*/
	float mag_y;    /*!< Magnetometer measurement in Y-direction*/
	float mag_z;   /*!< Magnetometer measurement in Z-direction*/
} mpu9150_float_data_s;	//Struct, in der alle Sensorwerte gespeichert werden

typedef struct {
	float temperature;   /*!< Temperature [deg C]*/
	float height;  /*!< height [m]*/
} mpl3115a2_data_s;	//Struct, in der alle Sensorwerte gespeichert werden

typedef struct {
	mpu9150_data_s mpu9150_data;
	mpl3115a2_data_s mpl3115a2_data;
	uint32_t iic_errors; /*!< number of IIC-Errors of sensors occurred since initialization*/
	float temperature_zynq;  /*!< Temperature [deg C]*/
	float voltage_logic; /*!< Battery voltage of logic [V]*/
	float voltage_motor; /*!< Battery voltage of motors [V]*/
	uint32_t time_stamp; /*!< Last time stamp of sensor data */
	uint8_t data_valid; /*!< Valid flag for copied data */
} sensor_data_s;	//Struct, in der alle Sensorwerte gespeichert werden

typedef struct {
	mpu9150_float_data_s mpu9150_data;
	mpl3115a2_data_s mpl3115a2_data;
	float temperature_zynq;  /*!< Temperature [deg C]*/
	float voltage_logic; /*!< Battery voltage of logic [V]*/
	float voltage_motor; /*!< Battery voltage of motors [V]*/
	uint8_t data_valid; /*!< Valid flag for copied data */
} sensor_float_data_s;	//Struct, in der alle Sensorwerte gespeichert werden

void sensors_from_bram_init(void);
void sensors_from_bram(void);
void sensors_init_gyro_offsets(const uint16_t max_init_samples, const uint16_t init_counter);
void sensors_init_acc_offsets(const uint16_t max_init_samples, const uint16_t init_counter);
void sensors_init_height_offset(const uint16_t max_init_samples, const uint16_t init_counter);
void sensors_pre_calculations(void);
sensor_float_data_s sensors_get_data(void);

#endif /* SRC_SENSORS_H_ */
