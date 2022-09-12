#include "Power.hpp"

#include <zephyr.h>

void Power::Stop() {
    for(;;) {
        k_sleep(K_MSEC(1000));
    }
}