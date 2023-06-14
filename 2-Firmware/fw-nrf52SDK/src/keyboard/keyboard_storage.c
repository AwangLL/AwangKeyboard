#include "keyboard_storage.h"
#include "fds.h"
#include <stdbool.h>
#include <stdint.h>

#define FDS_READ 0x00
#define FDS_WRITE 0x01

#define TEMPERATURE_FLASH_ID 0x00A0
#define TEMPERATURE_FLASH_KEY 0x00B0

#define FDS_BUFFER_SIZE 200

//static void fds_callback();

//static bool volatile s_fdsIfInitialized;
//static fds_record_desc_t s_recordDesc;
//static uint8_t s_dataBuffer[FDS_BUFFER_SIZE];

//void keyboard_storage_init(void) {
//  ret_code_t err_code;
//  fds_record_t record;

//  fds_register(fds_callback); //register fds

//  err_code = fds_init(); //initialize fds
//  APP_ERROR_CHECK(err_code);
//  while (!s_fdsIfInitialized) {
//    sd_app_evt_wait();
//  }

//  err_code = fds_record_write(&s_recordDesc, &record);
//  APP_ERROR_CHECK(err_code);
//}