/*
 * mcp3201_lib.c
 *
 *  Created on: Jul 20, 2015
 *      Author: sschreiner
 */

#include "mcp3201_lib.h"
#include "spi_lib.h"
#include <math.h>

#define LOGIC_SLAVE_ID		1   /**< Slave Select Signal to address logic battery guard */
#define MOTOR_SLAVE_ID		2   /**< Slave Select Signal to address motor battery guard */

/*
 * to minimize the errors, an offline two point calibration for each battery guard was done.
 * Therefore two defined adc values are measured and saved to y1 and y2. The proper adc values saved to
 * x1 and x2. With theses values the gradient (m) and the offset (b) from these mcp3201 values are calculated.
 */
// logic
#define Y1_V_LOGIC			9.13f   /**< Measured Y1 voltage value (logic part) */
#define Y2_V_LOGIC			13.24f  /**< Measured Y2 voltage value (logic part) */
#define X1_BIT_LOGIC		682 /**< Measured X1 ADC value corresponding to Y1 (logic part) */
#define X2_BIT_LOGIC		981 /**< Measured X2 ADC value corresponding to Y2 (logic part) */
#define MULT_PER_LSB_LOGIC	fabsf((Y2_V_LOGIC - Y1_V_LOGIC)/(X2_BIT_LOGIC-X1_BIT_LOGIC)) /**< Gradient m (logic part) */
#define OFFSET_LOGIC		Y1_V_LOGIC - MULT_PER_LSB_LOGIC * X1_BIT_LOGIC    /**< Axis intercept b (logic part) */

// motor
#define Y1_V_MOTOR			12.05f  /**< Measured Y1 voltage value (motor part) */
#define Y2_V_MOTOR			17.05f  /**< Measured Y2 voltage value (motor part) */
#define X1_BIT_MOTOR		689 /**< Measured X1 ADC value corresponding to Y1 (motor part) */
#define X2_BIT_MOTOR		982 /**< Measured X2 ADC value corresponding to Y2 (motor part) */
#define MULT_PER_LSB_MOTOR	fabsf((Y2_V_MOTOR - Y1_V_MOTOR)/(X2_BIT_MOTOR-X1_BIT_MOTOR))    /**< Gradient m (motor part) */
#define OFFSET_MOTOR		Y1_V_MOTOR - MULT_PER_LSB_MOTOR * X1_BIT_MOTOR    /**< Axis intercept b (motor part) */

static uint32_t mcp3201_device_id = SPI_INTERFACE_ID_0;
static uint32_t mcp3201_errors = 0;

static float mcp3201_filter_voltage_sum_logic = 0.0f;
static float mcp3201_filter_voltage_sum_motor = 0.0f;
static uint16_t mcp3201_filter_length_logic = 1;
static uint16_t mcp3201_filter_length_motor = 1;

mcp3201_error_e mcp3201_init(void)
{
	spi_error_e spi_status = SPI_NO_ERROR;

	spi_status = spi_interface_init(mcp3201_device_id);

	if (spi_status != SPI_NO_ERROR)
	{
		mcp3201_errors++;
		return MCP3201_INIT_ERROR;
	}

	return MCP3201_NO_ERROR;
}

mcp3201_error_e mcp3201_init_filter(const uint8_t slave_id, const uint16_t filter_length)
{
	mcp3201_error_e status = MCP3201_NO_ERROR;
	float temp = 0.0f;

	status = mcp3201_get_data(slave_id, &temp);

	if (status != MCP3201_NO_ERROR)
	{
		return MCP3201_RECV_ERROR;
	}

	if (slave_id == LOGIC_SLAVE_ID)
	{
		mcp3201_filter_length_logic = filter_length;
		mcp3201_filter_voltage_sum_logic = temp*(float)mcp3201_filter_length_logic;
	}
	else if (slave_id == MOTOR_SLAVE_ID)
	{
		mcp3201_filter_length_motor = filter_length;
		mcp3201_filter_voltage_sum_motor = temp*(float)mcp3201_filter_length_motor;
	}
	else
	{
		mcp3201_errors++;
		return MCP3201_SLAVE_SELECT_ERROR;
	}

	return MCP3201_NO_ERROR;
}

mcp3201_error_e mcp3201_get_data(const uint8_t slave_id, float *value)
{
	spi_error_e spi_status = SPI_NO_ERROR;
	uint8_t recv_buffer[2] = {0, 0};
	uint16_t temp = 0;

	spi_status = spi_recv(slave_id, &recv_buffer[0], 2);

	if (spi_status != SPI_NO_ERROR)
	{
		mcp3201_errors++;
		return MCP3201_RECV_ERROR;
	}

	/*
	* Discard the initialization and null bits and the 2 LSBs to reduce bit
	* resolution and transform the 2 uint8_t into one uint16_t value
	*/
	recv_buffer[0] = recv_buffer[0] & 0x1F;
	temp = recv_buffer[0];
	temp = temp<<8;
	temp |= (uint16_t)recv_buffer[1];
	temp = temp>>3;

    //ADC value 1024 and above can never be reached
    if (temp >= 1024)
    {
    	mcp3201_errors++;
        return MCP3201_DATA_ERROR;
    }

	//Calculate the voltage in miliVolt from the read spi value
	if (slave_id == LOGIC_SLAVE_ID)
	{
		*value = (((float)temp * MULT_PER_LSB_LOGIC) + OFFSET_LOGIC);
	}
	else if (slave_id == MOTOR_SLAVE_ID)
	{
		*value = (((float)temp * MULT_PER_LSB_MOTOR) + OFFSET_MOTOR);
	}
	else
	{
		mcp3201_errors++;
		return MCP3201_SLAVE_SELECT_ERROR;
	}

	return MCP3201_NO_ERROR;
}

mcp3201_error_e mcp3201_get_filtered_data(const uint8_t slave_id, float *value)
{
	mcp3201_error_e status = MCP3201_NO_ERROR;
	float temp = 0.0f;

	status = mcp3201_get_data(slave_id, &temp);

	if (status != MCP3201_NO_ERROR)
	{
		return MCP3201_RECV_ERROR;
	}

	if (slave_id == LOGIC_SLAVE_ID)
	{
		mcp3201_filter_voltage_sum_logic -= (mcp3201_filter_voltage_sum_logic / (float)mcp3201_filter_length_logic);
		mcp3201_filter_voltage_sum_logic += temp;

		*value = mcp3201_filter_voltage_sum_logic / (float)mcp3201_filter_length_logic;
	}
	else if (slave_id == MOTOR_SLAVE_ID)
	{
		mcp3201_filter_voltage_sum_motor -= (mcp3201_filter_voltage_sum_motor / (float)mcp3201_filter_length_motor);
		mcp3201_filter_voltage_sum_motor += temp;

		*value = mcp3201_filter_voltage_sum_motor / (float)mcp3201_filter_length_motor;
	}
	else
	{
		mcp3201_errors++;
		return MCP3201_SLAVE_SELECT_ERROR;
	}

	return MCP3201_NO_ERROR;
}
