/*
 * Copyright (c) 2020-2020, BLUETRUM Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef AB32VG1_HAL_MCI_H__
#define AB32VG1_HAL_MCI_H__

#include "ab32vg1_hal_def.h"

#define SD0N            (0x00u)

/****** MCI Send Command Flags *****/
#define HAL_MCI_RESPONSE_Pos             0ul
#define HAL_MCI_RESPONSE_Msk            (3UL << HAL_MCI_RESPONSE_Pos)
#define HAL_MCI_RESPONSE_NONE           (0UL << HAL_MCI_RESPONSE_Pos)   ///< No response expected (default)
#define HAL_MCI_RESPONSE_SHORT          (1UL << HAL_MCI_RESPONSE_Pos)   ///< Short response (48-bit)
#define HAL_MCI_RESPONSE_SHORT_BUSY     (2UL << HAL_MCI_RESPONSE_Pos)   ///< Short response with busy signal (48-bit)
#define HAL_MCI_RESPONSE_LONG           (3UL << HAL_MCI_RESPONSE_Pos)   ///< Long response (136-bit)

#define HAL_MCI_RESPONSE_INDEX          (1UL << 2)  ///< Check command index in response
#define HAL_MCI_RESPONSE_CRC            (1UL << 3)  ///< Check CRC in response

#define HAL_MCI_WAIT_BUSY               (1UL << 4)  ///< Wait until busy before sending the command

#define HAL_MCI_TRANSFER_DATA           (1UL << 5)  ///< Activate Data transfer

#define HAL_MCI_CARD_INITIALIZE         (1UL << 6)  ///< Execute Memory Card initialization sequence

#define HAL_MCI_INTERRUPT_COMMAND       (1UL << 7)  ///< Send Interrupt command (CMD40 - MMC only)
#define HAL_MCI_INTERRUPT_RESPONSE      (1UL << 8)  ///< Send Interrupt response (CMD40 - MMC only)

#define HAL_MCI_BOOT_OPERATION          (1UL << 9)  ///< Execute Boot operation (MMC only)
#define HAL_MCI_BOOT_ALTERNATIVE        (1UL << 10) ///< Execute Alternative Boot operation (MMC only)
#define HAL_MCI_BOOT_ACK                (1UL << 11) ///< Expect Boot Acknowledge (MMC only)

#define HAL_MCI_CCSD                    (1UL << 12) ///< Send Command Completion Signal Disable (CCSD) for CE-ATA device
#define HAL_MCI_CCS                     (1UL << 13) ///< Expect Command Completion Signal (CCS) for CE-ATA device

/****** MCI Setup Transfer Mode *****/
#define HAL_MCI_TRANSFER_READ           (0UL << 0)  ///< Data Read Transfer (from MCI)
#define HAL_MCI_TRANSFER_WRITE          (1UL << 0)  ///< Data Write Transfer (to MCI)
#define HAL_MCI_TRANSFER_BLOCK          (0UL << 1)  ///< Block Data transfer (default)
#define HAL_MCI_TRANSFER_STREAM         (1UL << 1)  ///< Stream Data transfer (MMC only)

/* MCI control */
#define HAL_MCI_BUS_SPEED                    (0x01ul)
#define HAL_MCI_BUS_SPEED_MODE               (0x02ul)    /*!< Set the Bus Speed Mode.    */
#define HAL_MCI_BUS_CMD_MODE                 (0x03ul)    /*!< Set the CMD Line Mode.     */
#define HAL_MCI_BUS_DATA_WIDTH               (0x04ul)    /*!< Set data bus width.        */
#define HAL_MCI_DRIVER_STRENGTH              (0x05ul)    /*!< Set driver strength.       */
#define HAL_MCI_CONTROL_RESET                (0x06ul)
#define HAL_MCI_CONTROL_CLOCK_IDLE           (0x07ul)
#define HAL_MCI_UHS_TUNING_OPERATION	        (0x08ul)
#define HAL_MCI_UHS_TUNING_RESULT            (0x09ul)
#define HAL_MCI_DATA_TIMEOUT                 (0x0Aul)
#define HAL_MCI_CSS_TIMEOUT                  (0x0Bul)
#define HAL_MCI_MONITOR_SDIO_INTERRUPT       (0x0Cul)
#define HAL_MCI_CONTROL_READ_WAIT            (0x0Dul)
#define HAL_MCI_SUSPEND_TRANSFER             (0x0Eul)
#define HAL_MCI_RESUME_TRANSFER              (0x0Ful)

/* MCI arg */
/* Bus Speed Mode */
#define HAL_MCI_BUS_DEFAULT_SPEED            (0x00ul)    /*!< Default Speed mode up to [25;26]MHz    */
#define HAL_MCI_BUS_HIGH_SPEED               (0x01ul)    /*!< High Speed mode up to [50;52]MHz       */
#define HAL_MCI_BUS_UHS_SDR12                (0x02ul)
#define HAL_MCI_BUS_UHS_SDR25                (0x03ul)
#define HAL_MCI_BUS_UHS_SDR50                (0x04ul)
#define HAL_MCI_BUS_UHS_SDR104               (0x05ul)
#define HAL_MCI_BUS_UHS_DDR50                (0x06ul)

