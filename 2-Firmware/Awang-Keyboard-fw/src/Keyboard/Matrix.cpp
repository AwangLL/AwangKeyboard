#include "Matrix.hpp"
#include <string.h>
#include "Keycode.h"
#include "Power.hpp"
#include "../Config.h"

LOG_MODULE_REGISTER(Matrix, LOG_LEVEL_INF);

/**
 * @brief convert the index of spi read data to the index of keymap.
 * and the initial value are index of spi read data 
 * by the order of keymap.
 * The array will be processed in Init() function.
 * 
 */
uint8_t Matrix::MAP[NUM_OF_KEY + 1] = {
    5, 6, 12, 20, 28, 36, 35, 34, 33, 72, 78, 77, 83, 82,
    7, 23, 17, 29, 30, 31, 37, 40, 39, 45, 46, 47, 48, 75, 81,
    8, 38, 25, 26, 27, 32, 54, 55, 56, 44, 43, 42, 41, 76, 74,
    3, 18, 19, 24, 22, 21, 63, 52, 51, 50, 49, 53, 64, 73,
    2, 9, 10, 11, 16, 15, 60, 59, 58, 57, 61, 62, 79, 80,
    4, 1, 13, 14, 69, 70, 71, 65, 68, 67, 66
};

void Matrix::OnTimerCallback(void * arg) {
    matrix.Remap();
}

void Matrix::Init() {
    if(initialized) return;
    scanCE.Configure(GPIO_OUTPUT_HIGH);
    scanPL.Configure(GPIO_OUTPUT_HIGH);

    rx = {
        .buf = matrixScan,
        .len = NUM_OF_CHIPS
    };
    rxBuf = {
        .buffers = &rx,
        .count = 1
    };
    spiCfg = {
        .frequency = 4000000,
        .operation = SPI_WORD_SET(8) | SPI_OP_MODE_MASTER,
        .slave = 0,
    };
    dev = (device *)device_get_binding("SPI_1");

    if(dev == nullptr) {
        LOG_ERR("spi 1 not found");
        Power::Stop();
    }
    if(!device_is_ready(dev)) {
        LOG_ERR("spi 1 not ready");
        Power::Stop();
    }

    memset(matrixScan, 0, sizeof matrixScan);
    memset(matrixDebounce, 0, sizeof matrixDebounce);
    memset(matrixPrev, 0, sizeof matrixPrev);
    BufClear();

    // find the fn index
    for(int i = 0; i < NUM_OF_KEY; i++) {
        if(config.matrix.keymap[0][i] == KC_FN) {
            fnIndex = MAP[i] - 1;
            break;
        }
    }
    // process MAP[]
    uint8_t tempBuf[NUM_OF_KEY + 1];
    for(int i = 1; i <= NUM_OF_KEY; i++) {
        tempBuf[MAP[i - 1]] = i - 1;
    }
    memcpy(MAP, tempBuf, sizeof MAP);

    for(int i = 1; i <= NUM_OF_KEY; i++) {
        k_sleep(K_MSEC(10));
    }
    
    initialized = true;
}

void Matrix::Enable() {
    if(!initialized) Init();
    if(enabled) return;

    timer.Start(SCAN_INTERVAL);
    enabled = true;
}

void Matrix::Disable() {
    if(!enabled) return;
    
    timer.Stop();
    enabled = false;
}

void Matrix::Scan() {
    if(!enabled) return;
    // Load
    scanPL.Set(0);
    k_sleep(K_USEC(10));
    scanPL.Set(1);
    // Read
    scanCE.Set(0);
    int err;
    err = spi_transceive(dev, &spiCfg, NULL, &rxBuf);
    if(err) {
        LOG_WRN("spi read failed with %d", err);
    }
    scanCE.Set(1);
    

    // Debounce
    int8_t debounce = DEBOUNCE;
    while(debounce --) {
        // Load
        scanPL.Set(0);
        k_sleep(K_USEC(DEBOUNCE_DELAY));
        scanPL.Set(1);
        scanCE.Set(0);
        // Read
        rx.buf = matrixDebounce;
        err = spi_transceive(dev, &spiCfg, NULL, &rxBuf);
        if(err) {
            LOG_WRN("spi read failed with %d", err);
        }
        rx.buf = matrixScan;
        scanCE.Set(1);
        for(int i = 0; i < NUM_OF_CHIPS; i++) {
            matrixScan[i] &= matrixDebounce[i];
        }
    }
}

void Matrix::Remap() {
    uint8_t matrixChanged[NUM_OF_CHIPS];
    uint8_t layer = 0;
    bool changed = false;
    uint8_t keycode;
    bool key_down;

    Scan();

    for(int i = 0; i < NUM_OF_CHIPS; i++) {
        matrixScan[i] = ~matrixScan[i];
        matrixChanged[i] = matrixPrev[i] ^ matrixScan[i];
        matrixPrev[i] = matrixScan[i];
    }
    
    // if(matrixScan[fnIndex / 8] & (1 << (fnIndex - fnIndex / 8 * 8))) {
    if(matrixScan[fnIndex / 8] & (1 << (fnIndex / 8 * 8 + 7 - fnIndex))) {
        layer = 1;
    }

    for(int i = 0; i < NUM_OF_CHIPS; i++) {
        if(matrixChanged[i]) {
            changed = true;
            for(int j = 0; j < 8; j++) {
                uint8_t mask = 1 << j;
                if(matrixChanged[i] & mask) { // Key state changed(pressed or released)
                    if(matrixScan[i] & mask) {
                        key_down = true;
                    } else {
                        key_down = false;
                    }
                    
                    // get keycode
                    keycode = config.matrix.keymap[layer][MAP[i * 8 + 8 - j]];

                    // keycode type
                    if(keycode_is_common(keycode) || keycode_is_modifier(keycode)) {
                        if(key_down) BufAdd(keycode);
                        else BufRemove(keycode);
                    } else if(keycode_is_consumer(keycode)) {
                        // Host::SendConsumer(keycode, key_down);
                    } else if(keycode_is_function(keycode)) {

                    }
                }
            }
        }
    }
    if(changed && usb.State() == usb.Connected) {
        usb.hid.Send(usb.INPUT_REPORT_ID_KEYBOARD, buf);
    }
}

void Matrix::BufAdd(uint8_t keycode) {
    if(keycode_is_common(keycode)) {
        uint8_t index = keycode / 8 + 2;
        uint8_t pos = keycode % 8;
        buf[index] |= (1 << pos);
    } else if(keycode_is_modifier(keycode)) {
        buf[0] |= (1 << (keycode - 0xE0));
    }
}

void Matrix::BufRemove(uint8_t keycode) {
    if(keycode_is_common(keycode)) {
        uint8_t index = keycode / 8 + 2;
        uint8_t pos = keycode % 8;
        buf[index] &= ~(1 << pos);
    } else if(keycode_is_modifier(keycode)) {
        buf[0] &= ~(1 << (keycode - 0xE0));
    }
}

void Matrix::BufClear() {
    memset(buf, 0, sizeof buf);
}