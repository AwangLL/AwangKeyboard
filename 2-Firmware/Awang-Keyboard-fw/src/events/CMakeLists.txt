target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/keyboard_event.c
)

target_sources_ifdef(CONFIG_AWANG_MATRIX_ENABLE app
			PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/matrix_event.c)

target_sources_ifdef(CONFIG_AWANG_USB_ENABLE app
			PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hid_event.c)