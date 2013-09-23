#include "fir.h"
#include <math.h>

void generate_taps(void){
	//BLAM, lots of maths
	/*sample the the impulse response of the raised cosine
	filter at discrete intervals based on the defines in fir.h */
	int t;
	for(t=0-(TAPS-1)/2; t<(TAPS-1)/2; t++){
		taps[t+(TAPS-1)/2] = raised_cosine_impulse(t);
	}	
}

void reset_fir(void){
	//set all the pointers back to their origin
	symbol_ptr = 0;
	delay_offset = 0;
	//clear the circular buffer
	int i;
	for(i=0; i<SYMBOL_BUFFER; i++){
		symbol_buffer[0][i] = 0;
		symbol_buffer[1][i] = 0;
	}
}

float raised_cosine_impulse(int t){
#include <math.h>
	float h = 0;
    h = sin(PI * (t/TAPS_PER_SYMBOL));
	h = h*cos((PI * ROLLOFF * t)/TAPS_PER_SYMBOL);
	h = h / ( 1 - ((4*ROLLOFF*ROLLOFF*t*t)/(TAPS_PER_SYMBOL*TAPS_PER_SYMBOL)));
	return h;
}

void generate_sample(void){
	//perform the FIR fitler. Put results in next_transmit[]
	int i = symbol_ptr-1; //'i' is the location of the newest symbol here
	next_transmit[0] = 0; //I channel output
	next_transmit[1] = 0; //Q channel output
	int tap;
	for(tap = delay_offset; tap < TAPS; tap += TAPS_PER_SYMBOL){
		next_transmit[0] += taps[tap] * symbol_buffer[0][i];
		next_transmit[1] += taps[tap] * symbol_buffer[1][i];
		i--; //'i' decends, pointing at older and older symbols
		if(i==-1)
			i=SYMBOL_BUFFER-1;
	}
	//advance all the pointers and load a new symbol
	if (delay_offset<TAPS_BETWEEN_SYMBOLS)
		delay_offset++;
	else {
		delay_offset = 0;
		symbol_buffer[0][symbol_ptr] = packet_constellation[transmit_buffer][transmit_ptr][0]; //load I
		symbol_buffer[1][symbol_ptr] = packet_constellation[transmit_buffer][transmit_ptr][1]; //load Q
		symbol_ptr++;
		if (symbol_ptr==SYMBOL_BUFFER)
			symbol_ptr = 0;
	}
}
