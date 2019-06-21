/*
 * Title       :   main.c
 * Authors     :   Razi Seyyedi, Quentin Dariol
 * Company     :   OFFIS
 * Date        :   13 August 2018
 * Description :   main function of the BU Microblaze of the adapter board
 */

#include "xparameters.h"
#include "iic.h"

int main (void) {
	uint32_t height      =   0xDEADBEEF;
	uint32_t temperature =   0xBABEFACE;
	uint32_t status = 0;

	/*
	 * Writing some simple values in the BRAM
	 * They will be then sent through I2C to the
	 * flight control system
	 */
	bram_write (height,       BU_BRAM_ADDR_HEIGHT       );
	bram_write (temperature,  BU_BRAM_ADDR_TEMPERATURE  );

	/*
	 * Initialization of the I2C protocol
	 * between BU microblaze and flight control system
	 */
	if (I2CSlave_init() != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	else
	{
		/*
		 * Main loop executing the I2C Slave behavior
		 * (The I2C functions are called using an
		 * interrupt procedure, set by the function
		 * I2CSlave_init() )
		 */
		status = SlaveReadData1(&IicInstance1);
		status = SlaveReadData1(&IicInstance1);
		while (1)
		{
//			status = SlaveReadData1(&IicInstance1);
			status = SlaveWriteData1(&IicInstance1);
		}
	}
}

