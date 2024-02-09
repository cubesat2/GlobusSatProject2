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
#include <hcc/api_fat.h>
#include <hcc/api_fs_err.h>

#include <string.h>
#include <stdio.h>


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

	return r;
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

	return r;
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
	return r;
}

static int flush_fn(struct dblog_write_context *ctx) {
	f_flush(ctx->extra_context);
	return DBLOG_RES_OK;
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

	int res = dblog_init_for_append(&ctx);
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



Boolean m_db_init(void)
{
	for (int i = 0; i < TELEMETRY_TYPES_COUNT; ++i) {
		f_mkdir(folder_names[i]);
	}

	TRACE_INFO("\tDBLOG:   Started.");
	return TRUE;
}

