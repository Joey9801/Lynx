#include "pll.h"

void pll_setup(int r_div, int n_div, int o_div){

	char value;	//working value
	set_pll_stat(LOCK); //STAT pin is high when locked

	debug_send("pll_setup(): \"Powering down RF\"\n");
	//power down all the things: PDOUT, PDPLL, PDVCO
	value = pll_read(0x02)&(0b10001111);
	value |=	(1<<4)|	//Power down output stage
				(1<<5)|	//Power down VCO
				(1<<6);	//Power down PLL
	pll_write(0x02, value);

	debug_send("pll_setup(): \"setting R Div to ");
	debug_send_int(r_div);
	debug_send("\"\n");
	//set R Div
	//R div uses RD[0:9]
	//reg h04[0:7] is RD[0:7], reg h03[0:1] is RD[8:9]
	value = r_div&0xFF; //first 8 bits
	pll_write(0x04, value);
	value = pll_read(0x03)&0b11111100; //mask original contents of 0x03
	value |= (r_div&0x300)>>8; //most significant 2 bits
	pll_write(0x03, value);

	debug_send("pll_setup(): \"Setting N Div to ");
	debug_send_int(n_div);
	debug_send("\"\n");
	//set N Div
	//N div uses ND[0:15]
	//reg h05 is ND[8:15], reg h06 is ND[0:7]
	value = n_div&0xFF; //LS 8 bits
	pll_write(0x06, value);
	value = (r_div&0xFF00)>>8; //MS 8 bits
	pll_write(0x05, value);


	debug_send("pll_setup(): \"Setting O Div to ");
	debug_send_int(o_div);
	debug_send("\"\n");
	//set O Div to 3
	//O div uses OD[0:2]
	//OD[0:2] is on h08[0:2]
	value = pll_read(0x08)&(0b11111000);
	value |= o_div;
	pll_write(0x08, value);

	debug_send("pll_setup(): \"Setting CP output current to 11.2mA\"\n");
	//set CP[0:3] to '11'
	//CP[0:3] is on reg h09[0:3]
	value = pll_read(0x09)&(0b11110000);
	value |= 11;
	pll_write(0x09, value);

	debug_send("pll_setup(): \"Powering up the RF\"\n");
	value = pll_read(0x02)&(0b10001111); //power up the things
	value |= (1<<1); //mute the output
	pll_write(0x02, value);


	debug_send("pll_setup(): \"Waiting for lock\"");
	volatile int i;
	function_timer_setup(1, 500); //1KHz, period of 500
	timer_enable_counter(TIM5);
	while(i<10){
		//if locked, break
		if(timer_get_flag(TIM5, TIM_SR_UIF)){
			i++;
			debug_send(".");
			timer_clear_flag(TIM5, TIM_SR_UIF); //clear the flag
			if(i==5) { //total wait of 2 seconds
				debug_send("\npll_setup(): \"Lock failure - powering down\"\n\n");
				//Power down the PLL
				value = pll_read(0x02)&(0b10001111);
				value |=	(1<<4)|	//Power down output stage
							(1<<5)|	//Power down VCO
							(1<<6);	//Power down PLL
				pll_write(0x02, value);
			}
		}
		if((gpio_get(GPIOA, GPIO2))>>2) {
			debug_send("\npll_setup(): \"PLL reports lock - continuing\"\n\n");
		}
	}

}

//configures the STAT pin with the mask
void set_pll_stat(enum pll_stat_mask stat_mask) {
	pll_write(0x01, stat_mask);
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
