/*
 * commands.c
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <at91/utility/trace.h>

#include "modules/spl_packet.h"
#include "modules/m_time.h"
#include "modules/m_trxvu.h"

#include "modules/commands/trxvu/subcommands.h"
#include "modules/commands/command_router.h"

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

	TRACE_DEBUG("\r\nset time to arg->epoch: %" PRIu32 "\n", params->epoch);
	epoch = params->epoch;
	TRACE_DEBUG("\r\nset time to epoch: %u\n", epoch);
	m_time_settime(epoch);
	r = Time_getUnixEpoch(&epoch);
	if (r == 0) {
		printf("\nepoch now: ");
		m_time_print_epoch(epoch);
		printf("\r\n");
	}
	printf("\n\n");
}

static void activate_responder(SPL_Packet const* args)
{
	unsigned int minutes = *(unsigned int const*) args->data;
	TRACE_INFO("Got command to activate responder for %u minutes", minutes);
	TRACE_INFO("Activating responder for %u minutes", minutes);
}

static void ping(SPL_Packet const* args)
{
	Trx_Ping_Args const* params = (Trx_Ping_Args const*) args->data;
	int len = params->message_length;
	TRACE_DEBUG("\r\nPing: %.*s\r\n", len, params->message);

	SPL_Packet outgoing_packet;
	uint8_t message[] = ">> Pong!!";
	assemble_spl_reply_packet(&outgoing_packet, &args->header, sizeof(message), message);
	transmit_spl_packet(&outgoing_packet);
}

void trxvu_command_router(SPL_Packet const* packet)
{
	if (packet->header.cmd_type != CMD_TYPE_TRXVU) {
		TRACE_ERROR("should only get trxvu commands here");
		return;
	}

	switch (packet->header.cmd_subtype) {
	case TRXVU_CMD_SETTIME:
		set_rtc(packet);
		break;
	case TRXVU_CMD_ACTIVATE_RESPONDER:
		activate_responder(packet);
		break;
	case TRXVU_CMD_PING:
		ping(packet);
		break;
	default:
		printf("Unknown command: %" PRIu8 "\r\n", packet->header.cmd_subtype);
	}
}

