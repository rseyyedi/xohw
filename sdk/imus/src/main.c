/***************************** Include Files *********************************/

#include "xparameters.h"
#include "iic.h"
#include "main.h"


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

int main(void){
	uint32_t sent_value     = 0x00000001;
	uint32_t received_value = 0x00000000;

	while(1){

		X_mWriteReg(sent_value, 0xC0000000);

		while(received_value!=sent_value)
			received_value=X_mReadReg();

		if(sent_value<0xFFFFFFFF)
			sent_value++;

		else sent_value=0x00000001;

	}
}


//int main(void) {
//	int Status;
//	Status = I2CSlave();
//	if (Status != XST_SUCCESS) {
//		return XST_FAILURE;
//	}
//
//	return XST_SUCCESS;
//
//}
