#include "init.c"
#include "debug.c"
#include <libopencm3/stm32/f4/timer.h>

void timer_setup(void);
void nvic_setup(void);

int main(void){
	clock_setup();
	usart_setup();
	debug_send("\n\n\n*************************************\n");
	debug_send("*         Lynx playground           *\n");
	debug_send("*       Timer sheduling test        *\n");
	debug_send("*           Joe Roberts             *\n");
	debug_send("*        UROP - Summer 2013         *\n");
	debug_send("*************************************\n\n\n");
	debug_send("nvic_setup()\n");
	nvic_setup();
	debug_send("timer_setup()\n");
	timer_setup();

	while(1)
		__asm__("NOP"); //just chill here, the interrupts should do the fun things
}


void nvic_setup(void){
	int value = timer_get_counter(TIM3);
	nvic_clear_pending_irq(NVIC_TIM3_IRQ);
	nvic_enable_irq(NVIC_TIM3_IRQ);
	nvic_set_priority(NVIC_TIM3_IRQ, 1);
	debug_send_int(value);
	debug_send(" was the value of the counter when interrupts were enables\n\n");
}

void timer_setup(){
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM3EN);
	timer_reset(TIM3);
	timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_period(TIM3, 6720);
	timer_set_prescaler(TIM3, 25000); //reset every 2s @ 168MHz
	//timer_enable_preload(TIM2);

	timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_FROZEN);
	timer_enable_oc_preload(TIM3, TIM_OC1);
	timer_set_oc_value(TIM3, TIM_OC1, 3360); //1/2way through the the counting

	timer_enable_irq(TIM3, TIM_DIER_UIE); //update event interrupt
	timer_enable_irq(TIM3, TIM_DIER_CC1IE); //Output compare 1 interrupt

	timer_set_counter(TIM3, 0);
	debug_send_int(timer_get_counter(TIM3));
	debug_send(" is the value of counter at startup\n");
	debug_send_int(TIM_SR(TIM3));
	debug_send(" is the value of TIM3_SR at startup\n");
	timer_enable_counter(TIM3);

}

void tim3_isr(void){
	int value = timer_get_counter(TIM3);
	//debug_send("tim2_isr() has been called - ");

	if(timer_interrupt_source(TIM3, TIM_SR_CC1IF)){
		//debug_send("CC1IF was set\n");
		TIM_SR(TIM3) &= !(1<<1);
	}
	if(timer_interrupt_source(TIM3, TIM_SR_UIF)){
		//debug_send("UIF was set\n");
		TIM_SR(TIM3) &= !(1<<0);
	}
	debug_send_int(value);
	debug_send(" is the number in the counter\n\n");
	return;
}
