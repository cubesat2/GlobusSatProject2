/*
 * m_db.c
 *
 *  Created on: Feb 9, 2024
 *      Author: tryfinally
 */

#include "m_db.h"
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

Boolean db_write_data(DB_TELEMETRY_TYPE telemetry_type, void* record, unsigned int record_size)
{
	(void)record;
	(void)record_size;

	char data_file[64];
	strcpy(data_file, folder_names[telemetry_type]);
	strcat(data_file, "db");

	FN_FILE* file = f_open(data_file, "w+");
	if (!file) {
		printf("Failed on file open\n");
		printf("Error code: %d\n", f_getlasterror());
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
		printf("error dblog_init: %d\n", res);
		return FALSE;
	}

	for (int i = 0; i < 10; ++i) {
		unsigned int epoch;
		Time_getUnixEpoch(&epoch);
		res = dblog_set_col_val(&ctx, 1, DBLOG_TYPE_INT, &epoch, sizeof(int));

		int ival = 100 + i;
		res = dblog_set_col_val(&ctx, 2, DBLOG_TYPE_INT, &ival, sizeof(int));

		ival = i + 20;
		res = dblog_set_col_val(&ctx, 3, DBLOG_TYPE_INT, &ival, sizeof(int));

		double dval = 1.0 + (i+1)/10;
		res = dblog_set_col_val(&ctx, 4, DBLOG_TYPE_REAL, &dval, sizeof(int));

		dblog_append_empty_row(&ctx);
		delay_ms(10);
	}

	dblog_finalize(&ctx);
	f_close(file);
	return TRUE;
}

Boolean db_read_data(DB_TELEMETRY_TYPE telemetry_type, void* record, unsigned int record_size)
{
	(void)record;
	(void)record_size;

	char data_file[64];
	strcpy(data_file, folder_names[telemetry_type]);
	strcat(data_file, "db");

	FN_FILE* file = f_open(data_file, "w+");
	if (!file) {
		printf("Failed on file open\n");
		printf("Error code: %d\n", f_getlasterror());
		return FALSE;
	}

	struct dblog_read_context ctx;
	ctx.buf = buffers[telemetry_type];
	ctx.read_fn = read_fn_rctx;
	ctx.extra_context = file;

	int res = dblog_read_init(&ctx);
	if (res != 0) {
		f_close(file);
		printf("error dblog_read_init: %d\n", res);
		return FALSE;
	}

	dblog_read_first_row(&ctx);
	for (int i = 0; i < 10; ++i) {
		for (int column = 1; column < 4; ++column) {
			uint32_t col_type;
			const byte* col_val = dblog_read_col_val(&ctx, 1, &col_type);
			switch (col_type) {
				break; case 0:
					printf("null");
				break; case 1:
					printf(PRId8, *((int8_t *)col_val));
				break; case 2: {
					int16_t ival = read_int16(col_val);
					printf(PRId16, ival);
				}
				break; case 4: {
					int32_t ival = read_int32(col_val);
					printf(PRId32, ival);
				}
				break; case 6: {
					int64_t ival = read_int64(col_val);
					printf(PRId64, ival);
				}
			}

			dblog_read_next_row(&ctx);
		}
	}

	f_close(file);
	return TRUE;
}



Boolean m_db_init(void)
{
	for (int i = 0; i < TELEMETRY_TYPES_COUNT; ++i) {
		f_mkdir(folder_names[i]);
	}

	TRACE_INFO("\tDBLOG:   Started.");
	return TRUE;
}

