/***************************** Include Files *********************************/

#include "xparameters.h"
#include "iic.h"


/*****************************************************************************/
/**
 *
 * The purpose of this function is to illustrate how to use the GPIO
 * driver to turn on and off an LED.
 *
 * @param	None
 *
 * @return	XST_FAILURE to indicate that the GPIO Initialization had
 *		failed.
 *
 * @note		This function will not return if the test is running.
 *
 ******************************************************************************/
int main(void) {
	int Status;
	Status = I2CSlave();
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;

}
