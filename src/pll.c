#include "pll.h"

void pll_setup(void){

	char value;	//working value
	set_pll_stat(LOCK); //STAT pin is high when locked

	debug_send("pll_setup(): \"Powering down RF\"\n");
	//power down all the things: PDOUT, PDPLL, PDVCO
	value = pll_read(0x02)&(0b10001111);
	value |= (1<<4)|(1<<5)|(1<<6);
	pll_write(0x02, value);

	debug_send("pll_setup(): \"setting R Div to 63\"\n");
	//set R Div to 63

	debug_send("pll_setup(): \"Setting N Div to 16443\"\n");
	//set N Div to 16443

	debug_send("pll_setup(): \"Setting O Div to 3\"");
	//set O Div to 3

	debug_send("pll_setup(): \"Setting CP output current to 11.2mA\"\n");
	//set CP[0:3] to '11'
	value = pll_read(0x09)&(0b11110000);
	value |= 11;
	pll_write(0x09, value);

	debug_send("pll_setup(): \"Powering up the RF\"\n");
	//power up the vco and pll

	debug_send("pll_setup(): \"Waiting for lock\"");
	volatile int i;
	function_timer_setup(1, 500); //1KHz, period of 500
	timer_enable_counter(TIM5);
	while(i<10){
		//if locked, break
		if(timer_get_flag(TIM_SR_UIF)){
			i++;
			debug_send(".");
			if(i==5) { //total wait of 2 seconds
				debug_send("\npll_setup(): \"Lock failure - powering down\"\n\n");
				//Power down the PLL
			}
		}
		if((gpio_get(GPIOA, GPIO2)>>2) {
			debug_send("\npll_setup(): \"PLL reports lock - continuing\"\n\n");
	}
	//wait for lock with 1 second timeout from tim5

	//send some debug messages whether it locked or not

}

//configures the STAT pin with the mask
void set_pll_stat(enum pll_stat_mask stat_mask) {
	pll_send(0x01, stat_mask);
	return;
}


//sends data to the pll in a blocking way
void pll_write(char addr, char data) {
	addr = addr<<1;
	addr &= !(1<<0); //clear the read/write bit for writing

	gpio_clear(GPIOB, GPIO12);

	spi_send(SPI2, addr);
	spi_send(SPI2, data);
	while(SPI_SR(SPI1)&SPI_SR_BSY);

	gpio_set(GPIOB, GPIO12);
	return;
}


char pll_read(char addr) {
	addr = addr<<1;
	addr |= (1<<0); //set the read/write bit for reading

	gpio_clear(GPIOB, GPIO12);

	spi_send(SPI2, addr);
	spi_send(SPI2, 0);
	while(SPI_SR(SPI1)&SPI_SR_BSY);

	gpio_set(GPIOB, GPIO12);

	return spi_read(SPI2);
}
