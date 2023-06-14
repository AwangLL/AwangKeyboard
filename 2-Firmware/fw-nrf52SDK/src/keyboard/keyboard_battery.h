#pragma once

#include <stdint.h>

typedef struct
{
    uint16_t voltage;
    uint8_t percentage;
} BattInfo;

extern BattInfo battery_info;

void keyboard_battery_init(void);
void keyboard_battery_start(uint8_t time);
