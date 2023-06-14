#include "ble_hid_service.h"
#include <stdbool.h>
#include <string.h>
#include "ble_hids.h"

#include "ble_service.h"
#include "keyboard_report.h"
#include "keyboard_paraments.h"

#include "kb_nrf_print.h"

BLE_HIDS_DEF(m_hids, /**< Structure used to identify the HID service. */
             NRF_SDH_BLE_TOTAL_LINK_COUNT,
             INPUT_REPORT_LEN_KEYBOARD,
             INPUT_REPORT_LEN_CONSUMER,
             OUTPUT_REPORT_LEN_KEYBOARD);

static bool m_in_boot_mode = false; /**< Current protocol mode. */
static uint8_t keyboard_led_val_ble;

static void on_hid_rep_char_write(ble_hids_evt_t *p_evt);

/**@brief Function for handling HID events.
 *
 * @details This function will be called for all HID events which are passed to the application.
 *
 * @param[in]   p_hids  HID service structure.
 * @param[in]   p_evt   Event received from the HID service.
 */
static void on_hids_evt(ble_hids_t *p_hids, ble_hids_evt_t *p_evt)
{
    switch (p_evt->evt_type)
    {
    case BLE_HIDS_EVT_BOOT_MODE_ENTERED:
        m_in_boot_mode = true;
        break;

    case BLE_HIDS_EVT_REPORT_MODE_ENTERED:
        m_in_boot_mode = false;
        break;

    case BLE_HIDS_EVT_REP_CHAR_WRITE:
        on_hid_rep_char_write(p_evt);
        break;

    case BLE_HIDS_EVT_NOTIF_ENABLED:
        break;

    default:
        // No implementation needed.
        break;
    }
}

/**@brief Function for handling Service errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void service_error_handler(uint32_t nrf_error)
{
    kb_error(nrf_error);
}

/**@brief Function for initializing HID Service.
 */
