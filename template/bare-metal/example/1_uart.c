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

void uart1_putc(char ch)
{
    hal_uart_write(UART1N, ch);
    while(hal_uart_getflag(UART1N, UART_FLAG_TXPND) == 0);
}

static void uart_isr_func(int vector, void *param)
{
    if(hal_uart_getflag(UART0N, UART_FLAG_RXPND))       //RX one byte finish
    {
        hal_uart_clrflag(UART0N,  UART_FLAG_RXPND);
        rt_kprintf("%c", hal_uart_read(UART0N));
    }
    if(hal_uart_getflag(UART1N, UART_FLAG_RXPND))       //RX one byte finish
    {
        hal_uart_clrflag(UART1N,  UART_FLAG_RXPND);
        rt_kprintf("%c", hal_uart_read(UART1N));
    }
}

void example_uart(void)
{
    struct uart_handle huart = {0};
    huart.instance          = UART1N;
    huart.init.baud         = 1500000;
    huart.init.word_len     = UART_WORDLENGTH_8B;
    huart.init.stop_bits    = UART_STOPBITS_1;
    huart.init.mode         = UART_MODE_TX_RX;

    hal_uart_init(&huart);
    rt_kprintf("UART0CON=0x%X\n", UART0CON);

    while (1) {
        WDT_CLR();
        uart1_putc('U');
        if(hal_uart_getflag(UART1N, UART_FLAG_RXPND)) {
            hal_uart_clrflag(UART1N,  UART_FLAG_RXPND);
            rt_kprintf("%c", hal_uart_read(UART1N));
        }
        rt_thread_mdelay(1000);
    }
}
