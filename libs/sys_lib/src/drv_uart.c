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
 * 2019-01-26     corelink     the first version
 */


#include "soc_hal.h"
#include "drv_uart.h"
#include "drv_clock.h"
#include "drv_gpio.h"
#include "utils.h"


uart_dev_t* const UART0 = (uart_dev_t*)UART0_BASE_ADDR;
uart_dev_t* const UART1 = (uart_dev_t*)UART1_BASE_ADDR;
uart_dev_t* const UART2 = (uart_dev_t*)UART2_BASE_ADDR;
uart_dev_t* const UART3 = (uart_dev_t*)UART3_BASE_ADDR;
uart_dev_t* const UART4 = (uart_dev_t*)UART4_BASE_ADDR;
uart_dev_t* const UART5 = (uart_dev_t*)UART5_BASE_ADDR;


int uartx_init(uart_dev_t *uart)
{
	if (uart == NULL) return -1;
	
	if (uart == UART0)
	{
		gpio_func_set(UART0_TX, FUNC0_UART0_TX);
		gpio_func_set(UART0_RX, FUNC0_UART0_RX);
		gpio_mode_set(UART0_TX, GPIO_MODE_OUT_PP);
		gpio_mode_set(UART0_RX, GPIO_MODE_IN_FLOATING);

		CGREG |= CG_UART0;
	}
	else if (uart == UART1)
	{
		gpio_func_set(UART1_TX, FUNC0_UART1_TX);
		gpio_func_set(UART1_RX, FUNC0_UART1_RX);
		gpio_mode_set(UART1_TX, GPIO_MODE_OUT_PP);
		gpio_mode_set(UART1_RX, GPIO_MODE_IN_FLOATING);

		CGREG |= CG_UART1;
	}
	else if (uart == UART2)
	{
		gpio_func_set(UART2_TX, FUNC0_UART2_TX);
		gpio_func_set(UART2_RX, FUNC0_UART2_RX);
		gpio_mode_set(UART2_TX, GPIO_MODE_OUT_PP);
		gpio_mode_set(UART2_RX, GPIO_MODE_IN_FLOATING);

		CGREG |= CG_UART2;
	}
	else if (uart == UART3)
	{
		gpio_func_set(UART3_TX, FUNC0_UART3_TX);
		gpio_func_set(UART3_RX, FUNC0_UART3_RX);
		gpio_mode_set(UART3_TX, GPIO_MODE_OUT_PP);
		gpio_mode_set(UART3_RX, GPIO_MODE_IN_FLOATING);

		CGREG |= CG_UART3;
	}
	else if (uart == UART4)
	{
		gpio_func_set(UART4_TX, FUNC0_UART4_TX);
		gpio_func_set(UART4_RX, FUNC0_UART4_RX);
		gpio_mode_set(UART4_TX, GPIO_MODE_OUT_PP);
		gpio_mode_set(UART4_RX, GPIO_MODE_IN_FLOATING);

		CGREG |= CG_UART4;
	}
	else if (uart == UART5)
	{
		gpio_func_set(UART5_TX, FUNC1_UART5_TX);
		gpio_func_set(UART5_RX, FUNC1_UART5_RX);
		gpio_mode_set(UART5_TX, GPIO_MODE_OUT_PP);
		gpio_mode_set(UART5_RX, GPIO_MODE_IN_FLOATING);

		CGREG |= CG_UART5;
	}

	return 0;
}

int uartx_deinit(uart_dev_t *uart)
{
	if (uart == NULL) return -1;
	
	if (uart == UART0)
	{
		CGREG &= ~CG_UART0;
	}
	else if (uart == UART1)
	{
		CGREG &= ~CG_UART1;
	}
	else if (uart == UART2)
	{
		CGREG &= ~CG_UART2;
	}
	else if (uart == UART3)
	{
		CGREG &= ~CG_UART3;
	}
	else if (uart == UART4)
	{
		CGREG &= ~CG_UART4;
	}
	else if (uart == UART5)
	{
		CGREG &= ~CG_UART5;
	}

	return 0;
}

