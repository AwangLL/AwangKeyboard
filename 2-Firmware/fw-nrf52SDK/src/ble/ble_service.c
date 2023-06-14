#include "ble_service.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "app_error.h"
#include "app_timer.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_dis.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_soc.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "nrf_delay.h"

#include "ble_hid_service.h"
#include "ble_bas_service.h"

#include "keyboard_evt.h"
#include "keyboard_paraments.h"

#include "config.h"
#include "kb_nrf_print.h"
#include "app_scheduler.h"

#define APP_BLE_OBSERVER_PRIO 3 /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define APP_BLE_CONN_CFG_TAG 1  /**< A tag identifying the SoftDevice BLE configuration. */

#define MIN_CONN_INTERVAL MSEC_TO_UNITS(7.5, UNIT_1_25_MS) /**< Minimum connection interval (7.5 ms) */
#define MAX_CONN_INTERVAL MSEC_TO_UNITS(30, UNIT_1_25_MS)  /**< Maximum connection 5 (30 ms). */
#define SLAVE_LATENCY 6                                    /**< Slave latency. */
#define CONN_SUP_TIMEOUT MSEC_TO_UNITS(430, UNIT_10_MS)    /**< Connection supervisory timeout (430 ms). */
//Supervision Timeout > （1 +slaveLatency）* （connectionInterval）
#define FIRST_CONN_PARAMS_UPDATE_DELAY APP_TIMER_TICKS(5000) /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY APP_TIMER_TICKS(30000) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT 3                       /**< Number of attempts before giving up the connection parameter negotiation. */

#define APP_ADV_FAST_INTERVAL 0x0028 /**< Fast advertising interval (in units of 0.625 ms. This value corresponds to 25 ms.). */
#define APP_ADV_SLOW_INTERVAL 0x0C80 /**< Slow advertising interval (in units of 0.625 ms. This value corrsponds to 2 seconds). */
#define APP_ADV_FAST_DURATION 3000   /**< The advertising duration of fast advertising in units of 10 milliseconds. */
#define APP_ADV_SLOW_DURATION 18000  /**< The advertising duration of slow advertising in units of 10 milliseconds. */

#define SEC_PARAM_BOND 1                               /**< Perform bonding. */
#define SEC_PARAM_MITM 0                               /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC 0                               /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS 0                           /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES BLE_GAP_IO_CAPS_NONE /**< No I/O capabilities. */
#define SEC_PARAM_OOB 0                                /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE 7                       /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE 16                      /**< Maximum encryption key size. */

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID; /**< Handle of the current connection. */
static pm_peer_id_t m_peer_id;                           /**< Device reference handle to the current bonded central. */
static ble_uuid_t m_adv_uuids[] = {{BLE_UUID_HUMAN_INTERFACE_DEVICE_SERVICE, BLE_UUID_TYPE_BLE}};

static uint8_t _ble_state = BLE_STATE_STOP;
static bool disconnect_command = false;

NRF_BLE_QWR_DEF(m_qwr);             /**< Context for the Queued Write module.*/
NRF_BLE_GATT_DEF(m_gatt);           /**< GATT module instance. */
BLE_ADVERTISING_DEF(m_advertising); /**< Advertising module instance. */

/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    kb_error(nrf_error);
}

/**@brief Function for initializing the Queued Write Module.
 */
void qwr_init(void)
{
    ret_code_t err_code;
    nrf_ble_qwr_init_t qwr_init_obj = {0};

    qwr_init_obj.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init_obj);
    kb_error(err_code);
}

/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context)
{
    ret_code_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
    case BLE_GAP_EVT_CONNECTED:
        m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
        err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
        kb_error(err_code);

        kb_print("ble connected");
        _ble_state = BLE_STATE_CONNECTED;
        keyboard_evt_trigger_param(KBD_EVT_BLE, BLE_STATE_CONNECTED);

        break;

    case BLE_GAP_EVT_DISCONNECTED:
        // Dequeue all keys without transmission.
        //buf_dequeue();
        m_conn_handle = BLE_CONN_HANDLE_INVALID;

        kb_print("ble disconnected");
        _ble_state = BLE_STATE_DISCONNECT;
        keyboard_evt_trigger_param(KBD_EVT_BLE, BLE_STATE_DISCONNECT);
        if(disconnect_command == true)
        {
            advertising_stop();
            disconnect_command = false;
        }
        break; // BLE_GAP_EVT_DISCONNECTED

    case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
    {
        kb_print("PHY update request.");
        ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
        err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
        kb_error(err_code);
    }
    break;

    case BLE_GATTS_EVT_HVN_TX_COMPLETE:
        // Send next key event
        //(void) buffer_dequeue(true);
        break;

    case BLE_GATTC_EVT_TIMEOUT:
        // Disconnect on GATT Client timeout event.
        kb_print("GATT Client Timeout.");
        err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                         BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        kb_error(err_code);
        break;

    case BLE_GATTS_EVT_TIMEOUT:
        // Disconnect on GATT Server timeout event.
        kb_print("GATT Server Timeout.");
        err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                         BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        kb_error(err_code);
        break;

    default:
        // No implementation needed.
        break;
    }
}

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    kb_error(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    kb_error(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    kb_error(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}

/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
void gap_params_init(void)
{
    ret_code_t err_code;
    ble_gap_conn_params_t gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)keyboard_name,
                                          strlen(keyboard_name));
    kb_error(err_code);

    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_HID_KEYBOARD);
    kb_error(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    kb_error(err_code);
}

