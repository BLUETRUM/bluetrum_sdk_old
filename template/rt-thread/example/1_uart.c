/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-11-23     greedyhao         first version
 */

#include "ab32vg1_hal.h"
#include "drv_usart.h"

#define SAMPLE_UART_NAME       "uart1"
static rt_device_t serial;
static struct rt_semaphore rx_sem;
struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

void example_uart(void)
{
    char ch = 'U';

    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    serial = rt_device_find(SAMPLE_UART_NAME);

    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    rt_device_set_rx_indicate(serial, uart_input);

    WDT_DIS();
    while (1) {
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        ch = ch + 1;
        rt_device_write(serial, 0, &ch, 1);
    }
}
