/*
 * database_tests.c
 *
 *  Created on: Feb 9, 2024
 *      Author: tryfinally
 */

#include "tests/database_tests.h"
#include "tests/test_common.h"

#include "modules/db/m_db.h"
#include "modules/time_keeper/m_time.h"

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
	int sequence;
	unsigned int time_stamp;
	double temperature;
} SolarTelemetry;

static int sequence = 100;

static Boolean db_create_blob_test(void)
{
	SolarTelemetry solar;
	solar.sequence = 1;
	Time_getUnixEpoch(&solar.time_stamp);
	solar.temperature = 4.2;

	if (db_write_data_blob(TELEMETRY_SOLAR, &solar, sizeof(solar) , 1) == 1) {
		printf("Pass\n");
	} else {
		printf("Fail\n");
	}

	return TRUE;
}

static Boolean db_save_blob_test(void)
{
	SolarTelemetry solar[4];

	for (int i = 0; i < 4; ++i) {
		solar[i].sequence = sequence++;
		Time_getUnixEpoch(&solar[i].time_stamp);
		solar[i].temperature = 10.0  + i / (0.1 * sequence);
	}

	if (db_append_data_blob(TELEMETRY_SOLAR, solar, sizeof(SolarTelemetry), ARRAY_SIZE(solar))) {
		printf("Pass\n");
	} else {
		printf("Fail\n");
	}

	return TRUE;
}

static Boolean db_load_blob_test(void)
{
	SolarTelemetry solar[20];

	size_t res = db_read_data_blob(TELEMETRY_SOLAR, solar, sizeof(SolarTelemetry), 20);
	if (res == 0) {
		printf("Fail\r\n");
	}

	for (size_t i = 0; i < res; ++i) {
		printf("[%.3d] %.3d | ", i, solar[i].sequence);
		m_time_print_epoch(solar[i].time_stamp);
		printf("| %f\r\n", solar[i].temperature);
	}

	return TRUE;
}

static MenuAction menu[] = {
		{ db_write_simple_test, "DB create+write simple" },
		{ db_append_simple_test, "DB append simple" },
		{ db_read_simple_test, "DB read data simple" },
		{ db_delete_simple_test, "DB delete data simple" },
		{ db_create_blob_test, "DB Create Blob Data empty" },
		{ db_save_blob_test, "DB Save Blob data" },
		{ db_load_blob_test, "DB Read Blob data" },

		MENU_ITEM_END
};


Boolean database_tests(void)
{
	// Thursday, February 1, 2024 12:00:00 PM  GMT
	const unsigned int epoch_time = 1706788800;
	Time_setUnixEpoch(epoch_time);

	if (db_delete_data(TELEMETRY_SOLAR) == FALSE) {
		printf("error removon solar file\r\n");
	}
	MenuDisplay(menu);
	return TRUE;
}
