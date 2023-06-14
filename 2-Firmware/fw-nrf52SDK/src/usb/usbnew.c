#include "usbnew.h"
#include <stdint.h>
#include "nrfx_clock.h"
#include "nrfx_power.h"
#include "nrfx_usbd.h"
#include "nrf_drv_usbd.h"
#include "nrf_drv_power.h"
#include "string.h"
#include "../log/kb_nrf_print.h"

// #include "config.h"
/**
 * @brief Enable power USB detection
 *
 * Configure if example supports USB port connection
 */
#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif

static bool m_send_flag = 0;
// static uint8_t _usb_state = USB_STATE_NOT_CONNECT;
static uint8_t rxBuf[1 + 3]; // report id (1 byte) + dataType/ledState(1 byte) + dataValue(2 byte)

static nrfx_usbd_transfer_t transfer_out1 = {
    .p_data = {.rx = rxBuf},
    .size = 1 + 3,
    .flags = 0};

/**
 * @brief USB configured flag
 *
 * The flag that is used to mark the fact that USB is configured and ready
 * to transmit data
 */
static volatile bool m_usbd_configured = false;

/**
 * @brief USB suspended
 *
 * The flag that is used to mark the fact that USB is suspended and requires wake up
 * if new data is available.
 *
 * @note This variable is changed from the main loop.
 */
static bool m_usbd_suspended = false;

/**
 * @brief Mark the fact if remote wake up is enabled
 *
 * The internal flag that marks if host enabled the remote wake up functionality in this device.
 */
static
#if REMOTE_WU
    volatile // Disallow optimization only if Remote wakeup is enabled
#endif
    bool m_usbd_rwu_enabled = false;

/**
 * @brief System OFF request flag
 *
 * This flag is used in button event processing and marks the fact that
 * system OFF should be activated from main loop.
 */
static volatile bool m_system_off_req = false;

/**
 * @brief The requested suspend state
 *
 * The currently requested suspend state based on the events
 * received from USBD library.
 * If the value here is different than the @ref m_usbd_suspended
 * the state changing would be processed inside main loop.
 */
static volatile bool m_usbd_suspend_state_req = false;

/** Device descriptor **/
#define EP0_MAXPACKETSIZE NRFX_USBD_EPSIZE // Maximum size of the packed transfered by EP0
#define USBD_STRING_LANG_IX 0x00
#define USBD_STRING_MANUFACTURER_IX 0x01
#define USBD_STRING_PRODUCT_IX 0x02
#define USBD_STRING_SERIAL_IX 0x00
static const uint8_t usb_device_descriptor[] = {
    0x12,                        // bLength | size of descriptor
    0x01,                        // bDescriptorType | descriptor type
    0x00, 0x02,                  // bcdUSB | USB spec release (ver 2.0)
    0x00,                        // bDeviceClass ¦ class code (each interface specifies class information)
    0x00,                        // bDeviceSubClass ¦ device sub-class (must be set to 0 because class code is 0)
    0x00,                        // bDeviceProtocol | device protocol (no class specific protocol)
    EP0_MAXPACKETSIZE,           // bMaxPacketSize0 | maximum packet size (64 bytes)
    0x15, 0x19,                  // vendor ID
    0xEE, 0xEE,                  // product ID
    0x00, 0x01,                  // bcdDevice | final device release number in BCD Format
    USBD_STRING_MANUFACTURER_IX, // iManufacturer | index of manufacturer string
    USBD_STRING_PRODUCT_IX,      // iProduct | index of product string
    USBD_STRING_SERIAL_IX,       // iSerialNumber | Serial Number string
    0x01                         // bNumConfigurations | number of configurations
};

/** Report descriptor **/
static const uint8_t kbd_report_descriptor[] = {
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x06, // Usage (Keyboard)
    0xA1, 0x01, // Collection (Application)
    0x85, 1, // Report ID (1)

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

    0x05, 0x0C, // Usage Page (Consumer Devices)
    0x09, 0x01, // Usage (Consumer Control)
    0xA1, 0x01, // Collection (Application)
    0x85, 2, //   Report ID (2)

    0x95, 0x01,       // Report Count (1)
    0x75, 0x10,       // Report Size (16)
    0x19, 0x01,       // Usage Minimum (0x01)
    0x2A, 0x9C, 0x02, // Usage Maximum (0x29c)
    0x15, 0x01,       // Logical Minimum (0x01)
    0x26, 0x9C, 0x02, // Logical Maximum (0x29c)
    0x81, 0x00,       // Input (Data, Array) Key array(2 bytes)
    0xC0,             // End Collection (Application)

    0x09, 0x00, // Usage (0)
    0xA1, 0x01, // Collection (Application)
    0x85, 3, // Report ID (3)

    0x95, 0x03, // Report Count (3)
    0x75, 0x08, // Report Size (8)
    0x19, 0x01, // Usage Minimum (1)
    0x29, 0x08, // Usage Maximum (8)
    0x15, 0x00, // Logical Minimum (0)
    0x25, 0xFF, // Logical Maximum (255)
    0x81, 0x02, // Input (Data, Variable, Absolute)
    0x91, 0x03, // Output (Data, Variable, Absolute)
    0xC0
};

