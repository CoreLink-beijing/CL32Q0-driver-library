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
 * 2019-08-08     corelink     *
 */


#ifndef __STRING_LIB_H__
#define __STRING_LIB_H__


#include <stddef.h>
#include "soc_hal.h"


// putchar is defined as a macro which gets in the way of our prototype below
#ifdef putchar
#undef putchar
#endif


int putchar(int s);
int puts(const char *s);
int printf(const char *format, ...);

size_t strlen_c(const char *str);
int strcmp_c(const char *s1, const char *s2);
char* strcpy_c(char *s1, const char *s2);
void* memset_c(void *dest, int val, size_t len);


#endif	/* #ifndef __STRING_LIB_H__ */

