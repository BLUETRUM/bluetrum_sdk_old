#include "ab32vg1_hal.h"
#include <string.h>

//#define DBG_SD(...)
#define DBG_SD(...)     hal_printf(__VA_ARGS__)

/*****************************************************/

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


// #define HAL_SD_DRV_VERSION    HAL_DRIVER_VERSION_MAJOR_MINOR(1, 0) /* driver version */

// /* Driver Version */
// static const struct hal_drv_version DriverVersion = {
//     HAL_SD_API_VERSION,
//     HAL_SD_DRV_VERSION
// };

#define MCI_INIT            ((uint8_t)(1 << 0))
#define MCI_POWER           ((uint8_t)(1 << 1))
#define MCI_SETUP           ((uint8_t)(1 << 2))
#define MCI_RESP_LONG       ((uint8_t)(1 << 3))
#define MCI_RESP_CRC        ((uint8_t)(1 << 4))
#define MCI_DATA_XFER       ((uint8_t)(1 << 5))
#define MCI_DATA_READ       ((uint8_t)(1 << 6))
#define MCI_DATA_WAIT       ((uint8_t)(1 << 7))

struct mci_xfer
{
    uint8_t *buf;                       /* Data buffer                          */
    uint32_t cnt;                       /* Data bytes to transfer               */
};

struct mci_info
{
    hal_mci_signal_event_t   cb_event;   /* Driver event callback function     */
    struct hal_mci_status    status;     /* Driver status                      */
    uint32_t               *response;   /* Pointer to response buffer         */
    struct mci_xfer          xfer;       /* Data transfer description          */
    uint8_t volatile        flag;       /* Driver state flags                 */
    uint32_t                dctrl;      /* Data control register value        */
    uint32_t                dlen;       /* Data length register value         */
};
static struct mci_info _mci;

static struct hal_mci_capabilities driver_cap =
{
    .cd_state           = 1u,
    .sdio_interrupt     = 1u,
    .read_wait          = 1u,
    .suspend_resume     = 1u,
};

/*************************************************************************/

uint8_t sysclk_update_baud(uint8_t baud);

hal_bool_t sdio_cmd_pending(hal_sfr_t sdiox)
{
    if (sdiox[SDCON] & BIT(12)) {
        sdiox[SDCPND] = BIT(12);                                  //CLR PND
        return hal_false;
    }
    return hal_true;
}

hal_bool_t sdio_send_cmd(hal_sfr_t sdiox, uint32_t req, uint32_t arg)
{
    uint32_t time_out = (req & CBUSY) ? RSP_BUSY_TIMEOUT : RSP_TIMEOUT;

    DBG_SD("REQ: %02d, %08x %08x\n", req&0x3f, req, arg);
    sdiox[SDARG3] = arg;
    sdiox[SDCMD] = req;
    while (sdio_cmd_pending(sdiox)) {                            //Wait PND
        if (--time_out == 0) {
            DBG_SD("cmd time out\n");
            // sdcb->card.abend = 1;
            return hal_false;
        }
    }
/*
printf("CMD: %08x\n", sdcb->sfr[SDCMD]);
printf("ARG3: %08x\n", sdcb->sfr[SDARG3]);
printf("ARG2: %08x\n", sdcb->sfr[SDARG2]);
printf("ARG1: %08x\n", sdcb->sfr[SDARG1]);
printf("ARG0: %08x\n\n", sdcb->sfr[SDARG0]);
*/
    return !(sdiox[SDCON] & BIT(15));
}

uint32_t sdio_get_rsp(hal_sfr_t sdiox, uint32_t response)
{
    return sdiox[SDCMD];
}

static void sdio_setbaud(hal_sfr_t sdiox, uint8_t baud)
{
    sdiox[SDBAUD] = sysclk_update_baud(baud);
}

void sdcmd_go_idle_state(hal_sfr_t sdiox)
{
    sdio_send_cmd(sdiox, 0 | RSP_NO, 0);
}

