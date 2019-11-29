/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <event_manager.h>

#define MODULE main
#include "module_state_event.h"
#include <hal/nrf_power.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(MODULE);


void main(void)
{
	void *leak = k_malloc(1);
	(void)leak;
#if CONFIG_BT_CENTRAL
	{
				nrf_power_task_trigger(NRF_POWER,
						NRF_POWER_TASK_CONSTLAT);
				LOG_WRN("Constant latency enabled");
			}
#endif
	if (event_manager_init()) {
		LOG_ERR("Event manager not initialized");
	} else {
		module_set_state(MODULE_STATE_READY);
	}
}
