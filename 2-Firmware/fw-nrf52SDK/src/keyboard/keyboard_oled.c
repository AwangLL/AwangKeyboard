#include "keyboard_oled.h"
#include <stdint.h>
#include <stdbool.h>
#include "nrf_delay.h"

#include "oled/oled_api.h"
#include "keyboard_menu.h"
#include "keyboard_evt.h"

#include "kb_nrf_print.h"

extern uint8_t oled_display_buffer[];
extern uint8_t app_state;
extern bool selecting_app;
extern uint8_t seleting_state;

static const uint8_t menuIndex2Icon[] = {0, 1, 2, 3, 4, 5};

bool oled_state;

int yx(uint8_t t, int start, int end)
{
	int res;
	res = t * (200 - t);
	res = res * (end - start) / 10000 + start;
	return res;
}

bool keyboard_oled_state(void)
{
	return oled_state;
}

void keyboard_oled_switch(void)
{
	if (oled_state)
	{
		keyboard_oled_close();
		oled_switch_off();
		oled_state = false;
	}
	else
	{
		oled_switch_on();
		oled_state = true;
		keyboard_oled_open();
	}
}
void keyboard_oled_draw_border(char *str)
{
	uint8_t c = 0;
	oled_display_buffer[c++] = 0xF8;
	oled_display_buffer[c++] = 0xFC;
	oled_display_buffer[c++] = 0x06;
	while (c < 33)
		oled_display_buffer[c++] = 0x03;
	oled_display_buffer[c++] = 0x0F;
	oled_display_buffer[c++] = 0x1F;
	oled_display_buffer[c++] = 0x3F;
	oled_display_buffer[c++] = 0x7F;

	oled_display_buffer[c++] = 0xff;
	oled_display_buffer[OLED_SIZE_LENGTH + c] = 0x01;
	oled_display_buffer[c++] = 0xff;
	while (c < 91)
	{
		oled_display_buffer[OLED_SIZE_LENGTH + c] = 0x03;
		oled_display_buffer[c++] = 0xff;
	}
	oled_display_buffer[OLED_SIZE_LENGTH + c] = 0x01;
	oled_display_buffer[c++] = 0xff;
	oled_display_buffer[c++] = 0xff;

	oled_display_buffer[c++] = 0x7F;
	oled_display_buffer[c++] = 0x3F;
	oled_display_buffer[c++] = 0x1F;
	oled_display_buffer[c++] = 0x0F;
	while (c < 125)
		oled_display_buffer[c++] = 0x03;
	oled_display_buffer[c++] = 0x06;
	oled_display_buffer[c++] = 0xFC;
	oled_display_buffer[c++] = 0xF8;

	for (c = 1; c <= 6; c++)
	{
		oled_display_buffer[c * OLED_SIZE_LENGTH] = 0xFF;
		oled_display_buffer[c * OLED_SIZE_LENGTH + 1] = 0xFF;
		oled_display_buffer[c * OLED_SIZE_LENGTH + 126] = 0xFF;
		oled_display_buffer[c * OLED_SIZE_LENGTH + 127] = 0xFF;
	}

	oled_display_buffer[7 * OLED_SIZE_LENGTH] = 0x1F;
	oled_display_buffer[7 * OLED_SIZE_LENGTH + 1] = 0x3F;
	oled_display_buffer[7 * OLED_SIZE_LENGTH + 2] = 0x60;
	for (c = 3; c < 125; c++)
	{
		oled_display_buffer[7 * OLED_SIZE_LENGTH + c] = 0xC0;
	}
	oled_display_buffer[7 * OLED_SIZE_LENGTH + 125] = 0x60;
	oled_display_buffer[7 * OLED_SIZE_LENGTH + 126] = 0x3F;
	oled_display_buffer[7 * OLED_SIZE_LENGTH + 127] = 0x1F;

	oled_buffer_draw_font5x8_inverse(1, 0, str, TEXT_ALIGN_CENTER);
}
void keyboard_oled_open(void)
{
	oled_buffer_clear();
	oled_buffer_show_all();

	oled_buffer_draw_picture(8, 0, &pic_open, TEXT_ALIGN_LEFT);
	for (uint8_t c = 0; c < OLED_SIZE_LENGTH; c++)
	{
		for (uint8_t p = 0; p < OLED_SIZE_PAGE; p++)
		{
			nrf_delay_ms(1);
			oled_buffer_show_page(p, c, c);
		}
	}
}

