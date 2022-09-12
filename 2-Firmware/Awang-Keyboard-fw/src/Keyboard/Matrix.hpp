#pragma once

#include <stdint.h>
#include <drivers/spi.h>
#include "../Drivers/Usb.hpp"
#include "../Drivers/Gpio.hpp"
#include "../Drivers/Timer.hpp"

class Matrix {
public:
    static const int NUM_OF_LAYER = 2;
    static const int NUM_OF_KEY = 83;
private:
    static uint8_t MAP[NUM_OF_KEY + 1];
    static const int NUM_OF_CHIPS = 11;
    static const int DEBOUNCE = 1;
    static const int DEBOUNCE_DELAY = 100; // 100us
    static const int SCAN_INTERVAL = 10; // 10ms 
    bool initialized = false;
    bool enabled = false;
    int8_t fnIndex = -1;
    uint8_t buf[Usb::INPUT_REPORT_LEN_KEYBOARD];
    uint8_t matrixScan[NUM_OF_CHIPS];
    uint8_t matrixDebounce[NUM_OF_CHIPS];
    uint8_t matrixPrev[NUM_OF_CHIPS];
    spi_buf rx;
    spi_buf_set rxBuf;
    spi_config spiCfg;
    device *dev = nullptr;
    Timer timer;
    void BufAdd(uint8_t keycode);
    void BufRemove(uint8_t keycode);
    void BufClear();
    static void OnTimerCallback(void * arg);
public:
    Matrix():timer(OnTimerCallback){}
    void Init();
    void Enable();
    void Disable();
    void Scan();
    void Remap();
};

extern Matrix matrix;