#pragma once

#include <device.h>

#define MATRIX_NODE_LABEL DT_NODELABEL(matrix)
#define HC165_NUM DT_PROP(MATRIX_NODE_LABEL, chain_length)
#define KEY_NUM (HC165_NUM * 8)
#define DEBOUNCE CONFIG_AWANG_MATRIX_DEBOUNCE_TIMES
#define DEBOUNCE_DELAY CONFIG_AWANG_MATRIX_DEBOUNCE_DELAY