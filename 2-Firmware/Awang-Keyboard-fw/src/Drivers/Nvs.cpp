#include "Nvs.hpp"

#include <logging/log.h>
#include "../Keyboard/Power.hpp"

LOG_MODULE_REGISTER(Nvs);

void Nvs::Init() {
    if(initialized) return;
    int rc = 0;

    flash_dev = (device *)FLASH_AREA_DEVICE(storage);
    if(!device_is_ready(flash_dev)) {
        LOG_ERR("Flash device %s is not ready", flash_dev->name);
        Power::Stop();
    }

    fs.offset = FLASH_AREA_OFFSET(storage);
    rc = flash_get_page_info_by_offs(flash_dev, fs.offset, &info);
    if (rc) {
        LOG_ERR("Unable to get page info");
        Power::Stop();
    }
    fs.sector_size = info.size;
    fs.sector_count = 3U;

    rc = nvs_init(&fs, flash_dev->name);
    if (rc) {
        LOG_ERR("Flash Init failed");
        Power::Stop();
    }
    this->initialized = true;
}