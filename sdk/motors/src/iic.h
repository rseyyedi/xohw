#ifndef __IIC_H_
#define __IIC_H_

/***************************** Include Files **********************************/
#include "xparameters.h"
#include "xiic.h"
#include "xintc.h"
#include "xil_exception.h"

/************************** Constant Definitions ******************************/
#define IIC_M1_ID		XPAR_IIC_0_DEVICE_ID
#define IIC_M2_ID		XPAR_IIC_1_DEVICE_ID
#define IIC_M3_ID		XPAR_IIC_2_DEVICE_ID
#define IIC_M4_ID		XPAR_IIC_3_DEVICE_ID

#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID

#define IIC_INTR_ID_M1		XPAR_INTC_0_IIC_0_VEC_ID
#define IIC_INTR_ID_M2		XPAR_INTC_0_IIC_1_VEC_ID
#define IIC_INTR_ID_M3		XPAR_INTC_0_IIC_2_VEC_ID
#define IIC_INTR_ID_M4		XPAR_INTC_0_IIC_3_VEC_ID

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define IIC_M1_SLAVE_ADDR		0x29
#define IIC_M2_SLAVE_ADDR		0x2A
#define IIC_M3_SLAVE_ADDR		0x2B
#define IIC_M4_SLAVE_ADDR		0x2C

#define M1
#define M2
#define M3
#define M4
/**************************** Type Definitions ********************************/

/************************** Function Prototypes *******************************/

int I2CSlave();

/************************** Variable Definitions ******************************/
XIic IicInstance1, IicInstance2, IicInstance3, IicInstance4; /* The instance of the IIC device. */
XIntc InterruptController; /* The instance of the Interrupt Controller */

volatile u8 TransmitComplete1;
volatile u8 ReceiveComplete1;

volatile u8 TransmitComplete2;
volatile u8 ReceiveComplete2;

volatile u8 TransmitComplete3;
volatile u8 ReceiveComplete3;

volatile u8 TransmitComplete4;
volatile u8 ReceiveComplete4;

volatile u8 SlaveRead1;
volatile u8 SlaveWrite1;

volatile u8 SlaveRead2;
volatile u8 SlaveWrite2;

volatile u8 SlaveRead3;
volatile u8 SlaveWrite3;

volatile u8 SlaveRead4;
volatile u8 SlaveWrite4;


#endif
