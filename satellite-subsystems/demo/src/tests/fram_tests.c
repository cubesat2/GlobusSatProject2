/*
 * fram_tests.c
 *
 *  Created on: Mar 8, 2024
 *      Author: tryfinally
 */


#include "tests/fram_tests.h"
#include "tests/test_common.h"

#include "modules/m_configurations.h"

#include "utils/input.h"
#include "utils/timeutils.h"
#include "utils/menu_selection.h"

#include <hal/errors.h>
#include <at91/utility/trace.h>

static Boolean write_eps_conig(void)
{
	EPS_Params eps_params = {
			.v_normal_up = 7.6,
			.v_normal_down = 7.4,
			.v_safe_up = 7.1,
			.v_safe_down = 7.0
	};

	Boolean rv = m_config_write_eps(&eps_params);
	VERIFY(rv, "Writing EPS Config params to FRAM");

	return TRUE;
}

static Boolean read_eps_conig(void)
{
	EPS_Params eps_params = { 0 };

	Boolean rv = m_config_read_eps(&eps_params);
	VERIFY(rv, "Reading EPS Config params to FRAM");

	VERIFY(eps_params.v_normal_up == 7.6, "vBat Normal up is %f", eps_params.v_normal_up);
	VERIFY(eps_params.v_safe_up == 7.1, "vBat Safe up is %f", eps_params.v_normal_up);

	return TRUE;
}

static MenuAction menu[] = {
		{ write_eps_conig, "Write EPS Params to FRAM" },
		{ read_eps_conig, "Write EPS Params to FRAM" },
		MENU_ITEM_END
};


Boolean fram_tests(void)
{
	MenuDisplay(menu);
	return TRUE;
}
