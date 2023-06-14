#include "ws2812_api.h"
#include <stdint.h>
#include "nrfx_pwm.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "kb_nrf_print.h"

#define fast

#ifdef slow
#define RESET_ZEROS_AT_START 41
#define PERIOD_TICKS 20   //20/16MHz = 1.25us
#define ONE_HIGH_TICKS 14 //14/16MHz = 0.875us
#define ZERO_HIGH_TICKS 6 //6/16MHz = 0.375us (should be 0.35us +-150ns)
#endif

#ifdef fast
#define RESET_ZEROS_AT_START 45
#define PERIOD_TICKS 18   // 18/16MHz = 1.125us
#define ONE_HIGH_TICKS 13 // 13/16MHz = 0.8125us
#define ZERO_HIGH_TICKS 5 // 5 /16MHz = 0.3125us
#endif

const nrfx_pwm_t m_ws2812_pwm = NRFX_PWM_INSTANCE(0);
static nrf_pwm_values_common_t m_seq_values[numberOws2812 * 24 + RESET_ZEROS_AT_START + 1];
const nrf_pwm_sequence_t m_seq =
    {
        .values.p_common = m_seq_values,
        .length = NRF_PWM_VALUES_LENGTH(m_seq_values),
        .end_delay = 0,
        .repeats = 0};

static led_t leds[numberOws2812];

static void hsv2rgb(uint8_t id)
{
    if(id > sizeof(leds)/sizeof(led_t)) return;
    if (leds[id].hue == MAX_HUE)
        leds[id].hue = 0;
    rgb_t retV;
    float hh, ss, vv;
    float c, x, y, z;
    float r, g, b;
    uint8_t i = 0;

    hh = leds[id].hue;
    ss = leds[id].saturation * 1.0 / MAX_SATURATION;
    vv = leds[id].value * 1.0 / MAX_VALUE;
    c = x = y = z = 0;

    if (ss == 0)
        r = g = b = vv;
    else
    {
        hh = hh / 60;
        i = (int)hh;
        c = hh - i;

        x = vv * (1 - ss);
        y = vv * (1 - ss * c);
        z = vv * (1 - ss * (1 - c));

        switch (i)
        {
        case 0:
            r = vv;
            g = z;
            b = x;
            break;
        case 1:
            r = y;
            g = vv;
            b = x;
            break;
        case 2:
            r = x;
            g = vv;
            b = z;
            break;
        case 3:
            r = x;
            g = y;
            b = vv;
            break;
        case 4:
            r = z;
            g = x;
            b = vv;
            break;
        case 5:
            r = vv;
            g = x;
            b = y;
            break;
        }
    }
    leds[id].red = (int)(r * 255);
    leds[id].green = (int)(g * 255);
    leds[id].blue = (int)(b * 255);
}

void ws2812_init()
{
    ret_code_t err_code;

    nrf_gpio_cfg_output(WS2812_PIN);
    nrf_gpio_pin_clear(WS2812_PIN);

    nrfx_pwm_config_t const p_config =
        {
            .output_pins =
                {
                    WS2812_PIN,            //channel 0
                    NRFX_PWM_PIN_NOT_USED, //channel 1
                    NRFX_PWM_PIN_NOT_USED, //channel 2
                    NRFX_PWM_PIN_NOT_USED  //channel 3
                },
            .irq_priority = APP_IRQ_PRIORITY_LOW,
            .base_clock = NRF_PWM_CLK_16MHz,
            .count_mode = NRF_PWM_MODE_UP,
            .top_value = PERIOD_TICKS,
            .load_mode = NRF_PWM_LOAD_COMMON,
            .step_mode = NRF_PWM_STEP_AUTO};
    err_code = nrfx_pwm_init(&m_ws2812_pwm, &p_config, NULL);
    kb_error(err_code);
    
    memset(leds,0,sizeof(leds));

    for (int i = 0; i < NRF_PWM_VALUES_LENGTH(m_seq_values); i++)
    {
        m_seq_values[i] = ONE_HIGH_TICKS;
    }
    for (int i = 0; i < RESET_ZEROS_AT_START; i++)
    {
        m_seq_values[i] = 0x8000;
    }
    m_seq_values[numberOws2812 * 24 + RESET_ZEROS_AT_START] = 0x8000;

    nrfx_pwm_simple_playback(&m_ws2812_pwm, &m_seq, 1, 0);
}

void ws2812_set(uint8_t id, uint16_t hue, uint16_t saturation, uint16_t value)
{
    if (hue > MAX_HUE || saturation > MAX_SATURATION || value > MAX_VALUE)
        return;

    leds[id].hue = hue;
    leds[id].saturation = saturation;
    leds[id].value = value;
    leds[id].change = 1;
}
void ws2812_set_v(uint8_t id, uint16_t value)
{
    leds[id].value = value;
    leds[id].change = 1;
}

void ws2812_show(void)
{
    uint8_t flag = 0;
    for (uint8_t i = 0; i < (sizeof(leds) / sizeof(led_t)); i++)
    {
        if(leds[i].change == 1)
        {
            flag++;
            leds[i].change = 0;
        }

        hsv2rgb(i);

        //green
        for (uint8_t j = 0; j < 8; j++)
        {
            if ((leds[i].green << j) & 0x80)
                m_seq_values[RESET_ZEROS_AT_START + i * 24 + j] = ONE_HIGH_TICKS | 0x8000;
            else
                m_seq_values[RESET_ZEROS_AT_START + i * 24 + j] = ZERO_HIGH_TICKS | 0x8000;
        }
        //red
        for (uint8_t j = 0; j < 8; j++)
        {
            if ((leds[i].red << j) & 0x80)
                m_seq_values[RESET_ZEROS_AT_START + i * 24 + j + 8] = ONE_HIGH_TICKS | 0x8000;
            else
                m_seq_values[RESET_ZEROS_AT_START + i * 24 + j + 8] = ZERO_HIGH_TICKS | 0x8000;
        }
        //blue
        for (uint8_t j = 0; j < 8; j++)
        {
            if ((leds[i].blue << j) & 0x80)
                m_seq_values[RESET_ZEROS_AT_START + i * 24 + j + 16] = ONE_HIGH_TICKS | 0x8000;
            else
                m_seq_values[RESET_ZEROS_AT_START + i * 24 + j + 16] = ZERO_HIGH_TICKS | 0x8000;
        }
    }
    if(flag)
    {
        //kb_print("light update");
        nrfx_pwm_simple_playback(&m_ws2812_pwm, &m_seq, 1, 0);
        nrf_delay_us(10);
    }
}
