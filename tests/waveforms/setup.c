#include "setup.h"

/* this file sets up all the on-chip peripherals, and
initializes the other stuff on the board for use
elsewhere. Timer and interrupt setups are at the bottom
of the file.*/

void clock_setup(void){
    //usart1 clock
    rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_USART2EN);
    //usart1 pins clock
    rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);
	
    //failed attempt at fixing funny baud rate
    //rcc_set_ppre2(RCC_CFGR_PPRE_DIV_4);
    //rcc_ppre2_frequency = 168000000/4;


	/*Operate at 168MHz*/
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
}

void usart_setup(void){
	//the USART header is attached to usart2

	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE,
						GPIO2|
						GPIO3);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);

	//configuring the usart
	usart_set_baudrate(USART2, 38400);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	//enabling the usart
	usart_enable(USART2);
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
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14|GPIO12);
	gpio_clear(GPIOA, GPIO14|GPIO12); //set the clock low, sleep off
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
    //ppre1 is already div2
    //@168MHz main clock, this gives us 84MHz to work with
	int prescalar = 3; //gives a 21MHz timer clock (prescalar=1+value)
	int period = 21000/ksps;
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
