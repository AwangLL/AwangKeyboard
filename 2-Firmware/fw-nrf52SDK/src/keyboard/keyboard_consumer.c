#include "keyboard_consumer.h"
#include <stdint.h>
#include "keyboard_report.h"

void keyboard_consumer_command(uint16_t keycode)
{
    uint8_t data[2];
    data[0] = keycode & 0xFF;
    data[1] = (keycode >> 8) & 0xFF;
    keyboard_report_send(INPUT_REPORT_CONSUMER_ID, data);
}
