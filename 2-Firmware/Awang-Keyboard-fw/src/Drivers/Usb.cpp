#include "Usb.hpp"
#include "../Config.h"

#include <drivers/uart.h>

void Usb::Hid::Init() {
    hid_dev = (device *)device_get_binding("HID_0");
    usb_hid_register_device(hid_dev, hid_kbd_report_desc,
                sizeof hid_kbd_report_desc, &usb.ops);
    usb_hid_init(hid_dev);
}

void Usb::Hid::Send(uint8_t id, uint8_t* data) {
    if(!usb.enabled) usb.Enable();

    if(id == INPUT_REPORT_ID_KEYBOARD)
    {
        uint8_t buf[INPUT_REPORT_LEN_KEYBOARD + 1];
        memcpy(&buf[1], data, INPUT_REPORT_LEN_KEYBOARD);
        buf[0] = id;
        hid_int_ep_write(hid_dev, buf, INPUT_REPORT_LEN_KEYBOARD + 1, nullptr);
    }
    else if(id == INPUT_REPORT_ID_CONSUMER)
    {
        uint8_t buf[INPUT_REPORT_LEN_CONSUMER + 1];
        memcpy(&buf[1], data, INPUT_REPORT_LEN_CONSUMER);
        buf[0] = id;
        hid_int_ep_write(hid_dev, buf, INPUT_REPORT_LEN_CONSUMER + 1, nullptr);
    }
}

void Usb::Cdc::cdc_cb(const struct device *dev, void *user_data)
{
    ARG_UNUSED(user_data);

    while(uart_irq_update(usb.cdc.cdc_dev) && uart_irq_is_pending(usb.cdc.cdc_dev))
    {
        if(uart_irq_rx_ready(usb.cdc.cdc_dev))
        {
            size_t recv_len;
            uint8_t buffer[64];
            recv_len = uart_fifo_read(usb.cdc.cdc_dev, buffer, sizeof buffer);
            if(recv_len < 0)
            {
                printk("# Fail to read CDC DATA\n");
                recv_len = 0;
            }

            printk("# read CDC data:%s\n", buffer);
            // Host::ReadFromHost(buffer, recv_len);
        }
    }
}

void Usb::Cdc::cdc_dtr_get(void *) {
    uart_line_ctrl_get(usb.cdc.cdc_dev, UART_LINE_CTRL_DTR, &usb.cdc.dtr);
}

void Usb::Cdc::Init() {
    cdc_dev = (device *)device_get_binding("CDC_ACM_0");
    uart_irq_callback_set(cdc_dev, cdc_cb);
}

void Usb::Cdc::Enable() {
    if(!initialized) Init();
    uart_irq_rx_enable(cdc_dev);
    dtr = 0;
    dtr_timer.Start(1000);
    enabled = true;
}

void Usb::Cdc::Disable() {
    if(!initialized) Init();
    uart_irq_rx_disable(cdc_dev);
    dtr_timer.Stop();
    enabled = false;
}

void Usb::Cdc::Send(uint8_t* buf, size_t len)
{
    if(!enabled) Enable();
    
    uart_irq_tx_enable(cdc_dev);

	while(uart_irq_tx_ready(cdc_dev))
	{
		size_t send_len = uart_fifo_fill(cdc_dev, buf, len);
		if (send_len < len) {
			printk("Drop %d bytes\n", len - send_len);
		}
		uart_irq_tx_disable(cdc_dev);
		break;
	}
}

void Usb::in_ready_cb(const device *dev)
{
    ARG_UNUSED(dev);

    k_sem_give(&usb.usb_sem);
}

void Usb::status_cb(enum usb_dc_status_code status, const uint8_t *param)
{
	switch(status)
    {
        case USB_DC_CONNECTED:
            usb.state = Usb::STATE::Power;
            printk("usb power\n");
            break;
        case USB_DC_DISCONNECTED:
            usb.state = Usb::STATE::Disconnected;
            // usb.cdc.Disable();
            printk("usb disconnected\n");
            break;
        case USB_DC_CONFIGURED:
            usb.state = Usb::STATE::Connected;
            // usb.cdc.Enable();
            printk("usb connected\n");
            break;
        default:
            break;
    }
}

void Usb::Init() {
    if(initialized) return;
    ops.int_in_ready = in_ready_cb;

    k_sem_init(&usb_sem, 1, 1);

    hid.Init();
    cdc.Init();

    this->initialized = true;
}

void Usb::Enable() {
    if(!initialized) Init();

    int rc = usb_enable(status_cb);
    if(rc != 0)
    {
        printk("usb enable failed\n");
    }

    /* Wait 1 sec for the host to do all settings */
    cdc.Enable();
	k_busy_wait(USEC_PER_SEC);

    enabled = true;
}

void Usb::Disable() {
    if(!initialized) Init();
    usb_disable();
    cdc.Disable();
    enabled = false;
}

Usb::STATE Usb::State() {
    return state;
}

const uint8_t Usb::hid_kbd_report_desc[] = {
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x06, // Usage (Keyboard)
    0xA1, 0x01, // Collection (Application)
    0x85, INPUT_REPORT_ID_KEYBOARD, // Report ID (1)

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
    0x85, INPUT_REPORT_ID_CONSUMER, //   Report ID (2)

    0x95, 0x01,       // Report Count (1)
    0x75, 0x10,       // Report Size (16)
    0x19, 0x01,       // Usage Minimum (0x01)
    0x2A, 0x9C, 0x02, // Usage Maximum (0x29c)
    0x15, 0x01,       // Logical Minimum (0x01)
    0x26, 0x9C, 0x02, // Logical Maximum (0x29c)
    0x81, 0x00,       // Input (Data, Array) Key array(2 bytes)
    
    0xC0,             // End Collection (Application)
};