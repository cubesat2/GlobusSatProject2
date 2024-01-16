/*
 * filesystem_tests.c
 *
 *  Created on: Jan 15, 2024
 *      Author: Muhammad Zahalqa
 */

#include "filesystem_tests.h"
#include "utils/input.h"
#include "utils/menu_selection.h"

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
		return FALSE;
	}

	printf("Enter the lines you want to add to the file, end with empty line\n");
	char buffer[80] = {0};
	do {
		INPUT_GetSTRING("Line:", buffer, sizeof(buffer));
		long r = f_write(buffer, 1, sizeof(buffer)-1, file);
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
	long r = f_read(buffer, 1, sizeof(buffer)-1, file);

	if (r > 0) {
		printf("Read %ld chars\n", r);
		printf("File contains: %s\n", buffer);
	} else {
		printf("Error code on read: %d\n", f_getlasterror());
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

static MenuAction menu[] = {
		{ file_system_freespace, "Get SDCard free space" },
		{ file_write_test, "Perform file write" },
		{ file_read_test, "Perform file read" },
		{ stdio_file_test, "Test stdio files" },
		MENU_ITEM_END
};

Boolean filesystem_tests(void)
{
	MenuDisplay(menu);
	return TRUE;
}
