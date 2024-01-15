/*
 * filesystem_tests.c
 *
 *  Created on: Jan 15, 2024
 *      Author: Muhammad Zahalqa
 */

#include "filesystem_tests.h"

#include "utils/menu_selection.h"

#include <hcc/api_fat.h>

static Boolean file_system_freespace(void)
{

	return TRUE;
}


static Boolean file_write_test(void)
{
	return TRUE;
}

static Boolean file_read_test(void)
{
	return TRUE;
}

static MenuAction menu[] = {
		{ file_system_freespace, "Get SDCard free space" },
		{ file_write_test, "Perform file write" },
		{ file_read_test, "Perform file read" },
		MENU_ITEM_END
};

Boolean filesystem_tests(void)
{
	hcc_mem_init();
	int r = fs_init();

	MenuDisplay(menu);
	return TRUE;
}
