#include "keyboard_light.h"
#include "nrf_gpio.h"
#include "ws2812/ws2812_api.h"

#include "usb_custom_service.h"
#include "keyboard_keycode.h"
#include "keyboard_evt.h"

#include "kb_nrf_print.h"

#define LIGHT_TIME  250 // ms

static uint16_t value = 100;
static bool state = false;
static enum light_mode_t mode = light_mode_click_rainbow;
static uint8_t color = 1;
static uint16_t bri;
static uint16_t bri_[numberOws2812];
static bool dontUpdate;
static uint16_t ind_blink_interval = 0; // ms
static bool ind_light_or_dark = true;
static uint16_t ind_cnt = 0;

#define ind_value (value * 0.5)

static uint8_t randomNumber(void)
{
    return rand();
}   

void keyboard_light_update(void)
{
    if(ind_blink_interval != 0)
    {
        ind_cnt+=10;
        if(ind_cnt >= ind_blink_interval)
        {
            ind_cnt = 0;
            if(ind_light_or_dark)
            {
                ws2812_set_v(0, 0);
                ind_light_or_dark = false;
            }
            else
            {
                ws2812_set_v(0, ind_value);
                ind_light_or_dark = true;
            }
        }
    }

    if(!state)
    {
        ws2812_show();
        return;
    }
    switch (mode)
    {
    case light_mode_back:
        for(uint8_t i = 1; i < numberOws2812; i++)
        {
            ws2812_set(i, color_table[color][0], color_table[color][1], value);
        }
        ws2812_show();
        break;
    case light_mode_breath:
        for(uint8_t i = 1; i < numberOws2812; i++)
        {
            ws2812_set(i, color_table[color][0], color_table[color][1], bri_table[bri]*(value * 1.0 / ALLOW_VALUE));
        }
        ws2812_show();
        bri++;
        if(bri == 300) bri = 0;
        break;
    case light_mode_click:
    {
        for(uint8_t i = 1; i < numberOws2812; i++)
        {
            if(bri_[i] == LIGHT_TIME)
            {
                ws2812_set(i,color_table[color][0], color_table[color][1],255*(value * 1.0 / ALLOW_VALUE));
                bri_[i]--;
            }
            else if(bri_[i] >= 150 && bri_[i] < LIGHT_TIME)
            {
                bri_[i]--;
            }
            else if(bri_[i] > 0 && bri_[i] < 150)
            {
                ws2812_set_v(i,bri_table[bri_[i]--]*(value * 1.0 / ALLOW_VALUE));
            }
            else if(bri_[i] == 0)
            {
                ws2812_set_v(i,0);
            }
        }
        ws2812_show();
        break;
    }
    case light_mode_click_rainbow:
        for(uint8_t i = 1; i < numberOws2812; i++)
        {
            if(bri_[i] == LIGHT_TIME)
            {
                uint8_t randColor = randomNumber() % color_count;
                ws2812_set(i,color_table[randColor][0], color_table[randColor][1],255*(value * 1.0 / ALLOW_VALUE));
                bri_[i]--;
            }
            else if(bri_[i] >= 150 && bri_[i] < LIGHT_TIME)
            {
                bri_[i]--;
            }
            else if(bri_[i] > 0 && bri_[i] < 150)
            {
                ws2812_set_v(i,bri_table[bri_[i]--]*(value * 1.0 / ALLOW_VALUE));
            }
        }
        ws2812_show();
        break;
    }
}

void keyboard_light_switch_off(void)
{
    nrf_gpio_pin_set(pin_vcc);
    state = false;
    kb_print("light off");
}
void keyboard_light_switch_on(void)
{
    nrf_gpio_pin_clear(pin_vcc);
    state = true;
    kb_print("light on");
}

void keyboard_light_init(void)
{
    ret_code_t err_code;

    ws2812_init();

    nrf_gpio_cfg_output(pin_vcc);

    for(uint8_t i = 0; i < numberOws2812; i++)
    {
        bri_[i] = 0;
    }
}

