/*
 * m_time.h
 *
 *  Created on: Jan 27, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_M_TIME_H_
#define MODULES_M_TIME_H_

#include <hal/boolean.h>
#include <hal/Timing/Time.h>


Boolean init_time(void);

void print_time(Time const *time);

#endif /* MODULES_M_TIME_H_ */
