#include "setup.h"

/* this file sets up all the on-chip peripherals for 
use elsewhere. It does not talk to anything else. The 
timers and interrupts are setup in schedule.c */

void clock_setup(void){
	/*Operate at 168MHz*/
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
}

void usart_setup(void){
	//the USART header is attached to usart1

	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_USART1EN);
	//usart pins
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE,
						GPIO9|
						GPIO10);
	gpio_set_af(GPIOA, GPIO_AF7,		GPIO9|
						GPIO10);

	//configuring the usart
	usart_set_baudrate(USART1, 38400);
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

	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

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
	//dac_db bus
	//PC[0:7]   ->   DAC1[0:7]
	//PC[8:15]  ->   DAC2[0:7]
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPCEN);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_ALL);

	//dac-sleep -> PA11
	//dac-clk   -> PA12
	//both pins are shared between the 2 dac's	
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11|GPIO12);
	gpio_clear(GPIOA, GPIO11|GPIO12); //set the clock low, sleep off
}

void function_timer_setup(int timer_clock, int period){ //timer_clock in Khz

	int prescalar = (84000/timer_clock) -1;

	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM5EN);
	timer_reset(TIM5);
	timer_set_mode(TIM5, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_enable_irq(TIM5, TIM_DIER_UIE);	
	timer_set_period(TIM5, period);
	timer_set_prescaler(TIM5, prescalar);
}

void transmit_timer_setup(int ksps){
/*
To kick this off, just enable the timer with
timer_enable(TIM2);
*/
	int prescalar = 7; //gives a 12MHz timer clock
	int period = 12000/ksps;
	int half_period = period/2;

	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);
	timer_reset(TIM2);
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_period(TIM2, period);
	timer_set_prescaler(TIM2, prescalar); 

	timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_FROZEN);
	timer_enable_oc_preload(TIM2, TIM_OC1);
	timer_set_oc_value(TIM2, TIM_OC1, half_period);

	timer_enable_irq(TIM2, TIM_DIER_UIE); //Update event interrupt
	timer_enable_irq(TIM2, TIM_DIER_CC1IE); //Output compare 1 interrupt

	timer_set_counter(TIM2, 0);

	//enable interrupts on the timer
	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 1);
}
