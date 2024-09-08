/*
 * flight_params.h
 *
 *  Created on: Aug 1, 2024
 *      Author: tryfinally
 */

#ifndef CONFIG_FLIGHT_PARAMS_H_
#define CONFIG_FLIGHT_PARAMS_H_

#include <stdint.h>
#include <hal/boolean.h>

enum FRAM_ADDRESS {
	FRAM_BASE_ADDRESS = 0x0,
	EPS_ADDRESS = 0x100,
//	BEACON_ADDRESS = EPS_ADDRESS + sizeof(EPS_Params),
//	TRXVUE_ADDREES = BEACON_ADDRESS + sizeof(Beacon_Params)
};

typedef struct FlightParams {
    union {
        struct __attribute__ ((__packed__)) Fields2{
        	unsigned char sat_callsign[8];
        	unsigned char ground_callsign[8];
        	uint32_t 	reset_count;
        	uint32_t 	reset_cmd_count;
        	uint32_t 	sat_time;
        	uint32_t 	beacon_interval_sec;
        	Boolean 		txvu_muted;
            uint16_t 	responder_rssi;
            	uint32_t 	eps_tlm_interval_sec;
            	uint32_t 	solar_tlm_interval_sec;
            	uint32_t 	ant_tlm_interval_sec;
            	uint32_t 	trxvu_tlm_interval_sec;
        } fields;
        char raw[sizeof(struct Fields2)];
    };
} FlightParams;

typedef struct __attribute__ ((__packed__)) FlightConfiguration{
	struct __attribute__ ((__packed__)) {
		unsigned char sat_callsign[8];
		unsigned char ground_callsign[8];
		unsigned int last_epoch_set_from_earth;
		unsigned int last_epoch_updated;

		uint32_t 	reset_count;
		uint32_t 	reset_cmd_count;
		uint32_t 	sat_time;
	} general;
	struct __attribute__ ((__packed__)) {
		uint32_t 	beacon_interval_sec;
		Boolean 	txvu_muted;
		uint16_t 	responder_rssi;
		uint32_t 	eps_tlm_interval_sec;
		uint32_t 	solar_tlm_interval_sec;
		uint32_t 	ant_tlm_interval_sec;
		uint32_t 	trxvu_tlm_interval_sec;
	} telemetry;
	struct __attribute__ ((__packed__)) EPS {
        double v_normal_up;
        double v_normal_down;
        double v_safe_up;
        double v_safe_down;
	} eps;
} FlightConfiguration;

extern FlightConfiguration g_flight_config;

// <Satellite Management>
#define DEPLOYMENT_TIME_ADDR			0X05		//<! time at which the satellites starts deployment
#define DEPLOYMENT_TIME_SIZE			4			//<! size of parameter in bytes

#define SECONDS_SINCE_DEPLOY_ADDR		0x09		//<! counts how many seconds has past since wakup for use in deployment.
#define SECONDS_SINCE_DEPLOY_SIZE		4			//<! size of the parameter in bytes

#define DEFAULT_NO_COMM_WDT_KICK_TIME  (7*24*60*60)	//<! number of seconds in X days
#define NO_COMM_WDT_KICK_TIME_ADDR  	0x24		///< number of seconds of no communications before GS WDT kick
#define NO_COMM_WDT_KICK_TIME_SIZE		4

#define STOP_REDEPOLOY_FLAG_ADDR		0x30		//<! stop calling ant deploy every 30 min flag
#define STOP_REDEPOLOY_FLAG_SIZE		4			//<! length in bytes of

#define MUTE_END_TIME_ADDR				0x36		//<! mute end time
#define MUTE_END_TIME_SIZE				4			//<! length in bytes of

#define FIRST_ACTIVATION_FLAG_ADDR		0x42		//<! is this the first activation after launch flag
#define FIRST_ACTIVATION_FLAG_SIZE		4			//<! length in bytes of FIRST_ACTIVATION_FLAG

#define TRANSPONDER_END_TIME_ADDR		0x48		//<! mute end time
#define TRANSPONDER_END_TIME_SIZE		4			//<! length in bytes of

#define TRANSPONDER_RSSI_ADDR			0x52		//<! mute end time
#define TRANSPONDER_RSSI_SIZE			2			//<! length in bytes of

