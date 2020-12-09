#include "ab32vg1_hal.h"

// enum
// {
//     SDCON = 0, /* [20]:BUSY [19:17]:CRCS [16]:DCRCE [15]:NRPS [1]:Data bus width [0]:SD enable */
//     SDCPND,
//     SDBAUD,
//     SDCMD,
//     SDARG3,
//     SDARG2,
//     SDARG1,
//     SDARG0,
//     SDDMAADR,
//     SDDMACNT,
// };

// uint8_t sysclk_update_baud(uint8_t baud);

// void sdio_setbaud(hal_sfr_t sdiox, uint8_t baud)
// {
//     sdiox[SDBAUD] = sysclk_update_baud(baud);
// }

// void sdio_init(hal_sfr_t sdiox, sdio_init_t init)
// {
//     sdiox[SDCON] = 0;
//     hal_udelay(20);
//     sdiox[SDCON] |= BIT(0);     /* SD control enable */
//     sdio_setbaud(sdiox, 199);
//     sdiox[SDCON] |= BIT(3);     /* Keep clock output */
//     sdiox[SDCON] |= BIT(5);     /* Data interrupt enable */
//     hal_mdelay(40);
// }


