#include "keyboard_report.h"
#include <stdint.h>
#include "ble_service.h"
#include "ble_hid_service.h"
#include "usb_custom_service.h"
#include "keyboard_evt.h"

#include "nrf_delay.h"
#include "kb_nrf_print.h"

void keyboard_report_send(uint8_t report_id, uint8_t *data)
{
    if (usb_state() == USB_STATE_WORKING)
    {
        usb_hid_send(report_id, data);
    }
    else if (ble_state() == BLE_STATE_CONNECTED)
    {
        ble_hid_send(report_id, data);
    }

}
void keyboard_report_kbd(uint8_t *data)
{
    keyboard_report_send(INPUT_REPORT_KEYBOARD_ID, data);
}
void keyboard_report_consumer(uint8_t keycode)
{
    static uint8_t data[INPUT_REPORT_LEN_CONSUMER];
    keyboard_report_send(INPUT_REPORT_CONSUMER_ID, data);
}
void keyboard_report_custom(uint8_t *data)
{
    //keyboard_report_send(INPUT_REPORT_CUSTOM_ID_ID, data);
}