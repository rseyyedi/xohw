/*
 * Title       :   iic.h
 * Authors     :   Razi Seyyedi, Quentin Dariol
 * Company     :   OFFIS
 * Date        :   13 August 2018
 * Description :   Header for iic.c
 */

#ifndef __IIC_H_
#define __IIC_H_

/***************************** Include Files **********************************/
#include "xparameters.h"
#include "xiic.h"
#include "xintc.h"
#include "xil_exception.h"

/************************** Constant Definitions ******************************/
#define IIC_BU_ID		           XPAR_IIC_0_DEVICE_ID

#define INTC_DEVICE_ID		       XPAR_INTC_0_DEVICE_ID

#define IIC_INTR_ID_BU		       XPAR_INTC_0_IIC_0_VEC_ID

#define BU_BRAM_BASE_ADDRESS       XPAR_BU_AXI_BRAM_CTRL_1_S_AXI_BASEADDR
#define BU_BRAM_BASE_OFFSET        0x04

#define BU_BRAM_ADDR_HEIGHT        BU_BRAM_BASE_ADDRESS
#define BU_BRAM_ADDR_TEMPERATURE   BU_BRAM_ADDR_HEIGHT + BU_BRAM_BASE_OFFSET

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define IIC_BU_SLAVE_ADDR		0x60

/**************************** Type Definitions ********************************/

/************************** Function Prototypes *******************************/

int I2CSlave_init();
int SlaveReadData1(XIic *InstancePtr);
int SlaveWriteData1(XIic *InstancePtr);


/******************** Shortcuts for often used operations *********************/

/* Write data on the BU BRAM at given addr */
#define bram_write(data, addr) \
           *(uint32_t *)(addr) = ((uint32_t) data);

/* Read from the given addr of BU BRAM */
#define bram_read(addr) \
           *(uint32_t *)(addr);

/************************** Variable Definitions ******************************/
XIic IicInstance1; /* The instance of the IIC device. */
XIntc InterruptController; /* The instance of the Interrupt Controller */

volatile u8 TransmitComplete1;
volatile u8 ReceiveComplete1;

volatile u8 SlaveRead1;
volatile u8 SlaveWrite1;


#endif
