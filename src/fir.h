#define TAPS 51 //should be an odd number
#define ROLLOFF 0.5

#define TAPS_PER_SYMBOL 5 //should be a factor of taps-1
#define TAPS_BETWEEN_SYMBOLS (TAPS_PER_SYMBOL-1)
#define SYMBOL_BUFFER (TAPS/TAPS_PER_SYMBOL)

#define PI 3.14159265


float taps[TAPS];

//circular buffer for symbols in the filter delay line
unsigned char symbol_buffer[2][SYMBOL_BUFFER];
//points at the oldest value in the buffer
unsigned char symbol_ptr = 0;
//points at the current symbol in the packet
//how many 0's there currently are at the start of the delay line
unsigned char delay_offset = 0;

//generates taps for raised cosine filter
void generate_taps(void);

void reset_fir(void);
float raised_cosine_impulse(int t);



//computes raised cosine filter
//advances delay line by one
//called by timer interrupt
void generate_sample(void);
