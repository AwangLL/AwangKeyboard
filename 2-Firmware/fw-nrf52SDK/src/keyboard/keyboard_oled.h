#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "keyboard_menu.h"

void keyboard_oled_switch(void);
bool keyboard_oled_state(void);

void keyboard_oled_close(void);
void keyboard_oled_open(void);

void keyboard_oled_prev_app(menu_t tMenu);
void keyboard_oled_next_app(menu_t tMenu);
void keyboard_oled_enter(menu_t tMenu);
void keyboard_oled_exit(menu_t tMenu);
void keyboard_oled_prev_selection(void);
void keyboard_oled_next_selection(void);
void keyboard_oled_select(void);