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
 * 2019-01-27     corelink     the first version
 */
 

#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__


#include <stdint.h>
#include <stddef.h>
#include "soc_hal.h"


#ifdef __cplusplus
extern "C" {
#endif


#define SPI_HW_CS_EN			(0)		/* 0:SW, 1:HW */

#define	SPI0_BASE_ADDR			(0x1A12C000)
#define	SPI1_BASE_ADDR			(0x1A12D000)

#define SPI_QPI    				((uint32_t)1)
#define SPI_NO_QPI 				((uint32_t)0)

#define SPI_CMD_RD    			((uint32_t)0)
#define SPI_CMD_WR    			((uint32_t)1)
#define SPI_CMD_QRD   			((uint32_t)2)
#define SPI_CMD_QWR   			((uint32_t)3)
#define SPI_CMD_SWRST 			((uint32_t)4)

#define SPI_CSN0 				((uint32_t)0)
#define SPI_CSN1 				((uint32_t)1)
#define SPI_CSN2 				((uint32_t)2)
#define SPI_CSN3 				((uint32_t)3)

typedef struct _spi_dev
{
	volatile uint32_t status;			
	volatile uint32_t clkdiv;			
	volatile uint32_t spicmd;			
	volatile uint32_t spiadr;			
	volatile uint32_t spilen;			
	volatile uint32_t spidum;
	volatile uint32_t txfifo;
	         uint32_t reserved;
	volatile uint32_t rxfifo;
	volatile uint32_t intcfg;
	volatile uint32_t intsta;
} __attribute__((packed, aligned(4))) spi_dev_t; 

extern spi_dev_t* const SPI0;
extern spi_dev_t* const SPI1;

#define SPI0_CLK			(16)
#define SPI0_MOSI			(30)
#define SPI0_MISO			(18)

#define SPI1_CLK			(0)
#define SPI1_MOSI			(33)
#define SPI1_MISO			(2)

#define SPI0_CS0_PIN		(17)	/* can be modified by user */
#define SPI0_CS1_PIN		(24)	/* can be modified by user */
#define SPI0_CS2_PIN		(25)	/* can be modified by user */
#define SPI0_CS3_PIN		(26)	/* can be modified by user */

#define SPI1_CS0_PIN		(1)		/* can be modified by user */
#define SPI1_CS1_PIN		(12)	/* can be modified by user */
#define SPI1_CS2_PIN		(13)	/* can be modified by user */
#define SPI1_CS3_PIN		(14)	/* can be modified by user */

#define SPI0_HWCS0_PIN		(17)
#define SPI0_HWCS1_PIN		(24)
#define SPI0_HWCS2_PIN		(25)
#define SPI0_HWCS3_PIN		(26)

#define SPI1_HWCS0_PIN		(1)
#define SPI1_HWCS1_PIN		(12)
#define SPI1_HWCS2_PIN		(13)
#define SPI1_HWCS3_PIN		(14)


void spix_init(spi_dev_t *spi);
void spix_deinit(spi_dev_t *spi);

void spix_setup_master(spi_dev_t *spi);
int  spix_set_max_speed(spi_dev_t *spi, uint32_t speed);
void spix_attach_chip_select(spi_dev_t *spi, uint32_t cs);

void spix_setup_cmd_addr(spi_dev_t *spi, uint32_t cmd, uint32_t cmdlen, uint32_t addr, uint32_t addrlen);
void spix_set_datalen(spi_dev_t *spi, uint32_t datalen);
void spix_setup_dummy(spi_dev_t *spi, uint32_t dummy_rd, uint32_t dummy_wr);

void spix_start_transaction(spi_dev_t *spi, uint32_t trans_type, uint32_t cs);

void spix_write_fifo(spi_dev_t *spi, uint32_t *data, uint32_t datalen);
void spix_read_fifo(spi_dev_t *spi, uint32_t *data, uint32_t datalen);

int spix_get_status(spi_dev_t *spi);

#if (SPI_HW_CS_EN == 0)
	
void spix_setup_chip_select(spi_dev_t *spi, uint32_t cs);
void spix_release_chip_select(spi_dev_t *spi, uint32_t cs);

/* Easy API */
void spix_write_direct(spi_dev_t *spi, uint32_t cs, uint32_t *data, uint32_t datalen);
void spix_read_direct(spi_dev_t *spi, uint32_t cs, uint32_t *data, uint32_t datalen);
void spix_write_then_read_direct(spi_dev_t *spi, uint32_t cs, 
								uint32_t *data_wr, uint32_t datalen_wr, 
								uint32_t *data_rd, uint32_t datalen_rd);
void spix_write_then_write_direct(spi_dev_t *spi, uint32_t cs, 
								uint32_t *data_wr1, uint32_t datalen_wr1, 
								uint32_t *data_wr2, uint32_t datalen_wr2);

#endif


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef __DRV_SPI_H__ */
