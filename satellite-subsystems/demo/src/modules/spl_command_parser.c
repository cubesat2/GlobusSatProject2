/*
 * spl_command_parser.c
 *
 *  Created on: Jul 18, 2024
 *      Author: tryfinally
 */

#include <stdio.h>

#include <at91/utility/trace.h>

#include "spl_command_parser.h"
#include "spl_packet.h"
#include "commands/command_router.h"


void spl_parse_command(TrxvuRxFrame* frame)
{
	if (!frame) {
		return;
	}

	if (frame->length < sizeof(SPL_Header)) {
		TRACE_DEBUG("Bask Ax.25 frame? length is: %d\r\n", frame->length);
		return;
	}

	SPL_Packet* packet = (SPL_Packet*) frame->framedata;
	command_router(packet);
}
