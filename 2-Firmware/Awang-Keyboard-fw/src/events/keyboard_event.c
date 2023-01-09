#include <assert.h>
#include <stdio.h>

#include "keyboard_event.h"

static const char * const event_name[] = {
#define X(name) STRINGIFY(name),
    KEYBOARD_EVENT_LIST
#undef X
};

static int log_keyboard_event(const struct event_header *eh, char *buf,
			    size_t buf_len)
{
	const struct keyboard_event *event = cast_keyboard_event(eh);

    BUILD_ASSERT(ARRAY_SIZE(event_name) == KEYBOARD_EVENT_COUNT,
            "Invalide number of elements");
    
    __ASSERT_NO_MSG(event->event < KEYBOARD_EVENT_COUNT);

    EVENT_MANAGER_LOG(eh, "%s", event_name[event->event]);
    return 0;
}

static void profile_keyboard_event(struct log_event_buf *buf,
				 const struct event_header *eh)
{
	const struct keyboard_event *event = cast_keyboard_event(eh);

    profiler_log_encode_uint8(buf, event->event);
}

EVENT_INFO_DEFINE(keyboard_event,
		  ENCODE(PROFILER_ARG_U8),
		  ENCODE("event"),
		  profile_keyboard_event);

EVENT_TYPE_DEFINE(keyboard_event,
		  IS_ENABLED(CONFIG_AWANG_INIT_LOG_KEYBOARD_EVENT),
		  log_keyboard_event,
		  &keyboard_event_info);
