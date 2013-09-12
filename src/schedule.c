#include "schedule.h"
#include "schedule_setup.c"


//data is sent to the dacs in two stages with tim2
//tim3 is used to update the status led's in leds.c
//tim4 is used as a timeout for the raw data input.

//tim2 is only counting during an active transmit


//DAC control interrupt
void tim2_isr(void){
	if(timer_interrupt_source(TIM3, TIM_SR_CC1IF)){
		gpio_set(GPIOA, GPIO12); //set the pin high. Must do this first for timing
		timer_clear_flag(TIM2, TIM_SR_CC1IF);
		//detect end of packet transmission
		/* if(end of packet){
			currently_transmitting = false;
			timer_disable_counter(TIM2);
			timer_set_counter(TIM2, 0);
			gpio_clear(GPIOA, GPIO12); //set DAC-CLK low
		} */
	}
	if(timer_interrupt_source(TIM3, TIM_SR_UIF)){
		GPIO_DR(GPIOC) = out[1] | (out[0]<<8); //set the dac-db pins
		gpio_clear(GPIOA, GPIO12); //set DAC-CLK low
		timer_clear_flag(TIM2, TIM_SR_UIF); //reset the interrupt flag
		generate_sample();
	}
	return;
}

//input timeout interrupt
void tim4_isr(void){
	//stop the timer
	//cancle the current input read
	//fill the rest of the current input buffer with 0's
	//set the right status flags for read-in finish
	timer_clear_flag(TIM4, TIM_SR_UIF);
	return;
}
