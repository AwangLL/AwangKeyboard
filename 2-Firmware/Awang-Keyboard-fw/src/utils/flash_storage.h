#pragma once

#include <stdint.h>

int flash_storage_read(uint16_t id, uint8_t *data, uint16_t len);

int flash_storage_write(uint16_t id, uint8_t *data, uint16_t len);