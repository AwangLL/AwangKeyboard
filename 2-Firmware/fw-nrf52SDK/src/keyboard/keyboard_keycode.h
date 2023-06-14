#pragma once

#include <stdint.h>
#define KC_TRNS 0x01
enum hid_keyboard_page
{
    KC_NO = 0x00,    // 00 Reserved (no event indicated)
    KC_ROLL_OVER,    // 01 Keyboard ErrorRollOver
    KC_POST_FAIL,    // 02 Keyboard POSTFail
    KC_UNDEFINED,    // 03 Keyboard ErrorUndefined
    KC_A,            // 04 Keyboard a and A
    KC_B,            // 05 Keyboard b and B
    KC_C,            // 06 Keyboard c and C
    KC_D,            // 07 Keyboard d and D
    KC_E,            // 08 Keyboard e and E
    KC_F,            // 09 Keyboard f and F
    KC_G,            // 0A Keyboard g and G
    KC_H,            // 0B Keyboard h and H
    KC_I,            // 0C Keyboard i and I
    KC_J,            // 0D Keyboard j and J
    KC_K,            // 0E Keyboard k and K
    KC_L,            // 0F Keyboard l and L
    KC_M,            // 10 Keyboard m and M
    KC_N,            // 11 Keyboard n and N
    KC_O,            // 12 Keyboard o and O
    KC_P,            // 13 Keyboard p and P
    KC_Q,            // 14 Keyboard q and Q
    KC_R,            // 15 Keyboard r and R
    KC_S,            // 16 Keyboard s and S
    KC_T,            // 17 Keyboard t and T
    KC_U,            // 18 Keyboard u and U
    KC_V,            // 19 Keyboard v and V
    KC_W,            // 1A Keyboard w and W
    KC_X,            // 1B Keyboard x and X
    KC_Y,            // 1C Keyboard y and Y
    KC_Z,            // 1D Keyboard z and Z
    KC_1,            // 1E Keyboard 1 and !
    KC_2,            // 1F Keyboard 2 and @
    KC_3,            // 20 Keyboard 3 and #
    KC_4,            // 21 Keyboard 4 and $
    KC_5,            // 22 Keyboard 5 and %
    KC_6,            // 23 Keyboard 6 and ^
    KC_7,            // 24 Keyboard 7 and &
    KC_8,            // 25 Keyboard 8 and *
    KC_9,            // 26 Keyboard 9 and (
    KC_0,            // 27 Keyboard 0 and )
    KC_ENT,          // 28 Keyboard Return (ENTER)
    KC_ESC,          // 29 Keyboard ESCAPE
    KC_BSPC,         // 2A Keyboard DELETE (Backspace)
    KC_TAB,          // 2B Keyboard Tab
    KC_SPC,          // 2C Keyboard Spacebar
    KC_MINS,         // 2D Keyboard - and (underscore)
    KC_EQUL,         // 2E Keyboard = and +
    KC_LBRC,         // 2F Keyboard [ and {
    KC_RBRC,         // 30 Keyboard ] and }
    KC_BSLS,         // 31 Keyboard \ and |
    KC_NONUS_HASH,   // 32 Keyboard Non-US # and ~
    KC_SCLN,         // 33 Keyboard ; and :
    KC_QUOT,         // 34 Keyboard ‘ and “
    KC_GRV,          // 35 Keyboard Grave Accent and Tilde
    KC_COMM,         // 36 Keyboard , and <
    KC_DOT,          // 37 Keyboard . and >
    KC_SLSH,         // 38 Keyboard / and ?
    KC_CAPL,         // 39 Keyboard Caps Lock
    KC_F1,           // 3A Keyboard F1
    KC_F2,           // 3B Keyboard F1
    KC_F3,           // 3C Keyboard F1
    KC_F4,           // 3D Keyboard F1
    KC_F5,           // 3E Keyboard F1
    KC_F6,           // 3F Keyboard F1
    KC_F7,           // 40 Keyboard F1
    KC_F8,           // 41 Keyboard F1
    KC_F9,           // 42 Keyboard F1
    KC_F10,          // 43 Keyboard F1
    KC_F11,          // 44 Keyboard F1
    KC_F12,          // 45 Keyboard F1
    KC_PRSC,         // 46 Keyboard PrintScreen
    KC_SCRL,         // 47 Keyboard Scroll Lock
    KC_PAUS,         // 48 Keyboard Pause
    KC_INS,          // 49 Keyboard Insert
    KC_HOME,         // 4A Keyboard Home
    KC_PGUP,         // 4B Keyboard PageUp
    KC_DEL,          // 4C Keyboard Delete Forward
    KC_END,          // 4D Keyboard End
    KC_PGDN,         // 4E Keyboard PageDown
    KC_RIGT,         // 4F Keyboard RightArrow
    KC_LEFT,         // 50 Keyboard LeftArrow
    KC_DOWN,         // 51 Keyboard DownArrow
    KC_UP,           // 52 Keyboard UpArrow
    KC_KP_NUML,      // 53 Keypad Num Lock and Clear
    KC_KP_SLSH,      // 54 Keypad /
    KC_KP_ASTR,      // 55 Keypad *
    KC_KP_MINS,      // 56 Keypad -
    KC_KP_PLUS,      // 57 Keypad +
    KC_KP_ENT,       // 58 Keypad ENTER
    KC_KP_1,         // 59 Keypad 1 and End
    KC_KP_2,         // 5A Keypad 2 and Down Arrow
    KC_KP_3,         // 5B Keypad 3 and PageDn
    KC_KP_4,         // 5C Keypad 4 and Left Arrow
    KC_KP_5,         // 5D Keypad 5
    KC_KP_6,         // 5E Keypad 6 and Right Arrow
    KC_KP_7,         // 5F Keypad 7 and Home
    KC_KP_8,         // 60 Keypad 8 and Up Arrow
    KC_KP_9,         // 61 Keypad 9 and PageUp
    KC_KP_0,         // 62 Keypad 0 and Insert
    KC_KP_DOT,       // 63 Keypad . and Delete
    KC_NONUS_BSLASH, // 64 Keyboard Non-US \ and |
    KC_APP,          // 65 Keyboard Application

