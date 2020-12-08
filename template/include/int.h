#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "ab32vg1.h"

extern void *tbl_irq_vector[IRQ_TOTAL_NUM];
typedef  void (*isr_t)(void);
isr_t register_isr(int vector, isr_t isr);

void soft_interrupt_init(void);
void timer0_init(void);
void timer1_init(void);
void uart2_init(void);
void src_interrupt_init(void);

#endif // _INTERRUPTS_H
