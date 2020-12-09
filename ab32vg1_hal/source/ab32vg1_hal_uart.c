/*
 * Copyright (c) 2020-2020, BLUETRUM Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ab32vg1_hal.h"

#define UARTx_CON_RW(__ADDR__)              (SFR_RW((__ADDR__) + 0*4u))
#define UARTx_CPND_RW(__ADDR__)             (SFR_RW((__ADDR__) + 1*4u))
#define UARTx_BAUD_RW(__ADDR__)             (SFR_RW((__ADDR__) + 2*4u))
#define UARTx_DATA_RW(__ADDR__)             (SFR_RW((__ADDR__) + 3*4u))

const uint32_t uartx_reg[3] =
{
    [UART0N] = SFR0_BASE + 0x10*4,
    [UART1N] = SFR0_BASE + 0x30*4,
    [UART2N] = SFR9_BASE + 0x18*4,
};

const uint32_t uartx_rcu[3] =
{
    [UART0N] = RCU_UART0,
    [UART1N] = RCU_UART1,
    [UART2N] = RCU_UART2,
};

/**
 * @brief Set the UART baud rate.
 * 
 * @param uartx This parameter can be UARTxN where x can be (0.2).
 * @param baud Baud rate.
 */
void hal_uart_setbaud(uint32_t uartx, uint32_t baud)
{
    uint32_t baud_cfg;

    UARTx_CON_RW(uartx_reg[uartx]) |= UART_CLK_SRC1;
    baud_cfg = (26000000/2)/baud;   //1.5M
    UARTx_BAUD_RW(uartx_reg[uartx]) = (baud_cfg << 16) | baud_cfg;
}

/**
 * @brief Initialize the UART mode.
 * 
 * @param huart UART handle.
 * @return hal_error_t 
 */
hal_error_t hal_uart_init(struct uart_handle *huart)
{
    if (huart == HAL_NULL) {
        return -HAL_ERROR;
    }

    hal_uart_mspinit(huart);
    uart_config_all(huart);

    return HAL_EOK;
}

/**
 * @brief DeInitialize the UART peripheral.
 * 
 * @param uartx This parameter can be UARTxN where x can be (0.2).
 */
void hal_uart_deinit(uint32_t uartx)
{
    UARTx_CON_RW(uartx_reg[uartx]) = 0;
}

/**
 * @brief Initialize the UART MSP.
 * 
 * @param huart UART handle.
 */
WEAK void HAL_UART_MspInit(struct uart_handle *huart)
{}

/**
 * @brief Control the UART peripheral.
 * 
 * @param uartx This parameter can be UARTxN where x can be (0.2).
 * @param cntl 
 *      @arg UART_MODULE_ENABLE
 *      @arg UART_BIT9_ENABLE
 *      @arg UART_RXIT_ENABLE
 *      @arg UART_TXIT_ENABLE
 *      @arg UART_SB2_ENABLE
 *      @arg UART_CLK_SRC1
 *      @arg UART_1LINE_ENABLE
 *      @arg UART_RX_ENABLE
 * @param param 
 *      @arg HAL_DISABLE
 *      @arg HAL_ENABLE
 */
void hal_uart_control(uint32_t uartx, uint32_t cntl, uint32_t param)
{
    if (param == HAL_ENABLE) {
        UARTx_CON_RW(uartx_reg[uartx]) |= (cntl);
    } else {
        UARTx_CON_RW(uartx_reg[uartx]) &= ~(cntl);
    }
}

/**
 * @brief Send a character
 * 
 * @param uartx This parameter can be UARTxN where x can be (0.2).
 * @param data The characters that need to be sent
 */
void hal_uart_write(uint32_t uartx, uint8_t data)
{
    UARTx_DATA_RW(uartx_reg[uartx]) = data;
}

/**
 * @brief Receive a character.
 * 
 * @param uartx This parameter can be UARTxN where x can be (0.2).
 * @return uint8_t Received character.
 */
uint8_t hal_uart_read(uint32_t uartx)
{
    return (UARTx_DATA_RW(uartx_reg[uartx]) & 0xff);
}

/**
 * @brief Get the UART flag.
 * 
 * @param uartx This parameter can be UARTxN where x can be (0.2).
 * @param flag 
 *      @arg UART_FLAG_RXPND
 *      @arg UART_FLAG_TXPND
 * @return uint32_t 
 */
uint32_t hal_uart_getflag(uint32_t uartx, uint32_t flag)
{
    uint32_t ret = UARTx_CON_RW(uartx_reg[uartx]) & flag;
    return ret;
}

/**
 * @brief Clear the UART flag.
 * 
 * @param uartx This parameter can be UARTxN where x can be (0.2).
 * @param flag 
 *      @arg UART_FLAG_RXPND
 *      @arg UART_FLAG_TXPND
 */
void hal_uart_clrflag(uint32_t uartx, uint32_t flag)
{
    UARTx_CPND_RW(uartx_reg[uartx]) |= flag;
}

/**
 * @brief Configure the UART peripheral.
 * 
 * @param huart UART handle.
 */
void uart_config_all(struct uart_handle *huart)
{
    hal_uart_control(huart->instance, UART_MODULE_ENABLE, HAL_DISABLE);

    CLKCON1 |= BIT(14);
    hal_rcu_periph_clk_enable(uartx_rcu[huart->instance]);
    hal_uart_setbaud(huart->instance, huart->init.baud);

    if (huart->init.mode != UART_MODE_TX) {
        hal_uart_control(huart->instance, UART_RX_ENABLE, HAL_ENABLE);
    }
    hal_uart_control(huart->instance, UART_MODULE_ENABLE, HAL_ENABLE);
}
