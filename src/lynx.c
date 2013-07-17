#include "lynx.h"
#include "debug.c"
#include "ecc.c" 
#include "setup.c"
#include "pll.c"


int main (void){

	clock_setup();
	usart_setup();

	debug_send("*************************************\n");
	debug_send("*         Lynx starting up\n        *");
	debug_send("*           Joe Roberts\n           *");
	debug_send("*        UROP - Summer 2013\n       *");
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

	debug_send("timer_setup()");
	timer_setup();

	debug_send("nvic_setup()");
	nvic_setup();


	for ever {
		while(!status); //just chill while there isn't anything to do
		int i;
		for(i=0; i<=5; i++){
			if((status>>i)&1)
				break;
		}
			switch(i){
				case(0):
					//do some constellation mapping
					break;
				case(1):
					do_parity();
					break;
				case(2):
					//send the ready pin high
					//turn on the interrupts for data in
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
	debug_send("..This is a little sad, there's not much you can do to fix this\n\n");
	debug_send("goodbye, cruel world");
	while(1);
 return 1;
}
