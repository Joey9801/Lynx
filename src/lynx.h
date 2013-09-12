#include <stdbool.h>

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

//the next two analogue values to be sent to the dac pair
volatile char next_transmit[2];


volatile unsigned char input_buffer = 0;
volatile unsigned char ecc_buffer = 0;
volatile unsigned char constellation_buffer = 0;
volatile unsigned char transmit_buffer = 0;

//the address of the next byte to be read in
volatile unsigned short read_ptr = 0;

//points to the next symbol to be transmitted in packet_constellation
volatile unsigned short transmit_ptr = 0;

//There is a packet ready to be transmitted
volatile char transmits_ready = 0;

//There is a transmit currently happening
volatile bool currently_transmitting = false;

//All the buffers are full, ie, input buffer = transmit_buffer
volatile bool buffers_full = false;
