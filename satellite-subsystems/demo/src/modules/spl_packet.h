/*
 * spl_packet.h
 *
 *  Created on: Jul 18, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_SPL_PACKET_H_
#define MODULES_SPL_PACKET_H_


#include <stdint.h>
#include <hal/boolean.h>

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
//
//typedef struct __attribute__ ((__packed__)) SPL_SetRTC {
//	uint32_t epoch;
//} SPL_SetRTC;

//typedef struct __attribute__ ((__packed__)) SetNormal_ModeThresholds {
//	float v_upper;
//	float v_lower;
//} SPL_SetNormal_ModeThresholds;

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
	unsigned char data[MAX_COMMAND_DATA_LENGTH];
} SPL_Packet;


/**
 * Assemble SPL Packet to be sent by trxvu
 * @param packet			pointer to destination SPL_Packet
 * @param id			sat id
 * @param type			type of packet
 * @param subtype		subtype of packet
 * @param data_length	length of data for this cmd
 * @param data			byte array holding the data to be sent
 */
void spl_packet_assemble(SPL_Packet* packet, uint32_t id, uint8_t type, uint8_t subtype, uint16_t data_length, uint8_t const* data);

/**
 * Send a reply packet
 * @param packet			pointer to destination SPL_Packet
 * @param header			pointer to header of original command we are sending ack for
 * @param data_length	length of data
 * @param data			byte array holding the data
 */
void spl_packet_assemble_reply(SPL_Packet* packet, SPL_Header const* header, uint16_t data_length, uint8_t const* data);

/**
 * Transmit SPL packet using the trxvu default call sign
 * @param packet
 * @return TRUE of success
 */
Boolean spl_packet_transmit(SPL_Packet const* packet);

/**
 * Send a char message reply spl packet
 * @param header
 * @param message
 * @return
 */
Boolean spl_packet_send_reply_message(SPL_Header const* header, char const* message);
#endif /* MODULES_SPL_PACKET_H_ */
