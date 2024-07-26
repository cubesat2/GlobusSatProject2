/*
 * spl_packet.h
 *
 *  Created on: Jul 18, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_SPL_PACKET_H_
#define MODULES_SPL_PACKET_H_


#include <stdint.h>

typedef enum __attribute__ ((__packed__)) SPL_Command_Type {
	CMD_TYPE_TRXVU			= 0,
	CMD_TYPE_EPS			= 1,
	CMD_TYPE_TELEMETRY		= 2,
	CMD_TYPE_FILESYSTEM  	= 3,
	CMD_TYPE_MANAGMENT		= 4,
	CMD_TYPE_ACK			= 5,
	CMD_TYPE_DUMP			= 6
} SPL_Command_Type;


#define MAX_COMMAND_DATA_LENGTH 200 ///< maximum AX25 data field available for downlink
#define IMG_CHUNK_SIZE 50 //190 // leave room for chunk number and more

typedef struct __attribute__ ((__packed__)) SPL_SetRTC {
	uint32_t epoch;
} SPL_SetRTC;

typedef struct __attribute__ ((__packed__)) SetNormal_ModeThresholds {
	float v_upper;
	float v_lower;
} SPL_SetNormal_ModeThresholds;

typedef struct __attribute__ ((__packed__)) SPL_Header {
	struct __attribute__ ((__packed__)) {
		uint32_t id : 8;
		uint32_t sequence: 24;
	};
//	uint32_t ID;
	uint8_t cmd_type;
	uint8_t cmd_subtype;
	uint16_t length;
} SPL_Header;


typedef struct __attribute__ ((__packed__)) SPL_Packet {
	SPL_Header header;
	union __attribute__ ((__packed__)) {
		unsigned char data[MAX_COMMAND_DATA_LENGTH];
		SPL_SetRTC set_rtc;
		SPL_SetNormal_ModeThresholds set_normal_threshold;
	};
} SPL_Packet;


#endif /* MODULES_SPL_PACKET_H_ */
