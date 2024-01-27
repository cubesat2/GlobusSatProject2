/*
 * printutils.c
 *
 *  Created on: Jan 6, 2024
 *      Author: Muhammad Zahalqa
 */

#include "printutils.h"
#include <stdio.h>
#include <hal/Timing/Time.h>

void print_hex_array(unsigned char const* buffer, unsigned int length)
{
	if (!buffer || !length){
		return;
	}

	for (unsigned int i = 0; i < length; ++i) {
		printf("%02x ", buffer[i]);
	}

	printf("\r\n");
}

void print_time(Time const *time)
{
		//  y   m     d    h     m     s      reset   event
	printf("%4d, %.2d/%.2d %.2d:%.2d:%.2d",
			time->year + 2000,
			time->month,
			time->date,
			time->hours,
			time->minutes,
			time->seconds);
}
