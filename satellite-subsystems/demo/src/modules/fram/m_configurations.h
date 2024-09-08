/*
 * m_configurations.h
 *
 *  Created on: Mar 8, 2024
 *      Author: tryfinally
 */

#ifndef MODULES_M_CONFIGURATIONS_H_
#define MODULES_M_CONFIGURATIONS_H_

#include <stdint.h>

#include <hal/boolean.h>

typedef struct EPS_Params {
    union {
        struct __attribute__ ((__packed__)) {
            double v_normal_up;
            double v_normal_down;

            double v_safe_up;
            double v_safe_down;
        };
        char raw[sizeof(double)*4];
    };
} EPS_Params;


typedef struct Beacon_Params {
    union {
        struct __attribute__ ((__packed__)) Fields{
        	uint16_t period_ms;
            unsigned char sat_callsign[8];
            unsigned char ground_callsign[8];
            uint16_t rssi;
        } fields;
        char raw[sizeof(struct Fields)];
    };
} Beacon_Params;

Boolean m_config_init(void);

Boolean m_config_read_eps(EPS_Params* eps_params);

Boolean m_config_write_eps(EPS_Params const* eps_params);

Boolean m_config_write_param(uint32_t address, uint32_t offset, void* data, uint32_t length);

Boolean m_config_read_param(uint32_t address, uint32_t offset, void* data, uint32_t length);

#endif /* MODULES_M_CONFIGURATIONS_H_ */
