#define PI 3.14159265

volatile int n = 255; //number of samples per cycle

//the next two analogue values to be sent to the dac pair
volatile int next_transmit[2];

volatile int i = 0; //which sample is currently in next_transmit[]

void generate_sample(void);
