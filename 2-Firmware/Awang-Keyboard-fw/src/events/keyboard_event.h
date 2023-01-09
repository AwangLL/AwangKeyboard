#pragma once

/**
 * @file
 * @defgroup keyboard_event Keyboard Event
 * @{
 * @brief Keyboard Event.
 */

#include <event_manager.h>
#include <event_manager_profiler_tracer.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Keyboard event list */
#define KEYBOARD_EVENT_LIST \
    X(READY)\
	X(SLEEP)\
	X(AWAKE)\
	X(TICK)\
	X(LED_SWITCH)


/** @brief Keyboard event */
enum keyboard_event_list {
#define X(name) _CONCAT(KEYBOARD_EVENT_, name),
    KEYBOARD_EVENT_LIST
#undef X

    KEYBOARD_EVENT_COUNT
};

/** @brief Keyboard event.
 *
 * The keyboard event is submitted when a keyboard event occurred.
 */
struct keyboard_event {
	/** Event header. */
	struct event_header header;

	/** event of the keyboard */
	enum keyboard_event_list event;
};

EVENT_TYPE_DECLARE(keyboard_event);

static inline void trig_keyboard_event(enum keyboard_event_list e)
{
	__ASSERT_NO_MSG(state < KEYBOARD_EVENT_COUNT);

	struct keyboard_event *event = new_keyboard_event();

	event->event = e;
	EVENT_SUBMIT(event);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */