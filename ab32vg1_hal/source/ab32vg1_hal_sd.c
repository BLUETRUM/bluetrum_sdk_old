#include "ab32vg1_hal.h"
#include "ab32vg1_ll_sdio.h"

#ifdef HAL_SD_MODULE_ENABLED

#include <stdbool.h>

#define HAL_LOG(...)     hal_printf(__VA_ARGS__)

/*************************  LL ************************************/

#define CK8E            BIT(11)             //在命令/数据包后加上8CLK
#define CBUSY           BIT(10)             //Busy Check
#define CLRSP           BIT(9)              //17Byte Long Rsp
#define CRSP            BIT(8)              //Need Rsp

//0x40是CMD中的 01 开头。
#define RSP_NO          (0x40 | CK8E)
#define RSP_1           (0x40 | CRSP | CK8E)                //接收6BYTE
#define RSP_1B          (0x40 | CBUSY | CRSP | CK8E)        //接收6BYTE，并等待BUSY
#define RSP_2           (0x40 | CLRSP | CRSP | CK8E)        //接收17BYTE
#define RSP_3           (0x40 | CRSP | CK8E)                //接收6BYTE
#define RSP_6           (0x40 | CRSP | CK8E)                //接收6BYTE
#define RSP_7           (0x40 | CRSP | CK8E)                //接收6BYTE
#define REQ_MULTREAD    (18 | 0x40 | CRSP)                  //多块读时，不需要增加8CLK。其实，多块读先KICK DATA，这个是无所谓的

#define RSP_BUSY_TIMEOUT            2400000     //大约2s
#define RSP_TIMEOUT                 6000        //大约5ms

enum
{
    SDCON = 0, /* [20]:BUSY [19:17]:CRCS [16]:DCRCE [15]:NRPS [1]:Data bus width [0]:SD enable */
    SDCPND,
    SDBAUD,
    SDCMD,
    SDARG3,
    SDARG2,
    SDARG1,
    SDARG0,
    SDDMAADR,
    SDDMACNT,
};

uint8_t sysclk_update_baud(uint8_t baud);

void sdio_setbaud(hal_sfr_t sdiox, uint8_t baud)
{
    sdiox[SDBAUD] = sysclk_update_baud(baud);
}

void sdio_init(hal_sfr_t sdiox, sdio_init_t init)
{
    /* Set clock */
    sdio_setbaud(sdiox, 199);

    sdiox[SDCON] = 0;
    hal_udelay(20);
    sdiox[SDCON] |= BIT(0);     /* SD control enable */
    sdiox[SDCON] |= BIT(3);     /* Keep clock output */
    sdiox[SDCON] |= BIT(5);     /* Data interrupt enable */
    hal_mdelay(40);
}

/**
 * @brief Check if SDIO command is finished.
 *
 * @param sdiox
 * @return true is finished
 * @return false
 */
bool sdio_check_finish(hal_sfr_t sdiox)
{
    if (sdiox[SDCON] & BIT(12)) {
        sdiox[SDCPND] = BIT(12);
        return true;
    }
    return false;
}

/**
 * @brief Check if SDIO has a response.
 *
 * @param sdiox
 * @return true Have a response.
 * @return false No response.
 */
bool sdio_check_rsp(hal_sfr_t sdiox)
{
    return !(sdiox[SDCON] & BIT(15));
}

bool sdio_send_cmd(hal_sfr_t sdiox, uint32_t cmd, uint32_t arg)
{
    uint32_t time_out = (cmd & CBUSY) ? RSP_BUSY_TIMEOUT : RSP_TIMEOUT;
    sdiox[SDARG3] = arg;
    sdiox[SDCMD] = cmd;

    while (sdio_check_finish(sdiox) == false) {
        if (--time_out == 0) {
            HAL_LOG("cmd time out\n");
            // card.abend = 1;
            return false;
        }
    }

    return sdio_check_rsp(sdiox);
}

uint8_t sdio_get_cmd_rsp(hal_sfr_t sdiox)
{
    return sdiox[SDCMD];
}

uint32_t sdio_get_rsp(hal_sfr_t sdiox, uint32_t rsp)
{
    return sdiox[SDARG0];
}

void sdio_read_kick(hal_sfr_t sdiox, void* buf)
{}