    /* Modifiers */
    KC_LCTL = 0xE0, // E0 keyboard left ctrl
    KC_LSFT,        // E1 keyboard left shift
    KC_LALT,        // E2 keyboard left alt
    KC_LGUI,        // E3 keyboard left gui
    KC_RCTL,        // E4 keyboard right ctrl
    KC_RSFT,        // E5 keyboard right shift
    KC_RALT,        // E6 keyboard right alt
    KC_RGUI,        // E7 keyboard right gui
};

enum keyboard_consumer_keycode
{                              // 0xE8 - FF
    KC_VOLUMN_MUTE = 0xE8,     //E8 E2
    KC_VOLUMN_INCREASEMENT,    //E9 E9
    KC_VOLUMN_DECREASEMENT,    //EA EA
    KC_MEDIA_PLAY_PAUSE,       //EB CD
    KC_APPLAUNCHER_MAIL,       //EC 18A
    KC_APPLAUNCHER_CALCULATOR, //ED 192
    KC_APPLAUNCHER_COMPUTER,   //EE 194
    KC_BRIGHTNESS_INC,         //EF 6F
    KC_BRIGHTNESS_DEC          //F0 70
};

#define KC_CS_VOLUMN_MUTE 0xE2
#define KC_CS_VOLUMN_INCREASEMENT 0xE9
#define KC_CS_VOLUMN_DECREASEMENT 0xEA
#define KC_CS_MEDIA_PLAY_PAUSE 0xCD
#define KC_CS_APPLAUNCHER_MAIL 0x18A
#define KC_CS_APPLAUNCHER_CALCULATOR 0x192
#define KC_CS_APPLAUNCHER_COMPUTER 0x194
#define KC_CS_BRIGHTNESS_INC 0x6F
#define KC_CS_BRIGHTNESS_DEC 0x70


static uint16_t consumer_keycode[] = {
    KC_VOLUMN_MUTE, 
    KC_VOLUMN_INCREASEMENT,
    KC_VOLUMN_DECREASEMENT,
    KC_MEDIA_PLAY_PAUSE,
    KC_APPLAUNCHER_MAIL,
    KC_APPLAUNCHER_CALCULATOR,
    KC_APPLAUNCHER_COMPUTER,
    KC_BRIGHTNESS_INC,
    KC_BRIGHTNESS_DEC
};

enum keyboard_function_keycode
{                 // 0xA5-DF
    KC_FN = 0xA5, // A5 FN 键
    KC_FN0,       // A6 旋钮 按键
    KC_FN1,       // A7 旋钮 左旋
    KC_FN2,       // A8 旋钮 右旋
    KC_FN3,       // A9 light 灯效 上一个
    KC_FN4,       // AA light 灯效 下一个
    KC_FN5,       // AB light 亮度 降低
    KC_FN6,       // AC light 亮度 增加
    KC_FN7,       // AD light 颜色 上一个
    KC_FN8,       // AE light 颜色 下一个
    KC_FN9,       // AF light 开关
    KC_FN10,      // B0 关闭蓝牙
    KC_FN11,      // B1
    KC_FN12,      // B2
    KC_FN13,      // B3
    KC_FN14,      // B4
    KC_FN15,      // B5
    KC_FN16,      // B6
    KC_FN17,      // B7
    KC_FN18,      // B8
    KC_FN19,      // B9
    KC_FN20,      // BA
    KC_FN21,      // BB
    KC_FN22,      // BC
    KC_FN23,      // BD
    KC_FN24,      // BE
    KC_FN25,      // BF
};

#define KC_MUTE KC_VOLUMN_MUTE
#define KC_VOLU KC_VOLUMN_INCREASEMENT
#define KC_VOLD KC_VOLUMN_DECREASEMENT
#define KC_PLAY KC_MEDIA_PLAY_PAUSE
#define KC_MAIL KC_APPLAUNCHER_MAIL
#define KC_CALC KC_APPLAUNCHER_CALCULATOR
#define KC_COMP KC_APPLAUNCHER_COMPUTER
#define KC_BRI KC_BRIGHTNESS_INC
#define KC_BRD KC_BRIGHTNESS_DEC

#define KEYCODE_IS_COMMON(kc) (KC_A <= (kc) && (kc) <= KC_APP)
#define KEYCODE_IS_MODIFER(kc) (KC_LCTL <= (kc) && (kc) <= KC_RGUI)
#define KEYCODE_IS_CONSUMER(kc) (KC_VOLUMN_MUTE <= (kc) && (kc) <= KC_BRIGHTNESS_DEC)
#define KEYCODE_IS_EXFN(kc) (KC_FN0 <= (kc) && (kc) <= KC_FN25)