/** Configuration descriptor **/
#define DEVICE_SELF_POWERED 1
#define REMOTE_WU 1
#define USBD_CONFIG_DESCRIPTOR_SIZE 9
static const uint8_t usb_configuration_descriptor[9 + 9 + 9 + 7 + 7] = {
    /* configuration descriptor */
    0x09,                                                 // bLength | length of descriptor
    0x02,                                                 // bDescriptorType | descriptor type (CONFIGURATION)
    sizeof(usb_configuration_descriptor) & 0xff,          // wTotalLength | total length of descriptor(s) low
    (sizeof(usb_configuration_descriptor) >> 8) & 0xff,   // wTotalLength | total length of descriptor(s) high
    0x01,                                                 // bNumInterfaces
    0x01,                                                 // bConfigurationValue
    0x00,                                                 // index of string Configuration | configuration string index (not supported)
    0x80 | (DEVICE_SELF_POWERED << 6) | (REMOTE_WU << 5), // bmAttributes
    0xFA,                                                 // maximum power in steps of 2mA (500mA)
    /* interface descriptor */
    0x09, // bLength
    0x04, // bDescriptorType | descriptor type (INTERFACE)
    0x00, // bInterfaceNumber
    0x00, // bAlternateSetting
    0x02, // bNumEndpoints | number of endpoints (2)
    0x03, // bInterfaceClass | interface class (3..defined by USB spec: HID)
    0x00, // bInterfaceSubClass |interface sub-class (0.. no boot interface)
    0x00, // bInterfaceProtocol | interface protocol (0.. none) 
    0x00, // interface string index (not supported)
    /* hid descriptor */
    0x09,                                        // bLength | length of descriptor (9 bytes)
    0x21,                                        // bHIDDescriptor | descriptor type (HID)
    0x11, 0x01,                                  // HID wBcdHID | Spec version 01.11
    0x00,                                        // bCountryCode | HW Target country
    0x01,                                        // bNumDescriptors | Number of HID class descriptors to follow
    0x22,                                        // bDescriptorType | Report descriptor type is 0x22 (report)
    sizeof(kbd_report_descriptor) & 0xff,        // Total length of Report descr., low byte
    (sizeof(kbd_report_descriptor) >> 8) & 0xff, // Total length of Report descr., high byte
    /* endpoint descriptor (in)*/
    0x07,       // bLength | length of descriptor (7 bytes)
    0x05,       // bDescriptorType | descriptor type (ENDPOINT)
    0x81,       // bEndpointAddress | endpoint address (IN endpoint, endpoint 1)
    0x03,       // bmAttributes | endpoint attributes (interrupt)
    0x10, 0x00, // bMaxPacketSizeLowByte,bMaxPacketSizeHighByte | maximum packet size (16 bytes)
    0x0A,       // bInterval | polling interval (10ms)
    /* endpoint descriptor (out)*/
    0x07,       // bLength | length of descriptor (7 bytes)
    0x05,       // bDescriptorType | descriptor type (ENDPOINT)
    0x01,       // bEndpointAddress | endpoint address (OUT endpoint, endpoint 1)
    0x03,       // bmAttributes | endpoint attributes (interrupt)
    0x10, 0x00, // bMaxPacketSizeLowByte,bMaxPacketSizeHighByte | maximum packet size (16 bytes)
    0x0A,       // bInterval | polling interval (10ms)
};
static const uint8_t *usb_interface0_descriptor = &usb_configuration_descriptor[9];
static const uint8_t *usb_hid0_descriptor = &usb_configuration_descriptor[9 + 9];
static const uint8_t *usb_endpoint1_in_descriptor = &usb_configuration_descriptor[9 + 9 + 9];
static const uint8_t *usb_endpoint1_out_descriptor = &usb_configuration_descriptor[9 + 9 + 9 + 7];

