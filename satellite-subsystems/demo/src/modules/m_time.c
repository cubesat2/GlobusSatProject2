/*
 * m_time.c
 *
 *  Created on: Jan 27, 2024
 *      Author: tryfinally
 */

#include "m_time.h"

#include <hal/Timing/Time.h>
#include <hal/errors.h>
#include <at91/utility/trace.h>
#include <stdio.h>

Boolean m_time_init(void)
{
	// Monday, January 1, 2024 12:00:00 PM
	unsigned int epoch = 1704110400;
	unsigned int syncInterval = 60;

	Time time;
	Time_convertEpochToTime(epoch, &time);
	int result = Time_start(&time, syncInterval);
	if(result != 0) {
		TRACE_FATAL("\n\r Time Start failed: %d! \n\r", result);
		return FALSE;
	}
	return TRUE;
}


Boolean m_time_settime(uint32_t epoch)
{

	unsigned int unix_epoch = epoch;

	int result = Time_setUnixEpoch(unix_epoch);
	if(result != 0) {
		TRACE_FATAL("\n\r Set Time failed: %d! \n\r", result);
		return FALSE;
	}
	return TRUE;
}

void m_time_print_time(Time const *time)
{
		//  y   m     d    h     m     s
	printf("%4d, %.2d/%.2d %.2d:%.2d:%.2d",
			time->year + 2000,
			time->month,
			time->date,
			time->hours,
			time->minutes,
			time->seconds);
}

void m_time_print_epoch(unsigned int epoch)
{
	Time time;
	Time_convertEpochToTime(epoch, &time);
	m_time_print_time(&time);
}
