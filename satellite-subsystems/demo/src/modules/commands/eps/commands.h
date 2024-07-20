/*
 * commands.h
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_COMMANDS_EPS_COMMANDS_H_
#define MODULES_COMMANDS_EPS_COMMANDS_H_

#include "modules/spl_packet.h"

typedef enum __attribute__ ((__packed__)) EPS_Command_SubType {
	EPS_CMD_GET_VBAT				= 0,
	EPS_CMD_GET_V_THRESHOLDS			= 1,
	EPS_CMD_SET_V_THRESHOLDS			= 2,
} EPS_Command_SubType;

typedef struct __attribute__ ((__packed__)) Eps_Get_VBat_Args {
	uint32_t vbat;
} Eps_Get_VBAT_Args;

typedef struct __attribute__ ((__packed__)) Eps_V_Threshold_Args {
	uint32_t normal_low_v;
	uint32_t safe_high_v;
	uint32_t safe_low_v;
	uint32_t critical_high_v;
} Eps_V_Threshold_Args;


void eps_command_router(SPL_Packet const* packet);

#endif /* MODULES_COMMANDS_TRXVU_COMMANDS_H_ */
