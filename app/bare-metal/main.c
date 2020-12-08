#include "include.h"

#define TICK_1US 48
#define TICK_1MS (TICK_1US * 1000)
#define TICK_5MS (TICK_1MS * 5)

#define UART_BAUD           1500000
#define UART_BAUD_VAL       (((48000000 + (UART_BAUD / 2)) / UART_BAUD) - 1)

u8 test_buf[512] AT(.buf.test);
extern u32 __bss_start, __bss_size, __aram_start;
extern u32 __comm_vma, __comm_lma, __comm_size;

AT(.com_text.uart)
void uart_putchar(char ch)
{
    while (!(UART0CON & BIT(8)));
    UART0DATA = ch;
}

void rt_kprintf(const char *format, ...)
{
    va_list param;
    va_start(param, format);
    // my_printf(format, param);
    va_end(param);
}

//timer2用于delay函数
void timer2_init(void)
{
    TMR2CON = 0;                                            //select tmr_inc rising edge
    TMR2PR = 0xfffffffful;
    TMR2CNT = 0;
    TMR2CON |= BIT(0);                                      //TMR2 Start
}

AT(.com_text.timer)
u32 tick_get(void)
{
    return TMR2CNT;
}

AT(.com_text.timer)
bool tick_check_expire(u32 tick, u32 expire_val)
{
    return ((u32)(TMR2CNT - tick) >= expire_val);
}

//AT(.com_text.timer)
//void delay_us(uint nus)
//{
//    u32 tick = tick_get();
//    u32 tick_out = TICK_1US * nus;
//    while (!tick_check_expire(tick, tick_out)) {
//    }
//}

AT(.com_text.delay)
void delay_us(uint nus)
{
   int i;
   for (i = 0; i < nus*10; i++) {
        asm("nop");
   }
}

AT(.com_text.timer)
void delay_ms(uint n)
{
    u32 tick = tick_get();
    u32 tick_out = TICK_1MS * n;
    while (!tick_check_expire(tick, tick_out)) {
    }
}

AT(.com_text.timer)
void delay_5ms(uint n)
{
    u32 tick = tick_get();
    u32 tick_out = TICK_5MS * n;
    while (!tick_check_expire(tick, tick_out)) {
    }
}

#define RGB_BAUD    100
u8 rgb_buf[360];   //rgb数据BUF
void rgb_spi1_init(void)
{
    //SPI1_G1: CLK_PA3, DIO_PA4,   //CS_PA5
    GPIOAFEN |= BIT(3)|BIT(4);
    GPIOADE |=  BIT(3)|BIT(4);
    GPIOADIR &= ~(BIT(3) | BIT(4));  //CLK,DO output
    GPIOADIR |= BIT(4);  //DI input
    GPIOAPU |= BIT(4);   //DI pull up 10K
    FUNCMCON1 = (0x0F<<12);
    FUNCMCON1 = (0x01<<12);  //G1

//    //SPI1_G2:  DIO_PA7,  CLK_PA6, //CS_PA5
//    GPIOAFEN |= BIT(6)|BIT(7);
//    GPIOADE |= BIT(6)|BIT(7);
//    GPIOADIR &= ~ BIT(6) ;  //CLK output
//    GPIOADIR |= BIT(7);  //DI input
//    GPIOAPU |= BIT(7);   //DI pull up 10K
//    FUNCMCON1 = (0x0F<<12);
//    FUNCMCON1 = (0x02<<12); //G2

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    SPI1BAUD = 24000000/9 - 1;   //sysclk/baud - 1
    SPI1CON =  0x01 | (1<<2);
    memset(rgb_buf,0xa5,sizeof(rgb_buf));
}

