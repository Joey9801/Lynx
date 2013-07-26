#define ever (;;)

#define PACKETLENGTH 50	//Number of bytes in each transmit
#define BUFFERS 2	//Number of buffer sets
#define MAXPOINTS 64 	//the maximum number of points allowed in the constellation

//raw packets, 1 buffer and 1 active
volatile char packet_raw[BUFFERS][PACKETLENGTH];

//coded packet buffers
volatile short packet_ecc[BUFFERS][PACKETLENGTH*2];

//set of symbols to be transmitted
volatile short packet_constellation[BUFFERS][PACKETLENGTH*8][2];

volatile char next_transmit[2];


volatile unsigned char input_buffer = 0;
volatile unsigned char ecc_buffer = 0;
volatile unsigned char constellation_buffer = 0;
volatile unsigned char transmit_buffer = 0;

//points to the next symbol to be transmitted in packet_constellation
volatile unsigned short transmit_ptr = 0;

/*
The following are status values.
They are written to by the various algorithms when they start/finish.
They are read by the main loop, which then calls the relevant functions
*/

//There is a packet ready to be transmitted
volatile bool transmit_ready = false;
//There is a packet ready to be mapped to a constellation
volatile bool constellation_ready = false;
//There is a packet ready to be error coded
volatile bool ecc_ready = false;
//There is a free buffer for new data
volatile bool read_ready = true;
//There is a transmit currently happening
volatile bool currently_transmitting = false;
//The current read buffer is still being filled+hasn't timed out
volatile bool currently_reading = true;
