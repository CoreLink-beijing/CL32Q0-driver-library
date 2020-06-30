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


#include <stdio.h>
#include "soc_hal.h"
#include "irq.h"
#include "utils.h"


/* */
void default_irq_handler(void){ for(;;); }

/*
 * we can overwrite this function to provide custom exception handlers
 */
DEFINE_DEFAULT_EXCEPTION_HANDLER(default_exception_handler_c)
DEFINE_DEFAULT_EXCEPTION_HANDLER(illegal_insn_handler_c)
DEFINE_DEFAULT_EXCEPTION_HANDLER(ecall_insn_handler_c)

/*
 * defining all interrupt handlers, these functions can be redefined by user
 */
DEFINE_DEFAULT_IRQ_HANDLER(I2C5_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(I2C4_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(I2C3_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(I2C2_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(I2C1_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(I2C0_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(ADCH_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(ADCF_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(RTC_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(CAL32M_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(CAL32K_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(UART5_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(UART4_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(UART3_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(UART2_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(UART1_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(UART0_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(GPIO_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(SPIM1A_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(SPIM1B_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(SPIM0A_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(SPIM0B_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(TIMER0_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(TIMER1_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(TIMER2_IRQ_handler)
DEFINE_DEFAULT_IRQ_HANDLER(TIMER3_IRQ_handler)


static irq_handler_t isr_table[] =
{
	default_irq_handler,		//  0: reserved
	default_irq_handler,		//  1: reserved
	I2C5_IRQ_handler, 	        //  2: i2c5
	I2C4_IRQ_handler,	        //  3: i2c4
	I2C3_IRQ_handler, 	        //  4: i2c3
	I2C2_IRQ_handler, 	        //  5: i2c2
	I2C1_IRQ_handler, 	        //  6: i2c1
	I2C0_IRQ_handler, 	        //  7: i2c0
	ADCH_IRQ_handler, 	        //  8: adc_half
	ADCF_IRQ_handler, 	        //  9: adc_full
	RTC_IRQ_handler, 	        // 10: rtc
	CAL32M_IRQ_handler, 	    // 11: cal32m
	CAL32K_IRQ_handler, 	    // 12: cal32k
	UART5_IRQ_handler,        	// 13: uart5
	UART4_IRQ_handler,         	// 14: uart4
	UART3_IRQ_handler,         	// 15: uart3
	UART2_IRQ_handler,         	// 16: uart2
	UART1_IRQ_handler,         	// 17: uart1
	UART0_IRQ_handler,         	// 18: uart0
	GPIO_IRQ_handler,          	// 19: gpio
	SPIM1A_IRQ_handler,       	// 20: spi1	end of transmission
	SPIM1B_IRQ_handler,       	// 21: spi1 R/T finished
	SPIM0A_IRQ_handler,       	// 22: spi0	end of transmission
	SPIM0B_IRQ_handler,       	// 23: spi0 R/T finished
	TIMER0_IRQ_handler,     	// 24: timer0
	TIMER1_IRQ_handler,        	// 25: timer1
	TIMER2_IRQ_handler,        	// 26: timer2
	TIMER3_IRQ_handler,        	// 27: timer3
	default_irq_handler,  		// 28: reserved
	default_irq_handler,  		// 29: reserved
	default_irq_handler,  		// 30: reserved
	default_irq_handler,  		// 31: reserved
};

int request_irq(int irq, irq_handler_t handler)
{
	if ( ((irq < 2) || (irq > 27)) || (handler == NULL) ) return -1;

	isr_table[irq & 0x1F] = handler;

	return 0;
}

__attribute__((weak)) void system_irq_handler(uint32_t mcause)
{
    uint32_t int_num;

    if (mcause & 0x80000000) 	/* For external interrupt. */
    {
    	int_num = mcause & 0x1FUL;

#if 0
    	/* Clear pending flag in EVENT unit .*/
    	SOC_EVENT->icp = ( 1U << int_num );
    	SOC_EVENT->ecp = ( 1U << int_num );

    	/* Read back to make sure write finished. */
    	(void)(SOC_EVENT->icp);
    	(void)(SOC_EVENT->ecp);
#endif
        /* Now call the real irq handler for int_num */
        isr_table[int_num]();
    }
}


