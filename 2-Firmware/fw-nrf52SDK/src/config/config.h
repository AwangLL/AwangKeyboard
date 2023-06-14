#pragma once

#include <stdint.h>

/***  gpio config  ***/
#define GPIO_PIN(port, pin) (((port) << 5) | ((pin)&0x1F))
enum
{
    pin_0 = GPIO_PIN(1, 6),
    pin_1 = GPIO_PIN(1, 4),
    pin_2 = GPIO_PIN(1, 2),
    pin_3 = GPIO_PIN(1, 0),
    pin_4 = GPIO_PIN(0, 24),
    pin_5 = GPIO_PIN(0, 22),
    pin_6 = GPIO_PIN(0, 13),
    pin_7 = GPIO_PIN(0, 20),
    pin_8 = GPIO_PIN(0, 17),
    pin_9 = GPIO_PIN(0, 15),
    pin_10 = GPIO_PIN(0, 7),
    pin_11 = GPIO_PIN(0, 12),
    pin_12 = GPIO_PIN(0, 4),
    pin_13 = GPIO_PIN(1, 9),
    pin_14 = GPIO_PIN(0, 8),
    pin_15 = GPIO_PIN(0, 5),
    pin_16 = GPIO_PIN(0, 6),
    pin_17 = GPIO_PIN(0, 1),
    pin_18 = GPIO_PIN(0, 26),
    pin_19 = GPIO_PIN(0, 0),
    pin_20 = GPIO_PIN(0, 30),
    pin_21 = GPIO_PIN(0, 31),
    pin_22 = GPIO_PIN(0, 29),
    pin_23 = GPIO_PIN(0, 2),
    pin_24 = GPIO_PIN(1, 5),
    pin_25 = GPIO_PIN(0, 28),
    pin_adc = GPIO_PIN(0, 3),
    pin_vcc = GPIO_PIN(0, 25),
    pin_n1 = GPIO_PIN(0, 9),
    pin_n2 = GPIO_PIN(0, 10),
};
#define pin_col0 pin_0
#define pin_col1 pin_1
#define pin_col2 pin_2
#define pin_col3 pin_3
#define pin_col4 pin_4
#define pin_col5 pin_5
#define pin_col6 pin_6
#define pin_col7 pin_7
#define pin_col8 pin_8
#define pin_col9 pin_9
#define pin_col10 pin_10
#define pin_col11 pin_11
#define pin_col12 pin_12
#define pin_col13 pin_13
#define pin_col14 pin_14
#define pin_row0 pin_15
#define pin_row1 pin_16
#define pin_row2 pin_17
#define pin_row3 pin_18
#define pin_row4 pin_19
#define pin_row5 pin_20
#define pin_eca pin_21
#define pin_ecb pin_22
#define pin_scl pin_23
#define pin_sda pin_24
#define pin_rgb pin_25

/*** device config  ***/
#define DEVICE "Awang_keyboard"
#define MANUFACTURER "Nordic"

#define PNP_ID_VENDOR_ID_SOURCE 0x02  /**< Vendor ID Source. */
#define PNP_ID_VENDOR_ID 0x1915       /**< Vendor ID. */
#define PNP_ID_PRODUCT_ID 0xEEEE      /**< Product ID. */
#define PNP_ID_PRODUCT_VERSION 0x0100 /**< Product Version. */

/*** keyboard config ***/
#define MATRIX_ROWS 6  //按键矩阵行数
#define MATRIX_COLS 15 //按键矩阵列数
static const uint8_t row_pins[MATRIX_ROWS] = {pin_row0, pin_row1, pin_row2, pin_row3, pin_row4, pin_row5};
static const uint8_t col_pins[MATRIX_COLS] = {pin_col0, pin_col1, pin_col2, pin_col3, pin_col4, pin_col5, pin_col6, pin_col7,
                                              pin_col8, pin_col9, pin_col10, pin_col11, pin_col12, pin_col13, pin_col14};
#define DEBOUNCE 1

/*** driver ENABLE ***/
#define LOG_PRINT_ENABLE

#define WS2812_ENABLE //启用ws2812底灯
#define OLED_ENABLE   //启用OLED屏幕
#define EC11_ENABLE
#define BAT_DETECT_ENABLE

/*** driver config ***/
#ifdef LOG_PRINT_ENABLE
#define NRF_LOG_BACKEND_RTT_ENABLED 1
//#define NRF_LOG_BACKEND_UART_ENABLED 1
#define NRF_LOG_ENABLED 1
#endif

#ifdef WS2812_ENABLE
#define WS2812_PIN pin_rgb //ws2812灯Din引脚1`
#define numberOws2812 82   //ws2812灯的个数
#define NRFX_PWM_ENABLED 1
#define PWM_ENABLED 1
#define PWM0_ENABLED 1
//#define NRFX_SPIM_ENABLED   1
#endif

#ifdef OLED_ENABLE
#define OLED_SCL_PIN pin_scl               //OLED SCL引脚
#define OLED_SDA_PIN pin_sda               //OLED SDA引脚
#define OLED_SIZE_LENGTH 128               //OLED 长(像素数)
#define OLED_SIZE_WIDTH 64                 //OLED 宽(像素数)
#define OLED_SIZE_PAGE OLED_SIZE_WIDTH / 8 //OLED页数
#define NRFX_TWIM_ENABLED 1
#define NRFX_TWI_ENABLED 1
#define TWI_ENABLED 1
#define TWI0_ENABLED 1
#endif

#ifdef EC11_ENABLE
#define EC11_A_PIN pin_eca
#define EC11_B_PIN pin_ecb
#define QDEC_ENABLED 1
#endif

#ifdef BAT_DETECT_ENABLE
#define ADC_PIN pin_adc
#define CHARG_PIN P_CHRG
#define NRFX_SAADC_ENABLED 1
#define SAADC_ENABLED 1
#endif