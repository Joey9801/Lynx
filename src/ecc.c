#include "ecc.h"

//sets the 9th bit of each word as parity
void set_parity(void){
	coded_words=PACKETLENGTH;	//no extra words added
	coded_bits = 9;			//1 extra bit per word added
	int i;
	int ii;
	int n;
	for(i=0; i<PACKETLENGTH; i++){
		n = 0;
		for(ii=0; ii<7; ii++){
			if( (packet_raw[0][i]&(1<<ii)) == (1<<ii) )
				n++; //count set bits in message
		}
		packet_coded[0][i] = packet_raw[0][i];
		if(!(n%2)) //set even parity bit
			packet_coded[0][i] |= 1<<8;
	}
}
