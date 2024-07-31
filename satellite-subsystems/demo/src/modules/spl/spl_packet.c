/*
 * spl_packet.c
 *
 *  Created on: Jul 26, 2024
 *      Author: tryfinally
 */

#include "spl_packet.h"
#include "utils/timeutils.h"
#include "modules/trxvu/m_trxvu.h"

#include <at91/utility/trace.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



void spl_packet_assemble(SPL_Packet* packet, uint32_t id, uint8_t type, uint8_t subtype, uint16_t data_length, uint8_t const* data)
{
	packet->header.id = id;
	packet->header.cmd_type = type;
	packet->header.cmd_subtype = subtype;
	packet->header.length = data_length;
	size_t len = data_length;
	if (len > sizeof(packet->data)) {
		TRACE_INFO("truncated message %u to fit SPL frame", len);;
		len = sizeof(packet->data);
	};
	memcpy(packet->data, data, len);
}

void spl_packet_assemble_reply(SPL_Packet* packet, SPL_Header const* header, uint16_t data_length, uint8_t const* data)
{
	packet->header.id = header->id;
	packet->header.sequence = header->sequence;
	packet->header.cmd_type = header->cmd_type;
	packet->header.cmd_subtype = header->cmd_subtype;
	packet->header.length = data_length;
	size_t len = data_length;
	if (len > sizeof(packet->data)) {
		TRACE_INFO("truncated message %u to fit SPL frame", len);;
		len = sizeof(packet->data);
	};
	memcpy(packet->data, data, len);
}

Boolean spl_packet_transmit(SPL_Packet const* packet)
{
	uint8_t packet_len = sizeof(packet->header) + packet->header.length;
	return trxvu_send_buffer((uint8_t const*)packet, packet_len);
}

Boolean spl_packet_send_reply_message(SPL_Header const* header, char const* message)
{
	static SPL_Packet outgoing_packet;
	uint16_t length = strlen(message);
	spl_packet_assemble_reply(&outgoing_packet, header, length, (uint8_t const*)message);
	Boolean r = spl_packet_transmit(&outgoing_packet);
	if (r != TRUE) {
		TRACE_ERROR("failed to send reply message: %s", message);
	}
	return r;
}
