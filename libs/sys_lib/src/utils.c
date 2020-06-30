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
 * 2019-08-13     corelink		*
 */


#include <stdint.h>
#include <stdio.h>
#include "soc_hal.h"
#include "drv_gpio.h"
#include "utils.h"


// exit loop
void exit (int i)
{
	eoc(i);
	while (1);
}

// signal end of computation
void eoc(int i)
{
	if (i != 0)
		RES_STATUS = 1;
	else
		RES_STATUS = 0;
}

// sleep for 'iter' iterations. each iteration is approx 10 cycles
void sleep_busy(volatile int iter)
{
	for (int i=0;i<iter;i++)
		asm volatile ("nop");
}

/* Set the specified counter to the specified value */
void cpu_perf_set(unsigned int counterId, unsigned int value)
{
	printf("cpu_perf_set: not implemented yet\n");
}

unsigned int cpu_perf_get(unsigned int counterId)
{
	uint32_t value = 0;

	switch (counterId)
	{
	case  0: asm volatile ("csrr %0, 0x780" : "=r" (value)); break;
	case  1: asm volatile ("csrr %0, 0x781" : "=r" (value)); break;
	case  2: asm volatile ("csrr %0, 0x782" : "=r" (value)); break;
	case  3: asm volatile ("csrr %0, 0x783" : "=r" (value)); break;
	case  4: asm volatile ("csrr %0, 0x784" : "=r" (value)); break;
	case  5: asm volatile ("csrr %0, 0x785" : "=r" (value)); break;
	case  6: asm volatile ("csrr %0, 0x786" : "=r" (value)); break;
	case  7: asm volatile ("csrr %0, 0x787" : "=r" (value)); break;
	case  8: asm volatile ("csrr %0, 0x788" : "=r" (value)); break;
	case  9: asm volatile ("csrr %0, 0x789" : "=r" (value)); break;
	case 10: asm volatile ("csrr %0, 0x78A" : "=r" (value)); break;
	case 11: asm volatile ("csrr %0, 0x78B" : "=r" (value)); break;
	case 12: asm volatile ("csrr %0, 0x78C" : "=r" (value)); break;
	case 13: asm volatile ("csrr %0, 0x78D" : "=r" (value)); break;
	case 14: asm volatile ("csrr %0, 0x78E" : "=r" (value)); break;
	case 15: asm volatile ("csrr %0, 0x78F" : "=r" (value)); break;
	default: printf("CSR %d not yet supported\n", counterId); break;
	}

	return value;
}