void ble_hid_init(void)
{
    ret_code_t err_code;
    ble_hids_init_t hids_init_obj;
    ble_hids_inp_rep_init_t *p_input_report;
    ble_hids_outp_rep_init_t *p_output_report;
    uint8_t hid_info_flags;

    static ble_hids_inp_rep_init_t input_report_array[INPUT_REPORT_COUNT];
    static ble_hids_outp_rep_init_t output_report_array[OUTPUT_REPORT_COUNT];
    static uint8_t report_map_data[] = {
        0x05, 0x01, // Usage Page (Generic Desktop)
        0x09, 0x06, // Usage (Keyboard)
        0xA1, 0x01, // Collection (Application)
        0x85, INPUT_REPORT_KEYBOARD_ID, // Report ID (1)

        0x95, 0x08, // Report Count (8)
        0x75, 0x01, // Report Size (1)
        0x05, 0x07, // Usage Page (Key Codes)
        0x19, 0xe0, // Usage Minimum (224)
        0x29, 0xe7, // Usage Maximum (231)
        0x15, 0x00, // Logical Minimum (0)
        0x25, 0x01, // Logical Maximum (1)
        0x81, 0x02, // Input (Data, Variable, Absolute)

        0x95, 0x01, // Report Count (1)
        0x75, 0x08, // Report Size (8)
        0x81, 0x03, // Input (Constant, Variable, Absolute) reserved byte(1)

        0x95, 0x66, // Report Count (102)
        0x75, 0x01, // Report Size (1)
        0x19, 0x00, // Usage Minimum (0)
        0x29, 0x65, // Usage Maximum (101)
        0x15, 0x00, // Logical Minimum (0)
        0x25, 0x01, // Logical Maximum (1)
        0x81, 0x02, // Input (Data, Variable, Absolute)
        0x95, 0x01, // Report Count (2)
        0x75, 0x02, // Report Size (1)
        0x81, 0x03, // Input (Constant, Variable, Absolute) reserved byte(1)

        0x95, 0x05, // Report Count (5)
        0x75, 0x01, // Report Size (1)
        0x15, 0x00, // Logical Minimum (0)
        0x25, 0x01, // Logical Maximum (1)
        0x05, 0x08, // Usage Page (Page# for LEDs)
        0x19, 0x01, // Usage Minimum (1) (Num Lock)
        0x29, 0x05, // Usage Maximum (5) (Kana)
        0x91, 0x02, // Output (Data, Variable, Absolute), Led report
        0x95, 0x01, // Report Count (1)
        0x75, 0x03, // Report Size (3)
        0x91, 0x03, // Output (Const, Variable, Absolute), Led report padding

        0xC0, // End Collection (Application)

        0x05, 0x0C,                     // Usage Page (Consumer Devices)
        0x09, 0x01,                     // Usage (Consumer Control)
        0xA1, 0x01,                     // Collection (Application)
        0x85, INPUT_REPORT_CONSUMER_ID, //   Report ID (2)
        0x95, 0x01,                     // Report Count (1)
        0x75, 0x10,                     // Report Size (16)
        0x19, 0x01,                     // Usage Minimum (0x01)
        0x2A, 0x9C, 0x02,               // Usage Maximum (0x29c)
        0x15, 0x01,                     // Logical Minimum (0x01)
        0x26, 0x9C, 0x02,               // Logical Maximum (0x29c)
        0x81, 0x00,                     // Input (Data, Array) Key array(2 bytes)
        0xC0                            // End Collection (Application)
        };

    memset((void *)input_report_array, 0, sizeof(ble_hids_inp_rep_init_t));
    memset((void *)output_report_array, 0, sizeof(ble_hids_outp_rep_init_t));

    // Initialize HID Service
    p_input_report = &input_report_array[INPUT_REPORT_KEYBOARD_INDEX];
    p_input_report->max_len = INPUT_REPORT_LEN_KEYBOARD;
    p_input_report->rep_ref.report_id = INPUT_REPORT_KEYBOARD_ID;
    p_input_report->rep_ref.report_type = BLE_HIDS_REP_TYPE_INPUT;

    p_input_report->sec.cccd_wr = SEC_JUST_WORKS;
    p_input_report->sec.wr = SEC_JUST_WORKS;
    p_input_report->sec.rd = SEC_JUST_WORKS;

    p_input_report = &input_report_array[INPUT_REPORT_CONSUMER_INDEX];
    p_input_report->max_len = INPUT_REPORT_LEN_CONSUMER;
    p_input_report->rep_ref.report_id = INPUT_REPORT_CONSUMER_ID;
    p_input_report->rep_ref.report_type = BLE_HIDS_REP_TYPE_INPUT;

    p_input_report->sec.cccd_wr = SEC_JUST_WORKS;
    p_input_report->sec.wr = SEC_JUST_WORKS;
    p_input_report->sec.rd = SEC_JUST_WORKS;

    p_output_report = &output_report_array[OUTPUT_REPORT_KEYBOARD_INDEX];
    p_output_report->max_len = OUTPUT_REPORT_LEN_KEYBOARD;
    p_output_report->rep_ref.report_id = OUTPUT_REPORT_KEYBOARD_ID;
    p_output_report->rep_ref.report_type = BLE_HIDS_REP_TYPE_OUTPUT;

    p_output_report->sec.wr = SEC_JUST_WORKS;
    p_output_report->sec.rd = SEC_JUST_WORKS;
    memset(&hids_init_obj, 0, sizeof(hids_init_obj));

    hids_init_obj.evt_handler = on_hids_evt;
    hids_init_obj.error_handler = service_error_handler;
    hids_init_obj.is_kb = true;
    hids_init_obj.is_mouse = false;
    hids_init_obj.inp_rep_count = INPUT_REPORT_COUNT;
    hids_init_obj.p_inp_rep_array = input_report_array;
    hids_init_obj.outp_rep_count = OUTPUT_REPORT_COUNT;
    hids_init_obj.p_outp_rep_array = output_report_array;
    hids_init_obj.feature_rep_count = 0;
    hids_init_obj.p_feature_rep_array = NULL;
    hids_init_obj.rep_map.data_len = sizeof(report_map_data);
    hids_init_obj.rep_map.p_data = report_map_data;
    hids_init_obj.hid_information.bcd_hid = BASE_USB_HID_SPEC_VERSION;
    hids_init_obj.hid_information.b_country_code = 0;
    hids_init_obj.hid_information.flags = HID_INFO_FLAG_REMOTE_WAKE_MSK | HID_INFO_FLAG_NORMALLY_CONNECTABLE_MSK;
    hids_init_obj.included_services_count = 0;
    hids_init_obj.p_included_services_array = NULL;

    hids_init_obj.rep_map.rd_sec = SEC_JUST_WORKS;
    hids_init_obj.hid_information.rd_sec = SEC_JUST_WORKS;

    hids_init_obj.boot_kb_inp_rep_sec.cccd_wr = SEC_JUST_WORKS;
    hids_init_obj.boot_kb_inp_rep_sec.rd = SEC_JUST_WORKS;

    hids_init_obj.boot_kb_outp_rep_sec.rd = SEC_JUST_WORKS;
    hids_init_obj.boot_kb_outp_rep_sec.wr = SEC_JUST_WORKS;

    hids_init_obj.protocol_mode_rd_sec = SEC_JUST_WORKS;
    hids_init_obj.protocol_mode_wr_sec = SEC_JUST_WORKS;
    hids_init_obj.ctrl_point_wr_sec = SEC_JUST_WORKS;

    err_code = ble_hids_init(&m_hids, &hids_init_obj);
    kb_error(err_code);
}

