#include "keyboard_evt.h"
#include <stdlib.h>
#include "queue.h"

typedef struct
{
    enum kbd_evt_list evt;
    void *arg;
} event_data_t;

#define MAX_KBD_EVT_COUNT 20

NRF_SECTION_DEF(keyboard_event, KbEventHandler);
QUEUE(event_data_t, events_queue, MAX_KBD_EVT_COUNT);

/**
 * 调用外部模块的事件处理
 */
static void external_event_handler(enum kbd_evt_list event, void *arg)
{
    uint8_t event_number = NRF_SECTION_ITEM_COUNT(keyboard_event, KbEventHandler);
    KbEventHandler *event_func_name;
    for (uint8_t i = 0; i < event_number; i++)
    {
        event_func_name = NRF_SECTION_ITEM_GET(keyboard_event, KbEventHandler, i);
        (*event_func_name)(event, arg);
    }
}

/**
 * @brief 执行事件
 */
void execute_event()
{
    while (!events_queue_empty())
    {
        event_data_t *pt = events_queue_peek();
        events_queue_pop();
        external_event_handler(pt->evt, pt->arg);
    }
}

/**
 * 触发一个事件
 * 
 * @param event: 事件类型
 * @param arg: 事件参数
 */
void trig_event(enum kbd_evt_list event, void *arg)
{
    switch (event)
    {
    default:
    {
        event_data_t temp_evt = {
            .evt = event,
            .arg = arg,
        };
        events_queue_push(temp_evt);
    }
    break;
    }
}

/**
 * 触发一个事件
 * 
 * @param event: 事件类型
 * @param arg: 事件参数
 */
void keyboard_evt_trigger(enum kbd_evt_list event)
{
    trig_event(event, (void *)255);
}
void keyboard_evt_trigger_param(enum kbd_evt_list event, uint8_t arg)
{
    trig_event(event, (void *)(uint32_t)arg);
}