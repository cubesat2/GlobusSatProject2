/*
 * solar_panel_tests.c
 *
 *  Created on: Jan 9, 2024
 *      Author: Muhammad Zahalqa
 */

#include "tests/solar_panel_tests.h"
#include "tests/test_common.h"

#include "modules/m_solar_panels.h"


static Boolean get_solar_panels_sensor_state(void)
{
	if (solar_panels_is_awake()) {
		printf("The sensors are currently active.");
	} else {
		printf("The sensors aren't active.");
	}
	return TRUE;
}

static Boolean activate_solar_panels_sensor(void)
{
	if (solar_panels_wakeup()) {
		printf("Solar panel sensors activated.");
	} else {
		printf("Solar panel activation failed.");
	}
	return TRUE;
}

static Boolean shutdown_solar_panels_sensor(void)
{
	if (solar_panels_sleep()) {
		printf("The sensors have been shutdown.");
	} else {
		printf("Sensor shutdown failed.");
	}
	return TRUE;
}

static Boolean get_solar_panels_temperature(void)
{
	// we have ISIS_SOLAR_PANEL_COUNT solar panels

	return TRUE;
}

static MenuAction menu[] = {
			{ get_solar_panels_sensor_state, "Get Solar  panels temperature sensor state" },
			{ activate_solar_panels_sensor, "Activate Solar  panels temperature sensor" },
			{ shutdown_solar_panels_sensor, "Shutdown Solar  panels temperature sensor" },
			{ get_solar_panels_temperature, "Get Solar  panels temperature" },
			MENU_ITEM_END
};

Boolean solar_panels_tests(void)
{
	MenuDisplay(menu);
	return TRUE;
}
