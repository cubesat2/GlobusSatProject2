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

void assemble_spl_packet(SPL_Packet* packet, uint32_t id, uint8_t type, uint8_t subtype, uint16_t data_length, uint8_t const* data)
{
	packet->header.ID = id;
	packet->header.cmd_type = type;
	packet->header.cmd_subtype = subtype;
	packet->header.length = data_length;
	memcpy(packet->data, data, data_length);
}

Boolean transmit_spl_packet(SPL_Packet const* packet)
{
	uint8_t packet_len = sizeof(packet->header) + packet->header.length;
	return trxvu_send_buffer((uint8_t const*)packet, packet_len);
}

static void ping(SPL_Packet const* args)
{
	Trx_Ping_Args const* params = (Trx_Ping_Args const*) args->data;
	int len = params->message_length;
	TRACE_DEBUG("\r\nPing: %.*s\r\n", len, params->message);

	SPL_Packet packet;
	assemble_spl_packet(&packet, args->header.ID, args->header.cmd_type, args->header.cmd_subtype, 6, "Pong!!");
	transmit_spl_packet(&packet);
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

