#ifndef OLED_API_H
#define OLED_API_H

#include <stdint.h>
#include "oled_font.h"
#include "oled_pic.h"
#include "config.h"

enum text_align
{
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT,
};
/*** oled ***/
void oled_init(void);
void oled_show_data(uint8_t page, uint8_t col_start, uint8_t len, uint8_t *buf);
void oled_switch_on(void);
void oled_switch_off(void);
void oled_i2c_start(void);
void oled_i2c_stop(void);
/*** oled buffer ***/
void oled_buffer_draw_icon(uint8_t page, enum text_align align, uint8_t col, const pic_info_t *icon);
void oled_buffer_draw_font5x8(uint8_t row, uint8_t col, char *str, enum text_align align);
void oled_buffer_draw_font5x8_inverse(uint8_t row, uint8_t col, char *str, enum text_align align);
void oled_buffer_draw_picture(uint8_t row, uint8_t col, const pic_info_t *pic, enum text_align align);
void oled_buffer_show_page(uint8_t page, uint8_t startC, uint8_t endC);
void oled_buffer_show_all(void);
void oled_buffer_clear(void);
#endif