#include "lynx.h"

#include "setup.c"
#include "shedule.c"
#include "debug.c"
#include "ecc.c" 
#include "pll.c"
#include "constellation.c"
#include "fir.c"


int main (void){

	clock_setup(); //setup the main clock for 168MHz
	usart_setup(); //setup usart1 for debug messages

	debug_send("\n\n\n*************************************\n");
	debug_send("*         Lynx starting up          *\n");
	debug_send("*           Joe Roberts             *\n");
	debug_send("*        UROP - Summer 2013         *\n");
	debug_send("*************************************\n\n\n");

	debug_send("ledpins_setup()");
	ledpins_setup(); //setup the status led's gpio's

	debug_send("spi1_setup()");
	spi1_setup(); //setup spi1 as slave for reading data in

	debug_send("spi2_setup()");
	spi2_setup(); //setup spi2 as master for writing to the pll

	debug_send("dac_setup()");
	dac_setup(); //setup the dac gpio's

	debug_send("pll_setup(63, 16443, 3)");
	pll_setup(63, 16443, 3); //n_div, r_div, o_div

	debug_send("setup_timers(500)");
	setup_timers(500); //setting a 500ksps sample rate


	for ever {
		if(transmit_ready) { //test for the transmit ready flag
			if(currently_transmitting){
				//there's already a transmit happening
			}

			else{
				//turn on the transmit interrupt
			}
		}
		if(read_ready) { //test for the read ready flag
			if(currently_reading){
				//there's already a read happening
			}

			else{
				//turn on the read interrupt
				//set the read ready pin high
			}
		}
		if(constellation_ready) { //packet ready for constellation coding
			do_constellation();
		}
		else if(ecc_ready) { //packet ready for error coding
			do_parity();
		}
	}
	
	debug_send("We somehow escaped the for ever loop\n");
	debug_send("..This is a little sad, there's not much you can do to fix this\n\n\n\n");
	debug_send("goodbye, cruel world");
	while(1);
 return 1;
}