/** String descriptor **/
const static uint8_t usb_string_lang[] = {
    0x04, // length of descriptor
    0x03, // descriptor type
    0x09,
    0x04 // Supported LangID = 0x0409 (US-English) */
};
const static uint8_t usb_string_manufacturer[] = {
    14,        // length of descriptor (? bytes)
    0x03,      // descriptor type
    'N', 0x00, // Define Unicode String "Nordic"
    'o', 0x00, //
    'r', 0x00, //
    'd', 0x00, //
    'i', 0x00, //
    'c', 0x00, //
};
const static uint8_t usb_string_product[] = {
    30,        // length of descriptor (? bytes)
    0x03,      // descriptor type
    'A', 0x00, // generic unicode string for all devices : A
    'w', 0x00, // w
    'a', 0x00, // a
    'n', 0x00, // n
    'g', 0x00, // g
    ' ', 0x00, //
    'k', 0x00, // k
    'e', 0x00, // e
    'y', 0x00, // y
    'b', 0x00, // b
    'o', 0x00, // o
    'a', 0x00, // a
    'r', 0x00, // r
    'd', 0x00, // d
};

static const uint8_t get_config_resp_configured[] = {1};
static const uint8_t get_config_resp_unconfigured[] = {0};

static const uint8_t get_status_device_resp_nrwu[] = {
    ((DEVICE_SELF_POWERED) ? 1 : 0), // LSB first: self-powered, no remoteWk
    0};
static const uint8_t get_status_device_resp_rwu[] = {
    ((DEVICE_SELF_POWERED) ? 1 : 0) | 2, // LSB first: self-powered, remoteWk
    0};

static const uint8_t get_status_interface_resp[] = {0, 0};
static const uint8_t get_status_ep_halted_resp[] = {1, 0};
static const uint8_t get_status_ep_active_resp[] = {0, 0};

/**
 * @brief Setup all the endpoints for selected configuration
 *
 * Function sets all the endpoints for specific configuration.
 *
 * @note
 * Setting the configuration index 0 means technically disabling the HID interface.
 * Such configuration should be set when device is starting or USB reset is detected.
 *
 * @param index Configuration index
 *
 * @retval NRF_ERROR_INVALID_PARAM Invalid configuration
 * @retval NRFX_SUCCESS             Configuration successfully set
 */
static int ep_configuration(uint8_t index)
{
    kb_print("index = %d", index);
    if (index == 1)
    {
        nrfx_usbd_ep_dtoggle_clear(NRFX_USBD_EPIN1);
        nrfx_usbd_ep_stall_clear(NRFX_USBD_EPIN1);
        nrfx_usbd_ep_dtoggle_clear(NRFX_USBD_EPOUT1);
        nrfx_usbd_ep_dtoggle_clear(NRFX_USBD_EPOUT1);
        nrfx_usbd_ep_enable(NRFX_USBD_EPIN1);
        nrfx_usbd_ep_enable(NRFX_USBD_EPOUT1);

        kb_print("m_usbd_configured = true;");
        m_usbd_configured = true;
        nrfx_usbd_setup_clear();

        // ble_disconnect();
        // _usb_state = USB_STATE_WORKING;
        // keyboard_evt_trigger_param(KBD_EVT_USB, USB_STATE_WORKING);
    }
    else if (index == 0)
    {
        nrfx_usbd_ep_disable(NRFX_USBD_EPIN1);
        m_usbd_configured = false;
        nrfx_usbd_setup_clear();
    }
    else
    {
        // return NRFX_CHECK_INVALID_PARAM;
    }
    return NRFX_SUCCESS;
}

/**
 * @name Processing setup requests
 *
 * @{
 */
/**
 * @brief Respond on ep 0
 *
 * Auxiliary function for sending respond on endpoint 0
 * @param[in] p_setup Pointer to setup data from current setup request.
 *                    It would be used to calculate the size of data to send.
 * @param[in] p_data  Pointer to the data to send.
 * @param[in] size    Number of bytes to send.
 * @note Data pointed by p_data has to be available till the USBD_EVT_BUFREADY event.
 */
static void respond_setup_data(
    nrfx_usbd_setup_t const *const p_setup,
    void const *p_data, size_t size)
{
    /* Check the size against required response size */
    if (size > p_setup->wLength)
    {
        size = p_setup->wLength;
    }
    uint32_t ret;
    nrfx_usbd_transfer_t transfer =
        {
            .p_data = {.tx = p_data},
            .size = size};
    ret = nrfx_usbd_ep_transfer(NRFX_USBD_EPIN0, &transfer);
    if (ret != NRFX_SUCCESS)
    {
        kb_print("Transfer starting failed: %d", (uint32_t)ret);
    }
    // ASSERT(ret == NRFX_SUCCESS);
    // UNUSED_VARIABLE(ret);
}

