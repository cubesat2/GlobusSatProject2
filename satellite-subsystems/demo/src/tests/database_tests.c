/*
 * database_tests.c
 *
 *  Created on: Feb 9, 2024
 *      Author: tryfinally
 */

#include "database_tests.h"
#include "modules/m_db.h"
#include "modules/m_time.h"

#include "utils/input.h"
#include "utils/timeutils.h"
#include "utils/menu_selection.h"
#include "utils/ulog_sqlite.h"

#include <hal/Timing/Time.h>
#include <at91/utility/rand.h>
#include <hcc/api_fat.h>
#include <stdio.h>


static Boolean db_write_simple_test(void)
{
	if (db_write_data(TELEMETRY_RADFET_PAYLOAD, 0, 0)) {
		printf("Pass\n");
	} else {
		printf("Fail\n");
	}

	return TRUE;
}


static Boolean db_append_simple_test(void)
{
	if (db_append_data(TELEMETRY_RADFET_PAYLOAD, 0, 0)) {
		printf("Pass\n");
	} else {
		printf("Fail\n");
	}

	return TRUE;
}


static Boolean db_read_simple_test(void)
{
	if (db_read_data(TELEMETRY_RADFET_PAYLOAD, 0, 0)) {
		printf("Pass\n");
	} else {
		printf("Fail\n");
	}

	return TRUE;
}


static Boolean db_delete_simple_test(void)
{
	if (db_delete_data(TELEMETRY_RADFET_PAYLOAD)) {
		printf("Pass\n");
	} else {
		printf("Fail\n");
	}

	return TRUE;
}

typedef struct SolarTelemetry {
	unsigned int time_stamp;
	double temperature;
} SolarTelemetry;

static Boolean db_save_blob_test(void)
{
	SolarTelemetry solar[3];

	for (int i = 0; i < 3; ++i) {
		Time_getUnixEpoch(&solar[i].time_stamp);
		solar[i].temperature = 16.1 + i;
	}

	if (db_write_data_blob(TELEMETRY_SOLAR, solar, sizeof(SolarTelemetry), 3)) {
		printf("Pass\n");
	} else {
		printf("Fail\n");
	}

	return TRUE;
}

static Boolean db_load_blob_test(void)
{
	SolarTelemetry solar[3];

	Boolean res = db_read_data_blob(TELEMETRY_SOLAR, solar, sizeof(SolarTelemetry), 3);
	if (res == FALSE) {
		printf("Fail\r\n");
	}

	for (int i = 0; i < 3; ++i) {
		print_epoch(solar[i].time_stamp);
		printf("| %d, %f\r\n", solar[i].time_stamp, solar[i].temperature);
	}

	return TRUE;
}

static MenuAction menu[] = {
		{ db_write_simple_test, "DB create+write simple" },
		{ db_append_simple_test, "DB append simple" },
		{ db_read_simple_test, "DB read data simple" },
		{ db_delete_simple_test, "DB delete data simple" },
		{ db_save_blob_test, "DB Save Blob data" },
		{ db_load_blob_test, "DB Read Blob data" },

		MENU_ITEM_END
};


Boolean database_tests(void)
{
	// Thursday, February 1, 2024 12:00:00 PM  GMT
	const unsigned int epoch_time = 1706788800;
	Time_setUnixEpoch(epoch_time);

	m_db_init();

	MenuDisplay(menu);
	return TRUE;
}
