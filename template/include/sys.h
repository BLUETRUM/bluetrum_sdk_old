#ifndef _SYS_H
#define _SYS_H

#define VOL_MAX         16

typedef struct {
    u8 min;
    u8 sec;
} msc_time_t;

typedef struct {
    u8 pause;
    u8 play_mode;
    u8 vol;
    u8 music_type;
    u8 file_change;
    u32 total_time;
    u8 cur_dev;
    msc_time_t curtime;
} sys_cb_t;
extern sys_cb_t sys_cb;

void uart_putchar(char ch);
void uart1_putchar(char ch);
#endif
