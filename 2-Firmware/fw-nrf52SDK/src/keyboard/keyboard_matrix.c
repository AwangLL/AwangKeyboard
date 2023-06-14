#include "keyboard_matrix.h"
#include <stdint.h>
#include <string.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "keyboard_paraments.h"
#include "keyboard_evt.h"
#include "keyboard_light.h"
#include "keyboard_consumer.h"
#include "keyboard_fn.h"
#include "keyboard_report.h"

#include "config.h"
#include "kb_nrf_print.h"

#ifndef DEBOUNCE
#define DEBOUNCE 1
#endif

static uint8_t deboucing = DEBOUNCE;

static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static uint8_t buf[INPUT_REPORT_LEN_KEYBOARD];

void buf_print(void);
void matrix_print(void);
static void select_row(uint8_t row);
static void unselect_row(uint8_t row);
static matrix_row_t read_cols(uint8_t row);

void buf_append(uint8_t keycode)
{
    uint8_t index = keycode / 8 + 2;
    uint8_t pos = keycode % 8;
    buf[index] |= (1 << pos);
}
void buf_delete(uint8_t keycode)
{
    uint8_t index = keycode / 8 + 2;
    uint8_t pos = keycode % 8;
    buf[index] &= ~(1 << pos);
}
void buf_clear(void)
{
    memset(buf, 0, INPUT_REPORT_LEN_KEYBOARD);
}

void keyboard_matrix_init(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++)
    {
        nrf_gpio_cfg_output(row_pins[i]);
        nrf_gpio_pin_set(row_pins[i]);
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
    for (uint8_t i = 0; i < MATRIX_COLS; i++)
    {
        nrf_gpio_cfg_input(col_pins[i], NRF_GPIO_PIN_PULLUP);
    }
}

void matrix_scan(void)
{
    for (uint8_t r = 0; r < MATRIX_ROWS; r++)
    {
        select_row(r);
        nrf_delay_us(30);
        matrix_row_t cols = read_cols(r);

        matrix[r] = cols;
        unselect_row(r);
    }
    deboucing = DEBOUNCE;
    while (deboucing--)
    {
        nrf_delay_ms(1);
        for (uint8_t r = 0; r < MATRIX_ROWS; r++)
        {
            select_row(r);
            nrf_delay_us(30);
            matrix_row_t cols = read_cols(r);

            matrix_debouncing[r] = cols;
            unselect_row(r);
        }
    }
    for (uint8_t r = 0; r < MATRIX_ROWS; r++)
    {
        matrix[r] &= matrix_debouncing[r];
    }
}
void keyboard_matrix_task(void)
{
    static matrix_row_t matrix_prev[MATRIX_ROWS];
    static matrix_row_t matrix_change[MATRIX_ROWS];
    static uint8_t data[INPUT_REPORT_LEN_KEYBOARD];
    static uint8_t data_modifer = 0;
    static bool fn_down = false;
    static bool changed = false;
    static uint8_t keycode;
    static bool keyDown;

    matrix_scan();
    //检测按键状态是否改变
    for (uint8_t r = 0; r < MATRIX_ROWS; r++)
    {
        matrix_change[r] = matrix_prev[r] ^ matrix[r];
        matrix_prev[r] = matrix[r];
    }
    //检测fn是否按下
    if (matrix[fn_row] & (1 << fn_col))
        fn_down = true;
    else
        fn_down = false;

    changed = false;

    for (uint8_t r = 0; r < MATRIX_ROWS; r++)
    {
        for (uint8_t c = 0; c < MATRIX_COLS; c++)
        {
            //处理按键状态(通 断) 是否改变
            if (matrix_change[r] & (1 << c))
            {
                changed = true;
                if(matrix[r] & (1 << c))
                {
                    keyDown = true;
                    keyboard_light_keyPress(r,c);
                }
                else keyDown = false;

                //选择default keymap 或 fn keymap
                if (fn_down && (keymap[r][c] != KC_TRNS))
                    keycode = fn_keymap[r][c];
                else
                    keycode = keymap[r][c];

                /*** common key ***/
                if (KEYCODE_IS_COMMON(keycode))
                {
                    if (keyDown) //按键按下
                    {
                        buf_append(keycode);
                    }
                    else //按键抬起
                    {
                        buf_delete(keycode);
                    }
                }
                /*** modifier key ***/
                else if(KEYCODE_IS_MODIFER(keycode))
                {
                    if (keyDown) //按键按下
                    {
                        buf[0] |= (1 << (keycode - KC_LCTL));
                    }
                    else //按键抬起
                    {
                        buf[0] &= ~(1 << (keycode - KC_LCTL));
                    }
                }
                /*** consumer key ***/
                else if (KEYCODE_IS_CONSUMER(keycode))
                {
                    if (keyDown) //按下
                    {
                        keyboard_consumer_command(keycode);
                    }
                    else
                    {
                        keyboard_consumer_command(0);
                    }
                }
                /*** extra function ***/
                else if (KEYCODE_IS_EXFN(keycode))
                {
                    if (keyDown) //按下
                    {
                        keyboard_fn_command(keycode, 1);
                    }
                    else
                    {
                        keyboard_fn_command(keycode, 0);
                    }
                }
            }
        }
    }

    //如果有按键状态发生改变就发送
    if (changed)
    {
        //matrix_print();
        buf[1] = 0;
        keyboard_report_send(INPUT_REPORT_KEYBOARD_ID, buf);
    }
}

static void select_row(uint8_t row)
{
    nrf_gpio_pin_clear(row_pins[row]);
}
static void unselect_row(uint8_t row)
{
    nrf_gpio_pin_set(row_pins[row]);
}
static matrix_row_t read_cols(uint8_t row)
{
    matrix_row_t result = 0;
    for (uint8_t i = 0; i < MATRIX_COLS; i++)
    {
        if (nrf_gpio_pin_read(col_pins[i]) == 0)
        {
            result |= 1 << i;
        }
    }
    return result;
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void buf_print(void)
{
    kb_print("========");
    for(uint8_t i = 0; i < INPUT_REPORT_LEN_KEYBOARD; i++)
        {
            kb_print("%2x",buf[i]);
        }
}

void matrix_print(void)
{
    kb_print("===");
    for (uint8_t r = 0; r < MATRIX_ROWS; r++)
        for (uint8_t c = 0; c < MATRIX_COLS; c++)
        {
            if (matrix[r] & (1 << c))
            {
                kb_print("row %d , col %d", r, c);
            }
        }
}

/* Event Hanlder*/
static void keyboard_matrix_evt_hanlder(enum kbd_evt_list event, void *arg)
{
    switch (event)
    {
    case KBD_EVT_INIT:
        keyboard_matrix_init();
        kb_print("matrix module initialized.");
        break;
    case KBD_EVT_SLEEP:
        break;
    default:
        break;
    }
}
EVENT_HANDLER(keyboard_matrix_evt_hanlder);