static void usbd_setup_ClearFeature(nrfx_usbd_setup_t const *const p_setup)
{
    kb_print("clear feature request");
    if ((p_setup->bmRequestType) == 0x02) // standard request, recipient=endpoint
    {
        if ((p_setup->wValue) == 0)
        {
            if ((p_setup->wIndex) == NRFX_USBD_EPIN1)
            {
                nrfx_usbd_ep_stall_clear(NRFX_USBD_EPIN1);
                nrfx_usbd_setup_clear();
                return;
            }
        }
    }
    else if ((p_setup->bmRequestType) == 0x00) // standard request, recipient=device
    {
        if (REMOTE_WU)
        {
            if ((p_setup->wValue) == 1) // Feature Wakeup
            {
                m_usbd_rwu_enabled = false;
                nrfx_usbd_setup_clear();
                return;
            }
        }
    }
    kb_print("Unknown feature to clear");
    nrfx_usbd_setup_stall();
}
static void usbd_setup_SetFeature(nrfx_usbd_setup_t const *const p_setup)
{
    kb_print("set feature request");
    if ((p_setup->bmRequestType) == 0x02) // standard request, recipient=endpoint
    {
        if ((p_setup->wValue) == 0) // Feature HALT
        {
            if ((p_setup->wIndex) == NRFX_USBD_EPIN1)
            {
                nrfx_usbd_ep_stall(NRFX_USBD_EPIN1);
                nrfx_usbd_setup_clear();
                return;
            }
        }
    }
    else if ((p_setup->bmRequestType) == 0x0) // standard request, recipient=device
    {
        if (REMOTE_WU)
        {
            if ((p_setup->wValue) == 1) // Feature Wakeup
            {
                m_usbd_rwu_enabled = true;
                nrfx_usbd_setup_clear();
                return;
            }
        }
    }
    kb_print("Unknown feature to set");
    nrfx_usbd_setup_stall();
}
static void usbd_setup_GetDescriptor(nrfx_usbd_setup_t const *const p_setup)
{
    kb_print("get descriptor request");
    // determine which descriptor has been asked for
    switch ((p_setup->wValue) >> 8)
    {
    case 1: // Device
        kb_print("get device descriptor");
        respond_setup_data(
            p_setup,
            usb_device_descriptor,
            sizeof(usb_device_descriptor));
        return;
    case 2: // Configuration
        kb_print("get configuration descriptor");
        respond_setup_data(
            p_setup,
            usb_configuration_descriptor,
            sizeof(usb_configuration_descriptor));
        return;
    case 3: // String
        // Select the string
        switch ((p_setup->wValue) & 0xFF)
        {
        case USBD_STRING_LANG_IX:
            kb_print("get string descriptor - lang");
            respond_setup_data(
                p_setup,
                usb_string_lang,
                sizeof(usb_string_lang));
            return;
        case USBD_STRING_MANUFACTURER_IX:
            kb_print("get string descriptor - manufacturer");
            respond_setup_data(
                p_setup,
                usb_string_manufacturer,
                sizeof(usb_string_manufacturer));
            return;
        case USBD_STRING_PRODUCT_IX:
            kb_print("get string descriptor - product");
            respond_setup_data(
                p_setup,
                usb_string_product,
                sizeof(usb_string_product));
            return;
        default:
            break;
        }
        break;
    case 4: // Interface
        // Which interface?
        kb_print("get interface descriptor");
        if ((((p_setup->wValue) & 0xFF) == 0))
        {
            respond_setup_data(
                p_setup,
                usb_interface0_descriptor,
                9);
            return;
        }
        break;
    case 5: // Endpoint
        // Which endpoint?
        kb_print("get endpoint descriptor");
        if (((p_setup->wValue) & 0xFF) == 1)
        {
            // kb_print("endpoint wLength:%d",p_setup->wLength);
            respond_setup_data(
                p_setup,
                usb_endpoint1_in_descriptor,
                7);
            return;
        }
        else
        {
            // kb_print("unknown endpoint request wValue:%2x, wLength:%d",p_setup->wValue & 0xFF,p_setup->wLength);
        }
        break;
    case 0x21: // HID
        kb_print("get HID descriptor");
        if ((p_setup->bmRequestType) == 0x81)
        {
            // Which interface
            if (((p_setup->wValue) & 0xFF) == 0)
            {
                respond_setup_data(
                    p_setup,
                    usb_hid0_descriptor,
                    9);
                return;
            }
        }
        break;
    case 0x22: // HID report
        kb_print("get HID report descriptor");
        if ((p_setup->bmRequestType) == 0x81)
        {
            // Which interface?
            if (((p_setup->wValue) & 0xFF) == 0)
            {
                respond_setup_data(
                    p_setup,
                    kbd_report_descriptor,
                    sizeof(kbd_report_descriptor));
                return;
            }
        }
        break;
    default:
        break; // Not supported - go to stall
    }
    kb_print("Unknown descriptor requested: 0x%2x, type: 0x%2x or value: 0x%2x",
             p_setup->wValue >> 8,
             p_setup->bmRequestType,
             p_setup->wValue & 0xFF);
    nrfx_usbd_setup_stall();
}
static void usbd_setup_GetConfig(nrfx_usbd_setup_t const *const p_setup)
{
    kb_print("get config request");
    if (m_usbd_configured)
    {
        respond_setup_data(
            p_setup,
            get_config_resp_configured,
            sizeof(get_config_resp_configured));
    }
    else
    {
        respond_setup_data(
            p_setup,
            get_config_resp_unconfigured,
            sizeof(get_config_resp_unconfigured));
    }
}
static void usbd_setup_SetConfig(nrfx_usbd_setup_t const *const p_setup)
{
    kb_print("set config request");
    if ((p_setup->bmRequestType) == 0x00)
    {
        // accept only 0 and 1
        if (((p_setup->wIndex) == 0) && ((p_setup->wLength) == 0) &&
            ((p_setup->wValue) <= UINT8_MAX))
        {
            if (NRFX_SUCCESS == ep_configuration((uint8_t)(p_setup->wValue)))
            {
                nrfx_usbd_setup_clear();
                return;
            }
        }
    }
    kb_print("Wrong configuration: Index: 0x%2x, Value: 0x%2x.",
             p_setup->wIndex,
             p_setup->wValue);
    nrfx_usbd_setup_stall();
}
static void usbd_setup_SetIdle(nrfx_usbd_setup_t const *const p_setup)
{
    kb_print("set idle request");
    nrfx_usbd_setup_clear();
}
static void usbd_setup_SetInterface(nrfx_usbd_setup_t const *const p_setup)
{
    kb_print("set interface request");
    // no alternate setting is supported - STALL always
    kb_print("No alternate interfaces supported.");
    nrfx_usbd_setup_stall();
}
static void usbd_setup_SetProtocol(nrfx_usbd_setup_t const *const p_setup)
{
    kb_print("set protocol request");
    nrfx_usbd_setup_clear();
}
static void usbd_setup_GetStatus(nrfx_usbd_setup_t const *const p_setup)
{
    kb_print("get status request");
    switch (p_setup->bmRequestType)
    {
    case 0x80: // Device
        if (((p_setup->wIndex) & 0xff) == 0)
        {
            respond_setup_data(
                p_setup,
                m_usbd_rwu_enabled ? get_status_device_resp_rwu : get_status_device_resp_nrwu,
                sizeof(get_status_device_resp_nrwu));
            return;
        }
        break;
    case 0x81:                 // Interface
        if (m_usbd_configured) // Respond only if configured
        {
            if (((p_setup->wIndex) & 0xff) == 0) // Only interface 0 supported
            {
                respond_setup_data(
                    p_setup,
                    get_status_interface_resp,
                    sizeof(get_status_interface_resp));
                return;
            }
        }
        break;
    case 0x82:                               // Endpoint
        if (((p_setup->wIndex) & 0xff) == 0) // Endpoint 0
        {
            respond_setup_data(
                p_setup,
                get_status_ep_active_resp,
                sizeof(get_status_ep_active_resp));
            return;
        }
        if (m_usbd_configured) // Other endpoints responds if configured
        {
            if (((p_setup->wIndex) & 0xff) == NRFX_USBD_EPIN1)
            {
                if (nrfx_usbd_ep_stall_check(NRFX_USBD_EPIN1))
                {
                    respond_setup_data(
                        p_setup,
                        get_status_ep_halted_resp,
                        sizeof(get_status_ep_halted_resp));
                    return;
                }
                else
                {
                    respond_setup_data(
                        p_setup,
                        get_status_ep_active_resp,
                        sizeof(get_status_ep_active_resp));
                    return;
                }
            }
        }
        break;
    default:
        break; // Just go to stall
    }
    kb_print("Unknown status: 0x%2x", p_setup->bmRequestType);
    nrfx_usbd_setup_stall();
}
/** @} */ /* End of processing setup requests functions */

