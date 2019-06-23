/*
 * gam_lib.h
 *
 *  Created on: Jul 22, 2015
 *      Author: sschreiner
 */

#ifndef GAM_LIB_H_
#define GAM_LIB_H_

#include <stdint.h>

/*!
 * some enum for better readability
 */
typedef enum {
	LOGIC_BATT = 0,
	MOTOR_BATT = 1
} gam_battery_e;

typedef enum {
	ZYNQ_TEMP = 0,
	AMBIENT_TEMP = 1
} gam_temperature_e;

/*!
 * internal uart send status... needed for the state maschine
 */
typedef enum {
	DATA_REQUESET_RECEIVED, 	/*!< if an GAM request recieved */
	WAITING_FOR_DATA,			/*!< waiting for incoming data */
	IS_SENDING,					/*!< send GAM data */
	WRONG_DATA,					/*!< all data recieved no gam frame was requested */
	ALL_DATA_SEND				/*!< the hole frame was send */
} gam_status_e;

/*!
 * some enum easier understanding
 */
typedef enum {
	GAM_NO_ERROR = 0x00,				/*!< no gam error */
	GAM_INIT_ERROR = 0x01,		/*!< error while initialisation */
	GAM_WRONG_BATTERY_ERROR = 0x02,		/*!< error, wrong battery chosen, see /link gam_updateVoltage(u16 voltage, accu_e battery) /endlink */
	GAM_WRONG_TEMPSENSOR_ERROR = 0x04,	/*!< error, wrong tempsensor chosen, see /link gam_updateVoltage(u16 voltage, accu_e battery) /endlink */
 	GAM_NO_REQUEST = 0x08,
	GAM_IS_SENDING = 0x10
} gam_error_e;

/*!
 * Warning beep types, which can send to the RC to enable the sound
 */
typedef enum {
	NO_ALARM = 0,
	ERR_CALIBRATION = 1,
	ERR_RECIEVER = 2,
	ERR_DATABUS = 3,
	ERR_NAVI = 4,
	NOT_USED = 5,
	ERR_KOMPASS = 6,
	ERR_SENSOR = 7,
	ERR_GPS = 8,
	ERR_MOTOR = 9,
	MAX_TEMPERATUR = 10,
	ALTITUDE_REACHED = 11,
	WAYPOINT_REACHED = 12,
	NEXT_WAYPOINT = 13,
	LANDING = 14,
	GPS_FIX = 15,
	UNDERVOLTAGE = 16,
	GPS_HOLD = 17,
	GPS_HOME = 18,
	GPS_OFF = 19,
	JUST_NOISE = 20, // "HOTBUTTON" Sound
	MICROCOPTER = 21,
	CAPACITY = 22,
	WHAT_EVER = 23, // verstehe ich nicht
	CALIBRATION = 24,
	MAX_RANGE = 25,
	MAX_ALTITUDE = 26
} gam_beeps_e;

