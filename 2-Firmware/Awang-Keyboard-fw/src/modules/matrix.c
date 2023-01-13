#include <zephyr.h>
#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include <string.h>

#define MODULE matrix
#include <caf/events/module_state_event.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(MODULE, CONFIG_AWANG_MATRIX_LOG_LEVEL);

#include "matrix_def.h"
#include "hid_def.h"
#include "config.h"

#include "matrix_event.h"
#include "keyboard_event.h"
#include "hid_event.h"

static const uint16_t SCAN_TICK_INTERVAL = 
    CONFIG_AWANG_MATRIX_SCAN_INTERVAL / CONFIG_AWANG_CLOCK_TICK_INTERVAL;

/**
 * @brief map from key id to spi read buf index
 * @details the key id is defined by user: map[key_id] = spi_buf[index]
 *          for example: the first key is connected to the D7 port of the 
 *          second chip, the key_id = 1 and the spi_buf index 
 *          = (8 - 7) + (2 - 1) * 8 = 9
 *          so map[1] = 9
 */
uint8_t MAP[KEY_NUM] = {
    5, 6, 12, 20, 28, 36, 35, 34, 33, 72, 78, 77, 83, 82,
    7, 23, 17, 29, 30, 31, 37, 40, 39, 45, 46, 47, 48, 75, 81,
    8, 38, 25, 26, 27, 32, 54, 55, 56, 44, 43, 42, 41, 76, 74,
    3, 18, 19, 24, 22, 21, 63, 52, 51, 50, 49, 53, 64, 73,
    2, 9, 10, 11, 16, 15, 60, 59, 58, 57, 61, 62, 79, 80,
    4, 1, 13, 14, 69, 70, 71, 65, 68, 67, 66,
    84, 85, 86, 87, 88
};

static uint16_t tick_counter;
static bool initialized;
static bool enabled;
static int8_t fn_key_id = -1;
static uint8_t generic_buf[INPUT_REPORT_LEN_KEYBOARD];
static uint8_t scan_buf[HC165_NUM];
static uint8_t debounce_buf[HC165_NUM];
static uint8_t cur_buf[HC165_NUM];
static uint8_t prev_buf[HC165_NUM];
static struct spi_buf rx = {
    .buf = &scan_buf,
    .len = HC165_NUM
};
static struct spi_buf_set rx_buf = {
    .buffers = &rx,
    .count = 1
};
static struct spi_config spi_config = {
    .frequency = 4000000,
    .operation = SPI_WORD_SET(8) | SPI_OP_MODE_MASTER,
    .slave = 0
};
static const struct device* spi = DEVICE_DT_GET(DT_BUS(MATRIX_NODE_LABEL));
static const struct gpio_dt_spec scan_pl_gpio = 
    GPIO_DT_SPEC_GET(MATRIX_NODE_LABEL, pl_gpios);
static const struct gpio_dt_spec scan_ce_gpio = 
    GPIO_DT_SPEC_GET(MATRIX_NODE_LABEL, ce_gpios);


static void buf_add(uint8_t keycode) {
    if(keycode_is_common(keycode)) {
        uint8_t index = keycode / 8 + 2;
        uint8_t pos = keycode % 8;
        generic_buf[index] |= (1 << pos);
    } else if(keycode_is_modifier(keycode)) {
        generic_buf[0] |= (1 << (keycode - 0xE0));
    }
}

static void buf_remove(uint8_t keycode) {
    if(keycode_is_common(keycode)) {
        uint8_t index = keycode / 8 + 2;
        uint8_t pos = keycode % 8;
        generic_buf[index] &= ~(1 << pos);
    } else if(keycode_is_modifier(keycode)) {
        generic_buf[0] &= ~(1 << (keycode - 0xE0));
    }
}

static void buf_clear() {
    memset(generic_buf, 0, sizeof generic_buf);
}

static void matrix_init() {
    if(initialized) return;
    tick_counter = 1;

    if(!device_is_ready(spi)) {
        LOG_ERR("matrix is not ready");
        return;
    }

    if(!device_is_ready(scan_pl_gpio.port)) {
        LOG_ERR("scan pl gpio not ready");
        return;
    }

    if(!device_is_ready(scan_ce_gpio.port)) {
        LOG_ERR("scan ce gpio not ready");
        return;
    }

    if(gpio_pin_configure_dt(&scan_pl_gpio, 
            scan_pl_gpio.dt_flags | GPIO_OUTPUT_INACTIVE)) {
        LOG_ERR("scan pl gpio configuration failed");
    }

    if(gpio_pin_configure_dt(&scan_ce_gpio, 
            scan_ce_gpio.dt_flags | GPIO_OUTPUT_INACTIVE)) {
        LOG_ERR("scan ce gpio configuration failed");
    }

    memset(scan_buf, 0, sizeof scan_buf);
    memset(prev_buf, 0, sizeof prev_buf);
    buf_clear();

    // find the fn index
    for(int i = 0; i < HC165_NUM; i++) {
        if(config.matrix.keymap[0][i] == KC_FN) {
            fn_key_id = MAP[i] - 1;
            break;
        }
    }

    uint8_t temp_map[KEY_NUM];
    for(int i = 0; i < 83; i++) {
        uint8_t index = MAP[i] - 1;
        temp_map[index] = i;
    }
    memcpy(MAP, temp_map, sizeof MAP);
    
    LOG_INF("initialized");
    initialized = true;
}

