/*
 * m_db.c
 *
 *  Created on: Feb 9, 2024
 *      Author: tryfinally
 */

#include "m_db.h"

#include "modules/m_time.h"
#include "utils/ulog_sqlite.h"

#include "utils/timeutils.h"
#include "utils/menu_selection.h"

#include <hal/Timing/Time.h>
#include <at91/utility/rand.h>
#include <at91/utility/trace.h>
#include <hcc/api_fat.h>
#include <hcc/api_fs_err.h>

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


#define BUF_SIZE 512

#define TELEM_FILE_BASE "/db"

static byte buffers[TELEMETRY_TYPES_COUNT][BUF_SIZE];

static struct dblog_write_context write_contexts[TELEMETRY_TYPES_COUNT];

static char folder_names[TELEMETRY_TYPES_COUNT][8] = {
		"TRXVU",
		"ANTS",
		"SOLAR",
		"RADFET",
		"SEL",
		"ELOG"
};


static int32_t read_fn_wctx(struct dblog_write_context *ctx, void *buf, uint32_t pos, size_t len) {
	long r = f_seek(ctx->extra_context, pos, 0);
	if (r != 0) {
		return DBLOG_RES_SEEK_ERR;
	}

	r = f_read(buf, 1, len, ctx->extra_context);
	if ((size_t) r != len) {
		return DBLOG_RES_READ_ERR;
	}

	return len;
}

static int32_t read_fn_rctx(struct dblog_read_context *ctx, void *buf, uint32_t pos, size_t len) {
	long r = f_seek(ctx->extra_context, pos, 0);
	if (r != 0) {
		return DBLOG_RES_SEEK_ERR;
	}

	r = f_read(buf, 1, len, ctx->extra_context);
	if ((size_t)r != len) {
		return DBLOG_RES_READ_ERR;
	}

	return len;
}

static int32_t write_fn_wctx(struct dblog_write_context *ctx, void *buf, uint32_t pos, size_t len) {
	long r = f_seek(ctx->extra_context, pos, 0);
	if (r != 0) {
		return DBLOG_RES_SEEK_ERR;
	}

	r = f_write(buf, 1, len, ctx->extra_context);
	if ((size_t)r != len) {
		return DBLOG_RES_ERR;
	}

	r = f_flush(ctx->extra_context);
	if (r != F_NO_ERROR) {
		return DBLOG_RES_FLUSH_ERR;
	}

	return len;
}

static int flush_fn(struct dblog_write_context *ctx) {
	f_flush(ctx->extra_context);
	return DBLOG_RES_OK;
}


int16_t read_int16(const byte *ptr) {
	return (*ptr << 8) | ptr[1];
}

int32_t read_int32(const byte *ptr) {
	int32_t ret;
	ret  = ((int32_t)*ptr++) << 24;
	ret |= ((int32_t)*ptr++) << 16;
	ret |= ((int32_t)*ptr++) << 8;
	ret |= *ptr;
	return ret;
}

int64_t read_int64(const byte *ptr) {
	int64_t ret;
	ret  = ((int64_t)*ptr++) << 56;
	ret |= ((int64_t)*ptr++) << 48;
	ret |= ((int64_t)*ptr++) << 40;
	ret |= ((int64_t)*ptr++) << 32;
	ret |= ((int64_t)*ptr++) << 24;
	ret |= ((int64_t)*ptr++) << 16;
	ret |= ((int64_t)*ptr++) << 8;
	ret |= *ptr;
	return ret;
}

double read_double(const byte *ptr) {
	double ret;
	int64_t ival = read_int64(ptr);
	memcpy(&ret, &ival, sizeof(double));
	return ret;
}


static char const* make_file_path(DB_TELEMETRY_TYPE telemetry_type)
{
	static char file_path[64];
	strcpy(file_path, folder_names[telemetry_type]);
	strcat(file_path, TELEM_FILE_BASE);
	//TODO: add time stamp YYMMDD

	return file_path;
}


static F_FILE* open_file(DB_TELEMETRY_TYPE telemetry_type, const char* mode)
{
	char const* file_path = make_file_path(telemetry_type);
	// TODO: check if file exists? init for append r+ : init for write w+

	FN_FILE* file = f_open(file_path, mode);
	return file;
}

