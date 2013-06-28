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

/*
Accelerometer is on SPI1
CS -> PE3
*/

/* Set STM32 to 168 MHz. */
static void clock_setup(void){
	/*Operate at 169MHz*/
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
	/*enable the SPI clock*/
	rcc_peripheral_enable_clock(&RCC_APB2ENR,
				    RCC_APB2ENR_SPI1EN);
	/* Enable GPIOD/E clock. */
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN|
							RCC_AHB1ENR_IOPEEN|
							RCC_AHB1ENR_IOPAEN);
}

static void gpio_setup(void){
	/* Set GPIO12 (in GPIO port D) to 'output push-pull'. */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
}

static void spi_setup(void){

//PA5 -> SPI1 SCK
//PA6 -> SPI1 MISO
//PA7 -> SPI1 MOSI
//PE3 -> Accel CS

	/*initialise PE3 for CS*/
	gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
	gpio_set(GPIOE, GPIO3); //deselect the accelerometer
	
	/*Initialise SPI1 pins for AF*/
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5|GPIO6|GPIO7);
	gpio_set_af(GPIOA, GPIO_AF5, GPIO5|GPIO6|GPIO7);
	//GPIOA_AFRL = (5<<5*4)|(5<<6*4)|(5<<7*4);
	spi_enable(SPI1);
	spi_disable_crc(SPI1);
	
	spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_32,
			/* high or low for the peripheral device */
			SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
			/* CPHA: Clock phase: read on rising edge of clock */
			SPI_CR1_CPHA_CLK_TRANSITION_2,
			/* DFF: Date frame format (8 or 16 bit) */
			SPI_CR1_DFF_8BIT,
			/* Most or Least Sig Bit First */
			SPI_CR1_MSBFIRST);

	//spi_enable_software_slave_management(SPI1);
	//spi_set_nss_high(SPI1);
	
}

char read_addr(char addr)
{
	short return_value;
	char zeros = 0;
	gpio_clear(GPIOE, GPIO3); //select the accel
	int i;
	return_value = spi_xfer(SPI1, addr);
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
	spi_setup();
	int i;
	for(i=0; i<100000; i++) 
		__asm__("nop");
//LED's are on port D 12-15
	while(1){
		X = accel_read(0x29);
		Y = accel_read(0x2B);
		if(X>Y) gpio_set(GPIOD, GPIO12|GPIO13|GPIO14|GPIO15);
		else gpio_clear(GPIOD, GPIO12|GPIO13|GPIO14|GPIO15);
		for(i=0; i<10000; i++) 
			__asm__("nop");
	}
	return 0;
}
