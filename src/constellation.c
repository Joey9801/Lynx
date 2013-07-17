#include "constellation.h"


void set_constellation(int points){
	//some sort of magic to generate a constellation
	//set the current size variable

	//placeholder static constellation
	current_size = 4;
	constellation[0][0] = 100; constellation[0][1] = 100;
	constellation[1][0] = 100; constellation[1][1] = -100;
	constellation[2][0] = -100; constellation[2][1] = 100;
	constellation[3][0] = -100; constellation[3][1] = -100;
	return;
}

void do_constellation(void){

	//turn the ecc packet into a series of I/Q locations

	constellation_buffer++;
	if(constellation_buffer==BUFFERS) //advance the buffer pointer
		constellation_buffer = 0;
	if(constellation_buffer==ecc_buffer)
		status &= !(1<<0); //still waiting for the next dataset to be coded
	return;
}
