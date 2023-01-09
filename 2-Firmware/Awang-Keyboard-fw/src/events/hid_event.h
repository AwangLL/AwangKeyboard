#pragma once

/**
 * @brief Matrix Event
 * @defgroup matrix_event Matrix Event
 * @{
 */

#include <event_manager.h>

#include "hid_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Hid report send event.
 *
 * The hid report send event is submitted when a hid report was sent
 */
struct hid_report_send_event {
	/** Event header. */
	struct event_header header;

	uint8_t report_id;
    uint8_t *data;
};

EVENT_TYPE_DECLARE(hid_report_send_event);

static inline void trig_hid_report_send_event(uint8_t report_id, uint8_t *data)
{
	struct hid_report_send_event *event = new_hid_report_send_event();

	event->report_id = report_id;
	event->data = data;
	EVENT_SUBMIT(event);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */