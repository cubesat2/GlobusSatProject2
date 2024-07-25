/*
 * commands.c
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#include <stdio.h>
#include <inttypes.h>
#include <at91/utility/trace.h>

#include "modules/spl_packet.h"
#include "modules/m_time.h"
#include "commands.h"
#include "../command_router.h"


static void set_rtc(SPL_Packet const* args)
{
	Trx_SetRTC_Args const * params = (Trx_SetRTC_Args*) args->data;
	unsigned int epoch = 0;
	int r = Time_getUnixEpoch(&epoch);
	if (r == 0) {
		printf("\nepoch was: ");
		m_time_print_epoch(epoch);
		printf("\r\n");
	}

	TRACE_DEBUG("\r\nset time to epoch: %" PRIu32 "\n", params->epoch);
	m_time_settime(params->epoch);
	r = Time_getUnixEpoch(&epoch);
	if (r == 0) {
		printf("\nepoch now: ");
		m_time_print_epoch(epoch);
		printf("\r\n");
	}
	printf("\n\n");
}

static void ping(SPL_Packet const* args)
{
	Trx_Ping_Args const* params = (Trx_Ping_Args const*) args;
	int len = params->message_length;
	TRACE_DEBUG("\r\nPing: %.*s\r\n", len, params->message);

	// TODO: send pong
}

void trxvu_command_router(SPL_Packet const* packet)
{
	switch (packet->header.cmd_subtype) {
	case TRXVU_CMD_SETTIME:
		set_rtc(packet);
		break;
	case TRXVU_CMD_PING:
		ping(packet);
		break;
	default:
		printf("Unknown command: %" PRIu8 "\r\n", packet->header.cmd_subtype);
	}
}

