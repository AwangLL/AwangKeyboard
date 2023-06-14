#ifndef OLED_ICO_H
#define OLED_ICO_H

#include <stdint.h>

typedef struct
{
    const uint8_t *data;
    uint16_t length;
    uint16_t width;
} pic_info_t;

static const uint8_t oled_pic_open[] = {
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0x7C, 0x3C, 0xFC, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xC0, 0x20, 0x10, 0x10, 0x08, 0x88, 0xDC, 0x7C, 0x5C, 0xDC, 0xCC, 0x40, 0x40, 0x40, 0x40, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE0, 0xF8, 0xFF, 0x8F, 0x83, 0x80, 0x80, 0x81, 0x8F, 0xFF, 0xFE, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0x3F, 0xFF, 0xF0, 0x80, 0x00, 0x00, 0xF0, 0xFF, 0x1F, 0x1F, 0xFF,
    0xF8, 0x80, 0x00, 0x00, 0xE0, 0xFC, 0x7F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xE1, 0x61, 0x61, 0x61, 0x61, 0x63, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x06, 0x03,
    0x01, 0x01, 0x01, 0x01, 0x03, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x3C, 0xFF, 0xC3, 0x81, 0x81, 0x81, 0x81, 0xC3, 0xFF, 0x3D, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0,
    0x6E, 0x61, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x02, 0x02, 0x82, 0x83, 0x81, 0xC1, 0x40, 0x40, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0xE0, 0x80, 0x00,
    0x00, 0x38, 0x3F, 0x1F, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0F, 0x3F, 0x3E, 0x30, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1F, 0x3F, 0x38, 0x3F, 0x0F, 0x01, 0x00, 0x00, 0x00,
    0x0F, 0x3F, 0x3C, 0x3F, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x3F, 0x38, 0x30, 0x30, 0x30, 0x18, 0x1C, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0xC0, 0xEF, 0xFF, 0x38, 0x19, 0x19, 0x19, 0x19, 0x18, 0x38, 0xF0, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78,
    0x85, 0x42, 0x01, 0x00, 0x00, 0x00, 0x44, 0x66, 0x22, 0x33, 0xB3, 0xF3, 0xB7, 0x9F, 0x1B, 0x91, 0x81, 0x81, 0x90, 0xB0, 0xD0, 0x88, 0x08, 0x18, 0x10, 0x10, 0x10, 0x20, 0x20, 0x20, 0x3F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x07, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0E, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2F,
    0x30, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2C, 0x2F, 0x27, 0x23, 0x23, 0x10, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
    0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
    0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
    0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0};
static const pic_info_t pic_open = {oled_pic_open, 128, 48};

static const uint8_t oled_ico_usb[] = {
    0x40, 0xE0, 0x40, 0x04, 0xFE, 0x04, 0x30, 0xF0, 0x30,
    0x00, 0x01, 0x02, 0x74, 0x7F, 0x71, 0x01, 0x00, 0x00};
static const pic_info_t ico_usb = {oled_ico_usb, 9, 16};

static const uint8_t oled_ico_bluetooth[16] = {
    0x18, 0x30, 0x60, 0xFE, 0x8C, 0xD8, 0x70, 0x20,
    0x18, 0x0C, 0x06, 0x7F, 0x31, 0x1B, 0x0E, 0x04};
static const pic_info_t ico_bluetooth = {oled_ico_bluetooth, 8, 16};

static const uint8_t oled_ico_keyboard[] = {
    0xFF, 0x01, 0x01, 0x49, 0x01, 0x01, 0x49, 0x01, 0x01, 0x49, 0x01, 0x01, 0x49, 0x01, 0x01, 0x01, 0x01, 0xF1, 0x01, 0x01, 0xFF,
    0xFF, 0x80, 0x80, 0x92, 0x80, 0x80, 0x92, 0x90, 0x90, 0x92, 0x90, 0x90, 0x92, 0x90, 0x90, 0x82, 0x82, 0x93, 0x80, 0x80, 0xFF};
static const pic_info_t ico_keyboard = {oled_ico_keyboard, 21, 16};

static const uint8_t oled_ico_battery0[] = {
    0xFC, 0xFE, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0xFC, 0xF0,
    0x3F, 0x7F, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x3F, 0x0F};
static const pic_info_t ico_battery0 = {oled_ico_battery0, 17, 16};

static const uint8_t oled_ico_battery10[] = {
    0xFC, 0xFE, 0x06, 0xF6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0xFC, 0xF0,
    0x3F, 0x7F, 0x60, 0x6F, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x3F, 0x0F};
static const pic_info_t ico_battery10 = {oled_ico_battery10, 17, 16};

static const uint8_t oled_ico_battery_per[] = {
    0xF6,
    0x6F};
static const pic_info_t ico_battery_per = {oled_ico_battery_per, 1, 16};

static const uint8_t oled_ico_exit[] = {
    0x38, 0x44, 0x82, 0x92, 0x92, 0x54, 0x10, 0x00};
