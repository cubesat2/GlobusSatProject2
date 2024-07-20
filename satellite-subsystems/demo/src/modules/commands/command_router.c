/*
 * command_router.c
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#include <stdio.h>
#include <inttypes.h>

#include "command_router.h"
#include "trxvu/commands.h"
#include "eps/commands.h"

#define SAT_MYID		(14u)
#define SAT_ALLID  	(0u)

//static const char CallSign[] = "T14KFQ";


void no_such_command(SPL_Packet const* packet)
{
	if (packet) {
		printf("SPL for ID:%" PRIu32 "\n", packet->header.ID);
		printf("Command Type: %d \n", packet->header.cmd_type);
		printf("     Subtype: %d \n", packet->header.cmd_subtype);
		printf("Params Bytes: %d \n", packet->header.length);
	}
}

void command_router(SPL_Packet const* packet)
{
	if (!packet) {
		return;
	}

	if (packet->header.ID != SAT_MYID && packet->header.ID != SAT_ALLID) {
		return;
	}

	switch (packet->header.cmd_type) {
		case CMD_TYPE_TRXVU: 		trxvu_command_router(packet); 	break;
		// TODO: implement subsystem command routers
		case CMD_TYPE_EPS: 			eps_command_router(packet);		break;
//		case CMD_TYPE_FILESYSTEM: 	fs_command_router(packet);		break;
//		case CMD_TYPE_MANAGMENT: 	mng_command_router(packet);		break;
//		case CMD_TYPE_TELEMETRY: 	tlm_command_router(packet);		break;

		default: no_such_command(packet);
	}
}

