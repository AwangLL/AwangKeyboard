#pragma once

#include <stdbool.h>
#include <stdint.h>

void ble_service_init(void);

void advertising_start(bool erase_bonds);
void advertising_stop(void);

void ble_disconnect(void);
void delete_bonds(void);

uint16_t get_m_conn_handle(void);
uint8_t ble_state(void);