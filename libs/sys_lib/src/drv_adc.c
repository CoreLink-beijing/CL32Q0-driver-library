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


#include <stdio.h>
#include <string.h>
#include "soc_hal.h"
#include "drv_adc.h"


adc_dev_t* const ADC = (adc_dev_t*)ADC_BASE_ADDR;


void adc_deinit(void)
{
    ADC->config = (uint32_t)0x0000;
    CGREG &= ~CG_ADC;
}

void adc_init(void)
{
	CGREG |= CG_ADC;
	ADC->config = (uint32_t)0x0000;
}

int adc_conf_struct_init(adc_conf_struct_t *conf)
{
    if (conf == NULL) return -1;
    
    memset(conf, 0, sizeof(adc_conf_struct_t));

    conf->conf.power_down = ADC_POWER_UP;
    conf->conf.refp = ADC_VRP_VCC;
    conf->conf.refm = ADC_VRM_AVSS;
    conf->conf.clock_div = ADC_DIV128;
    conf->conf.sequential = ADC_SEQUENTIAL;
    conf->conf.one_shot = 0;
    conf->conf.scan = 0;
    conf->conf.select = ADC_SELECT;

    conf->round = 1;

    return 0;
}

int adc_set_conf(adc_conf_struct_t *conf)
{
	uint32_t config;

	if (conf == NULL) return -1;
    if ( (conf->conf.scan) && (conf->conf.select) ) return -1;
    if ( (conf->conf.one_shot == 0) && (conf->round == 0) ) return -1;
    if ( (conf->conf.sequential) && (conf->round == 0) ) return -1;
    
    memcpy(&config, (const void *)(&(conf->conf)), 4);
    ADC->config = config;
    ADC->round = conf->round;

    return 0;
}

void adc_start(void)
{
    volatile uint32_t v;

    v = ADC->ctrl;
    v &= ~ADC_START;
    v |=  ADC_START; 
    ADC->ctrl = v;
}

void adc_stop(void)
{
    volatile uint32_t v;

    v = ADC->ctrl;
    v &= ~ADC_CTRL_MASK;
    ADC->ctrl = v;
}

int adc_set_int_level(uint32_t level)
{
    if ( (level < 1) || (level > 16) ) return -1;
    
    ADC->fifo_int = (((uint32_t)16 << 16) | level);

    return 0;
}

void adc_int_enable(void)
{
    volatile uint32_t v;
    
    IER |= ADC_LVL1_EVT;
    EER |= ADC_LVL1_EVT;

    v = ADC->ctrl;
    v &= ~ADC_INT_MASK;
    v |=  ADC_LVL1_INT_EN; 
    ADC->ctrl = v;
}

void adc_int_disable(void)
{
    ADC->ctrl &= ~ADC_INT_MASK;
    
    IER &= ~ADC_LVL1_EVT;
    EER &= ~ADC_LVL1_EVT;
}

uint32_t adc_read_status(void)
{
    return ADC->fifo_state;
}

uint16_t adc_read_result(void)
{
    //return ADC_GET_DATA();

    return (uint16_t)ADC->dout;
}

/*
void ISR_ADC(void)
{
    volatile uint32_t status;
    
    status = ADC->fifo_state;
    if (status & ADC_LVL1_INT)
    {
        // todo: read n ADC10
    }

    ICP = ADC_LVL1_EVT;
    ECP = ADC_LVL1_EVT;
}
*/
