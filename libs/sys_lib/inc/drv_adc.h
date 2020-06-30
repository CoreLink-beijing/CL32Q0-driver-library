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
 * 2019-03-07     corelink     the first version
 */


#ifndef __DRV_ADC_H__
#define __DRV_ADC_H__


#include <stdint.h>
#include <stddef.h>
#include "soc_hal.h"


#ifdef __cplusplus
extern "C" {
#endif


#define	ADC_BASE_ADDR			(0x1B004000)

#define ADC_LVL1_EVT            (((uint32_t)0x1) << 8)
#define ADC_LVL2_EVT            (((uint32_t)0x1) << 9)


typedef struct _adc_dev
{
	volatile uint32_t config;			
    volatile uint32_t round;
    volatile uint32_t fifo_int;	
    volatile uint32_t ctrl;
    volatile uint32_t dout;
    volatile uint32_t fifo_state;		
} __attribute__((packed, aligned(4))) adc_dev_t;

extern adc_dev_t* const ADC;


typedef struct _adc_conf
{
    uint32_t power_down : 1; 
    uint32_t in_channel : 8;
    uint32_t refp       : 2;
    uint32_t refm       : 1;
    uint32_t reserved1  : 1; 
    uint32_t clock_div  : 2;  
    uint32_t reserved2  : 1; 
    uint32_t sequential : 1;
    uint32_t one_shot   : 1;
    uint32_t scan       : 1;
    uint32_t select     : 1;
    uint32_t reserved3  : 12;
} __attribute__((packed, aligned(4))) adc_conf_t;

typedef struct _adc_conf_struct
{
    adc_conf_t conf; 
    uint32_t round;   
} __attribute__((packed, aligned(4))) adc_conf_struct_t;


#define ADC_POWER_DOWN          ((uint32_t)0x1)
#define ADC_POWER_UP            ((uint32_t)0x0) 

#define ADC_IN0                 (((uint32_t)0x1) << 0)
#define ADC_IN1                 (((uint32_t)0x1) << 1)
#define ADC_IN2                 (((uint32_t)0x1) << 2)
#define ADC_IN3                 (((uint32_t)0x1) << 3)
#define ADC_IN4                 (((uint32_t)0x1) << 4)
#define ADC_IN5                 (((uint32_t)0x1) << 5)
#define ADC_IN6                 (((uint32_t)0x1) << 6)
#define ADC_IN7                 (((uint32_t)0x1) << 7)

#define ADC_VRP_VCC             ((uint32_t)0x0) 
#define ADC_VRP_EXT             ((uint32_t)0x1)
#define ADC_VRP                 ((uint32_t)0x2)

#define ADC_VRM_EXT             ((uint32_t)0x1)
#define ADC_VRM_AVSS            ((uint32_t)0x0)

#define ADC_DIV16               ((uint32_t)0x0) 
#define ADC_DIV32               ((uint32_t)0x1)   
#define ADC_DIV64               ((uint32_t)0x2) 
#define ADC_DIV128              ((uint32_t)0x3)

#define ADC_SEQUENTIAL          ((uint32_t)0x1)
#define ADC_ONE_SHOT            ((uint32_t)0x1)
#define ADC_SCAN                ((uint32_t)0x1)
#define ADC_SELECT              ((uint32_t)0x1)

#define ADC_INT_MASK            ((uint32_t)0xC)
#define ADC_LVL2_INT_EN         ((uint32_t)0x8)
#define ADC_LVL1_INT_EN         ((uint32_t)0x4)
#define ADC_CTRL_MASK           ((uint32_t)0x3)
#define ADC_START               ((uint32_t)0x2)
#define ADC_STOP                ((uint32_t)0x1)  

#define ADC_FULL                ((uint32_t)0x1) 
#define ADC_EPMTY               ((uint32_t)0x2) 
#define ADC_LVL2_INT            ((uint32_t)0x4) 
#define ADC_LVL1_INT            ((uint32_t)0x8)  

#define ADC_GET_FIFO_LEVEL()    ((ADC->fifo_state >> 16) & (uint32_t)0x1f)  
#define ADC_GET_DATA()          (ADC->dout & ((uint16_t)0x3ff))

#define ADC_INT_RESET()         do {ICP = ADC_LVL1_EVT; ECP = ADC_LVL1_EVT;} while (0)


void adc_deinit(void);
void adc_init(void);

int adc_conf_struct_init(adc_conf_struct_t *conf);
int adc_set_conf(adc_conf_struct_t *conf);

int adc_set_int_level(uint32_t level);
void adc_int_enable(void);
void adc_int_disable(void);

void adc_start(void);
void adc_stop(void);

uint32_t adc_read_status(void);
uint16_t adc_read_result(void);


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef __ADC_H__ */
