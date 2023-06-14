#pragma once

#include <stdint.h>

void ble_hid_init(void);

void ble_hid_send(uint8_t report_id,uint8_t *data);