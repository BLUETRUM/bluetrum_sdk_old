/* Support file for c based tests */

#include "sfr.h"

.section .reset, "ax"
_start:
    la      a0, __irq_stack_start           //Stack清成0x23
    li      a1, 0x23
    la      a2, __irq_stack_size
    la      x1, memset
    jalr    x1
    la      a0, __comm_vma                  //load comm
    la      a1, __comm_lma
    la      a2, __comm_size
    la      x1, memcpy
    jalr    x1
    la      x1, __irq_stack
    mv      sp, x1
    la      x1, main
    jr      x1

.section .vector,"ax"

    .org    0x40
    jal x0, low_prio_irq

    .org    0x84
    jal x0, _except_84

    .org    0x88
    jal x0, _except_88

    .org    0x8c
    jal x0, _except_8c

    mret

/*EXCEPT_BUSERR*/
_except_8c:
_except_84:
_except_88:
    mret

.macro save_context
	addi	sp, sp, -4 * 31
	/* Save Context */
	sw	x1, 0x0(sp)
	sw	x2, 1 * 4(sp)
	sw	x3, 2 * 4(sp)
	sw	x4, 3 * 4(sp)
	sw	x5, 4 * 4(sp)
	sw	x6, 5 * 4(sp)
	sw	x7, 6 * 4(sp)
	sw	x8, 7 * 4(sp)
	sw	x9, 8 * 4(sp)
	sw	x10, 9 * 4(sp)
	sw	x11, 10 * 4(sp)
	sw	x12, 11 * 4(sp)
	sw	x13, 12 * 4(sp)
	sw	x14, 13 * 4(sp)
	sw	x15, 14 * 4(sp)
	sw	x16, 15 * 4(sp)
	sw	x17, 16 * 4(sp)
	sw	x18, 17 * 4(sp)
	sw	x19, 18 * 4(sp)
	sw	x20, 19 * 4(sp)
	sw	x21, 20 * 4(sp)
	sw	x22, 21 * 4(sp)
	sw	x23, 22 * 4(sp)
	sw	x24, 23 * 4(sp)
	sw	x25, 24 * 4(sp)
	sw	x26, 25 * 4(sp)
	sw	x27, 26 * 4(sp)
	sw	x28, 27 * 4(sp)
	sw	x29, 28 * 4(sp)
	sw	x30, 29 * 4(sp)
	sw	x31, 30 * 4(sp)
	.endm

.macro restore_context
	lw	x1, 0x0(sp)
	lw	x4, 3 * 4(sp)
	lw	x5, 4 * 4(sp)
	lw	x6, 5 * 4(sp)
	lw	x7, 6 * 4(sp)
	lw	x8, 7 * 4(sp)
	lw	x9, 8 * 4(sp)
	lw	x10, 9 * 4(sp)
	lw	x11, 10 * 4(sp)
	lw	x12, 11 * 4(sp)
	lw	x13, 12 * 4(sp)
	lw	x14, 13 * 4(sp)
	lw	x15, 14 * 4(sp)
	lw	x16, 15 * 4(sp)
	lw	x17, 16 * 4(sp)
	lw	x18, 17 * 4(sp)
	lw	x19, 18 * 4(sp)
	lw	x20, 19 * 4(sp)
	lw	x21, 20 * 4(sp)
	lw	x22, 21 * 4(sp)
	lw	x23, 22 * 4(sp)
	lw	x24, 23 * 4(sp)
	lw	x25, 24 * 4(sp)
	lw	x26, 25 * 4(sp)
	lw	x27, 26 * 4(sp)
	lw	x28, 27 * 4(sp)
	lw	x29, 28 * 4(sp)
	lw	x30, 29 * 4(sp)
	lw	x31, 30 * 4(sp)
    addi	sp, sp, 4 * 31
    mret
	.endm

low_prio_irq:
    save_context
    jal cpu_low_irq_comm
    restore_context

    .global my_printf
    .global my_vprintf
    .global my_print_r
    .global my_print_r16
    .global my_print_r32
    .global my_printf_init

    .set    my_printf,          0x84004
    .set    my_vprintf,         0x84008
    .set    my_print_r,         0x8400c
    .set    my_print_r16,       0x84010
    .set    my_print_r32,       0x84014
    .set    my_printf_init,     0x8401c

    .global lock_cache
    .set    lock_cache,         0x84028

    .global sprintf
    .global snprintf
    .set    sprintf,            0x84030
    .set    snprintf,           0x84038

    .global memset
    .global memcpy
    .global memcmp
    .set    memset,             0x84040
    .set    memcpy,             0x84044
    .set    memcmp,             0x84048

    .global strlen
    .global strcpy
    .global strcmp
    .global strcat
    .global strstr
    .global strncpy
    .global strncmp
    .global strncat
    .set    strlen,             0x84050
    .set    strcpy,             0x84054
    .set    strcmp,             0x84058
    .set    strcat,             0x8405c
    .set    strstr,             0x84060
    .set    strncpy,            0x84064
    .set    strncmp,            0x84068
    .set    strncat,            0x8406c

    .global atoi
    .global calc_crc
    .global spi_cs_en
    .global spi_cs_dis
    .set    atoi,               0x84074
    .set    calc_crc,           0x84078
    .set    spi_cs_en,          0x84084
    .set    spi_cs_dis,         0x84088

    .global irtc_write
    .global irtc_read
    .global irtc_time_read
    .global irtc_time_write
    .global irtc_sfr_write
    .global irtc_sfr_read
    .global rtcram_write
    .global rtcram_read
    .set    irtc_write,         0x84090
    .set    irtc_read,          0x84094
    .set    irtc_time_read,     0x84098
    .set    irtc_time_write,    0x8409c
    .set    irtc_sfr_write,     0x840a0
    .set    irtc_sfr_read,      0x840a4
    .set    rtcram_write,       0x840a8
    .set    rtcram_read,        0x840ac

    .global put_be32
    .global put_be16
    .global get_be32
    .global swap32
    .global get_be16
    .global swap16
    .global bitset_cnt
    .set    put_be32,           0x840c0
    .set    put_be16,           0x840c8
    .set    get_be32,           0x840d2
    .set    swap32,             0x840d4
    .set    get_be16,           0x840da
    .set    swap16,             0x840de
    .set    bitset_cnt,         0x840e4

    .global a_printf
    .global a_vprintf
    .global a_print_r
    .global a_print_r16
    .global a_print_r32
    .global a_printf_init

    .set    a_printf,           0x84184
    .set    a_vprintf,          0x84188
    .set    a_print_r,          0x8418c
    .set    a_print_r16,        0x84190
    .set    a_print_r32,        0x84194
    .set    a_printf_init,      0x8419c

    .global tbl_dac_gain
    .set    tbl_dac_gain,       0x847c0

