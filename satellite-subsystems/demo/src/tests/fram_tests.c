/*
 * fram_tests.c
 *
 *  Created on: Mar 8, 2024
 *      Author: tryfinally
 */


#include "tests/fram_tests.h"
#include "tests/test_common.h"

#include "modules/fram/m_configurations.h"

#include "config/flight_params.h"

#include "utils/input.h"
#include "utils/timeutils.h"
#include "utils/menu_selection.h"

#include <hal/errors.h>
#include <at91/utility/trace.h>

#include <stddef.h>

static Boolean write_eps_conig(void)
{
	struct EPS eps = {
		.v_normal_up = 7.6,
		.v_normal_down = 7.4,
		.v_safe_up = 7.1,
		.v_safe_down = 7.0
	};

	g_flight_config.eps = eps;

	Boolean rv = m_config_write_param(FRAM_BASE_ADDRESS, offsetof(FlightConfiguration, eps), &eps, sizeof(eps));
//	Boolean rv = m_config_write_eps(&eps_params);
	VERIFY(rv, "Writing EPS Config params to FRAM");

	return rv;
}

static Boolean read_eps_conig(void)
{
	struct EPS eps = {};

	Boolean rv = m_config_read_param(FRAM_BASE_ADDRESS, offsetof(FlightConfiguration, eps), &eps, sizeof(eps));
	VERIFY(rv, "Reading EPS Config params to FRAM");

	VERIFY(eps.v_normal_up == 7.6, "vBat Normal up is %f", eps.v_normal_up);
	VERIFY(eps.v_safe_up == 7.1, "vBat Safe up is %f", eps.v_normal_up);

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