/* Bus CMD Line Mode */
#define HAL_MCI_BUS_CMD_PUSH_PULL            (0x00ul)    /*!< Set the Push-Pull CMD line             */
#define HAL_MCI_BUS_CMD_OPEN_DRAIN           (0x01ul)    /*!< Set the Open Drain CMD line (MMC only) */

/* Bus Data Width */
#define HAL_MCI_BUS_DATA_WIDTH_1             (0x00ul)
#define HAL_MCI_BUS_DATA_WIDTH_4             (0x01ul)
#define HAL_MCI_BUS_DATA_WIDTH_8             (0x02ul)

/* Driver Type */
#define HAL_MCI_DRIVER_TYPE_A                (0x01ul)    /*!< SD UHS-I Driver Type A.    */
#define HAL_MCI_DRIVER_TYPE_B                (0x00ul)    /*!< SD UHS-I Driver Type B.    */
#define HAL_MCI_DRIVER_TYPE_C                (0x02ul)    /*!< SD UHS-I Driver Type C.    */
#define HAL_MCI_DRIVER_TYPE_D                (0x03ul)    /*!< SD UHS-I Driver Type D.    */

/****** MCI Card Power *****/
#define HAL_MCI_POWER_VDD_Pos                 0
#define HAL_MCI_POWER_VDD_Msk                (0x0FUL << HAL_MCI_POWER_VDD_Pos)
#define HAL_MCI_POWER_VDD_OFF                (0x01UL << HAL_MCI_POWER_VDD_Pos)   ///< VDD (VCC) turned off
#define HAL_MCI_POWER_VDD_3V3                (0x02UL << HAL_MCI_POWER_VDD_Pos)   ///< VDD (VCC) = 3.3V
#define HAL_MCI_POWER_VDD_1V8                (0x03UL << HAL_MCI_POWER_VDD_Pos)   ///< VDD (VCC) = 1.8V
#define HAL_MCI_POWER_VCCQ_Pos                4
#define HAL_MCI_POWER_VCCQ_Msk               (0x0FUL << HAL_MCI_POWER_VCCQ_Pos)
#define HAL_MCI_POWER_VCCQ_OFF               (0x01UL << HAL_MCI_POWER_VCCQ_Pos)  ///< eMMC VCCQ turned off
#define HAL_MCI_POWER_VCCQ_3V3               (0x02UL << HAL_MCI_POWER_VCCQ_Pos)  ///< eMMC VCCQ = 3.3V
#define HAL_MCI_POWER_VCCQ_1V8               (0x03UL << HAL_MCI_POWER_VCCQ_Pos)  ///< eMMC VCCQ = 1.8V
#define HAL_MCI_POWER_VCCQ_1V2               (0x04UL << HAL_MCI_POWER_VCCQ_Pos)  ///< eMMC VCCQ = 1.2V

struct hal_mci_status
{
    uint32_t	command_active: 1;  //	Command active flag.
    uint32_t	command_timeout: 1; //	Command timeout flag (cleared on start of next command)
    uint32_t	command_error: 1;   //	Command error flag (cleared on start of next command)
    uint32_t	transfer_active: 1; //	Transfer active flag.
    uint32_t	transfer_timeout: 1;//	Transfer timeout flag (cleared on start of next command)
    uint32_t	transfer_error: 1;  //	Transfer error flag (cleared on start of next command)
    uint32_t	sdio_interrupt: 1;  //	SD I/O Interrupt flag (cleared on start of monitoring)
    uint32_t	ccs: 1;             //	CCS flag (cleared on start of next command)
};

/* SD events */
#define HAL_MCI_EVENT_CARD_INSERTED          (1ul << 0)  /*!< SD Card inserted.  */
#define HAL_MCI_EVENT_CARD_REMOVED           (1ul << 1)  /*!< SD Card removed.   */
#define HAL_MCI_EVENT_COMMAND_COMPLETE       (1ul << 2)  /*!< Command completed. */
#define HAL_MCI_EVENT_COMMAND_TIMEOUT        (1ul << 3)  /*!< Command timeout.   */
#define HAL_MCI_EVENT_COMMAND_ERROR          (1ul << 4)
#define HAL_MCI_EVENT_TRANSFER_COMPLETE      (1ul << 5)
#define HAL_MCI_EVENT_TRANSFER_TIMEOUT       (1ul << 6)
#define HAL_MCI_EVENT_TRANSFER_ERROR         (1ul << 7)
#define HAL_MCI_EVENT_SDIO_INTERRUPT         (1ul << 8)
#define HAL_MCI_EVENT_CCS                    (1ul << 9)
#define HAL_MCI_EVENT_CCS_TIMEOUT            (1ul << 10)

