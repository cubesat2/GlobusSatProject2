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
	TRXVU_CMD_PING				= 0,
	TRXVU_CMD_SETTIME			= 8,
} TRXVU_Command_SubType;

typedef struct __attribute__ ((__packed__)) Trx_SetRTC_Args {
	uint32_t epoch;
} Trx_SetRTC_Args;

typedef struct __attribute__ ((__packed__)) Trx_Ping_Args {
	uint8_t message_length;
	char message[];
} Trx_Ping_Args;

//             Cmd  SubCommand
// ID:14       TRX  SETTIME
// 0E 00 00 00 00   08       04  F0	B1 9B 66
//    Sequence               LEN DATA epoch = 1721478951
// Frame sent:
//  82 98 98 40 40 40 E0 68 F0 68 D0 E6 C6 61 03 F0 14 00 00 00 00 01 04 00 12 34 56 78 47 6D
// |00|01|02|03|04|05|06|07|08|09|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|

// Ping        TRX PNG LN:12  11 Major Domo!
// 0E 00 00 00 00  00  0C 00  0B 4D 61 6A 6F 72 20 44 6F 6D 6F 21

void trxvu_command_router(SPL_Packet const* packet);

#endif /* MODULES_COMMANDS_TRXVU_COMMANDS_H_ */
