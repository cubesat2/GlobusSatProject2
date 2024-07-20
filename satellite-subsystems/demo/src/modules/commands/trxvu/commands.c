/*
 * commands.c
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#include <stdio.h>
#include <inttypes.h>

#include "commands.h"
#include "../command_router.h"


static void set_rtc(Trx_SetRTC_Args const* args)
{
	uint32_t epoch = args->epoch;
	printf("set time to epoch: %" PRIu32 "\n", epoch);
}

static void ping(Trx_Ping_Args const* args)
{
	int len = args->message_length;
	printf("Ping: %.*s\r\n", len, args->message);
}

void trxvu_command_router(SPL_Packet const* packet)
{
	switch (packet->header.cmd_subtype) {
	case TRXVU_CMD_SETTIME:
		set_rtc((void*)packet->data);
		break;
	case TRXVU_CMD_PING:
		ping((void*)packet->data);
		break;
	default:
		printf("Unknown command: %" PRIu8 "\r\n", packet->header.cmd_subtype);
	}
}

