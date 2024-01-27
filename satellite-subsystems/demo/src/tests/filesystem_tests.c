/*
 * filesystem_tests.c
 *
 *  Created on: Jan 15, 2024
 *      Author: Muhammad Zahalqa
 */

#include "filesystem_tests.h"
#include "utils/input.h"
#include "utils/timeutils.h"
#include "utils/menu_selection.h"

#include <hal/Timing/Time.h>
#include <at91/utility/rand.h>
#include <hcc/api_fat.h>
#include <stdio.h>


static const char TEST_FILE[] = "test/kq-test.txt";


static Boolean file_system_freespace(void)
{
	F_SPACE space;
	int rv = f_getfreespace(0, &space);
	if (rv == F_NO_ERROR) {
		printf("Free: %lu\n", space.free);
		printf("Used: %lu\n", space.used);
	} else {
		printf("Error %d\n", rv);
	}

	return TRUE;
}


static Boolean file_write_test(void)
{
	f_mkdir("test");
	FN_FILE* file = f_open(TEST_FILE, "w");
	if (!file) {
		printf("Failed on file open\n");
		printf("Error code: %d\n", f_getlasterror());
		return TRUE;
	}

	printf("Enter the lines you want to add to the file, end with empty line\n");
	char buffer[80] = {0};
	do {
		INPUT_GetSTRING("Line:", buffer, sizeof(buffer));
		long r = f_write(buffer, 1, sizeof(buffer), file);
		printf("%ld chars written\n", r);
	} while (buffer[0] != 0);

	f_close(file);

	printf("File written\n");
	return TRUE;
}


static Boolean file_read_test(void)
{
	FN_FILE* file = f_open(TEST_FILE, "r");
	if (!file) {
		printf("Failed on file open\n");
		return FALSE;
	}

	char buffer[80] = {0};
	for(int i = 0; ; ++i) {
		int r = f_read(buffer, 1, sizeof(buffer), file);
		if (r == 0) {
			break;
		} else if ( r < 0) {
			printf("\n>> Error code on read: %d\n", f_getlasterror());
			break;
		}
		printf("%d| %.*s\n", i, r, buffer);
	}

	f_close(file);
	return TRUE;
}


Boolean stdio_file_test(void)
{
	FILE* f = fopen(TEST_FILE, "r");
	char buffer[32] = {0};
	fread(buffer, 1, 32, f);
	printf("Read: %s\n", buffer);
	fclose(f);

	return TRUE;
}


typedef union __attribute__ ((__packed__)) SEL_Record {
	struct __attribute__ ((__packed__)) SEL_Record_Fields {
		unsigned int time_stamp;	///< Unix epoch time
		unsigned int events;		///< Number of events
		unsigned int resets;		///< Number of resets
	} fields; 						///< Struct with fields of SEL experiment
	unsigned char raw[sizeof(struct SEL_Record_Fields)]; ///< Unformatted Science payload
} SEL_Record;

typedef union __attribute__ ((__packed__)) RADFET_Record {
	struct __attribute__ ((__packed__)) RADFET_Record_Fields {
		unsigned int time_stamp;	///< Unix epoch time
		unsigned int radiation;		///< radiation measured
		short temprature;			///< temperature measured
	} fields; 						///< Struct with fields of RAD-FET experiment
	unsigned char raw[sizeof(struct RADFET_Record_Fields)]; ///< Unformatted Science payload
} RADFET_Record;

#define DATA_FOLDER		"data"
#define RADFET_DATA 		"/radfet.dat"
#define SEL_DATA			"/sel.dat"

Boolean sel_data_save_test(void)
{
	f_mkdir(DATA_FOLDER );
	F_FILE* file = f_open(DATA_FOLDER SEL_DATA, "a");
	if (!file) {
		printf("Failed on file open\n");
		return FALSE;
	}

	int n = INPUT_GetUINT32("How many record you want to simulate: ");
	SEL_Record record;
	unsigned int epoch;
	for (int i = 0; i < n; ++i) {
		Time_getUnixEpoch(&epoch);
		record.fields.time_stamp = epoch;
		record.fields.events = i % 10;
		record.fields.resets = i;
		long r = f_write(record.raw, 1, sizeof(record), file);
		if (r != sizeof(record)) {
			printf("Error while saving record %d\n", i);
			break;
		}
		delay_ms(200);
	}

	f_close(file);
	printf("Saved.\n");

	return TRUE;
}


Boolean sel_data_read_test(void)
{
	int begin = INPUT_GetUINT32("First record to read: ");
	int last = INPUT_GetUINT32("Last record to read: ");

	if ( begin > last) {
		printf("You asked nothing to read\n");
		return FALSE;
	}

	F_FILE* file = f_open(DATA_FOLDER SEL_DATA, "r");
	if (!file) {
		printf("Failed on file open\n");
		return FALSE;
	}

	printf("reading records %d..%d\n", begin, last);

	SEL_Record record;
	long offset = sizeof(record) * begin;
	f_seek(file, offset, 0);

	for (int i = begin; i <= last; ++i) {
		int r = f_read(record.raw, 1, sizeof(record), file);
		if (r != sizeof(record)) {
			printf("Error reading record %d\n", i);
			break;
		}

		Time time;
		Time_convertEpochToTime(record.fields.time_stamp, &time);

		//      i    y   m     d    h     m     s      reset   event
		printf("%5d| %4d, %.2d/%.2d %.2d:%.2d:%.2d | %.6d %.4d\n",
				i,
				time.year + 2000,
				time.month,
				time.date,
				time.hours,
				time.minutes,
				time.seconds,
				record.fields.resets,
				record.fields.events);
	}

	f_close(file);

	return TRUE;
}


Boolean sel_data_delete_test(void)
{
	int r = f_delete(DATA_FOLDER SEL_DATA);
	if (r == F_NO_ERROR) {
		printf("file deleted\n");
	} else {
		printf("Error on delete.\n");
	}

	return TRUE;
}


static MenuAction menu[] = {
		{ file_system_freespace, "Get SDCard free space" },
		{ file_write_test, "Perform file write" },
		{ file_read_test, "Perform file read" },
		{ stdio_file_test, "Test stdio files" },
		{ sel_data_save_test, "Simulate SEL data" },
		{ sel_data_read_test, "Read SEL data" },
		{ sel_data_delete_test, "Delete SEL data file" },
		MENU_ITEM_END
};


Boolean filesystem_tests(void)
{
	// Thursday, February 1, 2024 12:00:00 PM  GMT
	const unsigned int epoch_time = 1706788800;
	Time_setUnixEpoch(epoch_time);

	MenuDisplay(menu);
	return TRUE;
}
