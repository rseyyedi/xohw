/*
 * attitude_altitude.h
 *
 *  Created on: Aug 7, 2015
 *      Author: sschreiner
 */

#ifndef ATTITUDE_ALTITUDE_H_
#define ATTITUDE_ALTITUDE_H_

#define IMU
#define BU


#include <stdint.h>

#include "platform/mpl3115a2_lib.h"
#include "platform/mpu9150_lib.h"

#define MIN_LIPO_CELL_VOLTAGE	34	/*!< 0.1V per bit; minimum cellvoltage of a LiPo battery */
#define MIN_VOLTAGE_LOGIC		(3 * MIN_LIPO_CELL_VOLTAGE)	/*!< Minimum voltage of the Logic battery; 0.1V per bit */
#define MIN_VOLTAGE_MOTOR		(4 * MIN_LIPO_CELL_VOLTAGE)	/*!< Minimum voltage of the Energy battery; 0.1V per bit */
#define MAX_TEMPERATURE_ZYNQ	81 /*!< Maximum temperature of the zynq in degree Celcius */

typedef enum {
	p1, 		 /*!<Read Acc+Gyro, start BMP-Temp measurement, start compass measurement */
    p2,			 /*!<Read Acc+Gyro */
    p3,			 /*!<Read Acc+Gyro */
    p4,			 /*!<Read Acc+Gyro, read BMP-Temp, start pressure measurement */
    p5,			 /*!<Read Acc+Gyro */
    p6,			 /*!<Read Acc+Gyro */
    p7,			 /*!<Read Acc+Gyro, read compass measurement */
    p8,			 /*!<Read Acc+Gyro, start compass measurement */
    p9,			 /*!<Read Acc+Gyro */
    p10,		 /*!<Read Acc+Gyro */
    p11,		 /*!<Read Acc+Gyro */
    p12,		 /*!<Read Acc+Gyro */
    p13,		 /*!<Read Acc+Gyro */
    p14,		 /*!<Read Acc+Gyro, read compass measurement */
    p15			 /*!<Read Acc+Gyro, read pressure measurement */
} sensor_read_state_e;

typedef struct {
	mpu9150_data_s mpu9150_data;
	mpl3115a2_data_s mpl3115a2_data;
	float temperature_zynq;  /*!< Temperature [deg C]*/
	float voltage_logic; /*!< Battery voltage of logic [V]*/
	float voltage_motor; /*!< Battery voltage of motors [V]*/
} sensor_data_s;	//Struct, in der alle Sensorwerte gespeichert werden

void sensors_read_data(void);
sensor_data_s sensors_get_data(void);
void sensors_to_bram_init(void);
void sensors_to_bram(void);

#endif /* ATTITUDE_ALTITUDE_H_ */