void test_sysclk_out(void)  //default PA3输出，显示需要选择为GUI_NO
{
    static bool init = false;
    if (!init) {
        rgb_spi1_init();
        init = true;
    }
    if (SPI1BAUD != 9) {   //输出频率 = 系统时钟/10
        SPI1BAUD = 9;
    }
//    print_info();
    SPI1DMAADR = DMA_ADR(rgb_buf);
    SPI1DMACNT = sizeof(rgb_buf);
    while(!(SPI1CON & BIT(16))){
        WDT_CLR();
    }
}

void low_prio_irq(void) {}
void soft_irq_init(void) {}
void cpu_irq_comm_do(void) {}

void system_init(void);
void set_sysclk(uint32_t sys_clk);

int main(void)
{
    int i;

    USBCON0 = 0;
    USBCON1 = 0;

    system_init();
    set_sysclk(SYSCLK_120M);

    WDT_DIS();
    timer2_init();

    FUNCMCON0 = (7 << 12) | (1 << 8);                   //RX0 Map To G1(PA7 DP), TX0 Map to RX0
    //UART0BAUD = (UART_BAUD_VAL << 16) | UART_BAUD_VAL;
    memset(&__bss_start, 0, (u32)&__bss_size);          //Clear BSS
    memset(&__aram_start, 0, 0x7a00);                   //Clear aecram, decram, encram
    PICADR = (u32)&__comm_vma;

    timer0_init();
    PICCON |= 0x10003;                                  //LOW PRIO interrupt enable

    //以下是测试代码
    printf("Hello Crown Proj\n");

#ifdef __GNUC__
    printf("__GNUC__\n");
#endif // __GNUC__

    printf("test %%d %%i -123: %d %i\n", -123, -123);
    printf("test %%u 456: %u\n", 456);
    printf("test %%x %%X 0x12ab: %x %X\n", 0x12ab, 0x12ab);

    printf("test %%ld %%li -12345678: %ld %li\n", -12345678, -12345678);
    printf("test %%lu 4567890: %lu\n", 4567890);
    printf("test %%lx %%lX 0xabcd6789: %lx %lX\n", 0xabcd6789, 0xabcd6789);

    printf("test %%4d %%04i -12: %4d %04i\n", -12, -12);
    printf("test %%-03lu 4567: %-03lu\n", 4567);
    printf("test %%-8lx %%08lX 0xabcd: %-8lx %08lX\n", 0xabcd, 0xabcd);

    printf("test %%c RT: %c%c\n", 'R', 'T');
    printf("test %%s: %s\n", "Success");

    for (i=0; i<512; i++) {
        test_buf[i] = i;
    }
    delay_us(1);                //延时1us
    delay_ms(1);                //延时1m
    delay_5ms(1);               //延时5ms
    print_r(test_buf, 64);      //打印数组，8bit格式
    print_r16(test_buf, 64);    //打印数组，16bit格式
    print_r32(test_buf, 64);    //打印数组，32bit格式

#if 1
    //测试PLL 1M 2M的那个问题
    printf("Test PLL");
    PLL0CON |= 1<<7; // PLL0 TEST EN
    PLL0CON |= 0<<8; // OUTPUT SEL,DEFAULT 0
    CLKCON0 |= 0<<19; // SEL PLL0 TS CK OUTPUT
    CLKCON0 |= 1<<22; // OUTPUT ENABLE
    GPIOBDIR &= ~(1<<2) ;// PB2 OUTPUT

    CLKCON0 |= 1<<13; // CLKOUT SEL XOSC 26 M
    //OUPUTPIN: PA5, PA6,PB0,PB1,PE5,PE6
    //IO MAP        1       2      3   4    5     6
    GPIOEDIR &= ~(1<<5); // PE5 OUTPUT
    FUNCMCON1 |= 5<<0; // io MAPPING 5

    //XO26MCON &= ~(0x3<<6); //select XOSC shapping method
    //XO26MCON |= 1<<6;

#endif

    while (1)
    {test_sysclk_out();}
    return 0;
}

int entry(void)
{
    main();
    return 0;
}