static void power_usb_event_handler(nrfx_power_usb_evt_t event)
{
    switch (event)
    {
    case NRFX_POWER_USB_EVT_DETECTED:
        if (!nrfx_usbd_is_enabled())
        {
            nrfx_usbd_enable();
        }
        // _usb_state = USB_STATE_POWER_CONNECT;
        // keyboard_evt_trigger_param(KBD_EVT_USB, USB_STATE_POWER_CONNECT);
        break;
    case NRFX_POWER_USB_EVT_REMOVED:
        kb_print("USB power removed");
        // advertising_start(false);
        m_usbd_configured = false;
        if (nrfx_usbd_is_started())
        {
            nrfx_usbd_stop();
        }
        if (nrfx_usbd_is_enabled())
        {
            nrfx_usbd_disable();
        }
        // _usb_state = USB_STATE_NOT_CONNECT;
        // keyboard_evt_trigger_param(KBD_EVT_USB, USB_STATE_NOT_CONNECT);
        break;
    case NRFX_POWER_USB_EVT_READY:
        kb_print("USB ready");
        if (!nrfx_usbd_is_started())
        {
            nrfx_usbd_start(true);
        }
        break;
    default:
        // ASSERT(false);
        break;
    }
}

static void usbd_event_handler(nrfx_usbd_evt_t const *const p_event)
{
    switch (p_event->type)
    {
    case NRFX_USBD_EVT_SUSPEND:
        kb_print("SUSPEND state detected");
        m_usbd_suspend_state_req = true;
        break;
    case NRFX_USBD_EVT_RESUME:
        kb_print("RESUMING from suspend");
        m_usbd_suspend_state_req = false;
        break;
    case NRFX_USBD_EVT_WUREQ:
        kb_print("RemoteWU initiated");
        m_usbd_suspend_state_req = false;
        break;
    case NRFX_USBD_EVT_RESET:
    {
        uint32_t ret = ep_configuration(0);
        // ASSERT(ret == NRFX_SUCCESS);
        // UNUSED_VARIABLE(ret);
        m_usbd_suspend_state_req = false;
        break;
    }
    case NRFX_USBD_EVT_SOF:
    {
        static uint32_t cycle = 0;
        ++cycle;
        if ((cycle % (m_usbd_configured ? 500 : 100)) == 0)
        {
            // bsp_board_led_invert(LED_USB_STATUS);
        }
        break;
    }
    case NRFX_USBD_EVT_EPTRANSFER:
        if (NRFX_USBD_EPIN1 == p_event->data.eptransfer.ep)
        {
            if (NRFX_USBD_EP_OK == p_event->data.eptransfer.status)
            {
                /* Transfer ok - allow status stage */
                nrfx_usbd_setup_clear();
            }
            else if (NRFX_USBD_EP_ABORTED == p_event->data.eptransfer.status)
            {
                /* Just ignore */
                kb_print("Transfer aborted event on EPIN1");
            }
            else
            {
                kb_print("Transfer failed on EPIN1: %d", p_event->data.eptransfer.status);
                nrfx_usbd_setup_stall();
            }
        }
        else if (NRFX_USBD_EPOUT1 == p_event->data.eptransfer.ep)
        {
            if (NRFX_USBD_EP_OK == p_event->data.eptransfer.status)
            {
                /* Transfer ok - allow status stage */

                NRFX_CHECK(nrfx_usbd_ep_transfer(NRFX_USBD_EPOUT1,
                                                  &transfer_out1));                                                ;
                if(rxBuf[0] == 1)
                {
                    kb_print("led state:%2x",rxBuf[1]);
                }
                else if(rxBuf[0] == 3)
                {
                    kb_print("data type:%2x, data value:%4x",rxBuf[1],rxBuf[2] + ((uint16_t)rxBuf[3]<<8));
                }

                nrfx_usbd_setup_clear();
            }
            else if (NRFX_USBD_EP_ABORTED == p_event->data.eptransfer.status)
            {
                /* Just ignore */
                kb_print("Transfer aborted event on EPOUT1");
            }
            else if (NRFX_USBD_EP_WAITING == p_event->data.eptransfer.status)
            {
                nrfx_usbd_ep_transfer(NRFX_USBD_EPOUT1,
                                         &transfer_out1);
            }
            else
            {
                kb_print("Transfer failed on EPOUT1: %d", p_event->data.eptransfer.status);
                nrfx_usbd_setup_stall();
            }
        }
        else if (NRFX_USBD_EPIN0 == p_event->data.eptransfer.ep)
        {
            if (NRFX_USBD_EP_OK == p_event->data.eptransfer.status)
            {
                /* Transfer ok - allow status stage */
                nrfx_usbd_setup_clear();
            }
            else if (NRFX_USBD_EP_ABORTED == p_event->data.eptransfer.status)
            {
                /* Just ignore */
                kb_print("Transfer aborted event on EPIN0");
            }
            else
            {
                kb_print("Transfer failed on EPIN0: %d", p_event->data.eptransfer.status);
                nrfx_usbd_setup_stall();
            }
        }
        else if (NRFX_USBD_EPOUT0 == p_event->data.eptransfer.ep)
        {
            /* NOTE: No EPOUT0 data transfers are used.
             * The code is here as a pattern how to support such a transfer. */
            if (NRFX_USBD_EP_OK == p_event->data.eptransfer.status)
            {
                /* Transfer ok - allow status stage */
                nrfx_usbd_setup_clear();
            }
            else if (NRFX_USBD_EP_ABORTED == p_event->data.eptransfer.status)
            {
                /* Just ignore */
                kb_print("Transfer aborted event on EPOUT0");
            }
            else
            {
                kb_print("Transfer failed on EPOUT0: %d", p_event->data.eptransfer.status);
                nrfx_usbd_setup_stall();
            }
        }
        else
        {
            /* Nothing to do */
        }
        break;
    case NRFX_USBD_EVT_SETUP:
    {
        nrfx_usbd_setup_t setup;
        nrfx_usbd_setup_get(&setup);

        // kb_print("bmRequestType:%2x,bRequest:%2x,wValue:%4x",setup.bmRequestType,setup.bRequest,setup.wValue);
        if ((setup.bmRequestType & 0x80) == 0x80) // data transfer direction (Device-to-host)
        {
            // kb_print("USB Device-to-host transfer request");
            switch ((setup.bmRequestType >> 5) & 0x03)
            {
            case 0: // standard request
                // kb_print("USB standard input request");
                switch (setup.bRequest)
                {
                case 0x00: // get status
                    usbd_setup_GetStatus(&setup);
                    break;
                case 0x06: // get descriptor
                    usbd_setup_GetDescriptor(&setup);
                    break;
                case 0x08: // get configuration
                    usbd_setup_GetConfig(&setup);
                    break;
                case 0x0A: // get interface
                    kb_print("get interface");
                    break;
                case 0x0C: // synch frame
                    kb_print("synch frame");
                    break;
                default:
                    kb_print("undefined standard input request");
                    nrfx_usbd_setup_stall();
                    break;
                }
                break;
            case 1: // class request
                // kb_print("USB class input request");
                break;
            case 2: // vendor request
                // kb_print("USB vendor input request");
                break;
            default:
                kb_print("undefined input request");
                kb_print("bmRequestType:%2x,bRequest:%2x,wValue:%4x", setup.bmRequestType, setup.bRequest, setup.wValue);
                nrfx_usbd_setup_stall();
                break;
            }
        }
        else // data transfer direction (Host-to-device)
        {
            // kb_print("USB Host-to-device transfer request");
            switch ((setup.bmRequestType >> 5) & 0x03)
            {
            case 0: // standard request
                // kb_print("USB standard output request");
                switch (setup.bRequest)
                {
                case 0x01: // clear feature
                    usbd_setup_ClearFeature(&setup);
                    break;
                case 0x03: // set feature
                    usbd_setup_SetFeature(&setup);
                    break;
                case 0x05: // set address
                    kb_print("set address");
                    // nothing to do
                    break;
                case 0x07: // set desciptor
                    kb_print("set desciptor");
                    break;
                case 0x09: // set configuration
                    usbd_setup_SetConfig(&setup);
                    break;
                case 0x0B: // set interface
                    usbd_setup_SetInterface(&setup);
                    break;
                default:
                    kb_print("undefined standard output request");
                    nrfx_usbd_setup_stall();
                    break;
                }
                break;
            case 1: // class request
                switch (setup.bRequest)
                {
                case 0x0A: // set idle
                    usbd_setup_SetIdle(&setup);
                    break;
                case 0x0B: // set protocol
                    usbd_setup_SetProtocol(&setup);
                    break;
                default:
                    kb_print("undefined class output request");
                    kb_print("bmRequestType:%2x,bRequest:%2x,wValue:%4x", setup.bmRequestType, setup.bRequest, setup.wValue);
                    kb_print("wIndex:%2x,wLength:%2x", setup.wIndex, setup.wLength);
                    nrfx_usbd_setup_stall();
                    break;
                }
                break;
            case 2: // vendor request
                kb_print("USB vendor output request");
                break;
            default:
                kb_print("undefined output request");
                nrfx_usbd_setup_stall();
                break;
            }
        }
        break;
    }
    default:
        break;
    }
}

