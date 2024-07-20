/*
 * commands.h
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_COMMANDS_TRXVU_COMMANDS_H_
#define MODULES_COMMANDS_TRXVU_COMMANDS_H_

#include "modules/spl_packet.h"

typedef enum __attribute__ ((__packed__)) TRXVU_Command_SubType {
	TRXVU_CMD_PING, //0
	TRXVU_CMD_SETTIME, //1
} TRXVU_Command_SubType;

typedef struct __attribute__ ((__packed__)) Trx_SetRTC_Args {
	uint32_t epoch;
} Trx_SetRTC_Args;

typedef struct __attribute__ ((__packed__)) Trx_Ping_Args {
	uint8_t message_length;
	char message[];
} Trx_Ping_Args;


void trxvu_command_router(SPL_Packet const* packet);

#endif /* MODULES_COMMANDS_TRXVU_COMMANDS_H_ */