hal_bool_t sdcmd_send_if_cond(hal_sfr_t sdcb)
{
    hal_bool_t ret = sdio_send_cmd(sdcb, 8 | RSP_7, 0x000001AA);
    
    return ret;
}

/********************************************************************/

void sd_initcard(hal_sfr_t sdiox)
{
    sdio_setbaud(sdiox, 199);

    sdiox[SDCON] = 0;
    hal_udelay(20);
    sdiox[SDCON] |= BIT(0);     /* SD control enable */
    sdiox[SDCON] |= BIT(3);     /* Keep clock output */
    sdiox[SDCON] |= BIT(5);     /* Data interrupt enable */
    hal_mdelay(40);
}

uint32_t sd_poweron(hal_sfr_t sdiox)
{
    sdcmd_go_idle_state(sdiox);
    sdcmd_send_if_cond(sdiox);
    return 0;
}

void sd_read_blocks()
{}

void sd_write_blocks()
{}

void sd_get_card_cid()
{}

void sd_get_card_csd()
{}

/*************************************************************************/

static struct hal_mci_capabilities mci_get_capabilities(void)
{
    return driver_cap;
}

static int32_t mci_init(hal_mci_signal_event_t cb_event)
{
//    hal_sfr_t sdiox = &SD0CON;

    memset(&_mci, 0, sizeof(struct mci_info));
    _mci.cb_event   = cb_event;
    _mci.flag       = MCI_INIT;

    return 0;
}

static int32_t mci_deinit(void)
{
    return 0;
}

static int32_t mci_power_control(enum hal_power_state state)
{
    hal_sfr_t sdiox = &SD0CON;
    int32_t status = HAL_EOK;

    switch (state)
    {
    case HAL_POWER_OFF:
        // gpio_deinit
        _mci.status.command_active      = 0u;
        _mci.status.command_timeout     = 0u;
        _mci.status.command_error       = 0u;
        _mci.status.transfer_active     = 0u;
        _mci.status.transfer_error      = 0u;
        _mci.status.sdio_interrupt      = 0u;
        _mci.status.ccs                 = 0u;

        _mci.flag &= ~MCI_POWER;
        break;

    case HAL_POWER_FULL:
        if ((_mci.flag & MCI_INIT)  == 0u) {
            return -HAL_ERROR;
        }
        if ((_mci.flag & MCI_POWER) != 0u) {
            return HAL_EOK;
        }
        hal_printf("HAL_POWER_FULL\n");

        _mci.response = HAL_NULL;
        _mci.xfer.cnt = 0u;

        hal_sd_mspinit(SD0N);
        sd_initcard(sdiox);

        sd_poweron(sdiox);
    default:
        status = -HAL_EINVAL;
    }

    return status;
}

static int32_t mci_card_power(uint32_t voltage)
{
    return 0;
}

static int32_t mci_read_cd(void)
{
    return 0;
}

static int32_t mci_read_wp(void)
{
    return 0;
}

static int32_t mci_send_cmd(uint32_t cmd, uint32_t arg, uint32_t flags, uint32_t *response)
{
    _mci.status.command_active  = 1u;

    // sdio_send_cmd();
    return 0;
}

static int32_t mci_setup_tx(uint8_t *data, uint32_t block_count, uint32_t block_size, uint32_t mode)
{
    return 0;
}

static int32_t mci_abort_tx(void)
{
    return 0;
}

static int32_t mci_control (uint32_t control, uint32_t arg)
{
    return 0;
}

static struct hal_mci_status mci_get_status(void)
{
    return _mci.status;
}

struct hal_drv_mci drv_mci =
{
    .get_capabilities   = mci_get_capabilities,
    .init               = mci_init,
    .deinit             = mci_deinit,
    .power_control      = mci_power_control,
    .card_power         = mci_card_power,
    .read_cd            = mci_read_cd,
    .read_wp            = mci_read_wp,
    .send_cmd           = mci_send_cmd,
    .setup_tx           = mci_setup_tx,
    .abort_tx           = mci_abort_tx,
    .control            = mci_control,
    .get_status         = mci_get_status,
};
