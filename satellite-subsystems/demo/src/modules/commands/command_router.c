/*
 * command_router.c
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#include "command_router.h"

#define SAT_MYID		(14u)
#define SAT_ALLID  	(0u)

static const char CallSign[] = "T14KFQ";


void command_router(SPL_Packet const* packet)
{
	if (!packet) {
		return;
	}

	printf("SPL for ID: %u \n", packet->header.ID);
	printf("Command Type: %d \n", packet->header.cmd_type);
	printf("     Subtype: %d \n", packet->header.cmd_subtype);
	printf(" With Params: %d \n", packet->header.length);

	if (packet->header.ID != SAT_MYID && packet->header.ID != SAT_ALLID) {
		printf("Ignore SPL not for me!\r\n");
		return;
	}

	switch (packet->header.cmd_type) {
		case CMD_TYPE_TRXVU: return trxvu_command_router(packet);
		case CMD_TYPE_EPS: return eps_command_router(packet);

		default: printf("Unknown command\r\n");
	}
}


void eps_command_router(SPL_Packet const* packet)
{

}
