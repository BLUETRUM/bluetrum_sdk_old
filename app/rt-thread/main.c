/*
 * Copyright (c) 2020-2020, BLUETRUM Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-11-10     greedyhao         first version
 */

#include <rtthread.h>
#include "board.h"

void example_uart(void);
void example_sd(void);

int main(void)
{
    uint32_t cnt = 0;
    uint8_t pin = rt_pin_get("PE.1");

    rt_kprintf("main thread\n");
    rt_kprintf("rt_hw_systick_init: %d %d\n", get_sysclk_nhz(), get_sysclk_nhz()/RT_TICK_PER_SECOND);
    rt_kprintf("PE.1=%d\n", pin);
    rt_pin_mode(pin, PIN_MODE_OUTPUT);

//    example_sd();

    while (1)
    {
        WDT_CLR();
        if (cnt % 2 == 0) {
            rt_pin_write(pin, PIN_LOW);
        } else {
            rt_pin_write(pin, PIN_HIGH);
        }
        cnt++;
        rt_thread_mdelay(1000);
    }
}