Boolean db_write_data_blob(DB_TELEMETRY_TYPE telemetry_type, void* record, size_t record_size, size_t number_of_records)
{
	FN_FILE* file = open_file(telemetry_type, "w+");
	if (!file) {
		TRACE_ERROR("open file error: %d\r\n", f_getlasterror());
		return FALSE;
	}

	struct dblog_write_context ctx;
	ctx.buf = buffers[telemetry_type];
	ctx.col_count = 2;
	ctx.page_resv_bytes = 0;
	ctx.page_size_exp = 9;
	ctx.max_pages_exp = 0;
	ctx.read_fn = read_fn_wctx;
	ctx.flush_fn = flush_fn;
	ctx.write_fn = write_fn_wctx;
	ctx.extra_context = file;

	int res = dblog_write_init(&ctx);
	if (res != 0) {
		f_close(file);
		TRACE_ERROR("dblog_write_init: %d\r\n", f_getlasterror());
		return FALSE;
	}

	for (size_t i = 0; i < number_of_records; ++i) {
		unsigned int epoch;
		Time_getUnixEpoch(&epoch);
		res = dblog_set_col_val(&ctx, 0, DBLOG_TYPE_INT, &epoch, sizeof(int));

		res = dblog_set_col_val(&ctx, 1, DBLOG_TYPE_BLOB, record, record_size);

		dblog_append_empty_row(&ctx);
	}

	dblog_finalize(&ctx);
	f_close(file);
	return TRUE;
}


Boolean db_read_data_blob(DB_TELEMETRY_TYPE telemetry_type, void* record, size_t record_size, size_t number_of_records)
{
	FN_FILE* file = open_file(telemetry_type, "r");
	if (!file) {
		TRACE_ERROR("open file error: %d\r\n", f_getlasterror());
		return FALSE;
	}

	struct dblog_read_context ctx;
	ctx.buf = buffers[telemetry_type];
	ctx.read_fn = read_fn_rctx;
	ctx.extra_context = file;

	int res = dblog_read_init(&ctx);
	if (res != 0) {
		f_close(file);
		TRACE_ERROR("error dblog_read_init: %d\r\n", res);
		return FALSE;
	}

	dblog_read_first_row(&ctx);
	for (size_t i = 0; i < number_of_records; ++i) {
		uint32_t col_type;
		const byte* col_val = dblog_read_col_val(&ctx, 0, &col_type);
		unsigned int ival = read_int32(col_val);
		ival = read_int32(ival);

		print_epoch(ival);
		printf("\r\n");
		memcpy(record, col_val, record_size);
		record = (char*) record + record_size;

		dblog_read_next_row(&ctx);
	}

	f_close(file);
	return TRUE;
}


Boolean db_write_data(DB_TELEMETRY_TYPE telemetry_type, void* record, unsigned int record_size)
{
	(void)record;
	(void)record_size;

	FN_FILE* file = open_file(telemetry_type, "w+");
	if (!file) {
		TRACE_ERROR("db_write_data: open file error: %d\r\n", f_getlasterror());
		return FALSE;
	}

	struct dblog_write_context ctx;
	ctx.buf = buffers[telemetry_type];
	ctx.col_count = 4;
	ctx.page_resv_bytes = 0;
	ctx.page_size_exp = 9;
	ctx.max_pages_exp = 0;
	ctx.read_fn = read_fn_wctx;
	ctx.flush_fn = flush_fn;
	ctx.write_fn = write_fn_wctx;
	ctx.extra_context = file;

	int res = dblog_write_init(&ctx);
	if (res != 0) {
		f_close(file);
		TRACE_ERROR("error dblog_write_init: %d\r\n", res);
		return FALSE;
	}

	for (int i = 0; i < 5; ++i) {
		unsigned int epoch;
		Time_getUnixEpoch(&epoch);
		res = dblog_set_col_val(&ctx, 0, DBLOG_TYPE_INT, &epoch, sizeof(int));

		int ival = 100 + i;
		res = dblog_set_col_val(&ctx, 1, DBLOG_TYPE_INT, &ival, sizeof(int));

		ival = i + 20;
		res = dblog_set_col_val(&ctx, 2, DBLOG_TYPE_INT, &ival, sizeof(int));

		double dval = 3.141592 + i/42.0;
		res = dblog_set_col_val(&ctx, 3, DBLOG_TYPE_REAL, &dval, sizeof(double));

		dblog_append_empty_row(&ctx);
	}

	dblog_finalize(&ctx);
	f_close(file);
	return TRUE;
}


