#include "ab32vg1_hal.h"

void hal_uart_mspinit(struct uart_handle *huart)
{
    struct gpio_init gpio_init;

    switch (huart->instance)
    {
    case UART0N:
        gpio_init.pin       = GPIO_PIN_7;
        gpio_init.pull      = GPIO_PULLUP;
        gpio_init.dir       = GPIO_DIR_INPUT;
        gpio_init.de        = GPIO_DIGITAL;
        gpio_init.alternate = GPIO_AF_MAP_Gx(UT0TXMAP_AF, GPIO_AF_G1) | UT0RXMAP_TX;
        gpio_init.af_con    = GPIO_AFEN | GPIO_AFCON0;
        hal_gpio_init(GPIOA_BASE, &gpio_init);
        break;

    case UART1N:
        gpio_init.pin       = GPIO_PIN_4;
        gpio_init.dir       = GPIO_DIR_OUTPUT;
        gpio_init.de        = GPIO_DIGITAL;
        gpio_init.alternate = GPIO_AF_MAP_Gx(UT1TXMAP_AF, GPIO_AF_G2);
        gpio_init.af_con    = GPIO_AFEN | GPIO_AFCON0;
        hal_gpio_init(GPIOA_BASE, &gpio_init);
        
        gpio_init.pin       = GPIO_PIN_3;
        gpio_init.pull      = GPIO_PULLUP;
        gpio_init.dir       = GPIO_DIR_INPUT;
        gpio_init.de        = GPIO_DIGITAL;
        gpio_init.alternate = GPIO_AF_MAP_Gx(UT1RXMAP_AF, GPIO_AF_G2);
        gpio_init.af_con    = GPIO_AFEN | GPIO_AFCON0;
        hal_gpio_init(GPIOA_BASE, &gpio_init);
        /* Interrupt */
        break;

    default:
        break;
    }
}
