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


#ifndef __CPU_HAL_H__
#define __CPU_HAL_H__


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


#define CSR_PCMR_ACTIVE 0x1

/* Configure the active events. eventMask is an OR of events got through SPR_PCER_EVENT_MASK */
static inline void cpu_perf_conf_events(unsigned int eventMask)
{
	asm volatile ("csrw 0x7A0, %0" : "+r" (eventMask));
}

/* Configure the mode. confMask is an OR of all SPR_PCMR_* macros */
static inline void cpu_perf_conf(unsigned int confMask)
{
	asm volatile ("csrw 0x7A1, %0" :: "r" (confMask));
}

/* Starts counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_start(unsigned int conf)
{
	cpu_perf_conf(conf | CSR_PCMR_ACTIVE); // TODO
}

/* Stops counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_stop(unsigned int conf)
{
	cpu_perf_conf(conf); // TODO
}

/* Set the specified counter to the specified value */
void cpu_perf_set(unsigned int counterId, unsigned int value);

/* Set all counters to the specified value */
static inline void cpu_perf_setall(unsigned int value)
{
	asm volatile ("csrw 0x79F, %0" :: "r" (value));
}

/* Return the value of the specified counter */
unsigned int cpu_perf_get(unsigned int counterId);


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef __CPU_HAL_H__ */

