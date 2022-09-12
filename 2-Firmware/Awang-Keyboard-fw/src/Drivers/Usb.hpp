#pragma once

#include <device.h>
#include <usb/usb_device.h>
#include <usb/class/usb_hid.h>

#include "../Drivers/Timer.hpp"

class Usb {
public:
    enum STATE {
        Power,
        Connected,
        Disconnected
    };
    static const uint8_t INPUT_REPORT_LEN_KEYBOARD = 15;
    static const uint8_t OUTPUT_REPORT_LEN_KEYBOARD = 1;
    static const uint8_t INPUT_REPORT_LEN_CONSUMER = 2;
    static const uint8_t INPUT_REPORT_ID_KEYBOARD = 0x01;
    static const uint8_t OUTPUT_REPORT_ID_KEYBOARD = 0x01;
    static const uint8_t INPUT_REPORT_ID_CONSUMER = 0x02;
    static const uint8_t hid_kbd_report_desc[98];
private:
    class Hid {
        private:
            bool initialized = false;
            device* hid_dev;
        public:
            void Init();
            void Send(uint8_t id, uint8_t* data);
    };
    class Cdc {
        private:
            bool initialized = false;
            bool enabled = false;
            device* cdc_dev;
            uint32_t dtr;
            static void cdc_cb(const struct device *dev, void *user_data);
            static void cdc_dtr_get(void *);
            Timer dtr_timer;
        public:
            Cdc():dtr_timer(cdc_dtr_get){}
            void Init();
            void Enable();
            void Send(uint8_t* buf, size_t len);
            void Disable();
    };

    bool initialized = false;
    bool enabled = false;
    
    k_sem usb_sem;
    hid_ops ops;

    static void in_ready_cb(const device *dev);
    static void status_cb(enum usb_dc_status_code status, const uint8_t *param);

    STATE state;
public:
    Hid hid;
    Cdc cdc;
    void Init();
    void Send();
    void Enable();
    void Disable();
    STATE State();
};

extern Usb usb;