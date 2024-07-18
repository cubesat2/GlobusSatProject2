/*
 * spl_command_parser.c
 *
 *  Created on: Jul 18, 2024
 *      Author: tryfinally
 */


#include "spl_command_parser.h"
#include "spl_packet.h"

#define SAT_ID
void spl_parse_command(TrxvuRxFrame* frame)
{
	if (!frame) {
		return;
	}

	printf("Frame length: %d \n", frame->length);

	if (frame->length < sizeof(SPL_Header)) {
		return;
	}

	SPL_Packet* packet = (SPL_Packet*) frame->framedata;
	printf("Sat ID: %d \n", packet->header.ID);
	printf("Sat Cmd: %d \n", packet->header.cmd_type);
	printf("Sat SubCmd: %d \n", packet->header.cmd_subtype);
	printf("Sat Len: %d \n", packet->header.length);

	if (packet->header.ID != SAT_MYID || packet->header.ID != SAT_ALLID) {
		return;
	}

//	switch(packet->header.cmd_type) {
//	case TRXVU_CMD_TYPE:
//
//	}

	if (packet->header.cmd_type == TRXVU_CMD_TYPE) {
		if (packet->header.cmd_subtype == TRXVU_CMD_SETTIME) {
			uint32_t epoch = packet->set_rtc.epoch;
			printf("SPL incoming: cmd:%d  sub:%d epoch:%d \n", TRXVU_CMD_TYPE, TRXVU_CMD_SETTIME, epoch);
		}
	}

}
