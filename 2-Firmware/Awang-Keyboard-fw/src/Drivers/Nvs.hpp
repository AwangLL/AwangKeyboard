#pragma once

#include <stdint.h>
#include <string.h>
#include <drivers/flash.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <device.h>

class Nvs
{
private:
    bool initialized = false;
    nvs_fs fs;
    flash_pages_info info;
    device* flash_dev;
public:
    void Init();
    template<class T>
    T &Read(uint16_t id, T &_t) {
        if(!initialized) {
            Init();
        }
        int rc = 0;

        rc = nvs_read(&fs, id, &_t, sizeof _t);
        if(rc > 0) {
            return _t;
        } else {
            memset(&_t, 0, sizeof _t);
            return _t;
        }
    }
    template<class T>
    void Write(uint16_t id, T &_t) {
        nvs_write(&fs, id, &_t, sizeof _t);
    }
};