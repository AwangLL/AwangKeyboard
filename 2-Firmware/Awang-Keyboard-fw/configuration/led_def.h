#pragma once

#include <device.h>

#define LED_NODE_LABEL DT_NODELABEL(led_strip)
#define LED_NUM DT_PROP(LED_NODE_LABEL, chain_length)
#define MAX_HUE 360
#define MAX_SATURATION 255
#define MAX_VALUE 255
#define ALLOW_VALUE 200
#define MAX_BRIGHTNESS 100
#define CALC_PRECSION 10000

#define LED_COLOR_LIST\
    X(WHITE)\
    X(RED)\
    X(CORAL)\
    X(ORANGE)\
    X(GOLDENROD)\
    X(GOLD)\
    X(YELLO)\
    X(CHARTREUSE)\
    X(GREEN)\
    X(SPRINGGREEN)\
    X(TURQUOISE)\
    X(CYAN)\
    X(BLUE)\
    X(PURPLE)\
    X(MAGENTA)\
    X(PINK)

enum led_color_list {
#define X(color) _CONCAT(LED_COLOR_, color),
    LED_COLOR_LIST
#undef X


    LED_COLOR_COUNT
};

/** 
 * @brief preserved rgb map
 */
extern const uint8_t colors[][3];
const uint8_t colors[][3] = {
    {255, 255,  255},
    {255, 0,   0},
    {255, 127, 80},
    {255, 165, 0},
    {218, 165, 32},
    {255, 185, 15},
    {255, 255, 0},
    {127, 255, 0},
    {0,   255, 0},
    {0,   255, 127},
    {64,  224, 208},
    {0,   255, 255},
    {0,   0,   255},
    {160, 32,  240},
    {255, 0,   255},
    {255, 192, 203}
};

/**
 * @brief led rgb
 */
struct lrgb {
    uint8_t v;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    struct led_rgb *pixel;
};