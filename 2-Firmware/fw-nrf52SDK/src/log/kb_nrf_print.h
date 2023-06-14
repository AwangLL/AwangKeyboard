#pragma once

#include "nrf_log.h"
#include "app_error.h"

#ifdef LOG_PRINT_ENABLE

#define kb_print(...) NRF_LOG_INFO(__VA_ARGS__)
#define kb_dprint(...) NRF_LOG_DEBUG(__VA_ARGS__)

#define kb_error(err_code) APP_ERROR_CHECK(err_code)

#else

#define kb_print(...) ((void)0)
#define kb_dprint(...) ((void)0)

#define kb_error(err_code) ((void)0)

#endif
void log_init(void);