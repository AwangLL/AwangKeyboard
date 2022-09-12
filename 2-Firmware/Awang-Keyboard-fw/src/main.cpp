#include <zephyr.h>
#include <logging/log.h>

#include "Config.h"
#include "Drivers/Nvs.hpp"
#include "Drivers/Timer.hpp"
#include "Drivers/Usb.hpp"
#include "Keyboard/Matrix.hpp"
#include "Keyboard/Led.hpp"

LOG_MODULE_REGISTER(main);

ConfigSet config;
Matrix matrix;
Usb usb;
Led led;

void OnTimerCallback(k_timer* timer_id);
void OnWorkCallback(k_work* work_id);

void LoadConfig() {
    Nvs nvs;
    nvs.Read(1, config);

    if(!config.saved) {
        LOG_INF("Unable to find saved config, choose the default config");
        config = defaultConfig;
        nvs.Write(1, config);
    }
}

/*
    uint32_t start = k_cycle_get_32();
    {


        
    }
    uint32_t stop = k_cycle_get_32();
    LOG_INF("time = %dns\n", (uint32_t)k_cyc_to_ns_floor64(stop - start));
*/

int main() {
    LoadConfig();

    matrix.Enable();
    usb.Enable();
    led.Enable();

    led.Set(0, 0, 255, 0);
    for(int i = 1 ; i <= 14; i++) {
        led.Set(i, (Led::Color)i);
    }

    for(;;) {
        k_sleep(K_SECONDS(1));
    }
}