/**@brief Function for handling the HID Report Characteristic Write event.
 *
 * @param[in]   p_evt   HID service event.
 */
static void on_hid_rep_char_write(ble_hids_evt_t *p_evt)
{
    if (p_evt->params.char_write.char_id.rep_type == BLE_HIDS_REP_TYPE_OUTPUT)
    {
        ret_code_t err_code;
        uint8_t report_val;
        uint8_t report_index = p_evt->params.char_write.char_id.rep_index;

        if (report_index == OUTPUT_REPORT_KEYBOARD_INDEX)
        {
            err_code = ble_hids_outp_rep_get(&m_hids,
                                             report_index,
                                             OUTPUT_REPORT_LEN_KEYBOARD,
                                             0,
                                             get_m_conn_handle(),
                                             &report_val);
            if (err_code == NRF_SUCCESS)
                keyboard_led_val_ble = report_val;
            kb_error(err_code);
        }
    }
}

void ble_hid_send(uint8_t report_id, uint8_t *data)
{
    ret_code_t err_code = NRF_SUCCESS;
    if (m_in_boot_mode)
    {
        if (report_id = INPUT_REPORT_KEYBOARD_ID)
        {
            err_code = ble_hids_boot_kb_inp_rep_send(&m_hids,
                                                     INPUT_REPORT_LEN_KEYBOARD,
                                                     data,
                                                     get_m_conn_handle());
        }
    }
    else
    {
        switch (report_id)
        {
        case INPUT_REPORT_KEYBOARD_ID:

            err_code = ble_hids_inp_rep_send(&m_hids,
                                             INPUT_REPORT_KEYBOARD_INDEX,
                                             INPUT_REPORT_LEN_KEYBOARD,
                                             data,
                                             get_m_conn_handle());
            break;
        case INPUT_REPORT_CONSUMER_ID:
            err_code = ble_hids_inp_rep_send(&m_hids,
                                             INPUT_REPORT_CONSUMER_INDEX,
                                             INPUT_REPORT_LEN_CONSUMER,
                                             data,
                                             get_m_conn_handle());
            break;
        default:
            break;
        }
    }
    if ((err_code != NRF_SUCCESS) &&
        (err_code != NRF_ERROR_INVALID_STATE) &&
        (err_code != NRF_ERROR_FORBIDDEN) &&
        (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING))
    {
        kb_error(err_code);
    }
}