#define MOST_UPDATED_SAT_TIME_ADDR		0x54		//<! this parameters saves the sat time to be read after resets
#define MOST_UPDATED_SAT_TIME_SIZE		4			//<! size of the parameter in bytes

#define NUMBER_OF_RESETS_ADDR			0x60		//<! counts how many restarts did the satellite endure
#define NUMBER_OF_RESETS_SIZE			4			//<! size of the parameter in bytes

#define RESET_CMD_FLAG_ADDR				0x105		//<! the flag is raised whenever a restart is commissioned
#define RESET_CMD_FLAG_SIZE				1			//<! size of the parameter in bytes

#define TLM_SAVE_PERIOD_START_ADDR		0x130		//<! start of the save periods in the FRAM

#define DEFAULT_EPS_SAVE_TLM_TIME		5			//<! save EPS TLM every 5 seconds
#define EPS_SAVE_TLM_PERIOD_ADDR		0x130		//<! address where the save tlm period will be

#define DEFAULT_TRXVU_SAVE_TLM_TIME		5			//<! save TRXVU TLM every 5 seconds
#define TRXVU_SAVE_TLM_PERIOD_ADDR		0x134		//<! address where the save tlm period will be

#define DEFAULT_ANT_SAVE_TLM_TIME		5			//<! save antenna TLM every 5 seconds
#define ANT_SAVE_TLM_PERIOD_ADDR		0x138		//<! address where the save tlm period will be

#define DEFAULT_SOLAR_SAVE_TLM_TIME		5			//<! save solar panel TLM every 5 seconds
#define SOLAR_SAVE_TLM_PERIOD_ADDR		0x142		//<! address where the save tlm period will be

#define DEFAULT_WOD_SAVE_TLM_TIME		5			//<! save WOD TLM every 5 seconds
#define WOD_SAVE_TLM_PERIOD_ADDR		0x146		//<! address where the save tlm period will be

#define NUMBER_OF_CMD_RESETS_ADDR		0x160		//<! counts how many restarts did the satellite endure from ground command
#define NUMBER_OF_CMD_RESETS_SIZE		4			//<! size of the parameter in bytes

#define DELAYED_CMD_FRAME_COUNT_ADDR	0X200		//<! Address where amount of frames in delayed buffer is held
#define DELAYED_CMD_FRAME_COUNT_SIZE	1			//<! number of bytes describing the frame count of delayed buffer

#define DEL_OLD_FILES_NUM_DAYS_ADDR		0X210		//<! Address where amount of frames in delayed buffer is held
#define DEL_OLD_FILES_NUM_DAYS_SIZE		4			//<! number of bytes describing the frame count of delayed buffer

#define TRANS_ABORT_FLAG_ADDR			0x500		//<! transmission abort request flag
#define TRANS_ABORT_FLAG_SIZE			1			//<! size of mute flag in bytes

#define EPS_THRESH_VOLTAGES_ADDR		0x666		//<! starting address for eps threshold voltages array
#define EPS_THRESH_VOLTAGES_SIZE (NUMBER_OF_THRESHOLD_VOLTAGES * sizeof(voltage_t)) //<! number of bytes in eps threshold voltages array

#define EPS_ALPHA_FILTER_VALUE_ADDR 0x550			//<! filtering value in the LPF formula
#define EPS_ALPHA_FILTER_VALUE_SIZE sizeof(float)	//<! size of double (alpha)

#define BEACON_INTERVAL_TIME_ADDR 		0x4590		//<! address of value of the delay between 2 beacons
#define BEACON_INTERVAL_TIME_SIZE 		4			//<! size of parameter in bytes

#define LAST_COMM_TIME_ADDR 			0X9485		//<! saves the last unix time at which communication has occured
#define LAST_COMM_TIME_SIZE				4			//<! size of last communication time in bytes

#define DEFAULT_BEACON_INTERVAL_TIME 	20			//<! how many seconds between two beacons [sec]
#define MAX_BEACON_INTERVAL				60			// beacon every 1 minute
#define MIN_BEACON_INTERVAL				10			// beacon every 10 secinds



#endif /* CONFIG_FLIGHT_PARAMS_H_ */
