#include <assert.h>
#include <stdio.h>

#include "hid_event.h"

static int log_hid_report_send_event(const struct event_header *eh, char *buf,
			    size_t buf_len)
{
	const struct hid_report_send_event *event = cast_hid_report_send_event(eh);

    EVENT_MANAGER_LOG(eh, "report:%d send", event->report_id);
    
    return 0;
}

EVENT_TYPE_DEFINE(hid_report_send_event,
        IS_ENABLED(CONFIG_AWANG_INIT_LOG_HID_EVENT),
        log_hid_report_send_event,
        NULL);
