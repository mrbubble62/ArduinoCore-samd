#ifndef SLEEP_H_INCLUDED
#define SLEEP_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

enum  	status_categories { 
  STATUS_CATEGORY_OK = 0x00, 
  STATUS_CATEGORY_COMMON = 0x10, 
  STATUS_CATEGORY_ANALOG = 0x30, 
  STATUS_CATEGORY_COM = 0x40, 
  STATUS_CATEGORY_IO = 0x50 
};

enum  status_code { 
  STATUS_OK = STATUS_CATEGORY_OK | 0x00, 
  STATUS_VALID_DATA = STATUS_CATEGORY_OK | 0x01, 
  STATUS_NO_CHANGE = STATUS_CATEGORY_OK | 0x02, 
  STATUS_ABORTED = STATUS_CATEGORY_OK | 0x04, 
  STATUS_BUSY = STATUS_CATEGORY_OK | 0x05, 
  STATUS_SUSPEND = STATUS_CATEGORY_OK | 0x06, 
  STATUS_ERR_IO = STATUS_CATEGORY_COMMON | 0x00, 
  STATUS_ERR_REQ_FLUSHED = STATUS_CATEGORY_COMMON | 0x01, 
  STATUS_ERR_TIMEOUT = STATUS_CATEGORY_COMMON | 0x02, 
  STATUS_ERR_BAD_DATA = STATUS_CATEGORY_COMMON | 0x03, 
  STATUS_ERR_NOT_FOUND = STATUS_CATEGORY_COMMON | 0x04, 
  STATUS_ERR_UNSUPPORTED_DEV = STATUS_CATEGORY_COMMON | 0x05, 
  STATUS_ERR_NO_MEMORY = STATUS_CATEGORY_COMMON | 0x06, 
  STATUS_ERR_INVALID_ARG = STATUS_CATEGORY_COMMON | 0x07, 
  STATUS_ERR_BAD_ADDRESS = STATUS_CATEGORY_COMMON | 0x08, 
  STATUS_ERR_BAD_FORMAT = STATUS_CATEGORY_COMMON | 0x0A, 
  STATUS_ERR_BAD_FRQ = STATUS_CATEGORY_COMMON | 0x0B, 
  STATUS_ERR_DENIED = STATUS_CATEGORY_COMMON | 0x0c, 
  STATUS_ERR_ALREADY_INITIALIZED = STATUS_CATEGORY_COMMON | 0x0d, 
  STATUS_ERR_OVERFLOW = STATUS_CATEGORY_COMMON | 0x0e, 
  STATUS_ERR_NOT_INITIALIZED = STATUS_CATEGORY_COMMON | 0x0f, 
  STATUS_ERR_SAMPLERATE_UNAVAILABLE = STATUS_CATEGORY_ANALOG | 0x00, 
  STATUS_ERR_RESOLUTION_UNAVAILABLE = STATUS_CATEGORY_ANALOG | 0x01, 
  STATUS_ERR_BAUDRATE_UNAVAILABLE = STATUS_CATEGORY_COM | 0x00, 
  STATUS_ERR_PACKET_COLLISION = STATUS_CATEGORY_COM | 0x01, 
  STATUS_ERR_PROTOCOL = STATUS_CATEGORY_COM | 0x02, 
  STATUS_ERR_PIN_MUX_INVALID = STATUS_CATEGORY_IO | 0x00 
};



/**
 * \brief Device sleep modes.
 *
 * List of available sleep modes in the device. A table of clocks available in
 * different sleep modes can be found in \ref asfdoc_sam0_system_module_overview_sleep_mode.
 */
enum system_sleepmode {
	/** IDLE 0 sleep mode. */
	SYSTEM_SLEEPMODE_IDLE_0,
	/** IDLE 1 sleep mode. */
	SYSTEM_SLEEPMODE_IDLE_1,
	/** IDLE 2 sleep mode. */
	SYSTEM_SLEEPMODE_IDLE_2,
	/** Standby sleep mode. */
	SYSTEM_SLEEPMODE_STANDBY,
};



/**
 * \name Device sleep
 * @{
 */

/**
 * \brief Set the sleep mode of the device
 *
 * Sets the sleep mode of the device; the configured sleep mode will be entered
 * upon the next call of the \ref system_sleep() function.
 *
 * For an overview of which systems are disabled in sleep for the different
 * sleep modes, see \ref asfdoc_sam0_system_module_overview_sleep_mode.
 *
 * \param[in] sleep_mode  Sleep mode to configure for the next sleep operation
 *
 * \retval STATUS_OK               Operation completed successfully
 * \retval STATUS_ERR_INVALID_ARG  The requested sleep mode was invalid or not
 *                                 available
 */
static inline enum status_code system_set_sleepmode(
	const enum system_sleepmode sleep_mode)
{
	switch (sleep_mode) {
		case SYSTEM_SLEEPMODE_IDLE_0:
		case SYSTEM_SLEEPMODE_IDLE_1:
		case SYSTEM_SLEEPMODE_IDLE_2:
			SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
			PM->SLEEP.reg = sleep_mode;
			break;

		case SYSTEM_SLEEPMODE_STANDBY:
			SCB->SCR |=  SCB_SCR_SLEEPDEEP_Msk;
			break;

		default:
			return STATUS_ERR_INVALID_ARG;
	}

	return STATUS_OK;
}

/**
 * \brief Put the system to sleep waiting for interrupt
 *
 * Executes a device DSB (Data Synchronization Barrier) instruction to ensure
 * all ongoing memory accesses have completed, then a WFI (Wait For Interrupt)
 * instruction to place the device into the sleep mode specified by
 * \ref system_set_sleepmode until woken by an interrupt.
 */
static inline void system_sleep(void)
{
	__DSB();
	__WFI();
}




#ifdef __cplusplus
}
#endif

#endif /* SLEEPM_H_INCLUDED */
