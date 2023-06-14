#include "keyboard_encoder.h"
#include <stdint.h>
#include "ec11/ec11_api.h"
#include "keyboard_evt.h"
#include "keyboard_menu.h"
#include "keyboard_consumer.h"
#include "keyboard_oled.h"
//#include "keyboard_keymap.h"
#include "keyboard_paraments.h"

#include "nrf_delay.h"
#include "kb_nrf_print.h"

/* Event Hanlder*/
static void keyboard_encoder_evt_hanlder(enum kbd_evt_list event, void *arg)
{
    switch (event)
    {
    case KBD_EVT_INIT:
        QDEC_init();
        kb_print("encoder module initialized.");
        break;
    case KBD_EVT_ENCODER:
        if (keyboard_oled_state())
        {
            keyboard_menu_encoderAction((uint8_t)arg); //right
        }
        else
            switch ((uint8_t)arg)
            {
            case EC_KEY_PRESSED:
                keyboard_consumer_command(ec_keymap[0]);
                break;
            case EC_KEY_RELEASED:
                keyboard_consumer_command(0);
                break;
            case EC_ROTATE_LEFT:
                keyboard_consumer_command(ec_keymap[1]);
                nrf_delay_us(10);
                keyboard_consumer_command(0);
                break;
            case EC_ROTATE_RIGHT:
                keyboard_consumer_command(ec_keymap[2]);
                nrf_delay_us(10);
                keyboard_consumer_command(0);
                break;
            default:
                break;
            }
        break;
    case KBD_EVT_SLEEP:
        QDEC_uninit();
        break;
    default:
        break;
    }
}
EVENT_HANDLER(keyboard_encoder_evt_hanlder);