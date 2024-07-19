/*
 * spl_command_parser.c
 *
 *  Created on: Jul 18, 2024
 *      Author: tryfinally
 */


#include "spl_command_parser.h"
#include "spl_packet.h"
#include "commands/command_router.h"

#define SAT_ID
void spl_parse_command(TrxvuRxFrame* frame)
{
	if (!frame) {
		return;
	}

	if (frame->length < sizeof(SPL_Header)) {
		printf("Bask Ax.25 frame? length is: %d\r\n", frame->length);
		return;
	}

	SPL_Packet* packet = (SPL_Packet*) frame->framedata;
	printf("AX25 frame length: %d \n", frame->length);
	command_router(packet);
	printf("done..\n");
}
