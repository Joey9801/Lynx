#include "lynx.h"

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

	//Left out because no PLL soldered
	//debug_send("pll_setup(63, 16443, 3)");
	//pll_setup(63, 16443, 3); //n_div, r_div, o_div

	debug_send("setup_timers(100)");
	setup_timers(100); //setting a 100ksps sample rate


	for ever {
		//ecc_ready is set when a packet has finished being read in
		if(ecc_ready) {
			do_ecc();
			ecc_buffer++;
			if(ecc_buffer==BUFFERS)
				ecc_buffer = 0;
			if(ecc_buffer==input_buffer)
				ecc_ready = false;
			
			//check this again so it doesn't wait too long
			if((transmit_ready>0)&(!currently_transmitting)
				tim_enable(TIM2);

			do_constellation();
			constellation_buffer++;
			if(constellation_buffer==BUFFERS)
				constellation_buffer = 0;

			if(!currently_transmitting)
				timer_enable_counter(TIM2);
			else
				transmits_ready++;
		
		}
	}
	
	debug_send("We somehow escaped the for ever loop\n");
	debug_send("..This is a little sad, there's not much you can do to fix this\n\n\n\n");
	debug_send("goodbye, cruel world");
	while(1);
 return 1;
}

void read_check(void){
	if(read_ready) { //test for the read ready flag
			if(currently_reading){
				//there's already a read happening
				//do nothing
				return;
			}

			else{
				//turn on the read interrupt
				currently_reading = true;
				read_ready = false; //dont bother checking all this again
				return;
			}
		}
}
