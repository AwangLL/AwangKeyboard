#include "keyboard_menu.h"
#include <stdbool.h>

#include "events.h"
#include "keyboard_evt.h"
#include "keyboard_oled.h"

#include "kb_nrf_print.h"

static menu_t cur_menu = menu[0];
bool RDone = true;
bool LDone = true;

void keyboard_menu_init(void)
{
	cur_menu = menu[0];
}

void keyboard_menu_encoderAction(uint8_t action)
{
	switch (action)
	{
	case EC_KEY_PRESSED:
		if (cur_menu.level == 1)
		{
			keyboard_oled_enter(cur_menu);
			//寻找第一个子节点
			for (uint8_t i = 0; i < sizeof(menu); i++)
			{
				if (menu[i].backIndex == cur_menu.curIndex)
				{
					cur_menu = menu[i];
					break;
				}
			}
		}
		else
		{
			keyboard_oled_exit(cur_menu);
			cur_menu = menu[cur_menu.backIndex];
		}
		break;
	case EC_ROTATE_LEFT:
		if (LDone)
		{
			LDone = false;
			if (cur_menu.level == 1)
			{
				keyboard_oled_prev_app(cur_menu);
				cur_menu = menu[cur_menu.prevBroIndex];
			}
			LDone = true;
		}
		else
			return;
		break;
	case EC_ROTATE_RIGHT:
		if (RDone)
		{
			RDone = false;
			if (cur_menu.level == 1)
			{
				keyboard_oled_next_app(cur_menu);
				cur_menu = menu[cur_menu.nextBroIndex];
			}
			RDone = true;
		}
		else
			return;
		break;
	default:
		break;
	}
}
/* Event Hanlder*/
static void keyboard_menu_evt_hanlder(enum kbd_evt_list event, void *arg)
{
	switch (event)
	{
	case KBD_EVT_INIT:
		// keyboard_menu_init();
		break;
	case KBD_EVT_START:
		// keyboard_oled_switch();
		// keyboard_oled_enter(menu[menu_index_keyboard]);
		break;
	case KBD_EVT_SLEEP:
		keyboard_oled_close();
		keyboard_oled_switch();
		kb_print("oled module stopped.");
		break;
	default:
		break;
	}
}
EVENT_HANDLER(keyboard_menu_evt_hanlder);