int uartx_set_conf(uart_dev_t *uart, uint32_t baudrate, 
					uart_databits_t databits, uart_stopbits_t stopbits, uart_parity_t parity)
{
	uint32_t system_clock;
	uint32_t clk_counter;
	uint32_t _databits;
	uint32_t _stopbits;
	uint32_t _parity;

	if (uart == NULL) return -1;
	
	_databits = databits;
	_stopbits = ((stopbits == UART_STOP_1) ? 0 : 1);
	_parity   = parity;

	/* get system clock */
	system_clock = system_clock_get();

	clk_counter = (system_clock / baudrate);
	
	uart->lcr = (0x80 | (_databits - 5) | (_stopbits << 2) | (_parity << 3));
	uart->dlm = ((clk_counter >> 8) & 0xFF);
	uart->dll = ( clk_counter       & 0xFF);
	uart->fcr = 0xA7; 		// enables 16byte FIFO and clear FIFOs
	uart->lcr = ((_databits - 5) | (_stopbits << 2) | (_parity << 3));

	return 0; 
}

int uartx_int_enable(uart_dev_t *uart)
{
	if (uart == NULL) return -1;
	
	uart->clr_int = 0x2;
	uart->clr_int = 0x0;
	uart->ier = IER_RX;

	if (uart == UART0)
	{
		IER |= UART0_EVT;
		EER |= UART0_EVT;
	}
	else if (uart == UART1)
	{
		IER |= UART1_EVT;
		EER |= UART1_EVT;
	}
	else if (uart == UART2)
	{
		IER |= UART2_EVT;
		EER |= UART2_EVT;
	}
	else if (uart == UART3)
	{
		IER |= UART3_EVT;
		EER |= UART3_EVT;
	}
	else if (uart == UART4)
	{
		IER |= UART4_EVT;
		EER |= UART4_EVT;
	}
	else if (uart == UART5)
	{
		IER |= UART5_EVT;
		EER |= UART5_EVT;
	}

	return 0;
}

int uartx_int_disable(uart_dev_t *uart)
{
	if (uart == NULL) return -1;
	
	uart->clr_int = 0x2;
	uart->clr_int = 0x0;
	uart->ier &= ~0x10;

	if (uart == UART0)
	{
		IER &= ~UART0_EVT;
		EER &= ~UART0_EVT;
	}
	else if (uart == UART1)
	{
		IER &= ~UART1_EVT;
		EER &= ~UART1_EVT;
	}
	else if (uart == UART2)
	{
		IER &= ~UART2_EVT;
		EER &= ~UART2_EVT;
	}
	else if (uart == UART3)
	{
		IER &= ~UART3_EVT;
		EER &= ~UART3_EVT;
	}
	else if (uart == UART4)
	{
		IER &= ~UART4_EVT;
		EER &= ~UART4_EVT;
	}
	else if (uart == UART5)
	{
		IER &= ~UART5_EVT;
		EER &= ~UART5_EVT;
	}

	return 0;
}

int uartx_int_reset(uart_dev_t *uart)
{
	if (uart == NULL) return -1;
	
	uart->clr_int = 0x2;
	uart->clr_int = 0x0;

	if (uart == UART0)
	{
		ICP = UART0_EVT;
		ECP = UART0_EVT;
	}
	else if (uart == UART1)
	{
		ICP = UART1_EVT;
		ECP = UART1_EVT;
	}
	else if (uart == UART2)
	{
		ICP = UART2_EVT;
		ECP = UART2_EVT;
	}
	else if (uart == UART3)
	{
		ICP = UART3_EVT;
		ECP = UART3_EVT;
	}
	else if (uart == UART4)
	{
		ICP = UART4_EVT;
		ECP = UART4_EVT;
	}
	else if (uart == UART5)
	{
		ICP = UART5_EVT;
		ECP = UART5_EVT;
	}

	return 0;
}

int uartx_send(uart_dev_t *uart, const char* str, uint32_t len) 
{
	if ((uart == NULL) || (str == NULL)) return -1;

	while (len > 0) 
	{
		// wait until there is space in the fifo
		while ( (uart->lsr & 0x20) == 0 );
		
		// load FIFO
		uart->thr = *str++;
		len--;
	}

	return 0;
}

char uartx_get_char(uart_dev_t *uart) 
{
	if (uart == NULL) return -1;
	
	while ((uart->lsr & 0x1) != 0x1 );

	return uart->rbr;
}

int uartx_send_char(uart_dev_t *uart, const char c) 
{
	if (uart == NULL) return -1;
	
	// wait until there is space in the fifo
	while ( (uart->lsr & 0x20) == 0 );

	// load FIFO
	uart->thr = c;

	return 0;
}

int uartx_wait_tx_done(uart_dev_t *uart) 
{
	if (uart == NULL) return -1;
	
	// wait until there is space in the fifo
	while ( (uart->lsr & 0x40) == 0 );

	return 0;
}

