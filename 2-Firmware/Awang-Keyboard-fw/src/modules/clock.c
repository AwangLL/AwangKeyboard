#include <zephyr.h>

#define MODULE clock

#include <logging/log.h>
LOG_MODULE_REGISTER(MODULE, CONFIG_AWANG_CLOCK_LOG_LEVEL);

#include "keyboard_event.h"

#define TICK_INTERVAL CONFIG_AWANG_CLOCK_TICK_INTERVAL

struct k_timer timer;
bool initialized = false;
bool enabled = false;

static void on_timer_callback(struct k_timer* timer_id) {
    trig_keyboard_event(KEYBOARD_EVENT_TICK);
}

static void clock_init() {
    if(initialized) return;
    k_timer_init(&timer, on_timer_callback, NULL);
    k_timer_start(&timer, K_MSEC(TICK_INTERVAL), K_MSEC(TICK_INTERVAL));
    LOG_INF("initialized");
    initialized = true;
}

static bool event_handler(const struct event_header *eh) {
    if(is_keyboard_event(eh)) {
        switch(cast_keyboard_event(eh)->event) {
        case KEYBOARD_EVENT_READY:
            clock_init();
            break;
        
        default:
            break;
        }
        return false;
    }
    
    return false;
}

EVENT_LISTENER(MODULE, event_handler);
EVENT_SUBSCRIBE_FINAL(MODULE, keyboard_event);