static void clock_irq_handler(nrfx_clock_evt_type_t evt)
{
    if (evt == NRFX_CLOCK_EVT_HFCLK_STARTED)
    {
        //m_clock_cb.hfclk_on = true;
        //clock_clk_started_notify(NRF_DRV_CLOCK_EVT_HFCLK_STARTED);
    }
    if (evt == NRFX_CLOCK_EVT_LFCLK_STARTED)
    {
        //m_clock_cb.lfclk_on = true;
        //clock_clk_started_notify(NRF_DRV_CLOCK_EVT_LFCLK_STARTED);
    }
}

void init_power_clock_(void)
{
    ret_code_t err_code;
    /* Initialize power and clock. */
    //err_code = nrf_drv_clock_init();
    err_code = nrfx_clock_init(clock_irq_handler);
    kb_error(err_code);
    nrfx_clock_enable();

    static const nrfx_power_config_t m_drv_power_config_default;
    //err_code = nrf_drv_power_in(NULL);
    err_code = nrfx_power_init(NULL);
    kb_error(err_code);

    // nrf_drv_clock_hfclk_request(NULL);
    //nrf_drv_clock_lfclk_request(NULL);
    nrfx_clock_lfclk_start();

    // while( !(nrf_drv_clock_hfclk_is_running() && nrf_drv_clock_lfclk_is_running()) )
    while (!nrfx_clock_lfclk_is_running())
    {
        /* Just waiting. */
    }
}

