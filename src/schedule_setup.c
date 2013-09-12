void setup_timers(int ksps){
	transmit_timer_setup(ksps);
	led_timer_setup();
	input_timeout_timer_setup();

	nvic_setup();
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
}

void led_timer_setup(void){
	//led update timer
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM3EN);
	timer_reset(TIM3);
	timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_enable_irq(TIM3, TIM_DIER_UIE);
	timer_set_period(TIM3, 1680);
	timer_set_prescaler(TIM3, 50000); //update once per second @ 84MHz
	timer_enable_preload(TIM3);
	timer_enable_counter(TIM3);
}

void input_timeout_timer_setup(void){
	//used for determining the end of packets that dont fill the whole buffer
	//reset the timer whenever a packet is received
	//turn off the timer and set to 0 at the end of a packet
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM4EN);
	timer_reset(TIM4);
	timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM4, 3); //timer clock runs @ 21MHz
	timer_set_period(TIM4, 21000); //1ms timeout
}

void nvic_setup(void){
	//setup all the interrupts for the timers + spi input
	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 1);	//main transmit sheduling interrupts

	nvic_enable_irq(NVIC_TIM3_IRQ);
	nvic_set_priority(NVIC_TIM3_IRQ, 1);	//led updates

	nvic_enable_irq(NVIC_TIM4_IRQ);
	nvic_set_priority(NVIC_TIM4_IRQ, 1);	//input timeouts
}
