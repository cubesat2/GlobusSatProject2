/*
 * database_tests.c
 *
 *  Created on: Feb 9, 2024
 *      Author: tryfinally
 */

#include "database_tests.h"
#include "modules/m_db.h"

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

static Boolean db_read_simple_test(void)
{
	if (db_read_data(TELEMETRY_RADFET_PAYLOAD, 0, 0)) {
		printf("Pass\n");
	} else {
		printf("Fail\n");
	}

	return TRUE;
}


static MenuAction menu[] = {
		{ db_write_simple_test, "DB write simple" },
		{ db_read_simple_test, "DB write simple" },

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