Boolean db_append_data(DB_TELEMETRY_TYPE telemetry_type, void* record, unsigned int record_size)
{
	(void)record;
	(void)record_size;

	FN_FILE* file = open_file(telemetry_type, "r+");
	if (!file) {
		TRACE_ERROR("Failed on file open: %d\r\n", f_getlasterror());
		return FALSE;
	}


	struct dblog_write_context ctx;
	ctx.buf = buffers[telemetry_type];
	ctx.col_count = 4;
	ctx.page_resv_bytes = 0;
	ctx.page_size_exp = 9;
	ctx.max_pages_exp = 0;
	ctx.read_fn = read_fn_wctx;
	ctx.flush_fn = flush_fn;
	ctx.write_fn = write_fn_wctx;
	ctx.extra_context = file;

	int res = dblog_init_for_append(&ctx);
	if (res != 0) {
		f_close(file);
		TRACE_ERROR("error dblog_init_for_append: %d\r\n", res);
		return FALSE;
	}

	for (int i = 0; i < 5; ++i) {
		unsigned int epoch;
		Time_getUnixEpoch(&epoch);
		res = dblog_set_col_val(&ctx, 0, DBLOG_TYPE_INT, &epoch, sizeof(int));

		int ival = 100 + i;
		res = dblog_set_col_val(&ctx, 1, DBLOG_TYPE_INT, &ival, sizeof(int));

		ival = i + 20;
		res = dblog_set_col_val(&ctx, 2, DBLOG_TYPE_INT, &ival, sizeof(int));

		double dval = 3.1415 + i/42.0;
		res = dblog_set_col_val(&ctx, 3, DBLOG_TYPE_REAL, &dval, sizeof(double));

		dblog_append_empty_row(&ctx);
	}

	dblog_finalize(&ctx);
	f_close(file);
	return TRUE;
}


Boolean db_read_data(DB_TELEMETRY_TYPE telemetry_type, void* record, unsigned int record_size)
{
	(void)record;
	(void)record_size;

	FN_FILE* file = open_file(telemetry_type, "r");
	if (!file) {
		TRACE_ERROR("error dblog_init_for_append: %d\r\n", f_getlasterror());
		return FALSE;
	}

	struct dblog_read_context ctx;
	ctx.buf = buffers[telemetry_type];
	ctx.read_fn = read_fn_rctx;
	ctx.extra_context = file;

	int res = dblog_read_init(&ctx);
	if (res != 0) {
		f_close(file);
		TRACE_ERROR("error dblog_read_init: %d\r\n", res);
		return FALSE;
	}

	dblog_read_first_row(&ctx);
	for (int i = 0; i < 20; ++i) {
		printf("%.3d | ", i);
		for (int column = 0; column < 4; ++column) {
			uint32_t col_type;
			const byte* col_val = dblog_read_col_val(&ctx, column, &col_type);
			switch (col_type) {
				break; case 0:
					printf("null");
				break; case 1: {
					int ival = *((int8_t *)col_val);
					printf("%d", ival);
				}
				break; case 2: {
					int ival = read_int16(col_val);
					printf("%d", ival);
				}
				break; case 4: {
					int ival = read_int32(col_val);
					printf("%d", ival);
				}
				break; case 6: {
					long ival = read_int64(col_val);
					printf("%ld", ival);
				}
				break; case 7: {
					double dval = read_double(col_val);
			        printf("%f", dval);
			        break;
				}
				break; default: printf("column of type %d", (int)col_type);
			}

			printf(" | ");
		}

		printf("\r\n");
		dblog_read_next_row(&ctx);
	}


	f_close(file);
	return TRUE;
}


Boolean db_delete_data(DB_TELEMETRY_TYPE telemetry_type)
{
	return f_delete(make_file_path(telemetry_type)) == F_NO_ERROR;
}


Boolean m_db_init(void)
{
	for (int i = 0; i < TELEMETRY_TYPES_COUNT; ++i) {
		f_mkdir(folder_names[i]);
	}

	TRACE_INFO("\tDBLOG:   Started.");
	return TRUE;
}

