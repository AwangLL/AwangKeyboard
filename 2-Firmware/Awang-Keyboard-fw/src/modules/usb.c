#include <zephyr.h>
#include <device.h>
#include <usb/usb_device.h>
#include <usb/class/usb_cdc.h>
#include <usb/class/usb_hid.h>
#include <drivers/uart.h>

#define MODULE usb
#include <caf/events/module_state_event.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(MODULE, CONFIG_AWANG_USB_LOG_LEVEL);

#include "hid_def.h"

#include "hid_event.h"
#include "keyboard_event.h"

static const struct device *hid_dev;
static const struct device *cdc_dev;
static struct k_sem usb_sem;
static struct hid_ops ops;

static bool initialized;
static bool enabled;
static uint32_t dtr;

static void usb_module_enable();
static void usb_module_disable();

static void in_ready_cb(const struct device *dev) {
    k_sem_give(&usb_sem);
}

static void status_cb(enum usb_dc_status_code status, const uint8_t *param)
{
	switch(status)
    {
        case USB_DC_CONNECTED:
            LOG_INF("usb power");
            break;
        case USB_DC_DISCONNECTED:
            usb_module_disable();
            LOG_INF("usb disconnected");
            break;
        case USB_DC_CONFIGURED:
            usb_module_enable();
            LOG_INF("usb connected");
            break;
        default:
            break;
    }
}

static void cdc_cb(const struct device *dev, void *user_data) {
    while(uart_irq_update(cdc_dev) && uart_irq_is_pending(cdc_dev))
    {
        if(uart_irq_rx_ready(cdc_dev))
        {
            size_t recv_len;
            uint8_t buffer[64];
            recv_len = uart_fifo_read(cdc_dev, buffer, sizeof buffer);
            if(recv_len < 0)
            {
                LOG_WRN("# Fail to read CDC DATA\n");
                recv_len = 0;
            }

            LOG_INF("# read CDC data:%s\n", buffer);
        }
    }
}

static void cdc_dtr_get() {
    uart_line_ctrl_get(cdc_dev, UART_LINE_CTRL_DTR, &dtr);
}

static void usb_module_init() {
    if(initialized) return;
    // usb
    ops.int_in_ready = in_ready_cb;

    k_sem_init(&usb_sem, 1, 1);
    // hid
    hid_dev = device_get_binding("HID_0");
    usb_hid_register_device(hid_dev, hid_kbd_report_desc_nkro,
                sizeof hid_kbd_report_desc_nkro, &ops);
    
    usb_hid_init(hid_dev);
    // cdc
    cdc_dev = device_get_binding("CDC-ACM-UART");
    uart_irq_callback_set(cdc_dev, cdc_cb);

    int rc = usb_enable(status_cb);
    if(rc) {
        LOG_ERR("fail to enable usb");
    }
    LOG_INF("initialized");
    initialized = true;
}

static void usb_module_enable() {
    if(enabled) return;

    uart_irq_rx_enable(cdc_dev);
    enabled = true;
}

static void usb_module_disable() {
    if(!enabled) return;

    usb_disable();
    uart_irq_rx_disable(cdc_dev);
    enabled = false;
}

static void usb_hid_send(struct hid_report_send_event *report) {
    if(!enabled) return;

    if(report->report_id == INPUT_REPORT_ID_KEYBOARD)
    {
        uint8_t buf[INPUT_REPORT_LEN_KEYBOARD + 1];
        memcpy(&buf[1], report->data, INPUT_REPORT_LEN_KEYBOARD);
        buf[0] = report->report_id;
        hid_int_ep_write(hid_dev, buf, INPUT_REPORT_LEN_KEYBOARD + 1, NULL);
    }
    else if(report->report_id == INPUT_REPORT_ID_CONSUMER)
    {
        uint8_t buf[INPUT_REPORT_LEN_CONSUMER + 1];
        memcpy(&buf[1], report->data, INPUT_REPORT_LEN_CONSUMER);
        buf[0] = report->report_id;
        hid_int_ep_write(hid_dev, buf, INPUT_REPORT_LEN_CONSUMER + 1, NULL);
    }
}

static void usb_cdc_send(uint8_t* buf, size_t len) {
    if(!enabled) return;

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

static bool event_handler(const struct event_header *eh) {
    if(is_hid_report_send_event(eh)) {
        if(!enabled) return;
        usb_hid_send(cast_hid_report_send_event(eh));
        return false;
    }
    
    if(is_keyboard_event(eh)) {
        switch(cast_keyboard_event(eh)->event) {
        case KEYBOARD_EVENT_READY:
            usb_module_init();
            break;
        default:
            break;
        }
        return false;
    }

    return false;
}

EVENT_LISTENER(MODULE, event_handler);
EVENT_SUBSCRIBE(MODULE, hid_report_send_event);
EVENT_SUBSCRIBE(MODULE, keyboard_event);