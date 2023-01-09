#include "flash_storage.h"

#include <logging/log.h>
#define MODULE flash_storage

#include <zephyr.h>
#include <drivers/flash.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <device.h>

static bool initialized;
static struct nvs_fs fs;
static struct flash_pages_info info;
static const struct device* flash = FLASH_AREA_DEVICE(storage);

static void flash_storage_init() {
    if(initialized) return;
    int rc = 0;

    if(!device_is_ready(flash)) {
        // LOG_ERR("Flash device %s is not ready", flash->name);
        return;
    }

    fs.offset = FLASH_AREA_OFFSET(storage);
    rc = flash_get_page_info_by_offs(flash, fs.offset, &info);
    if (rc) {
        // LOG_ERR("Unable to get page info");
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = 3U;

    nvs_init(&fs, flash->name);

    initialized = true;
}

int flash_storage_read(uint16_t id, uint8_t *data, uint16_t len) {
    if(!initialized) flash_storage_init();

    return nvs_read(&fs, id, data, len);
}

int flash_storage_write(uint16_t id, uint8_t *data, uint16_t len) {
    if(!initialized) flash_storage_init();

    return nvs_write(&fs, id, data, len);
}