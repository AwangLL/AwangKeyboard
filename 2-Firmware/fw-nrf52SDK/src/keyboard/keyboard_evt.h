#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "nrf_section.h"
#include "events.h"

/**
 * @brief UserEventHandler 是自定义的事件处理函数
 * 
 * @param uint8_t code: 事件代码，见events.h内的 enum user_event 定义
 * @param void* arg: 事件参数的指针，详见文档。
 */
typedef void (*KbEventHandler)(uint8_t, void *);

/**
 * @brief 将一个函数注册为事件处理函数
 */
#define EVENT_HANDLER(_func) NRF_SECTION_ITEM_REGISTER(keyboard_event, const KbEventHandler _pt_##_func) = &_func

void execute_event(void);

void keyboard_evt_trigger(enum kbd_evt_list event);
void keyboard_evt_trigger_param(enum kbd_evt_list event, uint8_t arg);
