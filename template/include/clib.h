#ifndef _CLIB_H
#define _CLIB_H

#define printf(...)                     my_printf(__VA_ARGS__)
#define vprintf(...)                    my_vprintf(__VA_ARGS__)
#define print_r(...)                    my_print_r(__VA_ARGS__)
#define print_r16(...)                  my_print_r16(__VA_ARGS__)
#define print_r32(...)                  my_print_r32(__VA_ARGS__)

void my_printf_init(void (*putchar)(char));
void my_printf(const char *format, ...);
void my_vprintf(const char *format, va_list param);

void my_print_r(const void *buf, uint cnt);
void my_print_r16(const void *buf, uint cnt);
void my_print_r32(const void *buf, uint cnt);

int sprintf(char *buffer, const char *format, ...);
int vsprintf(char *buffer, const char *format, va_list param);
int snprintf(char *buffer, uint maxlen, const char *format, ...);
int vsnprintf(char *buffer, uint maxlen, const char *format, va_list param);

u32 swap32(u32 val);
u16 swap16(u16 val);
uint get_be16(void *ptr);
u32 get_be32(void *ptr);
void put_be16(void *ptr, uint val);
void put_be32(void *ptr, u32 val);

uint bitset_cnt(u32 val);

void irtc_sfr_write(u32 cmd, u8 dat);
u8 irtc_sfr_read(u32 cmd);

u32 irtc_time_read(u32 cmd);
void irtc_time_write(u32 cmd, u32 dat);

void rtcram_write(u8 *buf, u8 addr, u8 cnt);
void rtcram_read(u8 *buf, u8 addr, u8 cnt);

void delay_us(uint nus);
void delay_ms(uint n);
void delay_5ms(uint n);

#endif
