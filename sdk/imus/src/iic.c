/*****************************************************************************/
/**
 * @file iic.c
 *
 * iic driver
 *
 * </pre>
 *
 ****************************************************************************/

#include "iic.h"

int SlaveWriteData1(u8 *SendBuffer, u16 ByteCount, XIic *InstancePtr);
int SlaveWriteData2(u8 *SendBuffer, u16 ByteCount, XIic *InstancePtr);
int SlaveReadData1(u8 *BufferPtr, u16 ByteCount, XIic *InstancePtr);
int SlaveReadData2(u8 *BufferPtr, u16 ByteCount, XIic *InstancePtr);
static int SetupInterruptSystem(XIic * IicInstPtr, u8 IntrID);
static void StatusHandler1(XIic *InstancePtr, int Event);
static void StatusHandler2(XIic *InstancePtr, int Event);
static void SendHandler1(XIic *InstancePtr);
static void SendHandler2(XIic *InstancePtr);
static void ReceiveHandler1(XIic *InstancePtr);
static void ReceiveHandler2(XIic *InstancePtr);

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
int I2CSlave ()
{
	int Status;
	int ByteCount = 1;
	u8 RecvBuffer = 0;
	u8 SendBuffer1 = 0x1;
	u8 SendBuffer2 = 0x2;

	XIic_Config *ConfigPtr1; /* Pointer to configuration data */
	XIic_Config *ConfigPtr2; /* Pointer to configuration data */

	/*
	 * Initialize the IIC driver so that it is ready to use.
	 */
#ifdef IMU1
	ConfigPtr1 = XIic_LookupConfig(IIC_IMU1_ID);
	if (ConfigPtr1 == NULL) {
		return XST_FAILURE;
	}
#endif
#ifdef IMU2
	ConfigPtr2 = XIic_LookupConfig(IIC_IMU2_ID);
	if (ConfigPtr2 == NULL) {
		return XST_FAILURE;
	}
#endif

#ifdef IMU1
	Status = XIic_CfgInitialize(&IicInstance1, ConfigPtr1,
			ConfigPtr1->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif
#ifdef IMU2
	Status = XIic_CfgInitialize(&IicInstance2, ConfigPtr2,
			ConfigPtr2->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif

	/*
	 * Setup the Interrupt System.
	 */
#ifdef IMU1
	Status = SetupInterruptSystem(&IicInstance1, IIC_INTR_ID_IMU1);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif
#ifdef IMU2
	Status = SetupInterruptSystem(&IicInstance2, IIC_INTR_ID_IMU2);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif

	/*
	 * Include the Slave functions.
	 */
	XIic_SlaveInclude();

	/*
	 * Set the Transmit, Receive and Status Handlers.
	 */
#ifdef IMU1
	XIic_SetStatusHandler(&IicInstance1, &IicInstance1,
				  (XIic_StatusHandler) StatusHandler1);
	XIic_SetSendHandler(&IicInstance1, &IicInstance1,
				(XIic_Handler) SendHandler1);
	XIic_SetRecvHandler(&IicInstance1, &IicInstance1,
				(XIic_Handler) ReceiveHandler1);
#endif
#ifdef IMU2
	XIic_SetStatusHandler(&IicInstance2, &IicInstance2,
			(XIic_StatusHandler) StatusHandler2);
	XIic_SetSendHandler(&IicInstance2, &IicInstance2,
			(XIic_Handler) SendHandler2);
	XIic_SetRecvHandler(&IicInstance2, &IicInstance2,
			(XIic_Handler) ReceiveHandler2);
#endif

	/*
	 * Set the Address as a RESPOND type.
	 */
#ifdef IMU1
	Status = XIic_SetAddress(&IicInstance1, XII_ADDR_TO_RESPOND_TYPE, IIC_IMU1_SLAVE_ADDR);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif
#ifdef IMU2
	Status = XIic_SetAddress(&IicInstance2, XII_ADDR_TO_RESPOND_TYPE, IIC_IMU2_SLAVE_ADDR);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif

#ifdef IMU1
	SlaveReadData1(&RecvBuffer, ByteCount, &IicInstance1);
	SlaveWriteData1(&SendBuffer1, ByteCount, &IicInstance1);
#endif

#ifdef IMU2
	SlaveReadData2(&RecvBuffer, ByteCount, &IicInstance2);
	SlaveWriteData2(&SendBuffer2, ByteCount, &IicInstance2);
#endif

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
int SlaveWriteData1(u8 *SendBuffer, u16 ByteCount, XIic *InstancePtr) {
	int Status;

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

	return XST_SUCCESS;
}

int SlaveWriteData2(u8 *SendBuffer, u16 ByteCount, XIic *InstancePtr) {
	int Status;

	/*
	 * Set the defaults.
	 */
	TransmitComplete2 = 1;

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
	while ((TransmitComplete2) || (XIic_IsIicBusy(InstancePtr) == TRUE)) {
		if (SlaveWrite2) {
			XIic_SlaveSend(InstancePtr, SendBuffer, ByteCount);
			SlaveWrite2 = 0;
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

	return XST_SUCCESS;
}

int SlaveReadData1(u8 *ReadBuffer, u16 ByteCount, XIic *InstancePtr) {
	int Status;

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
	 * Wait for AAS interrupt and completion of data reception.
	 */
	while ((ReceiveComplete1) || (XIic_IsIicBusy(InstancePtr) == TRUE)) {
		if (SlaveRead1) {
			XIic_SlaveRecv(InstancePtr, ReadBuffer, ByteCount);
			SlaveRead1 = 0;
		}
	}

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

	return XST_SUCCESS;
}

int SlaveReadData2(u8 *ReadBuffer, u16 ByteCount, XIic *InstancePtr) {
	int Status;

	/*
	 * Set the defaults.
	 */
	ReceiveComplete2 = 1;

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
	 * Wait for AAS interrupt and completion of data reception.
	 */
	while ((ReceiveComplete2) || (XIic_IsIicBusy(InstancePtr) == TRUE)) {
		if (SlaveRead2) {
			XIic_SlaveRecv(InstancePtr, ReadBuffer, ByteCount);
			SlaveRead2 = 0;
		}
	}

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

static void StatusHandler2(XIic *InstancePtr, int Event)
{
	/*
	 * Check whether the Event is to write or read the data from the slave.
	 */
	if (Event == XII_MASTER_WRITE_EVENT) {
		/*
		 * Its a Write request from Master.
		 */
		SlaveRead2 = 1;
	} else {
		/*
		 * Its a Read request from the master.
		 */
		SlaveWrite2 = 1;
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

static void SendHandler2(XIic *InstancePtr)
{
	TransmitComplete2 = 0;
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

static void ReceiveHandler2(XIic *InstancePtr)
{
	ReceiveComplete2 = 0;
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