void keyboard_light_keyPress(uint8_t row, uint8_t col)
{
    if(ledmap[row][col] != NO)
        bri_[ledmap[row][col]] = LIGHT_TIME;
}

/* Event Hanlder*/
static void keyboard_light_evt_hanlder(enum kbd_evt_list event, void *arg)
{
    switch (event)
    {
    case KBD_EVT_INIT:
        keyboard_light_init();
        kb_print("keyboard light module initialized.");
        break;
    case KBD_EVT_START:
        //if(!state) keyboard_light_switch_on();
        kb_print("keyboard light module started.");
        break;
    case KBD_EVT_SLEEP:
        if(state) keyboard_light_switch_off();
        kb_print("keyboard light module started.");
        break;
    case KBD_EVT_BLE:
        if(usb_state() == USB_STATE_WORKING) return;
        switch ((uint8_t)arg)
        {
        case BLE_STATE_CONNECTED:
            ind_blink_interval = 0;
            ws2812_set(0, color_table[light_ind_bleConnect][0], color_table[light_ind_bleConnect][1], ind_value);
            break;
        case BLE_STATE_DISCONNECT:
            ind_blink_interval = 0;
            ws2812_set(0, color_table[light_ind_bleDisconn][0], color_table[light_ind_bleDisconn][1], ind_value);
            break;
        case BLE_STATE_STOP:
            ind_blink_interval = 0;
            ws2812_set(0, color_table[light_ind_deviceStop][0], color_table[light_ind_deviceStop][1], ind_value);
            break;
        case BLE_STATE_FAST_ADV:
            ind_blink_interval = 500;
            ind_light_or_dark = true;
            ind_cnt = 0;
            ws2812_set(0, color_table[light_ind_bleFastAdv][0], color_table[light_ind_bleFastAdv][1], ind_value);
            break;
        case BLE_STATE_SLOW_ADV:
            ind_blink_interval = 1000;
            ind_light_or_dark = true;
            ind_cnt = 0;
            ws2812_set(0, color_table[light_ind_bleSlowAdv][0], color_table[light_ind_bleSlowAdv][1], ind_value);
            break;
        }
        keyboard_light_update();
        break;
    case KBD_EVT_USB:
        switch ((uint8_t)arg)
        {
        case USB_STATE_WORKING:
            ind_blink_interval = 0;
            ws2812_set(0, color_table[light_ind_usbConnect][0], color_table[light_ind_usbConnect][1], ind_value);
            break;
        // case USB_STATE_NOT_CONNECT:
        //     ind_blink_interval = 0;
        //     ws2812_set(0, color[light_ind_deviceStop][0], color[light_ind_deviceStop][1], 200);
        //     break;
        default:
            break;
        }
        break;
    case KBD_EVT_LIGHT:
        dontUpdate = false;
        switch ((uint8_t)arg)
        {
        case LIGHT_MODE_PREV:
            if (mode == 0) mode = light_mode_count - 1;
            else mode--;
            break;
        case LIGHT_MODE_NEXT:
            if (mode == light_mode_count - 1) mode = 0;
            else mode++;
            break;
        case LIGHT_BRI_DEC:
            if (value == 10) value = 10;
            else value -= 10;
            break;
        case LIGHT_BRI_INC:
            if (value == 200) value = 200;
            else value += 10;
            break;
        case LIGHT_COLOR_PREV:
            if (color == 0) color = color_count - 1;
            else color--;
            break;
        case LIGHT_COLOR_NEXT:
            if (color == color_count - 1) color = 0;
            else color++;
            break;
        case LIGHT_SWITCH:
            if(!state) keyboard_light_switch_on();
            else keyboard_light_switch_off();
            break;
        default:
            break;
        }
        keyboard_light_update();
        break;
    default:
        break;
    }
}
EVENT_HANDLER(keyboard_light_evt_hanlder);