/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ab32vg1_hal.h"

static void sd_event(uint32_t event)
{}

void example_sd(void)
{
    drv_mci.init(sd_event);
    drv_mci.power_control(HAL_POWER_FULL);
}
