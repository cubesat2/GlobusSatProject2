/*
 * obc_tests.c
 *
 *  Created on: Mar 8, 2024
 *      Author: tryfinally
 */


#include "tests/obc_tests.h"
#include "tests/system_c_tests.h"
#include "tests/fram_tests.h"
#include "tests/filesystem_tests.h"
#include "tests/database_tests.h"
#include "tests/time_tests.h"

#include "tests/test_common.h"


static MenuAction menu[] = {
		{ system_C_tests, "C language tests" },
		{ time_tests, "Time and RTC tests" },
		{ fram_tests, "FRAM tests" },
		{ filesystem_tests, "File System tests" },
		{ database_tests, "DB tests" },
		MENU_ITEM_END
};


Boolean obc_tests(void)
{
	MenuDisplay(menu);
	return TRUE;
}
