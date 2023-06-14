#include "kb_nrf_print.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/**@brief Function for initializing the nrf log module.
 */
void log_init(void)
{
#ifdef LOG_PRINT_ENABLE
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    kb_error(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
#endif
}

