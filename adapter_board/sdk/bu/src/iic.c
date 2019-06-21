/*
 * Title       :   iic.c
 * Authors     :   Razi Seyyedi, Quentin Dariol
 * Company     :   OFFIS
 * Date        :   13 August 2018
 * Description :   Functions defining the behavior of the BU MicroBlaze
 *                 regarding the I2C bus
 */

#include "iic.h"
#include <stdlib.h>

int SlaveWriteData1(XIic *InstancePtr);
int SlaveReadData1(XIic *InstancePtr);
static int SetupInterruptSystem(XIic * IicInstPtr, u8 IntrID);
static void StatusHandler1(XIic *InstancePtr, int Event);
static void SendHandler1(XIic *InstancePtr);
static void ReceiveHandler1(XIic *InstancePtr);

int SetupInitialized = 0;

/*****************************************************************************/
/**
* This function writes and reads the data as a slave. The IIC master on the bus
* initiates the transfers.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int I2CSlave_init()
{
	int Status;

	XIic_Config *ConfigPtr1; /* Pointer to configuration data */



	/*
	 * Initialize the IIC driver so that it is ready to use.
	 */
	ConfigPtr1 = XIic_LookupConfig(IIC_BU_ID);
	if (ConfigPtr1 == NULL) {
		return XST_FAILURE;
	}

	Status = XIic_CfgInitialize(&IicInstance1, ConfigPtr1,
			ConfigPtr1->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Setup the Interrupt System.
	 */
	Status = SetupInterruptSystem(&IicInstance1, IIC_INTR_ID_BU);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Include the Slave functions.
	 */
	XIic_SlaveInclude();

	/*
	 * Set the Transmit, Receive and Status Handlers.
	 */
	XIic_SetStatusHandler(&IicInstance1, &IicInstance1,
				  (XIic_StatusHandler) StatusHandler1);
	XIic_SetSendHandler(&IicInstance1, &IicInstance1,
				(XIic_Handler) SendHandler1);
	XIic_SetRecvHandler(&IicInstance1, &IicInstance1,
				(XIic_Handler) ReceiveHandler1);

	/*
	 * Set the Address as a RESPOND type.
	 */
	Status = XIic_SetAddress(&IicInstance1, XII_ADDR_TO_RESPOND_TYPE, IIC_BU_SLAVE_ADDR);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

//	SlaveReadData1(&IicInstance1);
//	SlaveWriteData1(&IicInstance1);

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* This function writes a buffer of bytes to the IIC bus when the IIC master
* initiates a read operation.
*
* @param	ByteCount contains the number of bytes in the buffer to be
*		written.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int SlaveWriteData1(XIic *InstancePtr) {
	int Status;
	u16 ByteCount = 6; // 32-bit value
	u8* SendBuffer;
	u32 Buffer_height, Buffer_temperature;

	Buffer_height       = bram_read(BU_BRAM_ADDR_HEIGHT      );
	Buffer_temperature  = bram_read(BU_BRAM_ADDR_TEMPERATURE );

	/*
	 * Allocation of memory for the Send Buffers
	 */
	SendBuffer=(u8*)malloc(ByteCount*sizeof(u8));
	if(SendBuffer==NULL) return XST_MEMTEST_FAILED;

	// ENDIAN and SPLITING
	// Big Endian Configuration
	SendBuffer[0]=(Buffer_height&0x000F);
	SendBuffer[1]=(Buffer_height&0x00F0)>>8;
	SendBuffer[2]=(Buffer_height&0x0F00)>>16;
	SendBuffer[3]=(Buffer_height&0xF000)>>24;
	SendBuffer[4]=(Buffer_temperature&0x000F);
	SendBuffer[5]=(Buffer_temperature&0x00F0)>>8;
//	SendBuffer[6]=(Buffer_temperature&0x0F00)>>16;
//	SendBuffer[7]=(Buffer_temperature&0xF000)>>24;
	/*
	 * Set the defaults.
	 */
	TransmitComplete1 = 1;

	/*
	 * Start the IIC device.
	 */
	Status = XIic_Start(InstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/*
	 * Set the Global Interrupt Enable.
	 */
	XIic_IntrGlobalEnable(InstancePtr->BaseAddress);

	/*
	 * Wait for AAS interrupt and transmission to complete.
	 */
	while ((TransmitComplete1) || (XIic_IsIicBusy(InstancePtr) == TRUE)) {
		if (SlaveWrite1) {
			XIic_SlaveSend(InstancePtr, SendBuffer, ByteCount);
			SlaveWrite1 = 0;
		}
	}

	/*
	 * Disable the Global Interrupt Enable bit.
	 */
	XIic_IntrGlobalDisable(InstancePtr->BaseAddress);

	/*
	 * Stop the IIC device.
	 */
	Status = XIic_Stop(InstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Free the memory used for the SendBuffer
	 */
	free(SendBuffer);

	return XST_SUCCESS;
}

int SlaveReadData1(XIic *InstancePtr) {
	int Status;
	u8* ReadBuffer;
	u32 Buffer_height;
	u32 Buffer_temperature;
	u16 ByteCount = 8;

	/*
	 * Set the defaults.
	 */
	ReceiveComplete1 = 1;

	/*
	 * Start the IIC device.
	 */
	Status = XIic_Start(InstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the Global Interrupt Enable.
	 */
	XIic_IntrGlobalEnable(InstancePtr->BaseAddress);

	/*
	 * Allocation of memory for the Read Buffers
	 */
	ReadBuffer=(u8*)malloc(8*sizeof(u8));
	if(ReadBuffer==NULL) return XST_MEMTEST_FAILED;

	/*
	 * Wait for AAS interrupt and completion of data reception.
	 */
	while ((ReceiveComplete1) || (XIic_IsIicBusy(InstancePtr) == TRUE)) {
		if (SlaveRead1) {
			XIic_SlaveRecv(InstancePtr, ReadBuffer, ByteCount);
			SlaveRead1 = 0;
		}
	}

	// ENDIAN and CONCATINATION
	// Big endian configuration

	Buffer_temperature=0;
	Buffer_temperature|=ReadBuffer[7];
	Buffer_temperature<<=8;
	Buffer_temperature|=ReadBuffer[6];
	Buffer_temperature<<=8;
	Buffer_temperature|=ReadBuffer[5];
	Buffer_temperature<<=8;
	Buffer_temperature|=ReadBuffer[4];
	Buffer_temperature<<=8;

	Buffer_height=0;
	Buffer_height|=ReadBuffer[3];
	Buffer_height<<=8;
	Buffer_height|=ReadBuffer[2];
	Buffer_height<<=8;
	Buffer_height|=ReadBuffer[1];
	Buffer_height<<=8;
	Buffer_height|=ReadBuffer[0];

//	bram_write (Buffer_height,       BU_BRAM_ADDR_HEIGHT      );
//	bram_write (Buffer_temperature,  BU_BRAM_ADDR_TEMPERATURE );

	/*
	 * Disable the Global Interrupt Enable.
	 */
	XIic_IntrGlobalDisable(InstancePtr->BaseAddress);

	/*
	 * Stop the IIC device.
	 */
	Status = XIic_Stop(InstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Free the memory used for the ReadBuffer
	 */
	free(ReadBuffer);

	return XST_SUCCESS;
}

/****************************************************************************/
/**
* This Status handler is called asynchronously from an interrupt context and
* indicates the events that have occurred.
*
* @param	InstancePtr is not used, but contains a pointer to the IIC
* 		device driver instance which the handler is being called for.
* @param	Event indicates whether it is a request for a write or read.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
static void StatusHandler1(XIic *InstancePtr, int Event)
{
	/*
	 * Check whether the Event is to write or read the data from the slave.
	 */
	if (Event == XII_MASTER_WRITE_EVENT) {
		/*
		 * Its a Write request from Master.
		 */
		SlaveRead1 = 1;
	} else {
		/*
		 * Its a Read request from the master.
		 */
		SlaveWrite1 = 1;
	}
}

/****************************************************************************/
/**
* This Send handler is called asynchronously from an interrupt
* context and indicates that data in the specified buffer has been sent.
*
* @param	InstancePtr is a pointer to the IIC driver instance for which
*		the handler is being called for.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
static void SendHandler1(XIic *InstancePtr)
{
	TransmitComplete1 = 0;
}

/****************************************************************************/
/**
* This Receive handler is called asynchronously from an interrupt
* context and indicates that data in the specified buffer has been Received.
*
* @param	InstancePtr is a pointer to the IIC driver instance for which
* 		the handler is being called for.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
static void ReceiveHandler1(XIic *InstancePtr)
{
	ReceiveComplete1 = 0;
}

/****************************************************************************/
/**
* This function setups the interrupt system so interrupts can occur for the
* IIC. The function is application-specific since the actual system may or
* may not have an interrupt controller. The IIC device could be directly
* connected to a processor without an interrupt controller. The user should
* modify this function to fit the application.
*
* @param	IicInstPtr contains a pointer to the instance of the IIC  which
*		is going to be connected to the interrupt controller.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
****************************************************************************/
static int SetupInterruptSystem(XIic * IicInstPtr, u8 IntrID) {
	int Status;

	if (!SetupInitialized)
	{
		/*
		 * Initialize the interrupt controller driver so that it's ready to use.
		 */
		Status = XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}
		SetupInitialized = 1;
	}
	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above
	 * performs the specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(&InterruptController, IntrID,
			(XInterruptHandler) XIic_InterruptHandler, IicInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/*
	 * Start the interrupt controller so interrupts are enabled for all
	 * devices that cause interrupts.
	 */
	Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the interrupts for the IIC device.
	 */
	XIntc_Enable(&InterruptController, IntrID);

	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				 (Xil_ExceptionHandler) XIntc_InterruptHandler,
				 &InterruptController);

	/*
	 * Enable non-critical exceptions.
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

