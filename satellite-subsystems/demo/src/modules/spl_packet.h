/*
 * spl_packet.h
 *
 *  Created on: Jul 18, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_SPL_PACKET_H_
#define MODULES_SPL_PACKET_H_


#include <stdint.h>
#include "modules/m_trxvu.h"

typedef enum __attribute__ ((__packed__)) SPL_Command_Type {
	CMD_TYPE_TRXVU			= 0,
	CMD_TYPE_EPS			= 1,
	CMD_TYPE_TELEMETRY		= 2,
	CMD_TYPE_FILESYSTEM  	= 3,
	CMD_TYPE_MANAGMENT		= 4,
	CMD_TYPE_ACK			= 5,
	CMD_TYPE_DUMP			= 6
} SPL_Command_Type;

typedef enum __attribute__ ((__packed__)) SPL_TRXVU_Command_SubType {
	TRXVU_CMD_PING, //0
	TRXVU_CMD_SETTIME, //1
} SPL_TRXVU_Command_SubType;

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
	uint32_t ID;
	uint8_t cmd_type;
	uint8_t cmd_subtype;
	uint16_t length;
} SPL_Header;

// ID:42       TRX SETTIME
// 2A 00 00 00 00  01 04 00 4A 10 9A 66
//                    LEN   epoch = 1721372746
// Frame sent:
// 82 98 98 40 40 40 E0 68 F0 68 D0 E6 C6 61 03 F0 2A 00 00 00 00 01 04 00 12 34 56 78 47 6D
// |00|01|02|03|04|05|06|07|08|09|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|


typedef struct __attribute__ ((__packed__)) SPL_Packet {
	SPL_Header header;
	union __attribute__ ((__packed__)) {
		unsigned char data[MAX_COMMAND_DATA_LENGTH];
		SPL_SetRTC set_rtc;
		SPL_SetNormal_ModeThresholds set_normal_threshold;
	};
} SPL_Packet;


#endif /* MODULES_SPL_PACKET_H_ */
