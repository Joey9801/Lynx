volatile int coded_words;	//how many words the coded message contains
volatile int coded_bits; 	//how many bits in each coded word

void set_parity(void);	//sets the 9th bit of each word as parity