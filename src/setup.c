#include "setup.h"

/* this file sets up all the on-chip peripherals for 
use elsewhere. It does not talk to anything else. The 
timers and interrupts are setup in schedule.c */

void clock_setup(void){
	/*Operate at 168MHz*/
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_48MHZ]);
}

void usart_setup(void){
	//the USART header is attached to usart2

	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_USART1EN);
	//usart pins
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE,
						GPIO9|
						GPIO10);
	gpio_set_af(GPIOA, GPIO_AF7,		GPIO2|
						GPIO3);

	//configuring the usart
	usart_set_baudrate(USART2, 38400);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	//enabling the usart
	usart_enable(USART1);
}


void ledpins_setup(void){
	//leds are all on port b
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN);
	gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 
						STAT1_GR| 
						STAT1_RE| 
						STAT2_GR| 
						STAT2_RE);
}


//spi1 -> data input
//spi2 -> pll control

void spi1_setup(void){
	//setup as slave with an interrupt for incoming data
}

void spi2_setup(void){

	//setup the clocks for gpio pins and spi2
	//setup the chip select
	//may be able to use hardware instead of the hacky software

	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, 
						GPIO13|
						GPIO14|
						GPIO15);
	gpio_set_af(GPIOA, GPIO_AF5, GPIO5|GPIO6|GPIO7);
	
	spi_init_master(SPI2, SPI_CR1_BAUDRATE_FPCLK_DIV_32,
			/* high or low for the peripheral device */
			SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
			/* CPHA: Clock phase: read on rising edge of clock */
			SPI_CR1_CPHA_CLK_TRANSITION_2,
			/* DFF: Date frame format (8 or 16 bit) */
			SPI_CR1_DFF_8BIT,
			/* Most or Least Sig Bit First */
			SPI_CR1_MSBFIRST);

	spi_enable_software_slave_management(SPI2);
	spi_set_nss_high(SPI2);


	//enable SPI1
	spi_enable(SPI1);
}

void dac_setup(void){
	//initialise the db gpios
	//initialise the dac-clk gpio
}

void timer_setup(void){
	//4 timers:
	//dac-db update + calculate next fir sample
	//dac-clk pulse, linked with above
	//input timeout - fill the remainder of the packet with 0's
}

void nvic_setup(void){
	//setup all the interrupts for the timers + spi input
}



