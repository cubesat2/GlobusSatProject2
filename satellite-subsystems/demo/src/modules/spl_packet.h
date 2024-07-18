/*
 * spl_packet.h
 *
 *  Created on: Jul 18, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_SPL_PACKET_H_
#define MODULES_SPL_PACKET_H_


#include <stdint.h>
#include <modules/m_trxvu.h>

typedef enum __attribute__ ((__packed__)) SPL_Command_Type {
	TRXVU_CMD_TYPE, //0
	EPS_CMD_TYPE, //1
	TELEMETRY_CMD_TYPE, //2
	FILESYSTEM_CMD_TYPE, //3
	MANAGMENT_CMD_TYPE, //4
	ACK_TYPE,// 5
	DUMP_TYPE // 6
} SPL_Command_Type;

typedef enum __attribute__ ((__packed__)) SPL_TRXVU_Command_SubType {
	TRXVU_CMD_HELLO, //0
	TRXVU_CMD_SETTIME, //1
} SPL_TRXVU_Command_SubType;

#define MAX_COMMAND_DATA_LENGTH 200 ///< maximum AX25 data field available for downlink
#define IMG_CHUNK_SIZE 50 //190 // leave room for chunk number and more

#define SAT_MYID		(42u)
#define SAT_ALLID  	(0u)

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

typedef struct __attribute__ ((__packed__)) SPL_Packet {
	SPL_Header header;
	union __attribute__ ((__packed__)) {
		unsigned char data[MAX_COMMAND_DATA_LENGTH];
		SPL_SetRTC set_rtc;
		SPL_SetNormal_ModeThresholds set_normal_threshold;
	};
} SPL_Packet;


#endif /* MODULES_SPL_PACKET_H_ */
