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


#ifndef __UTILS_H__
#define __UTILS_H__


#include "soc_hal.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Write to CSR.
 * @param CSR register to write.
 * @param Value to write to CSR register.
 * @return void
 *
 * Function to handle CSR writes.
 *
 */
#define csrw(csr, value)  asm volatile ("csrw\t\t" #csr ", %0" : /* no output */ : "r" (value));

/**
 * @brief Read from CSR.
 * @param void
 * @return 32-bit unsigned int
 *
 * Function to handle CSR reads.
 *
 */
#define csrr(csr, value)  asm volatile ("csrr\t\t%0, " #csr "": "=r" (value));

/**
 * @brief Request to put the core to sleep.
 * @param void
 *
 * Set the core to sleep state and wait for events/interrupt to wake up.
 *
 */
static inline void sleep(void)
{
	SCR = 0x01;
	asm volatile ("nop;nop;wfi");
}

/* Loops/exits simulation */
void exit(int i);

/* end of computation */
void eoc(int i);

// sleep some cycles
void sleep_busy(volatile int);


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef __UTILS_H__ */


