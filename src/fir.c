#include "fir.h"

void generate_taps(void){
	//BLAM, lots of maths
	/*sample the the impulse response of the raised cosine
	filter at discrete intervals based on the defines in fir.h */
}

int generate_sample(void){
	//perform the FIR fitler

	//advance all the pointers and load a new symbol
	if (delay_offset<taps_between_symbols)
		delay_offset++;
	else {
		delay_offset = 0;
		symbols[0][symbol_ptr] = packet[0][packet_ptr]; //load I
		symbols[1][symbol_ptr] = packet[1][packet_ptr]; //load Q
		symbol_ptr++;
		if (symbol_ptr==symbol_buffer)
			symbol_prt = 0;
	}
