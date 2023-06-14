#include "keyboard_battery.h"
#include <stdbool.h>
#include "app_timer.h"
#include "nrfx_saadc.h"

#include "keyboard_evt.h"

#include "kb_nrf_print.h"
#include "nrf_delay.h"

#define ADC_BUFFER_SIZE 6
#define BAT_ADC_CHANNEL 0
#define ADC_SAMPLE_INTERVAL 60 //1 minute

APP_TIMER_DEF(bat_adc_timer);

static const nrfx_saadc_config_t saadc_config =
    {
        .resolution = 2,     //12bit
        .oversample = 0,     //disable
        .low_power_mode = 0, //disable
        .interrupt_priority = NRFX_SAADC_CONFIG_IRQ_PRIORITY};
static const nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);

BattInfo battery_info;
static bool adc_initialized;
static nrf_saadc_value_t buf11[ADC_BUFFER_SIZE];
static nrf_saadc_value_t adc_buf[6];

void bat_adc_uninit(void)
{
    adc_initialized = false;
    nrfx_saadc_uninit();
}

static void adc_result_handler(nrf_saadc_value_t value)
{
    //saadc measure value = voltage * gain / reference * 2^(Resolution - m)
    //
    //gain = 1/2
    //reference = 0.6
    //Resolution = 12
    //m = 0
    battery_info.voltage = value * 1.2 / 4096 * 1000 / 0.18;

    if (battery_info.voltage >= 4100)
        battery_info.percentage = 100;
    else if (battery_info.voltage >= 3335)
        battery_info.percentage = 15 + (battery_info.voltage - 3335) / 9;
    else if (battery_info.voltage >= 2900)
        battery_info.percentage = (battery_info.voltage - 2900) / 29;
    else
        battery_info.percentage = 0;
    kb_print("v = %d,per = %d", battery_info.voltage, battery_info.percentage);
}

static void saadc_event_handler(nrfx_saadc_evt_t const *p_evt)
{
    nrf_saadc_value_t result = 0;
    ret_code_t err_code;

    if (p_evt->type == NRFX_SAADC_EVT_DONE)
    {
        result = 0;
        err_code = nrfx_saadc_buffer_convert(p_evt->data.done.p_buffer, ADC_BUFFER_SIZE);
        kb_error(err_code);

        for (uint8_t i = 0; i < ADC_BUFFER_SIZE; i++)
        {
            result += p_evt->data.done.p_buffer[i];
        }
        result /= ADC_BUFFER_SIZE;
        adc_result_handler(result);

        keyboard_evt_trigger(KBD_EVT_BATTUPDATE);

        keyboard_battery_start(ADC_SAMPLE_INTERVAL);
    }
}

void bat_adc_init(void)
{
    ret_code_t err_code;

    adc_initialized = true;

    err_code = nrfx_saadc_init(&saadc_config, saadc_event_handler);
    kb_error(err_code);

    err_code = nrfx_saadc_channel_init(BAT_ADC_CHANNEL, &channel_config);
    kb_error(err_code);

    err_code = nrfx_saadc_buffer_convert(adc_buf, ADC_BUFFER_SIZE);
    kb_error(err_code);
}

static void bat_adc_sample(void *p_context) //sample once
{
    ret_code_t err_code;
    nrf_saadc_value_t saadc_value;

    if (!adc_initialized)
        bat_adc_init();

    for (int i = 0; i < ADC_BUFFER_SIZE; i++)
    {
        err_code = nrfx_saadc_sample();
        kb_error(err_code);
        nrf_delay_us(15);
    }
    bat_adc_uninit();
}

void keyboard_battery_start(uint8_t time)
{
    app_timer_start(bat_adc_timer,
                    APP_TIMER_TICKS(time * 1000),
                    NULL);
}

void keyboard_battery_init(void)
{
    ret_code_t err_code;

    bat_adc_init();

    err_code = app_timer_create(&bat_adc_timer,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                //APP_TIMER_MODE_REPEATED,
                                bat_adc_sample);
    kb_error(err_code);
}

/* Event Hanlder*/
static void keyboard_battery_evt_hanlder(enum kbd_evt_list event, void *arg)
{
    switch (event)
    {
    case KBD_EVT_INIT:
        keyboard_battery_init();
        kb_print("keyboard battery detection module initialized.");
        break;
    case KBD_EVT_START:
        keyboard_battery_start(5);
        kb_print("keyboard battery detection module started.");
        break;
    case KBD_EVT_SLEEP:
        app_timer_stop(bat_adc_timer);
        bat_adc_uninit();
        break;
    case KBD_EVT_USB:
        if((uint8_t)arg == USB_STATE_POWER_CONNECT)
        {
            app_timer_stop(bat_adc_timer);
            bat_adc_uninit();
        }
        else if((uint8_t)arg == USB_STATE_NOT_CONNECT)
        {
            keyboard_battery_start(5);
        }
        break;
    default:
        break;
    }
}
EVENT_HANDLER(keyboard_battery_evt_hanlder);