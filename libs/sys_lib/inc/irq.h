/* Copyright (c) 2019-2020, corelink inc., www.corelink.vip
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-12     corelink     the first version
 */


#ifndef __IRQ_H__
#define __IRQ_H__


#ifdef __cplusplus
extern "C" {
#endif


/* Number of interrupt handlers - really depends on PIC width in OR1200*/
#define MAX_INT_HANDLERS  32


/**
 * \brief Disables interrupts globally.
 * \param void
 * \return void
 *
 * By writing 1 to the ie (interruptenable) bit
 * interrupts are globally disable.
 */
static inline void int_disable(void)
{
	// read-modify-write
	int mstatus;
	asm volatile ("csrr %0, mstatus": "=r" (mstatus));
	mstatus &= 0xFFFFFFF7;
	asm volatile ("csrw mstatus, %0" : /* no output */ : "r" (mstatus));
	asm("csrw 0x300, %0" : : "r" (0x0) );
}

/**
 * \brief Enables interrupts globally.
 * \param void
 * \return void
 *
 * By writing 1 to the ie (interruptenable) bit
 * interrupts are globally enabled.
 */
static inline void int_enable(void)
{
	// read-modify-write
	int mstatus;
	asm volatile ("csrr %0, mstatus": "=r" (mstatus));
	mstatus |= 0x08;
	asm volatile ("csrw mstatus, %0" : /* no output */ : "r" (mstatus));
}

typedef void (*irq_handler_t)(void);
int request_irq(int irq, irq_handler_t handler);

#define DEFINE_DEFAULT_IRQ_HANDLER(irq_handler) \
		__attribute__((weak)) void irq_handler(void) { for (; ;) ; }
#define DEFINE_DEFAULT_EXCEPTION_HANDLER(exception_handler) \
		__attribute__((interrupt)) __attribute__((weak)) void exception_handler(void) { for (; ;) ; }
#define DEFINE_IRQ_HANDLER(irq, driver_irq_handler) \
		void irq##_handler(void) { driver_irq_handler(); }

/*
 * defining all interrupt handelrs, these functions can be redefined by users
 */

/* 00 */
void default_irq_handler(void);
/* 01 */
void default_irq_handler(void);
/* 02 */
void I2C5_IRQ_handler(void);
/* 03 */
void I2C4_IRQ_handler(void);
/* 04 */
void I2C3_IRQ_handler(void);
/* 05 */
void I2C2_IRQ_handler(void);
/* 06 */
void I2C1_IRQ_handler(void);
/* 07 */
void I2C0_IRQ_handler(void);
/* 08 */
void ADCH_IRQ_handler(void);
/* 09 */
void ADCF_IRQ_handler(void);
/* 10 */
void RTC_IRQ_handler(void);
/* 11 */
void CAL32M_IRQ_handler(void);
/* 12 */
void CAL32K_IRQ_handler(void);
/* 13 */
void UART5_IRQ_handler(void);
/* 14 */
void UART4_IRQ_handler(void);
/* 15 */
void UART3_IRQ_handler(void);
/* 16 */
void UART2_IRQ_handler(void);
/* 17 */ 
void UART1_IRQ_handler(void);
/* 18 */
void UART0_IRQ_handler(void);
/* 19 */
void GPIO_IRQ_handler(void);
/* 20 */
void SPIM1A_IRQ_handler(void);
/* 21 */
void SPIM1B_IRQ_handler(void);
/* 22 */
void SPIM0A_IRQ_handler(void);
/* 23 */
void SPIM0B_IRQ_handler(void);
/* 24 */
void TIMER0_IRQ_handler(void);
/* 25 */
void TIMER1_IRQ_handler(void);
/* 26 */
void TIMER2_IRQ_handler(void);
/* 27 */
void TIMER3_IRQ_handler(void);
/* 28 */
void default_irq_handler(void);
/* 29 */
void default_irq_handler(void);
/* 30 */
void default_irq_handler(void);
/* 31 */
void default_irq_handler(void);


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef __IRQ_H__ */


