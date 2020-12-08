#include "include.h"

void *tbl_irq_vector[IRQ_TOTAL_NUM] AT(.buf.irq_tbl);

AT(.com_text.isr)
isr_t register_isr(int vector, isr_t isr)
{
	isr_t old = tbl_irq_vector[vector];
	tbl_irq_vector[vector] = isr;
	return old;
}

AT(.com_text.isr)
void cpu_low_irq_comm(void)
{
	void (*pfnct)(void);
	for (int i = 0; i < IRQ_TOTAL_NUM; i++) {
        if (PICPND & BIT(i)) {
            pfnct = tbl_irq_vector[i];
            if (pfnct) {
                pfnct();				/* call ISR */
            }
        }
	}
}

//注册一个系统中断
bool sys_irq_init(int vector, int pr, isr_t isr)
{
    u32 irq_ins;
    u32 *irq_adr;
    u32 jumpoffset;
    if (vector >= IRQ_TOTAL_NUM) {
        return false;
    }
    if (pr == 0) {
        register_isr(vector, isr);
        PICPR &= ~BIT(vector);
        PICEN |= BIT(vector);
    } else {
        irq_adr = (u32 *)(PICADR + 0x80 + (vector * 0x10));
        jumpoffset = (u32)isr - (u32)irq_adr;
        if (jumpoffset >= (1 << 20)) {
            return false;
        }
        irq_ins = ((jumpoffset & 0x100000) << 11) | ((jumpoffset & 0x7fe) << 20) | ((jumpoffset & 0x800) << 9) | (jumpoffset & 0xff000) | 0x0ef;
        *irq_adr = irq_ins;
        PICPR |= BIT(vector);
        PICEN |= BIT(vector);
    }
    return true;
}

//timer0 1ms interrupt
AT(.com_text.isr)
void timer0_isr(void)
{
    static uint tick_cnt = 0;
    TMR0CPND = BIT(9);              //Clear Pending
    tick_cnt++;

    if ((tick_cnt % 5) == 0) {      //5ms

    }

    if ((tick_cnt % 1000) == 0) {   //1s
        tick_cnt = 0;
        printf(".");
    }
}

void timer0_init(void)
{
//    register_isr(IRQ_TMR0_VECTOR, timer0_isr);
	TMR0CON =  BIT(7); //TIE
	TMR0CNT = 0;
//	TMR0PR  = 48000 - 1;       //1ms interrupt
	TMR0PR  = 120000 - 1;       //1ms interrupt
	TMR0CON |= BIT(0); // EN
//	PICPR &= ~BIT(IRQ_TMR0_VECTOR);
	PICPR |= BIT(IRQ_TMR0_VECTOR);
	PICEN |= BIT(IRQ_TMR0_VECTOR);
	PICCON |= BIT(2);

	sys_irq_init(IRQ_TMR0_VECTOR, 1, timer0_isr);
}
