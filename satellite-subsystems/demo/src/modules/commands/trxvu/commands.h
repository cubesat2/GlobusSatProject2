/*
 * commands.h
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_COMMANDS_TRXVU_COMMANDS_H_
#define MODULES_COMMANDS_TRXVU_COMMANDS_H_

#include "modules/spl_packet.h"

typedef struct __attribute__ ((__packed__)) Cmd_SetRTC_Args {
	uint32_t epoch;
} Cmd_SetRTC_Args;

typedef struct __attribute__ ((__packed__)) Cmd_Ping_Args {
	uint8_t message_length;
	char message[];
} Cmd_Ping_Args;


void trxvu_command_router(SPL_Packet const* packet);

#endif /* MODULES_COMMANDS_TRXVU_COMMANDS_H_ */
