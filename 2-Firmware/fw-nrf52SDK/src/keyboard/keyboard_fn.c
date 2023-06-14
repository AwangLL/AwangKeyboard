#include "keyboard_fn.h"
#include <stdint.h>
#include "ble_service.h"
#include "keyboard_keycode.h"
#include "keyboard_evt.h"
#include "keyboard_consumer.h"
#include "ble_service.h"

#include "kb_nrf_print.h"

void keyboard_fn_command(uint8_t keycode, uint8_t state)
{
    switch (keycode)
    {
    case KC_FN0:
        if (state)
            keyboard_evt_trigger_param(KBD_EVT_ENCODER, EC_KEY_PRESSED);
        else
            keyboard_evt_trigger_param(KBD_EVT_ENCODER, EC_KEY_RELEASED);
        break;
    case KC_FN1:
        keyboard_evt_trigger_param(KBD_EVT_ENCODER, EC_ROTATE_LEFT);
        break;
    case KC_FN2:
        keyboard_evt_trigger_param(KBD_EVT_ENCODER, EC_ROTATE_RIGHT);
        break;
    case KC_FN3:
        keyboard_evt_trigger_param(KBD_EVT_LIGHT, LIGHT_MODE_PREV);
        break;
    case KC_FN4:
        keyboard_evt_trigger_param(KBD_EVT_LIGHT, LIGHT_MODE_NEXT);
        break;
    case KC_FN5:
        keyboard_evt_trigger_param(KBD_EVT_LIGHT, LIGHT_BRI_DEC);
        break;
    case KC_FN6:
        keyboard_evt_trigger_param(KBD_EVT_LIGHT, LIGHT_BRI_INC);
        break;
    case KC_FN7:
        keyboard_evt_trigger_param(KBD_EVT_LIGHT, LIGHT_COLOR_PREV);
        break;
    case KC_FN8:
        keyboard_evt_trigger_param(KBD_EVT_LIGHT, LIGHT_COLOR_NEXT);
        break;
    case KC_FN9:
        kb_print("switch");
        if(state == 1) keyboard_evt_trigger_param(KBD_EVT_LIGHT, LIGHT_SWITCH);
        break;
    case KC_FN10:
        ble_disconnect();
        break;
    case KC_FN11:
        advertising_start(false);
        break;
    case KC_FN12:
        break;
    case KC_FN13:
        break;
    case KC_FN14:
        break;
    default:
        break;
    }
}