void sdio_write_kick(hal_sfr_t sdiox, void* buf)
{}

bool sdio_isbusy(hal_sfr_t sdiox)
{
    return false;
}

bool sdmmc_cmd_go_idle_state(hal_sfr_t sdiox)
{
    // hal_sfr_t sdiox = hsd->instance;
    return sdio_send_cmd(sdiox, 0 | RSP_NO, 0);
}

bool sdmmc_cmd_send_if_cond(hal_sfr_t sdiox)
{
    // hal_sfr_t sdiox = hsd->instance;
    return sdio_send_cmd(sdiox, 8 | RSP_7, SDMMC_CHECK_PATTERM);
}

bool sdmmc_cmd_app(hal_sfr_t sdiox, uint32_t rca)
{
    // hal_sfr_t sdiox = hsd->instance;
    return sdio_send_cmd(sdiox, 55 | RSP_1, rca);
}

bool sdmmc_acmd_op_cond(hal_sfr_t sdiox, uint32_t voltage, uint32_t rca)
{
    if (sdmmc_cmd_app(sdiox, rca)) {
        if (sdio_send_cmd(sdiox, 41 | RSP_3, voltage)) {
            return true;
        }
    }
    return false;
}

/*************************  HAL ************************************/

static bool sd_type_identification(sd_handle_t hsd)
{
    uint8_t retry = 3;
    while (retry-- > 0)
    {
        sdmmc_cmd_go_idle_state(hsd->instance);
        if (sdmmc_cmd_send_if_cond(hsd->instance)) {
            if (sdio_get_cmd_rsp(hsd->instance) == 0x08) {
                hsd->sdcard.type = CARD_V2;
                HAL_LOG("SD 2.0\n");
                return true;
            }
            continue;
        }
        if (sdmmc_acmd_op_cond(hsd->instance, 0x00ff8000, 0u)) {
            hsd->sdcard.type = CARD_V1;
            HAL_LOG("SD 1.0\n");
            return true;
        }
        hal_mdelay(20);
    }
    return false;
}

static bool sd_go_ready_try(sd_handle_t hsd)
{
    uint32_t tmp = 0;
    switch (hsd->sdcard.type)
    {
    case CARD_V1:
        sdmmc_acmd_op_cond(hsd->instance, 0x00ff8000, hsd->sdcard.rca);
        break;
    
    case CARD_V2:
        sdmmc_acmd_op_cond(hsd->instance, 0x40ff8000, hsd->sdcard.rca);
        break;
    default:
        break;
    }

    if (0 == (hsd->instance[SDARG3] & BIT(31))) {
        HAL_LOG("SDARG3=0x%x\n", hsd->instance[SDARG3]);
        return false; // no ready
    }

    if ((hsd->sdcard.type == CARD_V2) && (hsd->instance[SDARG3] & BIT(30))) {
        HAL_LOG("SDHC\n");
    }

    return true;
}

static bool sd_go_ready(sd_handle_t hsd)
{
    if (hsd->sdcard.type == CARD_INVAL) {
        return false;
    }

    uint8_t retry = 10;
    while (retry-- > 0)
    {
        if (sd_go_ready_try(hsd) == true) {
            return true;
        }
        hal_mdelay(20);
    }

    return false;
}

static bool sd_poweron(sd_handle_t hsd)
{
    if (sd_type_identification(hsd) == false) {
        HAL_LOG("card invalid\n");
        return false;
    }

    if (sd_go_ready(hsd) == false) {
        HAL_LOG("no ready\n");
        return false;
    }

    return true;
}

void hal_sd_initcard(sd_handle_t hsd)
{
    struct sdio_init init = {0};
    hal_sfr_t sdiox = hsd->instance;

    sdio_init(sdiox, &init);
    sd_poweron(hsd);
}

WEAK void hal_sd_mspinit(sd_handle_t hsd)
{
}

hal_error_t hal_sd_init(sd_handle_t hsd)
{
    if (hsd == HAL_NULL) {
        return -HAL_ERROR;
    }

    hal_sd_mspinit(hsd);
    hal_sd_initcard(hsd);

    return -HAL_ERROR;
}

void hal_sd_deinit(uint32_t sdx)
{
}

#endif
