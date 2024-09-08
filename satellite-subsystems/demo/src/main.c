/*
 * main.c
 *      Author: Akhil
 */

//#define DEBUG_BAUD_RATE 115200
#define DEBUG_BAUD_RATE 2000000

#define  OBC_LAUNCH_TEST_TASK

#ifdef OBC_LAUNCH_TEST_TASK
#include "tests/all_tests.h"
#endif

#include <modules/sat_id.h>
#include "modules/modules_manager.h"
#include "modules/trxvu/m_trxvu.h"
#include "utils/timeutils.h"
#include "utils/menu_selection.h"

#include <satellite-subsystems/version/version.h>

#include <at91/utility/exithandler.h>
#include <at91/commons.h>
#include <at91/utility/trace.h>
#include <at91/peripherals/cp15/cp15.h>
#include <at91/peripherals/pio/pio_it.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <hal/Utility/util.h>
#include <hal/Timing/WatchDogTimer.h>
#include <hal/Drivers/LED.h>
#include <hal/Drivers/SPI.h>
#include <hal/boolean.h>
#include <hal/version/version.h>

#include <stdlib.h>
#include <stdio.h>
#include "config/flight_params.h"
#define ENABLE_MAIN_TRACES 1
#if ENABLE_MAIN_TRACES
	#define MAIN_TRACE_INFO			TRACE_INFO
	#define MAIN_TRACE_DEBUG		TRACE_DEBUG
	#define MAIN_TRACE_WARNING		TRACE_WARNING
	#define MAIN_TRACE_ERROR		TRACE_ERROR
	#define MAIN_TRACE_FATAL		TRACE_FATAL
#else
	#define MAIN_TRACE_INFO(...)	{ }
	#define MAIN_TRACE_DEBUG(...)	{ }
	#define MAIN_TRACE_WARNING(...)	{ }
	#define MAIN_TRACE_ERROR		TRACE_ERROR
	#define MAIN_TRACE_FATAL		TRACE_FATAL
#endif


void infinite_looper()
{
	for(;;) {
		LED_toggle(led_1);
		vTaskDelay(500);
	}
}

void taskTestMain()
{
	delay_ms(4200);
	do {
		LED_toggle(led_1);
	} while(all_tests());

	infinite_looper();
}

void taskMain()
{
	mm_init();

#ifdef OBC_LAUNCH_TEST_TASK
	xTaskHandle taskTestMainHandle;
	xTaskGenericCreate(taskTestMain, (const signed char*)"taskTestMain", 4096, NULL, configMAX_PRIORITIES-2, &taskTestMainHandle, NULL, NULL);
#endif

	TRACE_INFO("%s Flight Software Ready", SAT_CALLSIGN );
	for (;;) {
		trxvu_logic();

		delay_ms(200);
	}

	infinite_looper();
}


int main()
{
	TRACE_CONFIGURE_ISP(DBGU_STANDARD, DEBUG_BAUD_RATE, BOARD_MCK);
	// Enable the Instruction cache of the ARM9 core. Keep the MMU and Data Cache disabled.
	CP15_Enable_I_Cache();

	LED_start();

	// The actual watchdog is already started, this only initializes the watchdog-kick interface.
	WDT_start();
	PIO_InitializeInterrupts(AT91C_AIC_PRIOR_LOWEST+4);
	WDT_startWatchdogKickTask(10 / portTICK_RATE_MS, FALSE);

	//printf("\n\n" SAT_CALLSIGN " CubeSat Flight Software. built on %s at %s\n", __DATE__, __TIME__);
	printf("* Sat Subsys lib version %s.%s.%s built on %s at %s\n",
			SatelliteSubsystemsVersionMajor, SatelliteSubsystemsVersionMinor, SatelliteSubsystemsVersionRevision,
			SatelliteSubsystemsCompileDate, SatelliteSubsystemsCompileTime);
	printf("* HAL lib version %s.%s.%s built on %s at %s\n", HalVersionMajor, HalVersionMinor, HalVersionRevision,
			HalCompileDate, HalCompileTime);

	LED_wave(1);
	LED_waveReverse(1);
	LED_wave(1);
	LED_waveReverse(1);


	MAIN_TRACE_INFO("\t main: Starting tasks... \n\r");

	xTaskHandle taskMainHandle;
	xTaskGenericCreate(taskMain, (const signed char*)"taskMain", 4096, NULL, configMAX_PRIORITIES-2, &taskMainHandle, NULL, NULL);


	MAIN_TRACE_DEBUG("\t main: Starting scheduler.. \n\r");
	vTaskStartScheduler();

	// This part should never be reached.
	MAIN_TRACE_ERROR("\t main: Unexpected end of scheduling \n\r");

	//Flash some LEDs for about 100 seconds
	for (int i=0; i < 2500; i++)
	{
		LED_wave(1);
		MAIN_TRACE_DEBUG("MAIN: STILL ALIVE %d\n\r", i);
	}
	exit(0);
}
