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


Boolean m_init_time(void);

void print_time(Time const *time);

void print_epoch(unsigned int epoch);


#endif /* MODULES_M_TIME_H_ */
