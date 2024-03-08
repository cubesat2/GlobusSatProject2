/*
 * test_common.h
 *
 *  Created on: Mar 8, 2024
 *      Author: tryfinally
 */

#ifndef TESTS_TEST_COMMON_H_
#define TESTS_TEST_COMMON_H_

#include <hal/boolean.h>
#include <hal/errors.h>
#include <at91/utility/trace.h>

#include "utils/menu_selection.h"

#define VERIFY(RESULT, formatLiteral, ...)	do { if (RESULT) TRACE_INFO(formatLiteral "Pass!", ## __VA_ARGS__); \
												 else TRACE_ERROR(formatLiteral "Failed!", ## __VA_ARGS__); } while(0)

#endif /* TESTS_TEST_COMMON_H_ */
