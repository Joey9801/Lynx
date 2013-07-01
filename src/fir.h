#define taps 50
#define rolloff 0.5

#define taps_per_symbol 5 //should be a factor of taps
#define taps_between_symbols (taps_per_symbol-1)
#define symbol_buffer (taps/taps_per_symbol)


volatile const int taps[TAPS];
volatile const int out[2]; //0->I, 1->Q

//circular buffer for symbols in the filter delay line
volatile const char symbols[2][symbol_buffer];
//points at the oldest value in the buffer
volatile const char symbol_ptr = 0;
//points at the current symbol in the packet
//how many 0's there currently are at the start of the delay line
volatile const char delay_offset = 0;

//generates taps for raised cosine filter
void generate_taps(void);

//computes raised cosine filter
//advances delay line by one
//called by timer interrupt
void generate_sample(void);

