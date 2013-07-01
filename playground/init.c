static void clock_setup(void){
	/*Operate at 168MHz*/
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_48MHZ]);

	//usart2
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_USART2EN);

	//usart port
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);

	//led port
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
}

static void gpio_setup(void){
	//setup the led pins
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 
						GPIO12| 
						GPIO13| 
						GPIO14| 
						GPIO15);
	//usart pins
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE,
						GPIO2|
						GPIO3);
	gpio_set_af(GPIOA, GPIO_AF7,		GPIO2|
						GPIO3);
}

void nvic_setup(void){
	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 1);
}

void timer_setup(){
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);
	timer_reset(TIM2);
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	//timer_enable_update_event(TIM2);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
	timer_set_period(TIM2, 25000);
	timer_set_prescaler(TIM2, 1000);
	timer_enable_preload(TIM2);
	timer_enable_counter(TIM2);
	//TIM_CNT(TIM2) = 1; //Start counting at 1
	//TIM_PSC(TIM2) = 1500; //Arbitrary prescalar so we can see the blinking
	//TIM_ARR(TIM2) = 10000; //Timer end value
	//TIM_DIER(TIM2) |= TIM_DIER_UIE; //Enable interrupt on update
	//TIM_CR1(TIM2) |= (1<<4); //Downcounting mode
	//TIM_CR1(TIM2) |= TIM_CR1_CEN; //start the timer
	
}

void usart_setup(void){
	//using UART4 on PC10(tx) and PC11(rx)
	/* set the baudrate
	set the data and stop bits
	set the mode (tx/rx etc)
	set parity bits
	set flow control - may not apply for uart
	enable the thing */
	usart_set_baudrate(USART2, 38400);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	usart_enable(USART2);
}