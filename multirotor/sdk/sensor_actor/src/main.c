/*
 * main.c
 *
 *  Created on: Jul 17, 2015
 *      Author: sschreiner
 */

#include "platform.h"

int main(void)
{
	// Init platform
	platform_error_e status = PLATFORM_NO_ERROR;
	status = platform_init();

	if (status == PLATFORM_NO_ERROR)
	{
		// Execute platform if initialization was fine
		while (1)
		{
			platform_execute();
		}
	}

	return 0;
}
