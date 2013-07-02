#include "init.h"


void clock_init(void){
	//setup the arm clock
}

void dac_init(void){
	//setup the relevant gpio pins
	//any pause required for setup
	//any initializing commands needed by the dac
}

void pll_init(void){
	//setup the gpio's needed to talk to the pll
	//setup SPI2
	debug_send("Initalizing PLL");
	//talk nicely to the pll
	//wait for lock
	debug_send("PLL locked");
}

void data_init(void){
	//initialize SPI1 in slave mode for incoming data stream
	//init gpio status pins (CTS etc..)
}

void led_init(void){
	//setup the gpio pins for the status leds
}

void nvic_init(void){
	//setup the timer interrupts for status leds
	//"" "" new sample
	//"" "" new symbol
}
