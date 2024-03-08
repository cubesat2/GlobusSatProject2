/*
 * m_filesystem.c
 *
 *  Created on: Jan 15, 2024
 *      Author: tryfinally
 */

#include "m_filesystem.h"

#include <hcc/api_fat.h>
#include <hcc/api_fs_err.h>
#include <hcc/api_hcc_mem.h>
#include <hcc/api_mdriver_atmel_mcipdc.h>

#include <at91/utility/trace.h>

Boolean m_filesystem_init(void)
{
	int rv = hcc_mem_init();
	if (rv != HCC_MEM_SUCCESS) {
		TRACE_ERROR("\n\r hcc_mem_init() failed\n\r");
		return FALSE;
	}

	rv = fs_init();
	if (rv != F_NO_ERROR) {
		TRACE_ERROR("\n\r fs_init() failed\n\r");
		return FALSE;
	}

	rv = f_enterFS();
	if (rv != F_NO_ERROR) {
		TRACE_ERROR("\n\r f_enterFS() failed\n\r");
		return FALSE;
	}

	rv = f_initvolume(0, atmel_mcipdc_initfunc, 0);
	if (rv != F_NO_ERROR) {
		TRACE_ERROR("\n\r f_initvolume() failed\n\r");
		return FALSE;
	}

	f_format(0, F_FAT32_MEDIA);

	rv = f_checkvolume(0);
	if (rv != F_NO_ERROR) {
		TRACE_ERROR("\n\r f_checkvolume() failed\n\r");
		return FALSE;
	}

	F_SPACE space;
	rv = f_getfreespace(0, &space);
	if (rv == F_NO_ERROR) {
		TRACE_INFO("\tSD-FS:   Started.");
		TRACE_INFO("\t\tFree space: %lu", space.free);
		TRACE_INFO("\t\tUsed space: %lu", space.used);
		TRACE_INFO("\t\tBad  space: %lu", space.bad);
	} else {
		TRACE_ERROR("\n\r f_getfreespace() failed\n\r");
	}

	return TRUE;
}
