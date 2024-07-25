/*
 * spl_command_parser.c
 *
 *  Created on: Jul 18, 2024
 *      Author: tryfinally
 */

#include "spl_command_parser.h"

#include "m_ids.h"
#include "spl_packet.h"
#include "commands/command_router.h"

#include <stdio.h>
#include <at91/utility/trace.h>


void spl_parse_command(TrxvuRxFrame* frame)
{
	if (!frame) {
		return;
	}

	if (frame->length < sizeof(SPL_Header)) {
		TRACE_DEBUG("Ax.25 frame to short? length is: %d\r\n", frame->length);
		return;
	}

	SPL_Packet* packet = (SPL_Packet*) frame->framedata;
	if (packet->header.ID != SAT_MYID && packet->header.ID != SAT_ALLID) {
		TRACE_DEBUG("Packet not for us");
		return;
	}
	command_router(packet);
}
