/*
 * Copyright (c) 2020-2020, BLUETRUM Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef AB32VG1_HAL_GPIO_H__
#define AB32VG1_HAL_GPIO_H__

#include "ab32vg1_hal_def.h"

struct gpio_init
{
    uint8_t  pin;
    uint8_t  dir;
    uint8_t  de;      
    uint8_t  pull;
    uint32_t alternate;
    uint32_t af_con;        /*!< Alternate function control
                                [4:0]: Mapping name
                                [6:5]: Mapping control register
                                [7]:   Mapping enable bit */
};
typedef struct gpio_init *gpio_init_t;

enum
{
    GPIOxSET = 0x00,
    GPIOxCLR,
    GPIOx,
    GPIOxDIR,
    GPIOxDE,
    GPIOxFEN,
    GPIOxDRV,
    GPIOxPU,
    GPIOxPD,
    GPIOxPU200K,
    GPIOxPD200K,
    GPIOxPU300K,
    GPIOxPD300K,
};
/* Private constants */
// #define GPIOxSET            0x00
// #define GPIOxCLR            0x01
// #define GPIOx               0x02
// #define GPIOxDIR            0x03
// #define GPIOxDE             0x04
// #define GPIOxFEN            0x05
// #define GPIOxDRV            0x06
// #define GPIOxPU             0x07   /* Only valid when port is used as input */
// #define GPIOxPD             0x08
// #define GPIOxPU200K         0x09
// #define GPIOxPD200K         0x0A
// #define GPIOxPU300K         0x0B
// #define GPIOxPD300K         0x0C
#define GPIOx_REG(port, func)   *(volatile uint32_t*)(SFR6_BASE + (func + (port << 4))*4)

#define FUNCMCONx(x)            *(volatile uint32_t*)(SFR0_BASE + (0x07 + (x))*4)

/* Exported constants */
#define GPIO_DIR_INPUT      (0x01u)
#define GPIO_DIR_OUTPUT     (0x02u)

#define GPIO_DIGITAL        (0x01u)
#define GPIO_ANALOG         (0x02u)

#define GPIO_AFDIS              (0u << 7)
#define GPIO_AFEN               (1u << 7)
#define GPIO_AFCON0             (0u << 5)     /*!< When using UARTT0 UART1 HSUART SPI0 and SD0 */
#define GPIO_AFCON1             (1u << 5)     /*!< When using LPWM0 LPWM1 LPWM2 LPWM3 SPI1 UART2 and CLKOUT */
#define GPIO_AFCON2             (2u << 5)     /*!< When using IR TIMER3 TIMER4 TIMER5 and IIS */
#define GPIO_AFCON_MASK         (0x3u << 5)
#define GPIO_GET_AFCON(af_con)  (uint8_t)(((af_con) & (GPIO_AFCON_MASK)) >> 5)

#define GPIO_NOPULL         (0x00u)
#define GPIO_PULLUP         (0x01u)
#define GPIO_PULLDOWN       (0x02u)

#define GPIO_PIN_LOW        (0x00u)
#define GPIO_PIN_HIGH       (0x01u)

#define GPIOA_BASE          ((hal_sfr_t)(&GPIOASET))
#define GPIOB_BASE          ((hal_sfr_t)(&GPIOBSET))
#define GPIOE_BASE          ((hal_sfr_t)(&GPIOESET))
#define GPIOF_BASE          ((hal_sfr_t)(&GPIOFSET))

#define GPIO_PIN_0          (BIT(0))
#define GPIO_PIN_1          (BIT(1))
#define GPIO_PIN_2          (BIT(2))
#define GPIO_PIN_3          (BIT(3))
#define GPIO_PIN_4          (BIT(4))
#define GPIO_PIN_5          (BIT(5))
#define GPIO_PIN_6          (BIT(6))
#define GPIO_PIN_7          (BIT(7))

#define __HAL_GPIO_SET_DIR(__PORT__, __PIN__, __DIR__) (__DIR__) ? (GPIOx_REG((__PORT__), (GPIOxDIR)) |= BIT(__PIN__)) : (GPIOx_REG((__PORT__), (GPIOxDIR)) &= ~BIT(__PIN__))

/* Include GPIO HAL Extended module */
#include "ab32vg1_hal_gpio_ex.h"

/* Initialization and de-initialization functions */
void hal_gpio_init(hal_sfr_t gpiox, gpio_init_t gpio_init);

/* IO operation function */
uint8_t hal_gpio_read(hal_sfr_t gpiox, uint8_t pin);
void hal_gpio_write(hal_sfr_t gpiox, uint8_t pin, uint8_t state);
void hal_gpio_toggle(hal_sfr_t gpiox, uint8_t pin);

#endif