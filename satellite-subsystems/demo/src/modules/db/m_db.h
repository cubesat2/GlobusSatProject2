/*
 * m_db.h
 *
 *  Created on: Feb 9, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_M_DB_H_
#define MODULES_M_DB_H_

#include <hal/boolean.h>
#include <stddef.h>


typedef enum TELEMETRY_TYPE {
	TELEMETRY_TRXVU,
	TELEMETRY_ANTS,
	TELEMETRY_SOLAR,
	TELEMETRY_RADFET_PAYLOAD,
	TELEMETRY_SEL_PAYLOAD,
	TELEMETRY_ERROR_LOG,

	TELEMETRY_TYPES_COUNT
} DB_TELEMETRY_TYPE;

size_t db_write_data_blob(DB_TELEMETRY_TYPE telemetry_type, void* record, size_t record_size, size_t number_of_records);

size_t db_append_data_blob(DB_TELEMETRY_TYPE telemetry_type, void* record, size_t record_size, size_t number_of_records);

size_t db_read_data_blob(DB_TELEMETRY_TYPE telemetry_type, void* record, size_t record_size, size_t number_of_records);

Boolean db_write_data(DB_TELEMETRY_TYPE telemetry_type, void* record, unsigned int record_size);

Boolean db_append_data(DB_TELEMETRY_TYPE telemetry_type, void* record, unsigned int record_size);

Boolean db_read_data(DB_TELEMETRY_TYPE telemetry_type, void* record, unsigned int record_size);

Boolean db_delete_data(DB_TELEMETRY_TYPE telemetry_type);

Boolean m_db_init(void);

#endif /* MODULES_M_DB_H_ */