/* struct */
struct sd_init
{

};

struct sd_handle
{
    uint32_t            instance;
    struct sd_init      init;
};
typedef struct sd_handle* sd_handle_t;

struct hal_mci_capabilities
{
    uint32_t cd_state          : 1;       ///< Card Detect State available
    uint32_t cd_event          : 1;       ///< Signal Card Detect change event
    uint32_t wp_state          : 1;       ///< Write Protect State available
    uint32_t vdd               : 1;       ///< Supports VDD Card Power Supply Control
    uint32_t vdd_1v8           : 1;       ///< Supports 1.8 VDD Card Power Supply
    uint32_t vccq              : 1;       ///< Supports VCCQ Card Power Supply Control (eMMC)
    uint32_t vccq_1v8          : 1;       ///< Supports 1.8 VCCQ Card Power Supply (eMMC)
    uint32_t vccq_1v2          : 1;       ///< Supports 1.2 VCCQ Card Power Supply (eMMC)
    uint32_t data_width_4      : 1;       ///< Supports 4-bit data
    uint32_t data_width_8      : 1;       ///< Supports 8-bit data
    uint32_t data_width_4_ddr  : 1;       ///< Supports 4-bit data, DDR (Dual Data Rate) - MMC only
    uint32_t data_width_8_ddr  : 1;       ///< Supports 8-bit data, DDR (Dual Data Rate) - MMC only
    uint32_t high_speed        : 1;       ///< Supports SD/MMC High Speed Mode
    uint32_t uhs_signaling     : 1;       ///< Supports SD UHS-I (Ultra High Speed) 1.8V signaling
    uint32_t uhs_tuning        : 1;       ///< Supports SD UHS-I tuning
    uint32_t uhs_sdr50         : 1;       ///< Supports SD UHS-I SDR50  (Single Data Rate) up to  50MB/s
    uint32_t uhs_sdr104        : 1;       ///< Supports SD UHS-I SDR104 (Single Data Rate) up to 104MB/s
    uint32_t uhs_ddr50         : 1;       ///< Supports SD UHS-I DDR50  (Dual   Data Rate) up to  50MB/s
    uint32_t uhs_driver_type_a : 1;       ///< Supports SD UHS-I Driver Type A
    uint32_t uhs_driver_type_c : 1;       ///< Supports SD UHS-I Driver Type C
    uint32_t uhs_driver_type_d : 1;       ///< Supports SD UHS-I Driver Type D
    uint32_t sdio_interrupt    : 1;       ///< Supports SD I/O Interrupt
    uint32_t read_wait         : 1;       ///< Supports Read Wait (SD I/O)
    uint32_t suspend_resume    : 1;       ///< Supports Suspend/Resume (SD I/O)
    uint32_t mmc_interrupt     : 1;       ///< Supports MMC Interrupt
    uint32_t mmc_boot          : 1;       ///< Supports MMC Boot
    uint32_t rst_n             : 1;       ///< Supports RST_n Pin Control (eMMC)
    uint32_t ccs               : 1;       ///< Supports Command Completion Signal (CCS) for CE-ATA
    uint32_t ccs_timeout       : 1;       ///< Supports Command Completion Signal (CCS) timeout for CE-ATA
};

typedef void (*hal_mci_signal_event_t)(uint32_t event);

struct hal_drv_mci
{
    struct hal_mci_capabilities (*get_capabilities)(void);
    int32_t (*init)(hal_mci_signal_event_t cb_event);
    int32_t (*deinit)(void);
    int32_t (*power_control)(enum hal_power_state state);
    int32_t (*card_power)(uint32_t voltage);
    int32_t (*read_cd)(void);
    int32_t (*read_wp)(void);
    int32_t (*send_cmd)(uint32_t cmd, uint32_t arg, uint32_t flags, uint32_t *response);
    int32_t (*setup_tx)(uint8_t *data, uint32_t block_count, uint32_t block_size, uint32_t mode);
    int32_t (*abort_tx)(void);
    int32_t (*control)(uint32_t control, uint32_t arg);
    struct hal_mci_status (*get_status)(void);
};

extern struct hal_drv_mci drv_mci;

/* Initialization functions */
hal_error_t hal_sd_init(sd_handle_t hsd);
void hal_sd_deinit(uint32_t sdx);
void hal_sd_mspinit(uint32_t hsd);

hal_error_t hal_sd_control(uint32_t control, uint32_t arg);
void hal_sd_write(uint32_t sdx, uint32_t data);
uint32_t hal_sd_read(uint32_t sdx);
// void hal_uart_write_it(uint32_t uartx, uint8_t data);
// uint8_t hal_uart_read_it(uint32_t uartx);

#endif
