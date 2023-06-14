#include "keyboard_timer.h"
#include <stdint.h>
#include "nrfx_timer.h"
#include "nrf_gpio.h"

#include "keyboard_matrix.h"
#include "keyboard_evt.h"
#include "keyboard_light.h"

#include "kb_nrf_print.h"
#include "config.h"


const nrfx_timer_t keyboard_timer = NRFX_TIMER_INSTANCE(1);

void keyboard_timer_interrupt(nrf_timer_event_t event_type, void *p_context)
{
    static uint16_t cnt = 0;
    switch (event_type)
    {
    case NRF_TIMER_EVENT_COMPARE0:
        cnt++;
        if (cnt == 10) //矩阵扫描
        {
            cnt = 0;
            keyboard_matrix_task();

            keyboard_evt_trigger(KBD_EVT_LIGHT);
        }
        break;
    default:
        break;
    }
}

void keyboard_timer_init(void)
{
    uint32_t timer_tick;
    nrfx_timer_config_t p_config = {
        .bit_width = NRF_TIMER_BIT_WIDTH_16,
        .frequency = NRF_TIMER_FREQ_1MHz,
        .interrupt_priority = 7,
        .mode = NRF_TIMER_MODE_TIMER};
    nrfx_timer_init(&keyboard_timer, &p_config, keyboard_timer_interrupt);

    timer_tick = nrfx_timer_ms_to_ticks(&keyboard_timer, 1);
    nrfx_timer_extended_compare(&keyboard_timer,
                                NRF_TIMER_CC_CHANNEL0,
                                timer_tick,
                                NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                true);
}

/* Event Hanlder*/
static void keyboard_timer_evt_hanlder(enum kbd_evt_list event, void *arg)
{
    switch (event)
    {
    case KBD_EVT_INIT:
        keyboard_timer_init();
        kb_print("timer module initialized.");
        break;
    case KBD_EVT_START:
        nrfx_timer_enable(&keyboard_timer);
        kb_print("timer module started.");
        break;
    case KBD_EVT_SLEEP:
        nrfx_timer_disable(&keyboard_timer);
        nrfx_timer_uninit(&keyboard_timer);
        kb_print("timer module stop.");
        break;
    default:
        break;
    }
}
EVENT_HANDLER(keyboard_timer_evt_hanlder);