#pragma once

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "keyboard_keycode.h"

#include "config.h"

/* 键盘参数 */

/** 矩阵配置 **/
static uint8_t keymap[MATRIX_ROWS][MATRIX_COLS] = {
    {KC_ESC,  KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL},
    {KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQUL, KC_BSPC, KC_FN0},
    {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PRSC},
    {KC_CAPL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO,   KC_HOME},
    {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_NO,   KC_UP,   KC_END},
    {KC_LCTL, KC_LGUI, KC_LALT, KC_NO,   KC_NO,   KC_SPC,  KC_NO,   KC_NO,   KC_NO,   KC_RALT, KC_FN,   KC_LCTL, KC_LEFT, KC_DOWN, KC_RIGT}
};
static uint8_t fn_keymap[MATRIX_ROWS][MATRIX_COLS] = {
    {KC_TRNS, KC_NO,   KC_FN10, KC_FN11, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_FN5,  KC_FN6,  KC_FN9,  KC_TRNS},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_FN7,  KC_FN8,  KC_CALC, KC_PGUP},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_PGUP},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_FN4 , KC_NO,   KC_VOLU, KC_SCRL},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_NO,   KC_TRNS, KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLD, KC_TRNS},
};

static uint8_t fn_row = 5;
static uint8_t fn_col = 10;
static uint8_t matrix_scan_interval = 10;          // 键盘矩阵扫描间隔

/** 键盘基本信息配置 **/
static char *keyboard_name = "Awang Keyboard";    // 设备名
static char *keyboard_manufacture = "Nordic";     // 厂商
static uint16_t keyboard_version = 0x0101;        // 设备版本 1.1

static uint16_t ec_keymap[3] = {KC_CS_VOLUMN_MUTE, KC_CS_VOLUMN_DECREASEMENT, KC_CS_VOLUMN_INCREASEMENT};