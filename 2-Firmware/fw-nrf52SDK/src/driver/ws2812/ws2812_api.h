#ifndef WS2812_H
#define WS2812_H

#include <stdint.h>

#define MAX_HUE 360
#define MAX_SATURATION 100
#define MAX_VALUE 0xff
#define ALLOW_VALUE 200

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgb_t;

typedef struct 
{
    uint16_t hue;
    uint8_t saturation;
    uint8_t value; 
} hsv_t;

typedef struct 
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint16_t hue;
    uint8_t saturation;
    uint8_t value; 
    uint8_t change;
} led_t;

void ws2812_init();
void ws2812_set(uint8_t id, uint16_t hue, uint16_t saturation, uint16_t value);
void ws2812_set_v(uint8_t id, uint16_t value);
void ws2812_show(void);

#endif
