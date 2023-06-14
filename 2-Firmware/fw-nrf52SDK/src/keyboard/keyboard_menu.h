#pragma once

#include <stdint.h>

#define null_name ""
#define null_index 255

enum menu_index
{
    menu_index_keyboard = 0,
    menu_index_volumn,
    menu_index_bright,
    menu_index_color,
    menu_index_mode,
    menu_index_setting,
};

typedef struct
{
    uint8_t level;
    char *name;
    uint8_t curIndex;
    uint8_t backIndex;
    uint8_t prevBroIndex;
    uint8_t nextBroIndex;
} menu_t;

static const menu_t menu[] = {
    //一级菜单
    {1, "KEYBOARD", menu_index_keyboard, null_index, 0, 1},
    {1, "VOLUMN", menu_index_volumn, null_index, 0, 2},
    {1, "BRIGHT", menu_index_bright, null_index, 1, 3},
    {1, "COLOR", menu_index_color, null_index, 2, 4},
    {1, "MODE", menu_index_mode, null_index, 3, 5},
    {1, "SETTING", menu_index_setting, null_index, 4, 5},
    //二级菜单
    {2, null_name, 6, menu_index_keyboard, null_index, null_index},
    {2, null_name, 7, menu_index_volumn, null_index, null_index},
    {2, null_name, 8, menu_index_bright, null_index, null_index},
    {2, "red", 9, menu_index_color, null_index, 1},
    {2, "green", 10, menu_index_color, 0, 2},
    {2, "blue", 11, menu_index_color, 1, null_index},
};
void keyboard_menu_encoderAction(uint8_t action);