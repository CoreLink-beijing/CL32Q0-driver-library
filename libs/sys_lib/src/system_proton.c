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
 * 2019-08-08     corelink     the first version
 */


#include <stdint.h>
#include <stdbool.h>
#include "soc_hal.h"
#include "irq.h"
#include "drv_gpio.h"
#include "drv_clock.h"
#include "drv_clockcal.h"
#include "drv_eflash.h"
#include "drv_console.h"


extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;

extern uint32_t __bss_start__;
extern uint32_t __bss_end__;


static void copy_section(uint32_t * p_load, uint32_t * p_vma, uint32_t * p_vma_end)
{
    while(p_vma <= p_vma_end)
    {
        *p_vma = *p_load;
        ++p_load;
        ++p_vma;
    }
}

static void zero_section(uint32_t * start, uint32_t * end)
{
    uint32_t * p_zero = start;

    while(p_zero <= end)
    {
        *p_zero = 0;
        ++p_zero;
    }
}

static void time_delay(uint32_t delay)
{
	volatile uint32_t d = delay / 8;

	if (d == 0) d = 1;
	while (--d);
}

static void system_clock_calibrate(void)
{
	#define CLOCK_PARAM_PAGE	(65)

	uint32_t svs_hf;
	uint32_t svs_lf;
	uint32_t checksum;

	/* waiting for 32M-clock stabilization: 200 mS */
	time_delay(system_clock_get() / 5);

	eflash_read_word(CLOCK_PARAM_PAGE, 0, &svs_hf);
	eflash_read_word(CLOCK_PARAM_PAGE, 4, &svs_lf);
	eflash_read_word(CLOCK_PARAM_PAGE, 8, &checksum);

	if (   (svs_hf != 0) && (svs_hf != 0xffffffff)
		&& (svs_lf != 0) && (svs_lf != 0xffffffff)
		&& ((svs_hf + svs_lf) == checksum) )
	{
		clockcal_write_factor_register(svs_hf, svs_lf);
	}
	else
	{
		CGREG |= CG_CLOCK_CAL;

		ext_clock_enable(EXT_CLOCK_SOURCE_XTAL, true);
		lf_clock_select(LF_CLOCK_SOURCE_32K);

		gpio_func_set(35, FUNC0_XTAL_IN);
		gpio_func_set(36, FUNC0_XTAL_OUT);
		gpio_mode_set(35, GPIO_MODE_AN);
		gpio_mode_set(36, GPIO_MODE_AN);

		/* waiting for 32K-clock stabilization: 2000 mS */
		time_delay(system_clock_get() * 2);
		clockcal_calibrate();
		time_delay(system_clock_get() * 2);

		clockcal_read_factor_register(&svs_hf, &svs_lf);
		checksum = svs_hf + svs_lf;
		eflash_erase_page(CLOCK_PARAM_PAGE);
		eflash_program_word(CLOCK_PARAM_PAGE, 0, &svs_hf);
		eflash_program_word(CLOCK_PARAM_PAGE, 4, &svs_lf);
		eflash_program_word(CLOCK_PARAM_PAGE, 8, &checksum);
	}
}

__attribute__ ((weak)) void system_init_hook (void)
{
	system_clock_calibrate();

	console_init(115200);
}

void system_init(void)
{
	copy_section(&__etext, &__data_start__, &__data_end__);
	zero_section(&__bss_start__, &__bss_end__);

	system_init_hook();
}

