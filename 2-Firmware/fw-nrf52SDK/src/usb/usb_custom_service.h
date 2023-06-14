#pragma once

#include <stdbool.h>
#include <stdint.h>
void usb_event_excute(void);
void usb_service_init(void);

void usb_hid_send(uint8_t report_id, uint8_t *data);

uint8_t usb_state(void);