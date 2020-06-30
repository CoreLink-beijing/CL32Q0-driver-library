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
 * 2019-03-01     corelink     the first version
 */


#ifndef __SOC_HAL_H__
#define __SOC_HAL_H__


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


/* ROM */
#define ROM_BASE_ADDR               ( 0x8000     )

/* Code RAM */
#define INSTR_RAM_BASE_ADDR         ( 0x00       )
#define INSTR_RAM_START_ADDR        ( 0x80       )

/* Data RAM */
#define DATA_RAM_BASE_ADDR          ( 0x00100000 )

/** Registers and pointers */
#define REGP(x)                     ((volatile uint32_t*)(x))
#define REG(x)                      (*((volatile uint32_t*)(x)))
#define REGP_8(x)                   (((volatile uint8_t*)(x)))
#define R32(x)                      (*((volatile uint32_t*)(x)))

/* pointer to mem of apb proton unit - PointerSocCtrl */
#define __PSC__(a)                  *(unsigned volatile int*) (SOC_CTRL_BASE_ADDR + a)

/** Peripheral Clock gating */
#define CGREG                       __PSC__(0x04)

#define CG_UART0                    (((uint32_t)0x01) << 0) 
#define CG_UART1                    (((uint32_t)0x01) << 1) 
#define CG_UART2                    (((uint32_t)0x01) << 2) 
#define CG_UART3                    (((uint32_t)0x01) << 3) 
#define CG_UART4                    (((uint32_t)0x01) << 4) 
#define CG_UART5                    (((uint32_t)0x01) << 5) 
#define CG_I2C0                     (((uint32_t)0x01) << 6)
#define CG_I2C1                     (((uint32_t)0x01) << 7)
#define CG_I2C2                     (((uint32_t)0x01) << 8)
#define CG_I2C3                     (((uint32_t)0x01) << 9)
#define CG_I2C4                     (((uint32_t)0x01) << 10)
#define CG_I2C5                     (((uint32_t)0x01) << 11)
#define CG_TIMER                    (((uint32_t)0x01) << 12)
#define CG_CLOCK_CAL                (((uint32_t)0x01) << 13)
#define CG_WDT                      (((uint32_t)0x01) << 14)
#define CG_ADC                      (((uint32_t)0x01) << 15)
#define CG_PWM                      (((uint32_t)0x01) << 16)

/** Boot address register */
#define BOOTREG                     __PSC__(0x08)
#define RES_STATUS                  __PSC__(0x14)


typedef struct _soc_control_
{
			 uint32_t reserved1;
	volatile uint32_t cgreg;
	volatile uint32_t bootreg;
			 uint32_t reserved2;
			 uint32_t reserved3;
	volatile uint32_t res_status;
} soc_control_t;

#define SOC_CTRL_BASE_ADDR          ( 0x1A107000 )
#define SOC_CTRL 					((soc_control_t *)SOC_CTRL_BASE_ADDR)

#define REG_IRQ_ENABLE          	(0x00)
#define REG_IRQ_PENDING         	(0x04)
#define REG_IRQ_SET_PENDING     	(0x08)
#define REG_IRQ_CLEAR_PENDING   	(0x0C)

#define REG_EVENT_ENABLE        	(0x10)
#define REG_EVENT_PENDING       	(0x14)
#define REG_EVENT_SET_PENDING   	(0x18)
#define REG_EVENT_CLEAR_PENDING 	(0x1C)

#define REG_SLEEP_CTRL          	(0x20)
#define REG_SLEEP_STATUS        	(0x24)


// pointer to mem of event unit - PointerEventunit
#define __PE__(a) *(volatile unsigned int *) (EVENT_UNIT_BASE_ADDR + a)

// interrupt enable register
#define IER __PE__(REG_IRQ_ENABLE)

// interrupt pending register
#define IPR __PE__(REG_IRQ_PENDING)

// interrupt set pending register
#define ISP __PE__(REG_IRQ_SET_PENDING)

// interrupt clear pending register
#define ICP __PE__(REG_IRQ_CLEAR_PENDING)

// event enable register
#define EER __PE__(REG_EVENT_ENABLE)

// event pending register
#define EPR __PE__(REG_EVENT_PENDING)

// event set pending register
#define ESP __PE__(REG_EVENT_SET_PENDING)

// event clear pending register
#define ECP __PE__(REG_EVENT_CLEAR_PENDING)

// sleep control register
#define SCR __PE__(REG_SLEEP_CTRL)

// sleep status register
#define SSR __PE__(REG_SLEEP_STATUS)


typedef struct _soc_event
{
	volatile uint32_t ier;
	volatile uint32_t ipr;
	volatile uint32_t isp;
	volatile uint32_t icp;
	volatile uint32_t eer;
	volatile uint32_t epr;
	volatile uint32_t esp;
	volatile uint32_t ecp;
	volatile uint32_t scr;
	volatile uint32_t ssr;
} __attribute__((packed, aligned(4))) soc_event_t;

#define EVENT_UNIT_BASE_ADDR        ( 0x1A104000 )
#define SOC_EVENT 					((soc_event_t *)EVENT_UNIT_BASE_ADDR)

#define R32_BITS_SET(a, m, v)	\
do \
{\
	*((volatile uint32_t *)(a)) &= ~(m);\
	*((volatile uint32_t *)(a)) |= (v);\
}\
while (0)

#define R32_BITS_CLR(a, m)	\
do \
{\
	*((volatile uint32_t *)(a)) &= ~(m);\
}\
while (0)


#ifdef __cplusplus
}
#endif


#endif  /* #ifndef __SOC_HAL_H__ */
