#pragma once

/**
 * @brief Matrix Event
 * @defgroup matrix_event Matrix Event
 * @{
 */

#include <event_manager.h>

#include "matrix_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Matrix event.
 *
 * The matrix event is submitted when a key was pressed or released
 */
struct matrix_event {
	/** Event header. */
	struct event_header header;
	/**
	 * @brief id of key
	 * 
	 * the first key id is 1
	 * 0xFF marks the end of a scan
	 */
	uint8_t key_id;
    bool pressed;
};

EVENT_TYPE_DECLARE(matrix_event);

static inline void trig_matrix_event(uint8_t key_id, bool pressed)
{
	__ASSERT_NO_MSG(key_id < KEY_NUM);

	struct matrix_event *event = new_matrix_event();

	event->key_id = key_id;
    event->pressed = pressed;
	EVENT_SUBMIT(event);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */