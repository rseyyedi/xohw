/*
 * timer_lib.c
 *
 *  Created on: Jul 17, 2015
 *      Author: sschreiner
 */

#include "timer_lib.h"

#include "xintc.h"
#include "xil_exception.h"
#include "xtmrctr.h"

#define INTC					XIntc						/*!< global name define */
#define INTC_HANDLER			XIntc_InterruptHandler		/*!< global name define */
#define MAIN_TIMER_ID			0							/*!< timer id of the ip core */
#define TIMER_2ms				0xFFFCF2C1					/*!< timer reload value for 2ms cycle */

static XTmrCtr tmr_ctr; /*!< instance of the timer ip core */
static XIntc intc; /*!< Instance of the interrupt ip core*/

static volatile uint32_t time = 0;

static void timer_ctr_disable_intr(INTC* IntcInstancePtr, u16 IntrId);
static int timer_ctr_setup_intr_system(INTC* IntcInstancePtr, XTmrCtr* InstancePtr, u16 DeviceId, u16 IntrId, u8 TmrCtrNumber, u16 IntcDeviceId);
static void timer_counter_handler(void *CallBackRef, u8 TmrCtrNumber);

timer_error_e timer_init(void)
{
	uint32_t status = 0;
	timer_error_e result = TIMER_NO_ERROR;
	/************************** intial the 2ms maintimer  ************************************/
	/*
	 * The Interrupt controller is already initialized from the gam, so just the second timer
	 * the MAIN_TIMER has to Initialized
	 */

	/*
	 * Initialize the timer counter so that it's ready to use,
	 * specify the device ID that is generated in xparameters.h
	 */
	status = XTmrCtr_Initialize(&tmr_ctr, MAIN_TIMER_ID);

	if (status != XST_SUCCESS)
	{
		result |= TIMER_INIT_ERROR;
	}

	/* Perform a self-test to ensure that the hardware was built
	 * correctly, use the 1st timer in the device (0)
	 */
	status = XTmrCtr_SelfTest(&tmr_ctr, MAIN_TIMER_ID);

	if (status != XST_SUCCESS)
	{
		result |= TIMER_SELFTEST_ERROR;
	}

	/*
	 * Connect the timer counter to the interrupt subsystem such that
	 * interrupts can occur.  This function is application specific.
	 */
	status = timer_ctr_setup_intr_system(&intc,
					&tmr_ctr,
					XPAR_TMRCTR_0_DEVICE_ID,
					XPAR_INTC_0_TMRCTR_0_VEC_ID,
					MAIN_TIMER_ID,
					XPAR_INTC_0_DEVICE_ID);

	if (status != XST_SUCCESS)
	{
		result |= TIMER_INTERRUPT_ERROR;
	}

	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only
	 */
	XTmrCtr_SetOptions(&tmr_ctr, MAIN_TIMER_ID, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);

	XTmrCtr_SetResetValue(&tmr_ctr, MAIN_TIMER_ID, TIMER_2ms);

	XTmrCtr_SetHandler(&tmr_ctr, timer_counter_handler, &tmr_ctr);
	/*
	 * if there was any initialization errors, don't start the main timer
	 */
	if( result == TIMER_NO_ERROR )
	{
		XTmrCtr_Start(&tmr_ctr, MAIN_TIMER_ID);
	}

	return result;
}

/*****************************************************************************/
/**
* This function is the handler which performs processing for the timer counter.
* It is called from an interrupt context such that the amount of processing
* performed should be minimized.  It is called when the timer counter expires
* if interrupts are enabled.
*
* This handler provides an example of how to handle timer counter interrupts
* but is application specific.
*
* @param	CallBackRef is a pointer to the callback function
* @param	TmrCtrNumber is the number of the timer to which this
*		handler is associated with.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void *xps_timer_Addr =(void*) 0x41C00008;
uint32_t runs = 0;
void timer_counter_handler(void *CallBackRef, u8 TmrCtrNumber)
{
//	XTmrCtr *InstancePtr = (XTmrCtr *)CallBackRef;
	/*
	 * Check if the timer counter has expired, checking is not necessary
	 * since that's the reason this function is executed, this just shows
	 * how the callback reference can be used as a pointer to the instance
	 * of the timer counter that expired, increment a shared variable so
	 * the main thread of execution can see the timer expired
	 */
	time+=2;

	runs = *(int*)xps_timer_Addr;
//	if (XTmrCtr_IsExpired(InstancePtr, TmrCtrNumber)) {
//		// read sensor values
//		updateAttitude(&sensorData, &attitude);
//		bram_write_flight_attitude(FLIGHT_ATTITIUDE_ADR, &attitude);
//		// get called all 2ms
//		glb_timer_flag = TRUE;
//	}
}

/*****************************************************************************/
/**
* This function setups the interrupt system such that interrupts can occur
* for the timer counter. This function is application specific since the actual
* system may or may not have an interrupt controller.  The timer counter could
* be directly connected to a processor without an interrupt controller.  The
* user should modify this function to fit the application.
*
* @param	IntcInstancePtr is a pointer to the Interrupt Controller
*		driver Instance.
* @param	TmrCtrInstancePtr is a pointer to the XTmrCtr driver Instance.
* @param	DeviceId is the XPAR_<TmrCtr_instance>_DEVICE_ID value from
*		xparameters.h.
* @param	IntrId is XPAR_<INTC_instance>_<TmrCtr_instance>_VEC_ID
*		value from xparameters.h.
* @param	TmrCtrNumber is the number of the timer to which this
*		handler is associated with.
*
* @return	XST_SUCCESS if the Test is successful, otherwise XST_FAILURE.
*
* @note		This function contains an infinite loop such that if interrupts
*		are not working it may never return.
*
******************************************************************************/
static int timer_ctr_setup_intr_system(INTC* IntcInstancePtr,
				 XTmrCtr* TmrCtrInstancePtr,
				 u16 DeviceId,
				 u16 IntrId,
				 u8 TmrCtrNumber,
				 u16 IntcDeviceId)
{
	 uint32_t status;

#ifdef XPAR_INTC_0_DEVICE_ID
#ifndef TESTAPP_GEN
	/*
	 * Initialize the interrupt controller driver so that
	 * it's ready to use, specify the device ID that is generated in
	 * xparameters.h
	 */
	status = XIntc_Initialize(IntcInstancePtr, IntcDeviceId);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif
	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the specific
	 * interrupt processing for the device
	 */
	status = XIntc_Connect(IntcInstancePtr, IntrId,
				(XInterruptHandler)XTmrCtr_InterruptHandler,
				(void *)TmrCtrInstancePtr);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

#ifndef TESTAPP_GEN
	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts, specific real mode so that
	 * the timer counter can cause interrupts thru the interrupt controller.
	 */
	status = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif

	/*
	 * Enable the interrupt for the timer counter
	 */
	XIntc_Enable(IntcInstancePtr, IntrId);

#else

#ifndef TESTAPP_GEN
	XScuGic_Config *IntcConfig;

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(IntcDeviceId);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif /* TESTAPP_GEN */

	XScuGic_SetPriorityTriggerType(IntcInstancePtr, IntrId,
					0xA0, 0x3);

	/*
	 * Connect the interrupt handler that will be called when an
	 * interrupt occurs for the device.
	 */
	status = XScuGic_Connect(IntcInstancePtr, IntrId,
				 (Xil_ExceptionHandler)XTmrCtr_InterruptHandler,
				 TmrCtrInstancePtr);
	if (status != XST_SUCCESS) {
		return status;
	}

	/*
	 * Enable the interrupt for the Timer device.
	 */
	XScuGic_Enable(IntcInstancePtr, IntrId);
#endif /* XPAR_INTC_0_DEVICE_ID */


#ifndef TESTAPP_GEN
	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
					(Xil_ExceptionHandler)
					INTC_HANDLER,
					IntcInstancePtr);

	/*
	 * Enable non-critical exceptions.
	 */
	Xil_ExceptionEnable();

#endif
	return XST_SUCCESS;
}

/******************************************************************************/
/**
*
* This function disables the interrupts for the Timer.
*
* @param	IntcInstancePtr is a reference to the Interrupt Controller
*		driver Instance.
* @param	IntrId is XPAR_<INTC_instance>_<Timer_instance>_VEC_ID
*		value from xparameters.h.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void timer_ctr_disable_intr(INTC* IntcInstancePtr, u16 IntrId)
{
	/*
	 * Disable the interrupt for the timer counter
	 */
#ifdef XPAR_INTC_0_DEVICE_ID
	XIntc_Disable(IntcInstancePtr, IntrId);
#else
	/* Disconnect the interrupt */
	XScuGic_Disable(IntcInstancePtr, IntrId);
	XScuGic_Disconnect(IntcInstancePtr, IntrId);
#endif

	return;
}

/******************************************************************************/
/**
 *
 * This function gives back the current time in ms.
 *
 *
 * @return          time in ms
 *
 * @note                             None.
 *
 ******************************************************************************/
uint32_t timer_get_current_system_time_MS(void)
{
	return time;
}

////////////////////////////////////////////////////////////////////////////////////////////
////// Timer functions
////////////////////////////////////////////////////////////////////////////////////////////

uint32_t timer_set_delay(uint32_t delay)
{
  return(time + (delay - 1));
}

uint8_t timer_check_delay(uint32_t delay)
{
  return((((delay-1) - time) & 0x8000) >> 9);
}

void timer_wait_for(uint32_t delay)
{
	uint32_t delay_end = timer_set_delay(delay);

	while(!timer_check_delay(delay_end));
}
