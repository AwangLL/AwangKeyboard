#pragma once

#include <stdint.h>

typedef uint16_t matrix_row_t;

void keyboard_matrix_init(void);
void keyboard_matrix_task(void);

matrix_row_t matrix_get_row(uint8_t row);
