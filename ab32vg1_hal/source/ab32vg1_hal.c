#include "ab32vg1_hal.h"

static uint32_t hw_ticks = 0;

void timer0_cfg(uint32_t ticks);

static void (*tick_cfg_hook)(uint32_t ticks) = HAL_NULL;

void hal_set_tick_hook(void (*hook)(uint32_t ticks))
{
    tick_cfg_hook = hook;
}

void hal_set_ticks(uint32_t ticks)
{
    if (ticks != hw_ticks) {
        hw_ticks = ticks;
    }
    if (tick_cfg_hook != HAL_NULL) {
        tick_cfg_hook(hw_ticks);
    }
}

WEAK void hal_mdelay(uint32_t nms)
{

}

void hal_udelay(uint16_t nus)
{
   int i;
   for (i = 0; i < nus*10; i++) {
        asm("nop");
   }
}

WEAK void hal_printf(const char *format, ...)
{}

char *strrchr (const char * string, int ch)
{
	char *start = (char *)string;

	while (*string++)                       /* find end of string */
			;
											/* search towards front */
	while (--string != start && *string != (char)ch)
			;

	if (*string == (char)ch)                /* char found ? */
			return( (char *)string );

	return(HAL_NULL);
}

