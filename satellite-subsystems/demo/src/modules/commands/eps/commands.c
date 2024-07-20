/*
 * commands.c
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#include <stdio.h>

#include "commands.h"
#include "../command_router.h"


void eps_command_router(SPL_Packet const* packet)
{
	switch (packet->header.cmd_subtype) {

	default:
		no_such_command(packet);
	}
}

