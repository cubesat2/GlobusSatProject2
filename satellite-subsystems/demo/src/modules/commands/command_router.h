/*
 * command_router.h
 *
 *  Created on: Jul 19, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_COMMANDS_COMMAND_ROUTER_H_
#define MODULES_COMMANDS_COMMAND_ROUTER_H_

#include "modules/spl_packet.h"

void command_router(SPL_Packet const* packet);

void trx_command_router(SPL_Packet const* packet);
void eps_command_router(SPL_Packet const* packet);

#endif /* MODULES_COMMANDS_COMMAND_ROUTER_H_ */
