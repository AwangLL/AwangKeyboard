#include <zephyr.h>
#include <drivers/led_strip.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include <device.h>

#define MODULE led

#include <logging/log.h>
LOG_MODULE_REGISTER(MODULE, CONFIG_AWANG_LED_LOG_LEVEL);

#include "led_def.h"

#include "keyboard_event.h"

static const struct device *strip = DEVICE_DT_GET(LED_NODE_LABEL);
static const struct gpio_dt_spec power_supply_gpio = GPIO_DT_SPEC_GET(LED_NODE_LABEL, supply_gpios);

static bool initialized;
static bool enabled;
/** 
 * @brief indicate the maximum length of led strip that has changed
 * @param -1: no led changed
 * @param 0~LED_NUM-1: valid
 */
static int16_t changed_length;
static uint8_t brightness = 50;
static struct lrgb leds[LED_NUM];
static struct led_rgb pixels[LED_NUM];

/**
 * @brief help update rgb of pixels[id] referring to the value of leds[id] and brightness
 */
static void pixel_sync_xy(int id, int x, int y) {
    struct led_rgb* pixel = leds[id].pixel;
    if(y) {
        pixel->r = (uint8_t)(leds[id].r * x / y);
        pixel->g = (uint8_t)(leds[id].g * x / y);
        pixel->b = (uint8_t)(leds[id].b * x / y);
    } else {
        pixel->r = 0;
        pixel->g = 0;
        pixel->b = 0;
    }
}

/**
 * @brief calculate the value from rgb
 */
static uint8_t get_value_from_rgb(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t v = r > g ? r : g;
    return v > b ? v : b;
}

/**
 * @brief call when the rgb of the leds[id] changed
 */
static void sync_pixel(int id) {
    struct lrgb *led = &leds[id];
    pixel_sync_xy(id, (int)MAX_VALUE * brightness,
        (int)led->v * MAX_BRIGHTNESS);
    changed_length = MAX(changed_length, id);
}

/**
 * @brief call when the value of the leds[id] changed
 */
static void sync_pixel_value(int id, uint8_t value) {
    struct lrgb *led = &leds[id];
    leds[id].v = value;
    pixel_sync_xy(id, led->v * brightness,
        (int)get_value_from_rgb(led->r, led->g, led->b) * MAX_BRIGHTNESS);
}

/**
 * @brief call when the brightness changed
 */
static void sync_pixels() {
    for(int i = 0; i < LED_NUM; i++) {
        uint8_t led_value = get_value_from_rgb(leds[i].r, leds[i].g, leds[i].b);
        pixel_sync_xy(i, (int)leds[i].v * brightness,
            (int)led_value * MAX_BRIGHTNESS);
    }
    changed_length = LED_NUM - 1;
}

static void hsv_to_rgb(uint16_t hh, uint8_t ss, uint8_t vv, uint8_t *rr, uint8_t *gg, uint8_t *bb) {
    if(hh >= MAX_HUE) hh = 0;
    int c, x, y, z;
    int r, g, b;
    uint8_t i = 0;

    c = x = y = z = 0;

    if (ss == 0) r = g = b = vv;
    else {
        i = hh / 60;
        c = hh * 1000 / 60 - i * 1000;

        x = vv - vv * ss / 255;
        y = vv - vv * ss * c / 255000;
        z = vv * (255 - ss * (1000 - c) / 1000) / 255;
        switch (i) {
            case 0: r = vv; g = z; b = x; break;
            case 1: r = y; g = vv; b = x; break;
            case 2: r = x; g = vv; b = z; break;
            case 3: r = x; g = y; b = vv; break;
            case 4: r = z; g = x; b = vv; break;
            case 5: r = vv; g = x; b = y; break;
            default: r = g = b = 0; break;
        }
    }
    *rr = r;
    *gg = g;
    *bb = b;
}

