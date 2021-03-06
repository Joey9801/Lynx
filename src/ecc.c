#include "ecc.h"

void do_ecc(void){
	do_parity();
	return;
}

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
			if( (packet_raw[ecc_buffer][i]&(1<<ii)) == (1<<ii) )
				n++; //count set bits in message
		}
		packet_ecc[ecc_buffer][i] = packet_raw[ecc_buffer][i];
		if(!(n%2)) //set even parity bit
			packet_ecc[ecc_buffer][i] |= 1<<8;
	}
}