/*!
 *	This structure is the frame, which has to transmit once after the reciever call for response
*/
typedef struct {
    uint8_t start_byte;		    /*!<#01 start u8 constant value 0x7c */
    uint8_t gam_sensor_id; 	    /*!<#02 EAM sensort id. constat value 0x8d */
    uint8_t warning_beeps;	    /*!<#03 1=A 2=B ... 0x1a=Z  0 = no alarm */
								// Q	Min cell voltage sensor 1
								// R	Min Battery 1 voltage sensor 1
								// J	Max Battery 1 voltage sensor 1
								// F	Min temperature sensor 1
								// H	Max temperature sensor 1
								// S	Min Battery 2 voltage sensor 2
								// K	Max Battery 2 voltage sensor 2
								// G	Min temperature sensor 2
								// I	Max temperature sensor 2
								// W	Max current
								// V	Max capacity mAh
								// P	Min main power voltage
								// X	Max main power voltage
								// O	Min altitude
								// Z	Max altitude
								// C	negative difference m/s too high
								// A	negative difference m/3s too high
								// N	positive difference m/s too high
								// L	positive difference m/3s too high
								// T	Minimum RPM
								// Y	Maximum RPM

    uint8_t sensor_id;	        /*!<#04 constant value 0xd0 */
    uint8_t alarm_invers1;	    /*!<#05 alarm bitmask. Value is displayed inverted */
								//Bit#	Alarm field
								// 0	all cell voltage
								// 1	Battery 1
								// 2	Battery 2
								// 3	Temperature 1
								// 4	Temperature 2
								// 5	Fuel
								// 6	mAh
								// 7	Altitude
    uint8_t alarm_invers2;	    /*!<#06 alarm bitmask. Value is displayed inverted */
								//Bit#	Alarm Field
								// 0	main power current
								// 1	main power voltage
								// 2	Altitude
								// 3	m/s
								// 4	m/3s
								// 5	unknown
								// 6	unknown
								// 7	"ON" sign/text msg active

    uint8_t cell1;				/*!<#07 cell 1 voltage lower value. 0.02V steps, 124=2.48V */
    uint8_t cell2;				/*!<#08 cell 2 voltage lower value. 0.02V steps, 124=2.48V */
    uint8_t cell3;				/*!<#09 cell 3 voltage lower value. 0.02V steps, 124=2.48V */
    uint8_t cell4;				/*!<#10 cell 4 voltage lower value. 0.02V steps, 124=2.48V */
    uint8_t cell5;				/*!<#11 cell 5 voltage lower value. 0.02V steps, 124=2.48V */
    uint8_t cell6;				/*!<#12 cell 6 voltage lower value. 0.02V steps, 124=2.48V */
	uint8_t batt1_L;			/*!<#13 battery 1 voltage LSB value. 0.1V steps. 50 = 5.5V */
	uint8_t batt1_H;			/*!<#14 */
	uint8_t batt2_L;			/*!<#15 battery 2 voltage LSB value. 0.1V steps. 50 = 5.5V */
	uint8_t batt2_H;			/*!<#16 */
	uint8_t temperature1;		/*!<#17 temperature 1. offset of 20. a value of 20 = 0°C */
	uint8_t temperature2;		/*!<#18 temperature 2. offset of 20. a value of 20 = 0°C */
	uint8_t fuel_procent;		/*!<#19 Fuel capacity in %. Values 0--100
								 graphical display ranges: 0-25% 50% 75% 100% */
	uint8_t fuel_ml_L;			/*!<#20 Fuel in ml scale. Full = 65535! */
	uint8_t fuel_ml_H;			/*!<#21 */
	uint8_t rpm_L;				/*!<#22 RPM in 10 RPM steps. 300 = 3000rpm */
	uint8_t rpm_H;				/*!<#23 */
	uint8_t altitude_L;			/*!<#24 altitude in meters. offset of 500, 500 = 0m */
	uint8_t altitude_H;			/*!<#25 */
	uint8_t climbrate_L;		/*!<#26 climb rate in 0.01m/s. Value of 30000 = 0.00 m/s */
	uint8_t climbrate_H;		/*!<#27 */
	uint8_t climbrate3s;		/*!</#28 climb rate in m/3sec. Value of 120 = 0m/3sec */
	uint8_t current_L;			/*!<#29 current in 0.1A steps */
	uint8_t current_H;			/*!<#30 */
	uint8_t main_voltage_L;		/*!<#31 Main power voltage using 0.1V steps */
	uint8_t main_voltage_H;		/*!<#32 */
	uint8_t batt_cap_L;			/*!<#33 used battery capacity in 10mAh steps */
	uint8_t batt_cap_H;			/*!<#34 */
	uint8_t speed_L;			/*!<#35 (air?) speed in km/h(?) we are using ground speed here per default */
	uint8_t speed_H;			/*!<#36 */
	uint8_t min_cell_volt;		/*!<#37 minimum cell voltage in 2mV steps. 124 = 2,48V */
	uint8_t min_cell_volt_num;	/*!<#38 number of the cell with the lowest voltage */
	uint8_t rpm2_L;				/*!<#39 RPM in 10 RPM steps. 300 = 3000rpm */
	uint8_t rpm2_H;				/*!<#40 */
	uint8_t general_error_number;/*!<#41 Voice error == 12. TODO: more docu */
	uint8_t pressure;			/*!<#42 Pressure up to 16bar. 0,1bar scale. 20 = 2bar */
	uint8_t version;			/*!<#43 version number TODO: more info? */
	uint8_t stop_byte;			/*!<#44 stop u8 */
	//uint8_t parity;				/*!<#45 CRC/Parity (calculated dynamicaly) */
} gam_data_s;

/*!
 * Help structure to save and compare some parameters
 */
typedef struct {
	uint16_t voltage1;		/*!< LOGIC_ACCU	in 0.1 V per bit */
	uint16_t voltage2;		/*!< ENERGY_ACCU	in 0.1 V per bit */
	uint16_t minVoltage1;	/*!< LOGIC_ACCU	in 0.1 V per bit */
	uint16_t minVoltage2;	/*!< ENERGY_ACCU	in 0.1 V per bit */
	uint8_t  maxTemp1;		/*!< ZYNQ			in °C */
	uint8_t  maxTemp2;		/*!< TEMPSENSOR_2 in °C */
} gam_help_s;

gam_error_e gam_init(void);
gam_error_e gam_update_voltage(uint16_t voltage, gam_battery_e battery);
void gam_update_altitude(uint16_t altitude);
gam_error_e gam_update_temperature(uint8_t temperature, gam_temperature_e device);
void gam_calibrating_beep(uint8_t state);
void gam_warning_beep(gam_beeps_e warningbeep);
void gam_update_climbrate(int16_t climbrate);
void gam_min_cell_voltage(uint8_t cell_voltage);
gam_error_e gam_send_frame(void);

#endif /* GAM_LIB_H_ */
