/*
 * m_time.h
 *
 *  Created on: Jan 27, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_M_TIME_H_
#define MODULES_M_TIME_H_

#include <stdint.h>
#include <hal/boolean.h>
#include <hal/Timing/Time.h>


Boolean m_time_init(void);

Boolean m_time_settime(unsigned int epoch);

void m_time_print_time(Time const *time);

void m_time_print_epoch(unsigned int epoch);


#endif /* MODULES_M_TIME_H_ */
