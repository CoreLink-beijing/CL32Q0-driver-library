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
 * 2019-08-13     corelink     the first version
 */


#ifndef __PROTON_H__
#define __PROTON_H__


#ifdef __cplusplus
extern "C" {
#endif


/*
 * the lists can be modified by user
 */
#include "cpu_hal.h"
#include "soc_hal.h"
#include "drv_clock.h"
#include "drv_clockcal.h"
#include "drv_power.h"
#include "drv_scu.h"
#include "drv_uart.h"
#include "drv_console.h"
#include "drv_gpio.h"
#include "drv_i2c.h"
#include "drv_spi.h"
#include "drv_timer.h"
#include "drv_pwm.h"
#include "drv_wdt.h"
#include "drv_rtc.h"
#include "drv_adc.h"
#include "drv_eflash.h"
#include "irq.h"
#include "utils.h"
#include "system_proton.h"


#define CL_VERSION			0              	/**< major version number */
#define CL_SUBVERSION		9              	/**< minor version number */
#define CL_REVISION			0              	/**< revise version number */


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef __PROTON_H__ */