static void matrix_enable() {
    if(enabled) return;
    enabled = true;
}

static void matrix_disable() {
    if(!enabled) return;
    enabled = false;
}

static void matrix_scan() {
    // load
    gpio_pin_set_dt(&scan_pl_gpio, 1);
    k_sleep(K_USEC(10));
    gpio_pin_set_dt(&scan_pl_gpio, 0);
    // read
    gpio_pin_set_dt(&scan_ce_gpio, 1);
    rx.buf = scan_buf;
    int err;
    err = spi_transceive(spi, &spi_config, NULL, &rx_buf);
    if(err) {
        LOG_WRN("spi read failed with %d", err);
    }
    gpio_pin_set_dt(&scan_ce_gpio, 0);

    // debounce
    for(uint8_t debounce = 0; debounce < DEBOUNCE; debounce++) {
        k_sleep(K_USEC(DEBOUNCE_DELAY));
        // load
        gpio_pin_set_dt(&scan_pl_gpio, 1);
        k_sleep(K_USEC(10));
        gpio_pin_set_dt(&scan_pl_gpio, 0);
        // read
        gpio_pin_set_dt(&scan_ce_gpio, 1);
        rx.buf = debounce_buf;
        int err;
        err = spi_transceive(spi, &spi_config, NULL, &rx_buf);
        if(err) {
            LOG_WRN("spi read failed with %d", err);
        }
        gpio_pin_set_dt(&scan_ce_gpio, 0);

        for(uint8_t i = 0; i < HC165_NUM; i++) {
            scan_buf[i] |= debounce_buf[i];
        }
    }
}

static void matrix_remap() {
    uint8_t changed_buf[HC165_NUM];
    uint8_t layer = 0;
    bool changed = false;
    uint8_t keycode;
    bool key_down;
    
    matrix_scan();

    memcpy(cur_buf, scan_buf, sizeof cur_buf);



    for(int i = 0; i < HC165_NUM; i++) {
        cur_buf[i] = ~cur_buf[i];
        changed_buf[i] = prev_buf[i] ^ cur_buf[i];
        prev_buf[i] = cur_buf[i];
    }
    
    if(cur_buf[fn_key_id / 8] & (1 << (fn_key_id / 8 * 8 + 7 - fn_key_id))) {
        layer = 1;
    }

    for(int i = 0; i < HC165_NUM; i++) {
        if(changed_buf[i]) {
            changed = true;
            for(int j = 0; j < 8; j++) {
                uint8_t mask = 1 << j;
                if(changed_buf[i] & mask) { // Key state changed(pressed or released)
                    uint8_t map_index = i * 8 + 7 - j;
                    if(cur_buf[i] & mask) {
                        key_down = true;
                        trig_matrix_event(MAP[map_index], true);
                    } else {
                        key_down = false;
                        trig_matrix_event(MAP[map_index - 1], false);
                    }
                    
                    // get keycode
                    keycode = config.matrix.keymap[layer][MAP[map_index]];
                    if(layer && keycode == KC_NO) { //fn
                        keycode = config.matrix.keymap[0][MAP[map_index]];
                    }

                    // keycode type
                    if(keycode_is_common(keycode) || keycode_is_modifier(keycode)) {
                        if(key_down) buf_add(keycode);
                        else buf_remove(keycode);
                    } else if(keycode_is_consumer(keycode)) {
                        // Host::SendConsumer(keycode, key_down);
                    } else if(keycode_is_function(keycode)) {
                        
                    }
                }
            }
        }
    }
    
    if(changed) {
        trig_hid_report_send_event(INPUT_REPORT_ID_KEYBOARD, &generic_buf);
    }
    
}

static bool event_handler(const struct event_header *eh) {
    if(is_keyboard_event(eh)) {
        switch(cast_keyboard_event(eh)->event) {
        case KEYBOARD_EVENT_READY:
            matrix_init();
            matrix_enable();
            break;
        
        case KEYBOARD_EVENT_TICK:
            if(!enabled) break;
            if(tick_counter < SCAN_TICK_INTERVAL) {
                tick_counter++;
            } else {
                tick_counter = 1;
                // LOG_INF("remap start");
                matrix_remap();
                // LOG_INF("remap end");
            }
            break;
        
        default:
            break;
        }
        return false;
    }
    return false;
}

EVENT_LISTENER(MODULE, event_handler);
EVENT_SUBSCRIBE(MODULE, keyboard_event);