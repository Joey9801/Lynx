#include "waveforms.h"

#include "setup.c"
#include "fir.c"
#include "schedule.c"
#include "debug.c"
#include "ecc.c" 
#include "pll.c"
#include "constellation.c"
#include "read.c"


int main (void){

	clock_setup(); //setup the main clock for 168MHz
	usart_setup(); //setup usart1 for debug messages

	debug_send("\n\n\n*************************************\n");
	debug_send("*      Lynx test starting up        *\n");
	debug_send("*            Waveforms              *\n");
	debug_send("*           Joe Roberts             *\n");
	debug_send("*        UROP - Summer 2013         *\n");
	debug_send("*************************************\n\n\n");

	debug_send("ledpins_setup()");
	ledpins_setup(); //setup the status led's gpio's

	debug_send("dac_setup()");
	dac_setup(); //setup the dac gpio's

	debug_send("transmit_timer_setup(10)");
	transmit_timer_setup(10); //setup the transmit timer and its interrupt


	for ever {
		if(new instruction){
			send a debug message
			wait for the argument
			effect the instruction
			clear the usart
		}


	}
	
	debug_send("We somehow escaped the for ever loop\n");
	debug_send("..This is a little sad, there's not much you can do to fix this\n\n\n\n");
	debug_send("goodbye, cruel world");
	while(1);
 return 1;
}
