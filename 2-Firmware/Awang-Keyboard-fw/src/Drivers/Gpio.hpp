#pragma once

#include <device.h>
#include <drivers/gpio.h>

class Gpio {
private:
    static bool initialized;
    static device *port0, *port1;
    uint8_t port;
    uint8_t pin;
    gpio_flags_t flags = GPIO_DISCONNECTED;
public:
    Gpio(uint8_t port, uint8_t pin);
    static void Init();
    int Configure(gpio_flags_t flags);
    int Set(int value);
    int Get();
};

extern Gpio scanPL;
extern Gpio scanCE;