void keyboard_oled_close(void)
{
	uint8_t r, c, p, i;
	uint8_t data_buf[1] = {0};
	uint64_t temp_buf;
	for (r = 0; r < OLED_SIZE_WIDTH / 2; r++)
	{
		for (c = 0; c < OLED_SIZE_LENGTH; c++)
		{
			temp_buf = 0;
			for (p = 0; p < OLED_SIZE_PAGE; p++)
			{
				temp_buf |= (uint64_t)oled_display_buffer[p * OLED_SIZE_LENGTH + c] << (8 * p);
			}
			temp_buf &= ~((uint64_t)0x1 << r);
			temp_buf &= ~((uint64_t)0x1 << OLED_SIZE_WIDTH - r - 1);
			temp_buf |= (uint64_t)1 << r;
			temp_buf |= (uint64_t)1 << (OLED_SIZE_WIDTH - r - 1);
			if (r)
			{
				temp_buf &= ~((uint64_t)0x1 << r - 1);
				temp_buf &= ~((uint64_t)0x1 << OLED_SIZE_WIDTH - r);
			}
			for (p = 0; p < OLED_SIZE_PAGE; p++)
			{
				oled_display_buffer[p * OLED_SIZE_LENGTH + c] = temp_buf >> (8 * p);
			}
		}
		oled_buffer_show_all();
	}
	for (c = 0; c < OLED_SIZE_LENGTH / 2; c++)
	{
		nrf_delay_ms(2);
		oled_i2c_start();
		oled_show_data(3, c, 1, data_buf);
		oled_show_data(4, c, 1, data_buf);
		oled_show_data(3, OLED_SIZE_LENGTH - c - 1, 1, data_buf);
		oled_show_data(4, OLED_SIZE_LENGTH - c - 1, 1, data_buf);
		oled_i2c_stop();
	}
}

void keyboard_oled_prev_app(menu_t tMenu)
{
	uint8_t t;
	if (tMenu.prevBroIndex == tMenu.curIndex)
	{
		for (t = 0; t <= 100; t += 10)
		{
			oled_buffer_clear();
			oled_buffer_draw_picture(0, 32, &app_selector, TEXT_ALIGN_LEFT);
			oled_buffer_draw_picture(16, yx(t, 48, 96), &app[menuIndex2Icon[tMenu.curIndex]], TEXT_ALIGN_LEFT);
			oled_buffer_show_page(3, 0, 127);
			oled_buffer_show_page(4, 0, 127);
		}
		for (t = 0; t <= 100; t += 20)
		{
			oled_buffer_clear();
			oled_buffer_draw_picture(0, 32, &app_selector, TEXT_ALIGN_LEFT);
			oled_buffer_draw_picture(16, yx(t, 96, 48), &app[menuIndex2Icon[tMenu.curIndex]], TEXT_ALIGN_LEFT);
			oled_buffer_show_page(3, 0, 127);
			oled_buffer_show_page(4, 0, 127);
		}
		oled_buffer_draw_picture(16, 48, &app[menuIndex2Icon[tMenu.curIndex]], TEXT_ALIGN_LEFT);
		oled_buffer_draw_font5x8(0, 0, tMenu.name, TEXT_ALIGN_CENTER);
		oled_buffer_draw_font5x8(0, 8, "<<", TEXT_ALIGN_LEFT);
		oled_buffer_draw_font5x8(0, 127, ">>", TEXT_ALIGN_RIGHT);
		oled_buffer_show_all();
	}
	else
	{
		for (t = 0; t <= 100; t += 5)
		{
			oled_buffer_clear();
			oled_buffer_draw_picture(0, 32, &app_selector, TEXT_ALIGN_LEFT);
			oled_buffer_draw_picture(16, yx(t, 48, 128), &app[menuIndex2Icon[tMenu.curIndex]], TEXT_ALIGN_LEFT);
			oled_buffer_draw_picture(16, yx(t, 0, 79), &app[menuIndex2Icon[tMenu.prevBroIndex]], TEXT_ALIGN_RIGHT);
			oled_buffer_show_page(3, 0, 127);
			oled_buffer_show_page(4, 0, 127);
		}
		oled_buffer_draw_font5x8(0, 0, menu[tMenu.prevBroIndex].name, TEXT_ALIGN_CENTER);
		oled_buffer_draw_font5x8(0, 8, "<<", TEXT_ALIGN_LEFT);
		oled_buffer_draw_font5x8(0, 127, ">>", TEXT_ALIGN_RIGHT);
		oled_buffer_show_all();
	}
}

