volatile int coded_words;	//how many words the coded message contains
volatile int coded_bits; 	//how many bits in each coded word

void do_ecc(void);

void do_parity(void);	//sets the 9th bit of each word as parity