static const pic_info_t ico_exit = {oled_ico_exit, 8, 8};

static const uint8_t oled_ico_unselected[] = {
    0x00, 0x18, 0x24, 0x42, 0x42, 0x24, 0x18, 0x00};
static const pic_info_t ico_unselected = {oled_ico_unselected, 8, 8};

static const uint8_t oled_ico_selected[] = {
    0x00, 0x18, 0x24, 0x5A, 0x5A, 0x24, 0x18, 0x00};
static const pic_info_t ico_selected = {oled_ico_selected, 8, 8};

static const uint8_t oled_app[][128] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0xFF, 0x01, 0x01, 0x01, 0x11, 0x01, 0x01, 0x01, 0x11, 0x01, 0x01, 0x01, 0x11, 0x01, 0x01, 0x01, 0x11, 0x01, 0x01, 0x01, 0x01, 0xF1, 0x01, 0x01, 0x01, 0xFF, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0xFF, 0x80, 0x80, 0x80, 0x91, 0x80, 0x80, 0x80, 0x91, 0x90, 0x90, 0x90, 0x91, 0x90, 0x90, 0x90, 0x91, 0x90, 0x80, 0x81, 0x81, 0x91, 0x80, 0x80, 0x80, 0xFF, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //keyboard
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x40, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x40, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0xF0, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0C, 0x06, 0x03, 0x01, 0x00, 0x00, 0xFF, 0x00, 0x18, 0x71, 0xC3, 0x0E, 0x38, 0xE1, 0x03, 0x0E, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x0F, 0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x60, 0xC0, 0x80, 0x00, 0x00, 0xFF, 0x00, 0x18, 0x8E, 0xC3, 0x70, 0x1C, 0x87, 0xC0, 0x70, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //volumn
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x88, 0x1C, 0x18, 0xC1, 0xF3, 0xFB, 0xF8, 0xFC, 0xFD, 0xFD, 0xFC, 0xF8, 0xFB, 0xF3, 0xC1, 0x18, 0x9C, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x11, 0x38, 0x18, 0x83, 0xCF, 0xDF, 0x1F, 0x3F, 0xBF, 0xBF, 0x3F, 0x1F, 0xDF, 0xCF, 0x83, 0x18, 0x39, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //brightness
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0xC0, 0x20, 0x10, 0x08, 0x04, 0x64, 0xE2, 0xE1, 0xC1, 0x01, 0x18, 0x18, 0x1C, 0x0C, 0x00, 0x00, 0x00, 0x10, 0x3C, 0x1C, 0x19, 0x01, 0x01, 0x02, 0x02, 0x8C, 0x70, 0x00, 0x00,
     0x00, 0x00, 0x0F, 0x30, 0x40, 0x86, 0x87, 0x06, 0x00, 0x30, 0x78, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x50, 0x50, 0x24, 0x0A, 0x0A, 0x92, 0x61, 0x01, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //color
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x61, 0x80, 0x00, 0x00, 0x18, 0x60, 0x80, 0x80, 0x80, 0x60, 0x18, 0x00, 0x80, 0x61, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0x8E, 0x54, 0x54, 0x54, 0x57, 0x54, 0x57, 0x54, 0x54, 0xDE, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //mode
    {0x00, 0x00, 0x00, 0x00, 0xC0, 0x20, 0x10, 0x10, 0x20, 0x40, 0x80, 0x80, 0xC0, 0x78, 0x04, 0x04, 0x04, 0x04, 0x78, 0xC0, 0x80, 0x80, 0x40, 0x20, 0x10, 0x10, 0x20, 0xC0, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0xC0, 0x20, 0x20, 0x21, 0x32, 0x1C, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x20, 0x10, 0x10, 0x10, 0x10, 0x20, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x32, 0x21, 0x20, 0x20, 0xC0, 0x00, 0x00,
     0x00, 0x00, 0x03, 0x04, 0x04, 0x84, 0x4C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x08, 0x08, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x38, 0x4C, 0x84, 0x04, 0x04, 0x03, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x08, 0x04, 0x02, 0x01, 0x01, 0x03, 0x1E, 0x20, 0x20, 0x20, 0x20, 0x1E, 0x03, 0x01, 0x01, 0x02, 0x04, 0x08, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00}, //setting
};
static const pic_info_t app[] = {
    {oled_app[0], 32, 32}, //keyboard
    {oled_app[1], 32, 32}, //volumn
    {oled_app[2], 32, 32}, //bright
    {oled_app[3], 32, 32}, //color
    {oled_app[4], 32, 32}, //mode
    {oled_app[5], 32, 32}, //setting
};

static const uint8_t oled_app_selector[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0xF8, 0x78, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x78, 0xF8, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x1F, 0x1E, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1E, 0x1F, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const pic_info_t app_selector = {oled_app_selector, 64, 64};

#endif