/**@brief Function for initializing the GATT module.
 */
void gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    kb_error(err_code);
}

/**@brief Function for setting filtered device identities.
 *
 * @param[in] skip  Filter passed to @ref pm_peer_id_list.
 */
void identities_set(pm_peer_id_list_skip_t skip)
{
    pm_peer_id_t peer_ids[BLE_GAP_DEVICE_IDENTITIES_MAX_COUNT];
    uint32_t peer_id_count = BLE_GAP_DEVICE_IDENTITIES_MAX_COUNT;

    ret_code_t err_code = pm_peer_id_list(peer_ids, &peer_id_count, PM_PEER_ID_INVALID, skip);
    kb_error(err_code);

    err_code = pm_device_identities_list_set(peer_ids, peer_id_count);
    kb_error(err_code);
}

/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
void sleep_mode_enter(void)
{
    ret_code_t err_code;

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    kb_error(err_code);
}

/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    ret_code_t err_code;

    switch (ble_adv_evt)
    {
    case BLE_ADV_EVT_DIRECTED_HIGH_DUTY:
        kb_print("High Duty Directed advertising.");
        _ble_state = BLE_STATE_FAST_ADV;
        keyboard_evt_trigger_param(KBD_EVT_BLE, BLE_STATE_FAST_ADV);
        break;

    case BLE_ADV_EVT_DIRECTED:
        kb_print("Directed advertising.");
        _ble_state = BLE_STATE_SLOW_ADV;
        keyboard_evt_trigger_param(KBD_EVT_BLE, BLE_STATE_SLOW_ADV);
        break;

    case BLE_ADV_EVT_FAST:
        kb_print("Fast advertising.");
        _ble_state = BLE_STATE_FAST_ADV;
        keyboard_evt_trigger_param(KBD_EVT_BLE, BLE_STATE_FAST_ADV);
        break;

    case BLE_ADV_EVT_SLOW:
        kb_print("Slow advertising.");
        _ble_state = BLE_STATE_SLOW_ADV;
        keyboard_evt_trigger_param(KBD_EVT_BLE, BLE_STATE_SLOW_ADV);
        break;

    case BLE_ADV_EVT_FAST_WHITELIST:
        kb_print("Fast advertising with whitelist.");
        _ble_state = BLE_STATE_FAST_ADV;
        keyboard_evt_trigger_param(KBD_EVT_BLE, BLE_STATE_FAST_ADV);
        break;

    case BLE_ADV_EVT_SLOW_WHITELIST:
        kb_print("Slow advertising with whitelist.");
        _ble_state = BLE_STATE_SLOW_ADV;
        keyboard_evt_trigger_param(KBD_EVT_BLE, BLE_STATE_SLOW_ADV);
        break;

    case BLE_ADV_EVT_IDLE:
        //sleep_mode_enter();
        _ble_state = BLE_STATE_IDLE;
        keyboard_evt_trigger_param(KBD_EVT_BLE, BLE_STATE_IDLE);
        //keyboard_evt_trigger(KBD_EVT_SLEEP);
        break;

    case BLE_ADV_EVT_WHITELIST_REQUEST:
    {
        ble_gap_addr_t whitelist_addrs[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
        ble_gap_irk_t whitelist_irks[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
        uint32_t addr_cnt = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;
        uint32_t irk_cnt = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;

        err_code = pm_whitelist_get(whitelist_addrs, &addr_cnt,
                                    whitelist_irks, &irk_cnt);
        kb_error(err_code);
        kb_print("pm_whitelist_get returns %d addr in whitelist and %d irk whitelist",
                 addr_cnt, irk_cnt);

        // Set the correct identities list (no excluding peers with no Central Address Resolution).
        identities_set(PM_PEER_ID_LIST_SKIP_NO_IRK);

        // Apply the whitelist.
        err_code = ble_advertising_whitelist_reply(&m_advertising,
                                                   whitelist_addrs,
                                                   addr_cnt,
                                                   whitelist_irks,
                                                   irk_cnt);
        kb_error(err_code);
    }
    break; //BLE_ADV_EVT_WHITELIST_REQUEST

    case BLE_ADV_EVT_PEER_ADDR_REQUEST:
    {
        pm_peer_data_bonding_t peer_bonding_data;

        // Only Give peer address if we have a handle to the bonded peer.
        if (m_peer_id != PM_PEER_ID_INVALID)
        {
            err_code = pm_peer_data_bonding_load(m_peer_id, &peer_bonding_data);
            if (err_code != NRF_ERROR_NOT_FOUND)
            {
                kb_error(err_code);

                // Manipulate identities to exclude peers with no Central Address Resolution.
                identities_set(PM_PEER_ID_LIST_SKIP_ALL);

                ble_gap_addr_t *p_peer_addr = &(peer_bonding_data.peer_ble_id.id_addr_info);
                err_code = ble_advertising_peer_addr_reply(&m_advertising, p_peer_addr);
                kb_error(err_code);
            }
        }
    }
    break; //BLE_ADV_EVT_PEER_ADDR_REQUEST

    default:
        break;
    }
}

/**@brief Function for handling advertising errors.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void ble_advertising_error_handler(uint32_t nrf_error)
{
    kb_error(nrf_error);
}

/**@brief Function for initializing the Advertising functionality.
 */
void advertising_init(void)
{
    uint32_t err_code;
    uint8_t adv_flags;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    adv_flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    init.advdata.name_type = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance = true;
    init.advdata.flags = adv_flags;
    init.advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.advdata.uuids_complete.p_uuids = m_adv_uuids;

    init.config.ble_adv_whitelist_enabled = true;
    init.config.ble_adv_directed_high_duty_enabled = true;
    init.config.ble_adv_directed_enabled = false;
    init.config.ble_adv_directed_interval = 0;
    init.config.ble_adv_directed_timeout = 0;
    init.config.ble_adv_fast_enabled = true;
    init.config.ble_adv_fast_interval = APP_ADV_FAST_INTERVAL;
    init.config.ble_adv_fast_timeout = APP_ADV_FAST_DURATION;
    init.config.ble_adv_slow_enabled = true;
    init.config.ble_adv_slow_interval = APP_ADV_SLOW_INTERVAL;
    init.config.ble_adv_slow_timeout = APP_ADV_SLOW_DURATION;

    init.evt_handler = on_adv_evt;
    init.error_handler = ble_advertising_error_handler;

    err_code = ble_advertising_init(&m_advertising, &init);
    kb_error(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    kb_error(nrf_error);
}

/**@brief Function for initializing the Connection Parameters module.
 */
void conn_params_init(void)
{
    ret_code_t err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail = false;
    cp_init.evt_handler = NULL;
    cp_init.error_handler = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    kb_error(err_code);
}

/**@brief Function for setting filtered whitelist.
 *
 * @param[in] skip  Filter passed to @ref pm_peer_id_list.
 */
void whitelist_set(pm_peer_id_list_skip_t skip)
{
    pm_peer_id_t peer_ids[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
    uint32_t peer_id_count = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;

    ret_code_t err_code = pm_peer_id_list(peer_ids, &peer_id_count, PM_PEER_ID_INVALID, skip);
    kb_error(err_code);

    kb_print("\tm_whitelist_peer_cnt %d, MAX_PEERS_WLIST %d",
             peer_id_count + 1,
             BLE_GAP_WHITELIST_ADDR_MAX_COUNT);

    err_code = pm_whitelist_set(peer_ids, peer_id_count);
    kb_error(err_code);
}

/**@brief Clear bond information from persistent storage.
 */
void delete_bonds(void)
{
    ret_code_t err_code;

    kb_print("Erase bonds!");

    err_code = pm_peers_delete();
    kb_error(err_code);
}

/**@brief Function for starting advertising.
 */
void advertising_start(bool erase_bonds)
{
    if(_ble_state != BLE_STATE_STOP) return;
    if (erase_bonds == true)
    {
        delete_bonds();
        // Advertising is started by PM_EVT_PEERS_DELETE_SUCCEEDED event.
    }
    else
    {
        whitelist_set(PM_PEER_ID_LIST_SKIP_NO_ID_ADDR);

        ret_code_t ret = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
        kb_error(ret);
    }
}

void advertising_stop(void)
{
    if(_ble_state == BLE_STATE_STOP) return;
    ret_code_t err_code = NRF_ERROR_INVALID_STATE;
    err_code = sd_ble_gap_adv_stop(m_advertising.adv_handle);
    if(err_code == NRF_SUCCESS)
    {
        keyboard_evt_trigger_param(KBD_EVT_BLE, BLE_STATE_STOP);
        _ble_state = BLE_STATE_STOP;
    }
}

/**@brief Function for starting advertising.
 */
void ble_disconnect()
{
    ret_code_t err_code;
    //if (_ble_state != BLE_STATE_DISCONNECT && _ble_state != BLE_STATE_STOP)
    if(_ble_state == BLE_STATE_CONNECTED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        kb_error(err_code);
        disconnect_command = true;
    }
    else if(_ble_state != BLE_STATE_CONNECTED)
    {
        advertising_stop();
    }
}


/**@brief Function for handling Peer Manager events.
 *
 * @param[in] p_evt  Peer Manager event.
 */
static void pm_evt_handler(pm_evt_t const *p_evt)
{
    pm_handler_on_pm_evt(p_evt);
    pm_handler_flash_clean(p_evt);

    switch (p_evt->evt_id)
    {
    case PM_EVT_CONN_SEC_SUCCEEDED:
        m_peer_id = p_evt->peer_id;
        break;
    case PM_EVT_PEERS_DELETE_SUCCEEDED:
        advertising_start(false);
        break;
    case PM_EVT_PEER_DATA_UPDATE_SUCCEEDED:
        if (p_evt->params.peer_data_update_succeeded.flash_changed && (p_evt->params.peer_data_update_succeeded.data_id == PM_PEER_DATA_ID_BONDING))
        {
            kb_print("New Bond, add the peer to the whitelist if possible");
            // Note: You should check on what kind of white list policy your application should use.

            whitelist_set(PM_PEER_ID_LIST_SKIP_NO_ID_ADDR);
        }
        break;
    default:
        break;
    }
}

/**@brief Function for the Peer Manager initialization.
 */
void peer_manager_init(void)
{
    ble_gap_sec_params_t sec_param;
    ret_code_t err_code;

    err_code = pm_init();
    kb_error(err_code);

    memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

    // Security parameters to be used for all security procedures.
    sec_param.bond = SEC_PARAM_BOND;
    sec_param.mitm = SEC_PARAM_MITM;
    sec_param.lesc = SEC_PARAM_LESC;
    sec_param.keypress = SEC_PARAM_KEYPRESS;
    sec_param.io_caps = SEC_PARAM_IO_CAPABILITIES;
    sec_param.oob = SEC_PARAM_OOB;
    sec_param.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
    sec_param.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
    sec_param.kdist_own.enc = 1;
    sec_param.kdist_own.id = 1;
    sec_param.kdist_peer.enc = 1;
    sec_param.kdist_peer.id = 1;

    err_code = pm_sec_params_set(&sec_param);
    kb_error(err_code);

    err_code = pm_register(pm_evt_handler);
    kb_error(err_code);
}

/**@brief Function for initializing Device Information Service.
 */
void dis_init(void)
{
    ret_code_t err_code;
    ble_dis_init_t dis_init_obj;
    ble_dis_pnp_id_t pnp_id;

    pnp_id.vendor_id_source = PNP_ID_VENDOR_ID_SOURCE;
    pnp_id.vendor_id = PNP_ID_VENDOR_ID;
    pnp_id.product_id = PNP_ID_PRODUCT_ID;
    pnp_id.product_version = PNP_ID_PRODUCT_VERSION;

    memset(&dis_init_obj, 0, sizeof(dis_init_obj));

    ble_srv_ascii_to_utf8(&dis_init_obj.manufact_name_str, keyboard_manufacture);
    dis_init_obj.p_pnp_id = &pnp_id;

    dis_init_obj.dis_char_rd_sec = SEC_JUST_WORKS;

    err_code = ble_dis_init(&dis_init_obj);
    kb_error(err_code);
}

uint16_t get_m_conn_handle(void)
{
    return m_conn_handle;
}
uint8_t ble_state(void)
{
    return _ble_state;
}

void ble_service_init(void)
{
    ble_stack_init();

    gap_params_init();
    advertising_init();
    conn_params_init();

    peer_manager_init();

    gatt_init();
    qwr_init();
    dis_init();

    ble_hid_init();

    bas_init();
}

/* Event Hanlder*/
static void keyboard_ble_evt_hanlder(enum kbd_evt_list event, void *arg)
{
    switch (event)
    {
    case KBD_EVT_INIT:
        ble_service_init();
        kb_print("ble module initialized.");
        break;
    case KBD_EVT_START:
        advertising_start(false);
        kb_print("ble module started.");
        break;
    case KBD_EVT_BLE:
        break;
     case KBD_EVT_SLEEP:
         ble_disconnect();
         kb_print("ble module stop.");
         break;
    default:
        break;
    }
}
EVENT_HANDLER(keyboard_ble_evt_hanlder);