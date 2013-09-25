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

	int shift;
	int sym_per_byte;
	int overlap;

    switch(current_size){
        case(4):
            shift = 2;
			sym_per_byte = 4;
			overlap = 0;
			break;
		default:
			debug_send("\n\nUnrecognized constellation size: ");
			debug_send_int(current_size);
			debug_send("\nAborting constellation map\n");
			return;
	}

	int i;
	int j;
	unsigned char temp;
	for(i=0; i<=coded_words; i++){
		for(j=0; j<=sym_per_byte; j++){
			temp = packet_ecc[constellation_buffer][i];
			temp = 0x03 & (temp>>(shift*j));
			packet_constellation[constellation_buffer][(i+1)*j][0]
				= constellation[temp][0];
			packet_constellation[constellation_buffer][(i+1)*j][1]
				= constellation[temp][1];
		}

	}
	return;
}
