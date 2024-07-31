/*
 * m_configurations.c
 *
 *  Created on: Mar 8, 2024
 *      Author: tryfinally
 */


#include "m_configurations.h"

#include "config/i2c_address.h"

#include <hal/errors.h>
#include <hal/Storage/FRAM.h>
#include <at91/utility/trace.h>


enum FRAM_ADDRESS {
	EPS_ADDRESS = 0x100,
	BEACON_ADDRESS = EPS_ADDRESS + sizeof(EPS_Params),
	TRXVUE_ADDREES = BEACON_ADDRESS + sizeof(Beacon_Params)
};

Boolean m_config_init(void)
{
	int rv = FRAM_start();
	if(rv != E_NO_SS_ERR && rv != E_IS_INITIALIZED) {
		TRACE_ERROR("\n\r FRAM_start() failed; err=%d! Exiting ... \n\r", rv);
		return FALSE;
	}
	TRACE_INFO("\t\t\tFRAM:    Started.");
	return TRUE;
}

Boolean m_config_read_eps(EPS_Params* eps_params)
{
	int rv = FRAM_read((unsigned char *)eps_params, EPS_ADDRESS, sizeof(EPS_Params));
	if(rv != E_NO_SS_ERR && rv != E_IS_INITIALIZED) {
		TRACE_ERROR("\n\r FRAM_read() failed; err=%d! Exiting ... \n\r", rv);
		return FALSE;
	}
	return TRUE;
}

Boolean m_config_write_eps(EPS_Params const* eps_params)
{
	int rv = FRAM_write((const unsigned char *)eps_params, EPS_ADDRESS, sizeof(EPS_Params));
	if(rv != E_NO_SS_ERR && rv != E_IS_INITIALIZED) {
		TRACE_ERROR("\n\r FRAM_read() failed; err=%d! Exiting ... \n\r", rv);
		return FALSE;
	}
	return TRUE;
}
