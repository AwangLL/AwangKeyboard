#pragma once

#include <stdint.h>
#include <drivers/led_strip.h>
#include <device.h>
#include <drivers/spi.h>
#include <sys/util.h>
#include "../Drivers/Timer.hpp"

// 
struct LRGB {
    // value(in HSV) of the pixel
    // so the Led::GetValueFromRGB(pixel->r, pixel->g, pixel->b) == v
    uint8_t v;
    // red
    uint8_t r;
    // green
    uint8_t g;
    // blue
    uint8_t b;
    // pointer to the pixel mapping
    led_rgb *pixel;
    void Set(uint8_t r, uint8_t g, uint8_t b);
    void Sync(int x, int y);
    void Sync(uint8_t r, uint8_t g, uint8_t b);
    
};

class Led {
public:
    enum Color {
        White,  Red,    Coral,  Orange, Goldenrod,
        Gold,   Yellow, Chartreuse, Green,  Springgreen,
        Turquoise,  Cyan,   Blue,   Purple, Magenta,
        Pink,   COLOR_NUMBER
    };
    static const uint8_t LED_NUM = 100;
    static const uint16_t MAX_HUE = 360;
    static const uint8_t MAX_SATURATION = 0xFF;
    static const uint8_t MAX_VALUE = 0xFF;
    static const uint8_t ALLOW_VALUE = 200;
    static const uint8_t MAX_BRIGHTNESS = 100;
    static const int CALC_PRECSION = 1e4;
    static const uint8_t COLORS[][3];
    static uint8_t GetValueFromRGB(uint8_t r, uint8_t g, uint8_t b);
    static void HSV2RGB(uint16_t hh, uint8_t ss, uint8_t vv, uint8_t &rr, uint8_t &gg, uint8_t &bb);
    static void RGB2HSV(uint8_t rr, uint8_t gg, uint8_t bb, uint16_t &hh, uint8_t &ss, uint8_t &vv);
private:
    static void OnTimeCallback(void *);
    bool initialized = false;
    bool enabled = false;
    bool changed = false;
    device* strip;
    LRGB leds[LED_NUM];
    led_rgb pixels[LED_NUM];
    Timer timer;
    uint8_t brightness = 50;
    void SyncPixel();
    void SyncPixel(int id);
    void SyncPixel(int id, uint8_t value);
public:
    Led() : timer(OnTimeCallback) {}
    void Init();
    void Enable();
    void Disable();
    void Set(uint8_t id, uint8_t r, uint8_t g, uint8_t b);
    void Set(uint8_t id, Color color);
    void Set(uint8_t id, uint8_t value);
    void Set(uint8_t brightness);
    void ChangeBrightness(uint8_t brightness);
    void Update();
};

extern Led led;