/*
 * time_tests.c
 *
 *  Created on: Jan 27, 2024
 *      Author: tryfinally
 */

#include "time_tests.h"

#include "utils/input.h"
#include "utils/timeutils.h"
#include "utils/menu_selection.h"
#include "utils/printutils.h"

#include <hal/Timing/Time.h>
#include <at91/utility/rand.h>
#include <stdio.h>


Boolean init_time(void)
{
	Time time;
	time.year = 24;
	time.month = 2;
	time.date = 1;
	time.hours = 12;
	time.minutes = 30;
	time.seconds = 15;
	time.day = 5;

	unsigned int syncInterval = 10;
	int r = Time_start(&time, syncInterval);
	if (r == 0) {
		printf("Time initialized\n");
	} else {
		printf("Error initing time %d\n", r);
	}

	if (Time_isRTCworking()) {
		printf("RTC is working\n");
	} else {
		printf("RTC is NOT working\n");
	}

	return TRUE;
}

Boolean get_current_time(void)
{
	unsigned int epoch = 0;

	for (int i = 0; i < 10; ++i) {
		int r = Time_getUnixEpoch(&epoch);
		if (r == 0) {
			printf("Epoch: %u\n", epoch);
		} else {
			printf("get time failed\n");
			break;
		}
	}

	return TRUE;
}

Boolean set_current_time(void)
{
	unsigned int epoch_time = 1706788800;
	Time_setUnixEpoch(epoch_time);
	printf("Time set to:");

	Time time;
	Time_get(&time);
	print_time(&time);
	printf("\n");

	epoch_time = INPUT_GetUINT32("Enter new epoch:");
	int r = Time_setUnixEpoch(epoch_time);
	if (r == 0) {
		printf("Time set to:");
		Time_get(&time);
		print_time(&time);
		printf("\n");
	} else {
		printf("Error setting time: %d\n", r);
	}
	return TRUE;
}

static MenuAction menu[] = {
		{ init_time, "Initialize Time" },
		{ get_current_time, "Get Current time" },
		{ set_current_time, "Set Current time" },
		MENU_ITEM_END
};

Boolean time_tests(void)
{
	MenuDisplay(menu);
	return TRUE;
}
