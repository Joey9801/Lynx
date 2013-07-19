#include "lynx.h"

#include "setup.c"
#include "shedule.c"
#include "debug.c"
#include "ecc.c" 
#include "pll.c"
#include "constellation.c"


int main (void){

	clock_setup();
	usart_setup();

	debug_send("*************************************\n");
	debug_send("*         Lynx starting up          *\n");
	debug_send("*           Joe Roberts             *\n");
	debug_send("*        UROP - Summer 2013         *\n");
	debug_send("*************************************\n\n\n");

	debug_send("ledpins_setup()");
	ledpins_setup();

	debug_send("spi1_setup()");
	spi1_setup();

	debug_send("spi2_setup()");
	spi2_setup();

	debug_send("dac_setup()");
	dac_setup();

	debug_send("pll_setup()");
	pll_setup();

	debug_send("setup_timers()");
	setup_timers(500); //setting a 500ksps sample rate


	for ever {
		int i;
		for(i=0; i<=4; i++){
			if((status>>i)&1)
				break; //break with the highest priority task
		}
			switch(i){
				case(0):
					if((status>>4)&1)
						//there's already a transmit happening
						break;
					else{
						//turn on the transmit interrupt
					}
					break;
				case(1):
					do_constellation();
					break;
				case(2):
					do_parity();
					break;
				case(3):
					if((status>>5)&1)
						//we're already recieving something
						break;
					else{
						//send the ready pin high
						//turn on the interrupts for recieve
					}
					break;
				default:
					debug_send("Something funky happened to the status register\n");
					debug_send("The value of 'status', is ");
					debug_send_int(status);
					debug_send("\n");
					break;
		
			}
	}
	
	debug_send("We somehow escaped the for ever loop\n");
	debug_send("..This is a little sad, there's not much you can do to fix this\n\n\n\n");
	debug_send("goodbye, cruel world");
	while(1);
 return 1;
}
