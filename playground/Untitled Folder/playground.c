/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "playground.h"
#include "init.c"
#include "debug.c"

/*
Accelerometer is on SPI1
CS -> PE3
*/

/* Set STM32 to 168 MHz. */
static void clock_setup(void){
	/*Operate at 169MHz*/
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
	/* Enable GPIOD/E clock. */
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN|
							RCC_AHB1ENR_IOPAEN);
}

static void gpio_setup(void){
	/* Set GPIO12 (in GPIO port D) to 'output push-pull'. */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
}

char read_addr(char addr)
{
	short return_value;
	debug_send("selecting accel");
	gpio_clear(GPIOE, GPIO3); //select the accel
	int i;
	debug_send("sending address");
	spi_send(SPI1, addr);
	debug_send("sending zeros");
	spi_send(SPI1, 0);
	debug_send("reading rx data register");
	return_value = spi_read(SPI1);
	debug_send("deselecting accel");
	gpio_set(GPIOE, GPIO3); //deselect the accel

	return return_value;
}

char accel_read(char axis){
	char command;
	command =
	    /* READ bit */
	    (0x1 << 7) |
	    /* MS bit:  When 0 do not increment address */
	    (0x0 << 6) |
	    /* bits 2-7 are address */
	    (axis << 0);

	return read_addr(command);
}

int main(void)
{
	clock_setup();
	gpio_setup();
	usart_setup();
	debug_send("spi_setup() starting");
	spi_setup();
	debug_send("spi_setup() finished");
	int i;
	while(1){
		i = read_addr(0x29);
	}
//LED's are on port D 12-15
	/*while(1){
		X = accel_read(0x29);
		Y = accel_read(0x2B);
		if(X>Y) gpio_set(GPIOD, GPIO12|GPIO13|GPIO14|GPIO15);
		else gpio_clear(GPIOD, GPIO12|GPIO13|GPIO14|GPIO15);
		for(i=0; i<10000; i++) 
			__asm__("nop");
	} */
	return 0;
}
