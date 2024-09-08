/*
 * state_governer.h
 *
 *  Created on: Apr 15, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_STATE_GOVERNER_H_
#define MODULES_STATE_GOVERNER_H_

#include <hal/boolean.h>

struct StateParameters {
	int beacon_period;
	Boolean payload_active;
	int radfet_period;
	int sel_period;
	int housekeeping_period;
};

enum State { NORMAL, RELAXED, SAFE, CRITICAL };

struct StateParameters safe_mode = {
		.beacon_period = 20,
		.payload_active = TRUE,
		.radfet_period = 3600,
		.sel_period = 60,
		.housekeeping_period = 300
};

struct StateParameters critical_mode = {
		.beacon_period = 300,
		.payload_active = FALSE,
		.radfet_period = 360000,
		.sel_period = 360000,
		.housekeeping_period = 600
};

#endif /* MODULES_STATE_GOVERNER_H_ */
