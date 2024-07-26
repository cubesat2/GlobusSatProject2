/*
 * m_trxvu.h
 *
 *  Created on: Dec 28, 2023
 *      Author: Muhammad Zahalqa
 */

#ifndef MODULES_M_TRXVU_H_
#define MODULES_M_TRXVU_H_

#include "modules/spl_packet.h"

#include <stdint.h>
#include <hal/boolean.h>
#include <satellite-subsystems/IsisTRXVU.h>

#define M_TRXVU_RX_FRAME_SIZE		(30)
#define M_TRXVU_TX_FRAME_SIZE		(235)


typedef struct __attribute__ ((__packed__)) TrxvuRxFrame
{
    unsigned short length; ///< Reception frame length.
    double doppler; ///< Reception frame doppler measurement.
    double rssi; ///< Reception frame rssi measurement.
    unsigned char* framedata; ///< Pointer to an array receiving reception frame data.
} TrxvuRxFrame;

/**
 * Initialize the TRXVU component
 * @return TRUE on success
 */
Boolean m_trxvu_init(void);

/**
 * Activate ISIS TRXVU Responder mode
 */
Boolean trxvu_activate_responder(void);

/**
 * Deactivate ISIS TRXVU Responder mode
 */
Boolean trxvu_deactivate_responder(void);

/**
 * Activate auto responder for time period
 * @param minutes
 * @return
 */
Boolean trxvu_activate_responder_minutes(unsigned int minutes);

/**
 * Set RSSI Transponder Threshold
 * @param threshold RSSI threshold value. between 0 and 4095.
 * @return
 */
Boolean trxvu_set_responder_rssi_threshold(uint16_t rssi);

/**
 * Set bitrate for AX25
 * @param bitrate
 * @return success or failure
 */
Boolean trxvu_set_ax25_bitrate(ISIStrxvuBitrate bitrate);

/**
 * Send buffer
 * @param data
 * @param length
 * @return
 */
Boolean trxvu_send_buffer(uint8_t const* data, uint8_t length);

/**
 * Send a message using default to/from callsigns
 * @param data	message data
 * @param length message length
 * @return
 */
Boolean trxvu_send_message(char const* data, uint8_t length);

/**
 * Retrieve the number of telecommand frames present in the receive buffer of the TRXVU.
 * @return number of frames
 */
int trxvu_count_incoming_frames(void);

/**
 * Retrieve and delete a telecommand frame from the TRXVU.
 * @return pointer to static TrxvuRxFrame with static buffer for data
 */
TrxvuRxFrame* trxvu_get_frame();

/**
 * Implements periodic TRXVU logic for handling incoming commands and beacon
 */
void trxvu_logic(void);

#endif /* MODULES_M_TRXVU_H_ */
