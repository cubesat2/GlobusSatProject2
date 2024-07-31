/*
 * all_tests.c
 *
 *  Created on: Mar 8, 2024
 *      Author: tryfinally
 */


#include <modules/sat_id.h>
#include "tests/all_tests.h"

#include "tests/test_common.h"
#include "Demos/demos.h"
#include "tests/system_c_tests.h"
#include "tests/obc_tests.h"
#include "tests/trxvu_tests.h"
#include "tests/antenna_tests.h"
#include "tests/beacon_tests.h"
#include "tests/solar_panel_tests.h"
#include "tests/gom_eps_tests.h"


static MenuAction main_menu[] = {
			{ demo_tests, "Demo Tests"},
			{ obc_tests, "OBC Tests"},
			{ trxvu_tests, "New TRXVU Tests"},
			{ antenna_tests, "Antennas Tests" },
			{ beacon_tests, "Beacon Tests"},
			{ solar_panels_tests, "Solar Panels Tests" },
			{ gom_eps_test, "New GOMSpace Tests"},
			END_OF_MENU
};


Boolean all_tests(void)
{
	TRACE_INFO(SAT_CALLSIGN " testing task ready!\r\n\r\n\tQubeSat Flight Software Testing Software.\r\n");
	MenuDisplay(main_menu);
	return TRUE;
}