static void rgb_to_hsv(uint8_t rr, uint8_t gg, uint8_t bb, uint16_t *hh, uint8_t *ss, uint8_t *vv) {
    // avoid float calculation
    int r = rr * CALC_PRECSION / 0xFF;
    int g = gg * CALC_PRECSION / 0xFF;
    int b = bb * CALC_PRECSION / 0xFF;
    int max, min;
    int h, s, v;
    
    max = r > g ? r : g;
    max = max > b ? max : b;
    min = r < g ? r : g;
    min = min < b ? min : b;

    if(max == min) {
        h = 0;
        s = 0;
        v = r;
    } else {
        v = max;
        if(v != 0) {
            s = (max - min) * CALC_PRECSION / max;
        } else {
            s = 0;
        }
        if(r == max) {
            h = 60 * (g - b) / (max - min);
        } else if (g == max) {
            h = 120 + 60 * (b - r) / (max - min);
        } else {
            h = 240 + 60 * (r - g) / (max - min);
        }
    }
    
    if(h < 0) h += 360;

    *hh = h;
    *ss = s * MAX_SATURATION / CALC_PRECSION;
    *vv = v * MAX_VALUE / CALC_PRECSION;
}

static void led_init() {
    if(initialized) return;
    if(!device_is_ready(strip)) {
        LOG_ERR("strip is not ready");
        return;
    }
    if(!device_is_ready(power_supply_gpio.port)) {
        LOG_ERR("power supply gpio not ready");
        return;
    }

    if(gpio_pin_configure_dt(&power_supply_gpio, 
            power_supply_gpio.dt_flags | GPIO_OUTPUT_HIGH)) {
        LOG_ERR("power supply gpio configuration failed");
    }

    for(uint8_t i = 0; i < LED_NUM; i++) {
        leds[i].pixel = &pixels[i];
    }

    LOG_INF("initialized");
    initialized = true;
}

static void led_enable() {
    if(enabled) return;
    gpio_pin_set_dt(&power_supply_gpio, 1);
    LOG_INF("enabled");
    enabled = true;
}

static void led_disable() {
    if(!enabled) return;
    gpio_pin_set_dt(&power_supply_gpio, 0);
    LOG_INF("disabled");
    enabled = false;
}

/**
 * @brief set the rgb of leds[id]
 */
static void led_set_rgb(int id, uint8_t r, uint8_t g, uint8_t b) {
    if(id >= LED_NUM) return;
    leds[id].r = r;
    leds[id].g = g;
    leds[id].b = b;
    leds[id].v = get_value_from_rgb(r, g, b);
    sync_pixel(id);
}

/**
 * @brief set the color of leds[id]
 */
static void led_set_color(int id, enum led_color_list color) {
    if(id >= LED_NUM || color >= LED_COLOR_COUNT) return;
    led_set_rgb(id, colors[color][0],
        colors[color][1], colors[color][2]);
}

/**
 * @brief set the value of leds[id]
 */
static void led_set_value(int id, uint8_t value) {
    if(id >= LED_NUM) return;
    sync_pixel_value(id, value);
}

/**
 * @brief update the brightness of all the leds
 */
static void led_set_brightness(uint8_t _brightness) {
    if(_brightness >= MAX_BRIGHTNESS) return;
    brightness = _brightness;
    sync_pixels();
}

/**
 * @brief update leds
 */
static void led_update() {
    if(changed_length == -1) return;
    int rc = led_strip_update_rgb(strip, pixels, changed_length + 1);
    if (rc) {
        LOG_WRN("couldn't update strip: %d", rc);
    }
    changed_length = -1;
}

static bool event_handler(const struct event_header *eh) {
    if(is_keyboard_event(eh)) {
        switch(cast_keyboard_event(eh)->event) {
        case KEYBOARD_EVENT_READY:
            led_init();
            led_enable();
            led_set_rgb(0, 0, 255, 0);
            break;
        
        case KEYBOARD_EVENT_TICK:
            led_update();
            break;

        default:
            break;
        }
        return false;
    }
}

EVENT_LISTENER(MODULE, event_handler);
EVENT_SUBSCRIBE(MODULE, keyboard_event);
