#ifndef __IIC_H_
#define __IIC_H_

/***************************** Include Files **********************************/
#include "xparameters.h"
#include "xiic.h"
#include "xintc.h"
#include "xil_exception.h"

/************************** Constant Definitions ******************************/
#define IIC_IMU1_ID		XPAR_IIC_0_DEVICE_ID
#define IIC_IMU2_ID		XPAR_IIC_1_DEVICE_ID

#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID

#define IIC_INTR_ID_IMU1		XPAR_INTC_0_IIC_0_VEC_ID
#define IIC_INTR_ID_IMU2		XPAR_INTC_0_IIC_1_VEC_ID


/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define IIC_IMU1_SLAVE_ADDR		0x68
#define IIC_IMU2_SLAVE_ADDR		0x0C


#define IMU1
#define IMU2
/**************************** Type Definitions ********************************/

/************************** Function Prototypes *******************************/

int I2CSlave();

/************************** Variable Definitions ******************************/
XIic IicInstance1, IicInstance2; /* The instance of the IIC device. */
XIntc InterruptController; /* The instance of the Interrupt Controller */

volatile u8 TransmitComplete1;
volatile u8 ReceiveComplete1;

volatile u8 TransmitComplete2;
volatile u8 ReceiveComplete2;

volatile u8 SlaveRead1;
volatile u8 SlaveWrite1;

volatile u8 SlaveRead2;
volatile u8 SlaveWrite2;

/* Write to memory location or register */
#define X_mWriteReg(data, addr) \
           *(uint32_t *)(addr + 0x00) = ((uint32_t) data);
/* Read from memory location or register */
#define X_mReadReg(addr) \
           *(uint32_t *)(addr + 0x04);


#endif
