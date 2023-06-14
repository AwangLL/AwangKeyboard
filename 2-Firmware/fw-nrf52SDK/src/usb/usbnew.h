#pragma once

#include <stdbool.h>
#include <stdint.h>
#define BASE_USB_HID_SPEC_VERSION 0x0101 /**< Version number of base USB HID Specification implemented by this application. */

#define INPUT_REPORT_COUNT 2
#define OUTPUT_REPORT_COUNT 1

#define INPUT_REPORT_LEN_KEYBOARD 15 /**< Maximum length of the Input Report characteristic. */
#define OUTPUT_REPORT_LEN_KEYBOARD 1 /**< Maximum length of Output Report. */
#define INPUT_REPORT_LEN_CONSUMER 2
#define INPUT_REPORT_LEN_CUSTOM 3
#define OUTPUT_REPORT_LEN_CUSTOM 3

#define INPUT_REPORT_KEYBOARD_ID 0x01 /**< Id of reference to Keyboard Input Report. */
#define INPUT_REPORT_CONSUMER_ID 0x02
#define INPUT_REPORT_CUSTOM_ID 0x03
#define OUTPUT_REPORT_KEYBOARD_ID 0x01 /**< Id of reference to Keyboard Output Report. */
#define OUTPUT_REPORT_CUSTOM_ID 0x03

#define INPUT_REPORT_KEYBOARD_INDEX 0
#define INPUT_REPORT_CONSUMER_INDEX 1
#define OUTPUT_REPORT_KEYBOARD_INDEX 0

void init_power_clock_(void);
void usb_service_init_(void);