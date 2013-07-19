#include "shedule.h"
#include "shedule_setup.c"

void tim2_isr(void){
	if(timer_interrupt_source(TIM3, TIM_SR_CC1IF)){
		//set dac-clk
		TIM_SR(TIM3) &= !(1<<1);
	}
	if(timer_interrupt_source(TIM3, TIM_SR_UIF)){
		//set the dac-db port
		TIM_SR(TIM3) &= !(1<<0);
		//calculate the next filtered value and store it in the buffer
	}
	return;
}

void tim3_isr(void){
	//update those leds for whatever massive important task they have
	timer_clear_flag(TIM3, TIM_SR_UIF);
}

void tim4_isr(void){
	//stop the timer
	//cancle the current input read
	//fill the rest of the current input buffer with 0's
	//set the right status flags for read-in finish
	timer_clear_flag(TIM4, TIM_SR_UIF);
	return;
}
