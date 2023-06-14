#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#define NO 255
// 0号用作指示灯 不作为底灯
static const uint8_t ledmap[MATRIX_ROWS][MATRIX_COLS] = {
    {14, NO, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, NO},
    {42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, NO},
    {57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43},
    {71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, NO, 59, 58},
    {81, 80, 79, NO, NO, 78, NO, NO, NO, 77, 76, 75, 74, 73, 72}
};

/* 呼吸灯曲线表 https://blog.csdn.net/xiaoyuanwuhui/article/details/101028544 */
// 0-149 150-299
static const uint8_t bri_table[] ={
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8,
    8, 9, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 17, 17, 18, 19, 19, 20, 21, 22, 23, 24, 24, 25, 27,
    27, 29, 30, 31, 32, 33, 35, 36, 37, 39, 40, 42, 44, 45, 47, 49, 51, 53, 55, 57, 59, 61, 64, 66, 69, 71, 74, 77, 80, 83,
    86, 89, 93, 96, 100, 104, 108, 112, 116, 121, 125, 130, 135, 140, 145, 151, 157, 163, 169, 175, 182, 189, 196, 204, 212, 220, 228, 237, 246, 255,
    255, 246, 237, 228, 220, 212, 204, 196, 189, 182, 175, 169, 163, 157, 151, 145, 140, 135, 130, 125, 121, 116, 112, 108, 104, 100, 96, 93, 79, 86,
    83, 80, 77, 74, 71, 69, 66, 64, 61, 59, 57, 55, 53, 51, 49, 47, 45, 44, 42, 40, 39, 37, 36, 35, 33, 32, 31, 30, 29, 27,
    26, 25, 24, 24, 23, 22, 21, 20, 19, 19, 18, 17, 17, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 9, 8,
    8, 8, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#define light_ind_bleConnect 14
#define light_ind_usbConnect 8
#define light_ind_deviceStop 1
#define light_ind_bleDisconn 3
#define light_ind_bleFastAdv 12
#define light_ind_bleSlowAdv 12

enum light_mode_t
{
    light_mode_back,          // 背光
    light_mode_breath,        // 呼吸灯
    light_mode_click,         // 点击
    light_mode_click_rainbow, // 点击 - 炫彩
    light_mode_count
};

static const uint16_t color_table[][2] = {  // hue & saturation
    {0, 0},     // 0  white
    {0, 100},   // 1  red
    {16, 50},   // 2  coral
    {39, 100},  // 3  orange
    {43, 85},   // 4  goldenrod
    {51, 100},  // 5  gold
    {60, 100},  // 6  yellow
    {90, 100},  // 7  chartreuse
    {120, 100}, // 8  green
    {150, 100}, // 9  springgreen
    {174, 35},  // 10 turquoise
    {180, 100}, // 11 teal
    {180, 100}, // 12 cyan
    {186, 40},  // 13 azure
    {240, 100}, // 14 blue
    {270, 100}, // 15 purple
    {300, 100}, // 16 magenta
    {330, 50},  // 17 pink
};
static const uint8_t color_count = sizeof(color_table) / sizeof(uint16_t) / 2;

void keyboard_light_init();
void keyboard_light_switch_on();
void keyboard_light_update(void);
void keyboard_light_keyPress(uint8_t row, uint8_t col);
