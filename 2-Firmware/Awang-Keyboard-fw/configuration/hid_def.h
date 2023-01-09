#pragma once

#define INPUT_REPORT_LEN_KEYBOARD 15
#define OUTPUT_REPORT_LEN_KEYBOARD 1
#define INPUT_REPORT_LEN_CONSUMER 2
#define INPUT_REPORT_ID_KEYBOARD 0x01
#define OUTPUT_REPORT_ID_KEYBOARD 0x01
#define INPUT_REPORT_ID_CONSUMER 0x02

static const uint8_t hid_kbd_report_desc_nkro[] = {
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x06, // Usage (Keyboard)
    0xA1, 0x01, // Collection (Application)
    0x85, INPUT_REPORT_ID_KEYBOARD, // Report ID (1)

    0x95, 0x08, // Report Count (8)
    0x75, 0x01, // Report Size (1)
    0x05, 0x07, // Usage Page (Key Codes)
    0x19, 0xe0, // Usage Minimum (224)
    0x29, 0xe7, // Usage Maximum (231)
    0x15, 0x00, // Logical Minimum (0)
    0x25, 0x01, // Logical Maximum (1)
    0x81, 0x02, // Input (Data, Variable, Absolute)

    0x95, 0x01, // Report Count (1)
    0x75, 0x08, // Report Size (8)
    0x81, 0x03, // Input (Constant, Variable, Absolute) reserved byte(1)

    0x95, 0x66, // Report Count (102)
    0x75, 0x01, // Report Size (1)
    0x19, 0x00, // Usage Minimum (0)
    0x29, 0x65, // Usage Maximum (101)
    0x15, 0x00, // Logical Minimum (0)
    0x25, 0x01, // Logical Maximum (1)
    0x81, 0x02, // Input (Data, Variable, Absolute)
    0x95, 0x01, // Report Count (2)
    0x75, 0x02, // Report Size (1)
    0x81, 0x03, // Input (Constant, Variable, Absolute) reserved byte(1)

    0x95, 0x05, // Report Count (5)
    0x75, 0x01, // Report Size (1)
    0x15, 0x00, // Logical Minimum (0)
    0x25, 0x01, // Logical Maximum (1)
    0x05, 0x08, // Usage Page (Page# for LEDs)
    0x19, 0x01, // Usage Minimum (1) (Num Lock)
    0x29, 0x05, // Usage Maximum (5) (Kana)
    0x91, 0x02, // Output (Data, Variable, Absolute), Led report
    0x95, 0x01, // Report Count (1)
    0x75, 0x03, // Report Size (3)
    0x91, 0x03, // Output (Const, Variable, Absolute), Led report padding

    0xC0, // End Collection (Application)

    0x05, 0x0C, // Usage Page (Consumer Devices)
    0x09, 0x01, // Usage (Consumer Control)
    0xA1, 0x01, // Collection (Application)
    0x85, INPUT_REPORT_ID_CONSUMER, //   Report ID (2)

    0x95, 0x01,       // Report Count (1)
    0x75, 0x10,       // Report Size (16)
    0x19, 0x01,       // Usage Minimum (0x01)
    0x2A, 0x9C, 0x02, // Usage Maximum (0x29c)
    0x15, 0x01,       // Logical Minimum (0x01)
    0x26, 0x9C, 0x02, // Logical Maximum (0x29c)
    0x81, 0x00,       // Input (Data, Array) Key array(2 bytes)
    
    0xC0,             // End Collection (Application)
};

enum keycode_t
{
    KC_TRNS = 0x01,
    /* keyboard keycode */
    /* Common */
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
    KC_F2,           // 3B Keyboard F2
    KC_F3,           // 3C Keyboard F3
    KC_F4,           // 3D Keyboard F4
    KC_F5,           // 3E Keyboard F5
    KC_F6,           // 3F Keyboard F6
    KC_F7,           // 40 Keyboard F7
    KC_F8,           // 41 Keyboard F8
    KC_F9,           // 42 Keyboard F9
    KC_F10,          // 43 Keyboard F10
    KC_F11,          // 44 Keyboard F11
    KC_F12,          // 45 Keyboard F12
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

    /* consumer keycode */
    /* 0xE8 - 0xFF */
    KC_MUTE = 0xE8, //E8 E2  comsumer volumn mute
    KC_VOLU,        //E9 E9  consumer volumn increasement
    KC_VOLD,        //EA EA  consumer volumn decreasement
    KC_PLAY,        //EB CD  consumer media play & pause
    KC_MAIL,        //EC 18A consumer application mail
    KC_CALC,        //ED 192 consumer application calculator
    KC_COMP,        //EE 194 consumer application computer

    /* function keycode */
    /* 0xA5 - 0xDF */
    KC_FN = 0xA5, // A5 FN 键
    KC_FN0,       // A6 
    KC_FN1,       // A7
    KC_FN2,       // A8 
    KC_FN3,       // A9
    KC_FN4,       // AA
    KC_FN5,       // AB
    KC_FN6,       // AC
    KC_FN7,       // AD
    KC_FN8,       // AE
    KC_FN9,       // AF
    KC_FN10,      // B0
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

static const uint16_t consumer_keycode_map[] = {
    0xE2,
    0xE9,
    0xEA,
    0xCD,
    0x18A,
    0x192,
    0x194
};

static inline uint16_t get_consumer_by_keycode(uint8_t keycode)
{
    return consumer_keycode_map[keycode - 0xE8];
}

inline bool keycode_is_common(uint8_t kc)
{
    return KC_A <= kc && kc <= KC_APP;
}

inline bool keycode_is_modifier(uint8_t kc)
{
    return KC_LCTL <= kc && kc <= KC_RGUI;
}

inline bool keycode_is_consumer(uint8_t kc)
{
    return 0xE8 <= kc && kc <= 0xFF;
}

inline bool keycode_is_function(uint8_t kc)
{
    return 0xA5 <= kc && kc <= 0xDF;
}