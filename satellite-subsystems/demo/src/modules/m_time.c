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

Boolean init_time(void)
{
	// Thursday, February 1, 2024 1:30:15 PM
	unsigned int epoch = 1706794215;
	unsigned int syncInterval = 60;

	Time time;
	Time_convertEpochToTime(epoch, &time);
	int result = Time_start(&time, syncInterval);
	if(result != 0) {
		TRACE_FATAL("\n\r Time Start failed: %d! \n\r", result);
			return FALSE;
	}
	TRACE_INFO("\t\t\tTime:     Started.");
	return TRUE;
}

void print_time(Time const *time)
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

void print_epoch(unsigned int epoch)
{
	Time time;
	Time_convertEpochToTime(epoch, &time);
	print_time(&time);
}
