#include "ble_bas_service.h"
#include <string.h>
#include "ble_bas.h"
#include "ble_service.h"
#include "keyboard/keyboard_evt.h"
#include "keyboard/keyboard_battery.h"

#include "config.h"
#include "log/kb_nrf_print.h"

BLE_BAS_DEF(m_bas); /**< Structure used to identify the battery service. */

/**@brief Function for initializing Battery Service.
 */
void bas_init(void)
{
    ret_code_t err_code;
    ble_bas_init_t bas_init_obj;

    memset(&bas_init_obj, 0, sizeof(bas_init_obj));

    bas_init_obj.evt_handler = NULL;
    bas_init_obj.support_notification = true;
    bas_init_obj.p_report_ref = NULL;
    bas_init_obj.initial_batt_level = 100;

    bas_init_obj.bl_rd_sec = SEC_JUST_WORKS;
    bas_init_obj.bl_cccd_wr_sec = SEC_JUST_WORKS;
    bas_init_obj.bl_report_rd_sec = SEC_JUST_WORKS;

    err_code = ble_bas_init(&m_bas, &bas_init_obj);
    kb_error(err_code);

    kb_print("BLE battery service init.");
}

/**@brief Function for performing a battery measurement, and update the Battery Level characteristic in the Battery Service.
 */
static void battery_level_update(uint8_t percentage)
{
    ret_code_t err_code;

    if(ble_state() != BLE_STATE_CONNECTED) return;

    err_code = ble_bas_battery_level_update(&m_bas, percentage, BLE_CONN_HANDLE_ALL);
    if ((err_code != NRF_SUCCESS) &&
        (err_code != NRF_ERROR_BUSY) &&
        (err_code != NRF_ERROR_RESOURCES) &&
        (err_code != NRF_ERROR_FORBIDDEN) &&
        (err_code != NRF_ERROR_INVALID_STATE) &&
        (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING))
    {
        kb_error(err_code);
    }
}

static void bat_service_evt_hanlder(enum kbd_evt_list event, void *arg)
{
    switch (event)
    {
    case KBD_EVT_BATTUPDATE:
        battery_level_update(battery_info.percentage);
        break;
    default:
        break;
    }
}
EVENT_HANDLER(bat_service_evt_hanlder);