void usb_service_init_(void)
{
    ret_code_t ret;

    // log_resetreason();
    // nrf_power_resetreas_clear(nrf_power_resetreas_get());

    /* USB work starts right here */
    ret = nrfx_usbd_init(usbd_event_handler);
    kb_error(ret);

    /* Configure selected size of the packed on EP0 */
    nrfx_usbd_ep_max_packet_size_set(NRFX_USBD_EPOUT0, EP0_MAXPACKETSIZE);
    nrfx_usbd_ep_max_packet_size_set(NRFX_USBD_EPIN0, EP0_MAXPACKETSIZE);

    if (USBD_POWER_DETECTION)
    {
        static const nrfx_power_usbevt_config_t config =
            {
                .handler = power_usb_event_handler};
        nrfx_power_usbevt_init(&config);
        nrfx_power_usbevt_enable();
        kb_error(ret);
    }
}

void usb_event_excute_(void)
{
    /* Even if we miss an event enabling USB,
     * USB event would wake us up. */
    __WFE();
    /* Clear SEV flag if CPU was woken up by event */
    __SEV();
    __WFE();
    // bsp_board_led_on(LED_RUNNING);
}



void usb_hid_send_(uint8_t report_id, uint8_t *data)
{
    if (report_id == 1)
    {
        uint8_t tranData[INPUT_REPORT_LEN_KEYBOARD + 1];
        tranData[0] = report_id;
        memcpy(&tranData[1], data, INPUT_REPORT_LEN_KEYBOARD);

        nrfx_usbd_transfer_t transfer = {
            .p_data = {.tx = tranData},
            .size = INPUT_REPORT_LEN_KEYBOARD + 1};

        nrfx_usbd_ep_transfer(NRFX_USBD_EPIN1,
                                 &transfer);
    }
    else if (report_id == INPUT_REPORT_CONSUMER_ID)
    {
        uint8_t tranData[INPUT_REPORT_LEN_CONSUMER + 1];
        tranData[0] = report_id;
        memcpy(&tranData[1], data, INPUT_REPORT_LEN_CONSUMER);

        nrfx_usbd_transfer_t transfer = {
            .p_data = {.tx = tranData},
            .size = INPUT_REPORT_LEN_CONSUMER + 1};

        nrfx_usbd_ep_transfer(NRFX_USBD_EPIN1,
                                 &transfer);
    }
    else if (report_id == INPUT_REPORT_CUSTOM_ID)
    {
        uint8_t tranData[INPUT_REPORT_LEN_CUSTOM + 1];
        tranData[0] = report_id;
        memcpy(&tranData[1], data, INPUT_REPORT_LEN_CUSTOM);

        nrfx_usbd_transfer_t transfer = {
            .p_data = {.tx = tranData},
            .size = INPUT_REPORT_LEN_CUSTOM + 1};

        nrfx_usbd_ep_transfer(NRFX_USBD_EPIN1,
                                 &transfer);
    }
}

// /* Event Hanlder*/
// static void usb_evt_hanlder(enum kbd_evt_list event, void *arg)
// {
//     switch (event)
//     {
//     case KBD_EVT_INIT:
//         usb_service_init();
//         kb_print("usb module initialized.");
//         break;
//     case KBD_EVT_SLEEP:

//         kb_print("keyboard light module started.");
//         break;
//     default:
//         break;
//     }
// }
// EVENT_HANDLER(usb_evt_hanlder);