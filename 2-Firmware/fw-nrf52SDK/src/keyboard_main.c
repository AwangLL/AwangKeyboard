#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "app_error.h"
#include "app_timer.h"
#include "app_scheduler.h"
//#include "nrf_pwm.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log_ctrl.h"

#include "nrf_drv_clock.h"
#include "nrf_drv_power.h"

#include "nrf_gpio.h"
#include "keyboard_light.h"
#include "nrf_delay.h"

//#include "usb/usb_service.h"
#include "usb_custom_service.h"

#include "keyboard/keyboard_encoder.h"
#include "keyboard/keyboard_oled.h"

#include "keyboard/keyboard_evt.h"


#include "driver/ws2812/ws2812_api.h"
#include "keyboard/keyboard_timer.h"
#include "keyboard/keyboard_matrix.h"
#include "keyboard/keyboard_oled.h"

#include "usb/usbnew.h"
#include "ble_service.h"
#include "ble_hid_service.h"


#include "../config/config.h"
#include "log/kb_nrf_print.h"
#define DEAD_BEEF                           0xDEADBEEF                                 /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define SCHED_MAX_EVENT_DATA_SIZE           APP_TIMER_SCHED_EVENT_DATA_SIZE            /**< Maximum size of scheduler events. */
#ifdef SVCALL_AS_NORMAL_FUNCTION
#define SCHED_QUEUE_SIZE                    20                                         /**< Maximum number of events in the scheduler queue. More is needed in case of Serialization. */
#else
#define SCHED_QUEUE_SIZE                    10                                         /**< Maximum number of events in the scheduler queue. */
#endif


/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for the Event Scheduler initialization.
 */
static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module.
 */
static void timers_init(void)
{
    ret_code_t err_code;

    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}
//static void init_power_clock(void)
//{
//    ret_code_t err_code;
//    /* Initialize power and clock. */
//    err_code = nrfx_clock_init(NULL);
//    APP_ERROR_CHECK(err_code);

//     err_code = nrfx_power_init(NULL);
//     APP_ERROR_CHECK(err_code);

//    //nrf_drv_clock_hfclk_request(NULL);
//    nrf_drv_clock_lfclk_request(NULL);

//    //while( !(nrf_drv_clock_hfclk_is_running() && nrf_drv_clock_lfclk_is_running()) )
//    while( !nrf_drv_clock_lfclk_is_running() )
//    {
//	/* Just waiting. */
//    }
//}

/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    app_sched_execute();
    execute_event();
    app_sched_execute();

    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}

int main(void)
{
    //log_init();
    //kb_print("start");
    //init_power_clock_();
    //usb_service_init_();
    //while(1)
    //{
    //    //idle_state_handle();
    //}
    bool erase_bond = false ;
    //initialize.
    log_init();
    timers_init();
    power_management_init();
    scheduler_init();
    init_power_clock(); 

    keyboard_evt_trigger(KBD_EVT_INIT);
    keyboard_evt_trigger(KBD_EVT_START);

    for(;;)
    {
        nrf_delay_ms(100);
        idle_state_handle();
    }
}