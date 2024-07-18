/*
 * trxvu_tests.c
 *
 *  Created on: Dec 28, 2023
 *      Author: Muhammad Zahalqa
 */

#include "tests/trxvu_tests.h"
#include "tests/test_common.h"

#include "modules/m_gomeps.h"
#include "modules/m_trxvu.h"
#include "modules/spl_command_parser.h"

#include "utils/input.h"
#include "utils/timeutils.h"
#include "utils/error_report.h"
#include "utils/menu_selection.h"

#include <hal/errors.h>

#include <stdio.h>


static Boolean transmit_fixed_message_test(void)
{
	unsigned char buffer[] = "There's a starman waiting in the sky\n"
							 "He'd like to come and meet us\n"
							 "But he thinks he'd blow our minds\n"
							 "CUBESAT: K.Qari3.";
	printf("Message to be sent:\n%s\r\n", buffer);
	int repeats = INPUT_GetINT8("How many times do you want to send this: ");

	for (int i = 0; i < repeats; ++i) {
		if (trxvu_send_message(buffer, ARRAY_SIZE(buffer))) {
			printf("OK Transmit [%d]: %s\r\n", i, buffer);
		} else {
			printf("FAIL Transmit [%d]: %s\r\n", i, buffer);
		}

		delay_ms(200);
	}

	return TRUE;
}

static Boolean transmit_user_message_test(void)
{
	unsigned char buffer[81] = {0};
	INPUT_GetSTRING("What message do you want to send: ", (char*)buffer, 80);
	int repeats = INPUT_GetINT8("How many times do you want to send it: ");
	printf("Will send: %s \r\n", buffer);
	printf("  repeats: %d \r\n", repeats);

	for (int i = 0; i < repeats; ++i) {
		if (trxvu_send_message(buffer, ARRAY_SIZE(buffer))) {
			printf("OK Transmit [%d]: %s\r\n", i, buffer);
		} else {
			printf("FAIL Transmit [%d]: %s\r\n", i, buffer);
		}

		delay_ms(200);
	}

	return TRUE;
}

static Boolean set_ax25_bitrate_test(void)
{
	printf("Enter the new bitrate number:");
	int bitrate = INPUT_GetINT_MinMax("These are the allowed bitrates:\n1)1200\n2)2400\n3)4800\n4)9600",1200,9600);
	while (bitrate != 1200 && bitrate != 2400 && bitrate != 4800 && bitrate != 9600) {
		bitrate = INPUT_GetINT_MinMax("Invalid number , try again:",1200,9600);
	}
	Boolean isSuccesful = trxvu_set_ax25_bitrate(bitrate);
	if (isSuccesful) {
		printf("Bitrate changed successfully , New Bitrate has been set to: %d", bitrate);
	}
	else{
		printf("Bitrate change failed");
	}
	return isSuccesful;
}

static Boolean activate_responder_test(void)//
{
	if (trxvu_activate_responder()) {
		printf("it is Work");
	} else {
		printf("Falid work");
	}
	return TRUE;
}

static Boolean deactivate_responder_test(void)
{
	if (trxvu_deactivate_responder()){
		printf("it was deactivated\n");
	} else {
	    printf("Failed\n");
	}

	return TRUE;
}

static Boolean automatic_responder_stop_test(void)
{
	return TRUE;
}

static Boolean set_responder_rssi(void)
{
	return TRUE;
}

static Boolean get_incoming_frames_test(void)
{
	int frames = trxvu_count_incoming_frames();
	printf("there are %d incoming messages \n", frames);

	for(int i = 0; i < frames; i++) {
		TrxvuRxFrame* frame = trxvu_get_frame();
		printf("message %d: \n message RSSI: %f \n message data :%s \n", i, frame->rssi, frame->framedata);
	}

	return TRUE;
}

static Boolean parse_incoming_spl(void)
{
	int frames = trxvu_count_incoming_frames();
	printf("there are %d incoming messages \n", frames);

	for(int i = 0; i < frames; i++) {
		printf("Frame: %d \n", i);
		TrxvuRxFrame* frame = trxvu_get_frame();
		spl_parse_command(frame);
	}

	return TRUE;
}

static MenuAction menu[] = {
			{ transmit_fixed_message_test, "Transmit fixed text message" },
			{ transmit_user_message_test, "Transmit message defined by user" },
			{ set_ax25_bitrate_test, "Set AX.25 bit rate" },
			{ activate_responder_test, "Activate responder" },
			{ deactivate_responder_test, "Deactivate responder" },
			{ automatic_responder_stop_test, "Activate responder for a time interval" },
			{ set_responder_rssi, "Set Responder RSSI threshold test" },
			{ get_incoming_frames_test, "Get incoming RX frames test" },
			{ parse_incoming_spl, "Parse incoming SPL" },
			MENU_ITEM_END
};

Boolean trxvu_tests(void)
{
	MenuDisplay(menu);
	return TRUE;
}

