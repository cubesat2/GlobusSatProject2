/*
 * modules_manager.c
 *
 *  Created on: Dec 28, 2023
 *      Author: Muhammad Zahalqa
 */

#include "modules_manager.h"

#include "modules/fram/m_configurations.h"
#include "trxvu/m_trxvu.h"
#include "modules/eps/m_gomeps.h"
#include "modules/solar_panels/m_solar_panels.h"
#include "modules/antenna/m_antenna.h"
#include "modules/fat_fs/m_filesystem.h"
#include "modules/time_keeper/m_time.h"
#include "modules/db/m_db.h"

#include <hal/Drivers/I2C.h>
#include <at91/utility/trace.h>


#define I2C_BUS_SPEED_HZ		100000
#define I2C_TIMEOUT				10

static Boolean init_i2c(void)
{
	int retValInt = I2C_start(I2C_BUS_SPEED_HZ, I2C_TIMEOUT);
	if(retValInt != 0) {
		TRACE_FATAL("\n\r I2C_start_Master for demo: %d! \n\r", retValInt);
		return FALSE;
	}
	TRACE_INFO("\t\t\t\tI2C:     Started.");
	return TRUE;
}


Boolean mm_init()
{
	Boolean result = m_time_init();

	result &= init_i2c();
	result &= m_config_init();
	result &= m_trxvu_init();
	result &= m_gom_eps_init();
	result &= m_solar_panels_init();
	result &= m_antenna_init();
	result &= m_filesystem_init();
	result &= m_db_init();

	return result;
}
