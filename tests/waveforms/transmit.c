#include "transmit.h"
//#include <math.h>

void tim2_isr(void){
	if(timer_interrupt_source(TIM2, TIM_SR_CC1IF)){
		gpio_set(GPIOA, GPIO14);
		timer_clear_flag(TIM2, TIM_SR_CC1IF);

		//now to generate the next sample
		generate_sample();
		i++;
		if(i==n)
			i=0;
	}
	if(timer_interrupt_source(TIM2, TIM_SR_UIF)){
		gpio_clear(GPIOA, GPIO14); //set DAC-CLK low
		gpio_port_write(GPIOC, (next_transmit[1] | (next_transmit[0]<<8))); //set the dac-db pins
		timer_clear_flag(TIM2, TIM_SR_UIF);
	}
	return;
}

void generate_sample(void){
	switch(current_waveform){
		float temp;
		case(square):
			if(i>(n/2)){
				next_transmit[0] = 255;
				next_transmit[1] = 255;
			}
			else{
				next_transmit[0] = 0;
				next_transmit[1] = 0;
			}
			break;

		/*case(sinw):
			temp = (i/n)*2*PI;
			temp = sin(temp);
			temp = temp+1;
			temp = (temp/2)*256;
			next_transmit[0] = temp;
			next_transmit[1] = temp;
			break; */

		case(sawtooth):
			temp = (i*255)/n;
			next_transmit[0] = temp;
			next_transmit[1] = temp;
			break;

		case(triangle):
			if(i>(n/2)){
				temp = (i*255*2)/n;
				next_transmit[0] = temp;
				next_transmit[1] = temp;
			}
			else{
				temp = ((n-i)*255*2)/n;
				next_transmit[0] = temp;
				next_transmit[1] = temp;
			}
            break;
        default:
            break;
        }
return;
}
