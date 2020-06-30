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
 * 2019-08-08     corelink		*
 */


#include <stdarg.h>
#include "qprintf.h"
#include "drv_console.h"


#define PAD_RIGHT 1
#define PAD_ZERO  2

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 64

/* define LONG_MAX for int32 */
#define LONG_MAX 2147483647L

/* DETECTNULL returns nonzero if (long)X contains a NULL byte. */
#if LONG_MAX == 2147483647L
#define DETECTNULL(X) (((X) - 0x01010101) & ~(X) & 0x80808080)
#else
#if LONG_MAX == 9223372036854775807L
#define DETECTNULL(X) (((X) - 0x0101010101010101) & ~(X) & 0x8080808080808080)
#else
#error long int is not a 32bit or 64bit type.
#endif
#endif

/* Nonzero if either X or Y is not aligned on a "long" boundary. */
#define UNALIGNED(X, Y) \
  (((long)X & (sizeof (long) - 1)) | ((long)Y & (sizeof (long) - 1)))

static unsigned divu10(unsigned n)
{
	unsigned q, r;

	q = (n >> 1) + (n >> 2);
	q = q + (q >> 4);
	q = q + (q >> 8);
	q = q + (q >> 16);
	q = q >> 3;
	r = n - q * 10;

	return q + ((r + 6) >> 4);
}

char remu10_table[16] =
{
	0, 1, 2, 2, 3, 3, 4, 5,
	5, 6, 7, 7, 8, 8, 9, 0
};

static unsigned remu10(unsigned n)
{
	n = (0x19999999 * n + (n >> 1) + (n >> 3)) >> 28;
	return remu10_table[n];
}

int putchar(int s)
{
	console_send_char(s);
	return s;
}

static void qprintchar(char **str, int c)
{
	if (str)
	{
		**str = c;
		++(*str);
	}
	else
		putchar((char)c);
}

static int qprints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			qprintchar (out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		qprintchar (out, *string);
		++pc;
	}
	for ( ; width > 0; --width) {
		qprintchar (out, padchar);
		++pc;
	}

	return pc;
}

static int qprinti(char **out, int i, int b, int sg, int width, int pad, char letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int neg = 0, pc = 0;
	unsigned int u = i;

	if (i == 0)
	{
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return qprints (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0)
	{
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	// treat HEX and decimal differently
	if(b == 16) {
		// HEX
		while (u) {
			int t = u & 0xF;

			if (t >= 10)
				t += letbase - '0' - 10;

			*--s = t + '0';
			u >>= 4;
		}
	} else {
		// decimal
		while (u) {
			*--s = remu10(u) + '0';
			u = divu10(u);
		}
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) )
		{
			qprintchar (out, '-');
			++pc;
			--width;
		}
		else
		{
			*--s = '-';
		}
	}
	return pc + qprints (out, s, width, pad);
}

static int qprint(char **out, const char *format, va_list va)
{
	register int width, pad;
	register int pc = 0;
	char scr[2];

	for (; *format != 0; ++format)
	{
		if (*format == '%')
		{
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-')
			{
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0')
			{
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = va_arg(va, char*);
				pc += qprints (out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) {
				pc += qprinti (out, va_arg(va, int), 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'u' ) {
				pc += qprinti (out, va_arg(va, unsigned int), 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'x' ) {
				pc += qprinti (out, va_arg(va, uint32_t), 16, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'X' ) {
				pc += qprinti (out, va_arg(va, uint32_t), 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'c' ) {
				scr[0] = va_arg(va, int);
				scr[1] = '\0';
				pc += qprints (out, scr, width, pad);
				continue;
			}
		}
		else
		{
out:
			qprintchar (out, *format);
			++pc;
		}
	}
	if (out) **out = '\0';

	return pc;
}

/* redirect printf */
int printf(const char *format, ...)
{
	int pc;
	va_list va;

	va_start(va, format);

	pc = qprint(0, format, va);

	va_end(va);

	return pc;
}

/* redirect puts */
int puts(const char *s)
{
	int i=0;

	while(s[i] != '\0')
		putchar(s[i++]);

	putchar('\n');

	return i;
}

int strcmp_c(const char *s1, const char *s2)
{
	unsigned long *a1;
	unsigned long *a2;

	/* If s1 or s2 are unaligned, then compare bytes. */
	if (!UNALIGNED (s1, s2))
	{
		/* If s1 and s2 are word-aligned, compare them a word at a time. */
		a1 = (unsigned long*)s1;
		a2 = (unsigned long*)s2;
		while (*a1 == *a2)
		{
			/* To get here, *a1 == *a2, thus if we find a null in *a1,
			then the strings must be equal, so return zero.  */
			if (DETECTNULL (*a1))
				return 0;
			a1++;
			a2++;
		}
		/* A difference was detected in last few bytes of s1, so search bytewise */
		s1 = (char*)a1;
		s2 = (char*)a2;
	}

	while (*s1 != '\0' && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

void* memset_c(void *dest, int val, size_t length)
{
	unsigned char *ptr = dest;
	while (length > 0)
	{
		*ptr++ = val;
		length --;
	}
	return dest;
}

char* strcpy_c(char *s1, const char *s2)
{
	char *s1p = (char *)s1;
	char *s2p = (char *)s2;

	while (*s2p != '\0')
	{
		(*s1p) = (*s2p);

		++s1p;
		++s2p;
	}

	return s1;
}

size_t strlen_c(const char *str)
{
	char *s = (char *)str;
	size_t len = 0;

	if (s == NULL)
		return 0;

	while (*s++ != '\0')
		++len;
	return len;
}
