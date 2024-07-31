/*
 * command_router.h
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_COMMANDS_COMMAND_ROUTER_H_
#define MODULES_COMMANDS_COMMAND_ROUTER_H_

#include "modules/spl/spl_packet.h"

void no_such_command(SPL_Packet const* packet);
void command_router(SPL_Packet const* packet);

#endif /* MODULES_COMMANDS_COMMAND_ROUTER_H_ */
