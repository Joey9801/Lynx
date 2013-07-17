#include "ecc.h"

//sets the 9th bit of each word as parity
void do_parity(void){
	coded_words=PACKETLENGTH;	//no extra words added
	coded_bits = 9;			//1 extra bit per word added
	int i;
	int ii;
	int n;
	for(i=0; i<PACKETLENGTH; i++){
		n = 0;
		for(ii=0; ii<7; ii++){
			if( (packet_raw[coding_buffer][i]&(1<<ii)) == (1<<ii) )
				n++; //count set bits in message
		}
		packet_ecc[coding_buffer][i] = packet_raw[coding_buffer][i];
		if(!(n%2)) //set even parity bit
			packet_ecc[coding_buffer][i] |= 1<<8;
	}

	//set the status register and buffer pointers appropriately
	coding_buffer++;
	if(coding_buffer==BUFFERS) //advance the buffer pointer
		coding_buffer = 0;
	if(coding_buffer==input_buffer) //still waiting for a new dataset
		status &= !(1<<1);
}
