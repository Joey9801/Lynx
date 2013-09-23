#include "waveforms.h"

#include "setup.c"
#include "debug.c"
#include "transmit.c"



int main (void){

	clock_setup(); //setup the main clock for 168MHz
	usart_setup(); //setup usart1 for debug messages

	debug_send("\n\n\n*************************************\n");
	debug_send("*      Lynx test starting up        *\n");
	debug_send("*            Waveforms              *\n");
	debug_send("*           Joe Roberts             *\n");
	debug_send("*        UROP - Summer 2013         *\n");
	debug_send("*************************************\n\n\n");

	debug_send("ledpins_setup()\n");
	ledpins_setup(); //setup the status led's gpio's

	debug_send("dac_setup()\n");
	dac_setup(); //setup the dac gpio's

	debug_send("transmit_timer_setup(1)\n");
	transmit_timer_setup(1); //setup the transmit timer and its interrupt

    debug_send("Starting the transmission timer\n");
    timer_enable_counter(TIM2);


    while(1){
		usart_wait_recv_ready(USART2);
        timer_disable_counter(TIM2);
        timer_set_counter(TIM2, 0);
        char message = usart_recv_blocking(USART2);
	    debug_send("\nRecieved ");
		usart_send_blocking(USART2, message);
        bool done = false;
        char param;
        while(!done){
	        debug_send(": Send your parameter [0-9]\n");
            usart_wait_recv_ready(USART2);
            param = usart_recv_blocking(USART2);
            if((param>47)&&(param<58)){
                done = true;
                param = param-48; //ASCII to number
            }
            else
                debug_send("\nParameter must be [0-9] - try again\n");
        }
        if(message=='n')
            n = 1000*param;
        if(message=='t'){
            transmit_timer_setup(10*param);
            timer_enable_counter(TIM2);
        }
        if(message=='w')
            set_waveform(param);
	}

	
	debug_send("We somehow escaped the for ever loop\n");
	debug_send("..This is a little sad, there's not much you can do to fix this\n\n\n\n");
	debug_send("goodbye, cruel world");
	while(1);
 return 1;
}

void set_waveform(char param){
    switch(param){
        case(0):
            current_waveform = square;
            break;
        case(1):
            current_waveform = sinw;
            break;
        case(2):
            current_waveform = sawtooth;
            break;
        case(3):
            current_waveform = triangle;
            break;
        default:
            break;

    }
    return;
}
