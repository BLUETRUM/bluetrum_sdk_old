#include "ab32vg1_hal.h"

void example_sd(void)
{
    struct sd_handle _sd = {0};
    _sd.instance = &SD0CON;

    hal_sd_init(&_sd);
}
