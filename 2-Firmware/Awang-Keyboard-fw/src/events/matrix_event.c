#include <assert.h>
#include <stdio.h>

#include "matrix_event.h"

static int log_matrix_event(const struct event_header *eh, char *buf,
			    size_t buf_len)
{
	const struct matrix_event *event = cast_matrix_event(eh);

    EVENT_MANAGER_LOG(eh, "key %d %s", event->key_id, 
        event->pressed ? "pressed" : "released");;
    
    return 0;
}

EVENT_TYPE_DEFINE(matrix_event,
        IS_ENABLED(CONFIG_AWANG_INIT_LOG_MATRIX_EVENT),
        log_matrix_event,
        NULL);
