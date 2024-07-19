/*
 * commands.c
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */


#include "commands.h"

static void set_rtc(Cmd_SetRTC_Args const* args)
{
	uint32_t epoch = args->epoch;
	printf("set time to epoch: %u \n", epoch);
}

static void ping(Cmd_Ping_Args const* args)
{
	int len = args->message_length;
	printf("Ping: %.*s\r\n", len, args->message);
}

void trxvu_command_router(SPL_Packet const* packet)
{
	switch (packet->header.cmd_subtype) {
	case TRXVU_CMD_SETTIME:
		set_rtc(packet->data);
		break;
	case TRXVU_CMD_PING:
		ping(packet->data);
		break;
	default:
		printf("Unknown command: %u\r\n", packet->header.cmd_subtype);
	}
}

