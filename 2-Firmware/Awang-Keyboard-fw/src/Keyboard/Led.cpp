#include "Led.hpp"

#include <logging/log.h>
#include "Power.hpp"

LOG_MODULE_REGISTER(Led);

void LRGB::Set(uint8_t r, uint8_t g, uint8_t b) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->v = Led::GetValueFromRGB(r, g, b);
}

void LRGB::Sync(int x, int y) {
    if(y == 0) {
        *this->pixel = {
            .r = 0,
            .g = 0,
            .b = 0
        };
    } else {
        *this->pixel = {
            .r = (uint8_t)(r * x / y),
            .g = (uint8_t)(g * x / y),
            .b = (uint8_t)(b * x / y)
        };   
    }
}

void LRGB::Sync(uint8_t r, uint8_t g, uint8_t b) {
    *this->pixel = {
        .r = r,
        .g = g,
        .b = b
    };
}

void Led::OnTimeCallback(void *) {
    if(!led.enabled || !led.changed) return;
    led.Update();
    led.changed = false;
}

uint8_t Led::GetValueFromRGB(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t v = r > g ? r : g;
    return v > b ? v : b;
}

// call when the brightness changed
// Sync all the elements in pixels[]
void Led::SyncPixel() {
    for(auto led : leds) {
        uint8_t ledValue = GetValueFromRGB(led.r, led.g, led.b);
        led.Sync((int)led.v * brightness, (int)ledValue * MAX_BRIGHTNESS);
    }
    changed = true;
}

// call when the rgb of the leds[id] changed
void Led::SyncPixel(int id) {
    auto &led = leds[id];
    led.Sync((int)MAX_VALUE * brightness, (int)leds[id].v * MAX_BRIGHTNESS);
    changed = true;
}

// call when the value of the leds[id] changed
void Led::SyncPixel(int id, uint8_t value) {
    auto &led = leds[id];
    led.v = value;
    led.Sync((int)led.v * brightness, (int)GetValueFromRGB(led.r, led.g, led.b) * MAX_BRIGHTNESS);
    changed = true;
}

void Led::HSV2RGB(uint16_t hh, uint8_t ss, uint8_t vv, uint8_t &rr, uint8_t &gg, uint8_t &bb) {
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
    rr = r;
    gg = g;
    bb = b;
}

void Led::RGB2HSV(uint8_t rr, uint8_t gg, uint8_t bb, uint16_t &hh, uint8_t &ss, uint8_t &vv) {
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

    hh = h;
    ss = s * MAX_SATURATION / CALC_PRECSION;
    vv = v * MAX_VALUE / CALC_PRECSION;
}

void Led::Init() {
    strip = (device *)DEVICE_DT_GET(DT_ALIAS(led_strip));
    if(!device_is_ready(strip)) {
        LOG_ERR("Led strip is not ready");
        return;
    }
    for(uint8_t i = 0; i < LED_NUM; i++) {
        leds[i].pixel = &pixels[i];
    }
    initialized = true;
}

void Led::Enable() {
    if(!initialized) Init();
    if(enabled) return;
    timer.Start(10);
    enabled = true;
}

void Led::Disable() {
    if(!enabled) return;
    timer.Stop();
}

void Led::Set(uint8_t id, uint8_t r, uint8_t g, uint8_t b) {
    if(id > LED_NUM) return;
    leds[id].Set(r, g, b);
    SyncPixel(id);
}

void Led::Set(uint8_t id, Color color) {
    if(color >= COLOR_NUMBER) return;
    Set(id, COLORS[color][0], COLORS[color][1], COLORS[color][2]);
}

void Led::Set(uint8_t id, uint8_t value) {
    if(!led.enabled || !led.changed) return;
    if(id >= LED_NUM) return;
    SyncPixel(id, value);
    
}

void Led::ChangeBrightness(uint8_t brightness) {
    if(brightness >= MAX_BRIGHTNESS) this->brightness = MAX_BRIGHTNESS;
    this->brightness = brightness;
    SyncPixel();
}

void Led::Update() {
    int rc = led_strip_update_rgb(strip, pixels, LED_NUM);
    if (rc) {
        LOG_WRN("couldn't update strip: %d", rc);
    }
}

const uint8_t Led::COLORS[][3] = {
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