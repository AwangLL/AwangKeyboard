#include "Gpio.hpp"

#include <logging/log.h>
#include "../Keyboard/Power.hpp"

LOG_MODULE_REGISTER(Gpio);

device* Gpio::port0 = nullptr;
device* Gpio::port1 = nullptr;
bool Gpio::initialized = false;

Gpio::Gpio(uint8_t port, uint8_t pin) {
    if(port > 1) {
        printk("gpio port error, must be 0 or 1\n");
        return;
    }
    else if(pin > 32) {
        printk("gpio pin error, must less than or equal 31\n");
        return;
    }
    
    this->port = port;
    this->pin = pin;
}

void Gpio::Init() {
    if(initialized) return;
    port0 = (device *)device_get_binding("GPIO_0");
    port1 = (device *)device_get_binding("GPIO_1");

    if(port0 == nullptr) {
        LOG_ERR("gpio 0 not found");
        Power::Stop();
    }
    if(port1 == nullptr) {
        LOG_ERR("gpio 1 not found");
        Power::Stop();
    }
    if(!device_is_ready(port0)) {
        LOG_ERR("gpio 0 not ready");
        Power::Stop();
    }
    if(!device_is_ready(port1)) {
        LOG_ERR("gpio 1 not ready");
        Power::Stop();
    }

    initialized = true;
}

int Gpio::Configure(gpio_flags_t flags) {
    if(!initialized) Init();
    this->flags = flags;
    if(port) {
        return gpio_pin_configure(port1, pin, flags);
    } else {
        return gpio_pin_configure(port0, pin, flags);
    }
}

int Gpio::Set(int value) {
    if(flags == GPIO_DISCONNECTED) {
        LOG_WRN("gpio%d.%d not configurated", port, pin);
    }
    if(port) {
        return gpio_pin_set(port1, pin, value);
    } else {
        return gpio_pin_set(port0, pin, value);
    }
}

int Gpio::Get() {
    if(flags == GPIO_DISCONNECTED) {
        LOG_WRN("gpio%d.%d not configurated", port, pin);
    }
    if(port) {
        return gpio_pin_get(port1, pin);
    } else {
        return gpio_pin_get(port0, pin);
    }
}

Gpio scanPL(1, 10);
Gpio scanCE(0, 3);