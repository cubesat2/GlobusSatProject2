/*
 * m_db.h
 *
 *  Created on: Feb 9, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_M_DB_H_
#define MODULES_M_DB_H_

#include <hal/boolean.h>


typedef enum TELEMETRY_TYPE {
	TELEMETRY_TRXVU,
	TELEMETRY_ANTS,
	TELEMETRY_SOLAR,
	TELEMETRY_RADFET_PAYLOAD,
	TELEMETRY_SEL_PAYLOAD,
	TELEMETRY_ERROR_LOG,

	TELEMETRY_TYPES_COUNT
} DB_TELEMETRY_TYPE;

Boolean db_write_data(DB_TELEMETRY_TYPE telemetry_type, void* record, unsigned int record_size);

Boolean db_read_data(DB_TELEMETRY_TYPE telemetry_type, void* record, unsigned int record_size);

Boolean m_db_init(void);

#endif /* MODULES_M_DB_H_ */
