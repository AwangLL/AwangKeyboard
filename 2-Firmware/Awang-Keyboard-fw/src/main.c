/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <event_manager.h>

#define MODULE main

#include <logging/log.h>
LOG_MODULE_REGISTER(MODULE);

#include "utils/flash_storage.h"
#include "config.h"

#include "keyboard_event.h"

struct config_set config;

void main(void)
{
	if(flash_storage_read(1, (uint8_t *)&config, sizeof config) < 0) {
		LOG_ERR("flash storage read failed");
	}

	if(!config.saved) {
		LOG_INF("choose the default config");
		config = default_config;
		flash_storage_write(1, (uint8_t *)&config, sizeof config);
	} else {
		LOG_INF("successfully read the configuration");
	}

	if (event_manager_init()) {
		LOG_ERR("Event manager not initialized");
	} else {
		trig_keyboard_event(KEYBOARD_EVENT_READY);
	}
}