void keyboard_oled_next_app(menu_t tMenu)
{
	uint8_t t;
	if (tMenu.nextBroIndex == tMenu.curIndex)
	{
		for (t = 0; t <= 100; t += 10)
		{
			oled_buffer_clear();
			oled_buffer_draw_picture(0, 32, &app_selector, TEXT_ALIGN_LEFT);
			oled_buffer_draw_picture(16, yx(t, 79, 31), &app[menuIndex2Icon[tMenu.curIndex]], TEXT_ALIGN_RIGHT);
			oled_buffer_show_page(3, 0, 127);
			oled_buffer_show_page(4, 0, 127);
		}
		for (t = 0; t <= 100; t += 20)
		{
			oled_buffer_clear();
			oled_buffer_draw_picture(0, 32, &app_selector, TEXT_ALIGN_LEFT);
			oled_buffer_draw_picture(16, yx(t, 31, 79), &app[menuIndex2Icon[tMenu.curIndex]], TEXT_ALIGN_RIGHT);
			oled_buffer_show_page(3, 0, 127);
			oled_buffer_show_page(4, 0, 127);
		}
		oled_buffer_draw_picture(16, 79, &app[menuIndex2Icon[tMenu.curIndex]], TEXT_ALIGN_RIGHT);
		oled_buffer_draw_font5x8(0, 0, tMenu.name, TEXT_ALIGN_CENTER);
		oled_buffer_draw_font5x8(0, 8, "<<", TEXT_ALIGN_LEFT);
		oled_buffer_draw_font5x8(0, 127, ">>", TEXT_ALIGN_RIGHT);
		oled_buffer_show_all();
	}
	else
	{
		for (t = 0; t <= 100; t += 5)
		{
			oled_buffer_clear();
			oled_buffer_draw_picture(0, 32, &app_selector, TEXT_ALIGN_LEFT);
			oled_buffer_draw_picture(16, yx(t, 79, 0), &app[menuIndex2Icon[tMenu.curIndex]], TEXT_ALIGN_RIGHT);
			oled_buffer_draw_picture(16, yx(t, 127, 48), &app[menuIndex2Icon[tMenu.nextBroIndex]], TEXT_ALIGN_LEFT);
			oled_buffer_show_page(3, 0, 127);
			oled_buffer_show_page(4, 0, 127);
		}
		oled_buffer_draw_font5x8(0, 0, menu[tMenu.nextBroIndex].name, TEXT_ALIGN_CENTER);
		oled_buffer_draw_font5x8(0, 8, "<<", TEXT_ALIGN_LEFT);
		oled_buffer_draw_font5x8(0, 127, ">>", TEXT_ALIGN_RIGHT);
		oled_buffer_show_all();
	}
}
void keyboard_oled_enter(menu_t tMenu)
{
	switch (tMenu.curIndex)
	{
	case menu_index_keyboard:

		break;
	case menu_index_volumn:
		oled_buffer_clear();
		oled_buffer_draw_font5x8(0, 0, "volumn", TEXT_ALIGN_LEFT);
		oled_buffer_show_all();
		break;
	case menu_index_bright:
		oled_buffer_clear();
		oled_buffer_draw_font5x8(0, 0, "bright", TEXT_ALIGN_LEFT);
		oled_buffer_show_all();
		break;
	case menu_index_color:
		oled_buffer_clear();
		oled_buffer_draw_font5x8(0, 0, "color", TEXT_ALIGN_LEFT);
		oled_buffer_show_all();
		break;
	case menu_index_mode:
		oled_buffer_clear();
		oled_buffer_draw_font5x8(0, 0, "mode", TEXT_ALIGN_LEFT);
		oled_buffer_show_all();
		break;
	case menu_index_setting:
		oled_buffer_clear();
		oled_buffer_draw_font5x8(0, 0, "setting", TEXT_ALIGN_LEFT);
		oled_buffer_show_all();
		break;
	default:
		break;
	}
}
void keyboard_oled_exit(menu_t tMenu)
{
	uint8_t backIndex;
	backIndex = menuIndex2Icon[tMenu.backIndex];
	oled_buffer_clear();
	oled_buffer_draw_picture(16, 48, &app[backIndex], TEXT_ALIGN_LEFT);
	oled_buffer_show_all();
	//   uint8_t offset;
	//   uint64_t buf;
	//   for(uint8_t t = 0;t<=90;t+=10)
	//   {
	//offset = yx(t,OLED_SIZE_WIDTH,0) - yx(t+10,OLED_SIZE_WIDTH,0);
	//for(uint8_t j = 0;j<OLED_SIZE_LENGTH;j++)
	//{
	//    buf = 0;
	//    for(uint8_t k = 0;k<OLED_SIZE_PAGE;k++)
	//    {
	//	buf += (uint64_t)oled_display_buffer[k*OLED_SIZE_LENGTH+j]<<(8*k);
	//    }
	//    buf = buf>>offset;//上移
	//    for(uint8_t k = 0;k<OLED_SIZE_PAGE;k++)
	//    {
	//	oled_display_buffer[k*OLED_SIZE_LENGTH+j] = (uint8_t)(buf>>(8*k));
	//    }
	//}
	//oled_buffer_show_all();
	//   }
	//   nrf_delay_ms(100);
	//   oled_buffer_draw_icon(2,TEXT_ALIGN_LEFT,47,&app[app_state]);
	//   oled_buffer_show_all();
	//   nrf_delay_ms(100);
	//   oled_buffer_draw_picture(0,32,&app_selector,TEXT_ALIGN_LEFT);
	//   oled_buffer_draw_icon(2,TEXT_ALIGN_LEFT,47,&app[app_state]);
	//   oled_buffer_show_all();
	//   nrf_delay_ms(100);
	//   oled_buffer_draw_font5x8(0,0,(char *)app[app_state].name,TEXT_ALIGN_CENTER);
	//   oled_buffer_draw_font5x8(0,8,"<<",TEXT_ALIGN_LEFT);
	//   oled_buffer_draw_font5x8(0,8,">>",TEXT_ALIGN_RIGHT);
	//   oled_buffer_show_all();
}

void keyboard_oled_prev_selection(void)
{
	switch (app_state)
	{
	}
}
void keyboard_oled_next_selection(void)
{
	switch (app_state)
	{
	}
}
void keyboard_oled_select(void)
{
	switch (app_state)
	{
	}
}

/* Event Hanlder*/
static void keyboard_oled_evt_hanlder(enum kbd_evt_list event, void *arg)
{
	switch (event)
	{
	case KBD_EVT_INIT:
		// oled_init();
		// kb_print("oled module initialized.");
		break;
	case KBD_EVT_SLEEP:
		// keyboard_oled_close();
		// keyboard_oled_switch();
		// kb_print("oled module stopped.");
		break;
	default:
		break;
	}
}
EVENT_HANDLER(keyboard_oled_evt_hanlder);