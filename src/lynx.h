#define ever (;;)

#define PACKETLENGTH 50	//Number of bytes in each transmit
#define BUFFERS 2	//Number of buffer sets
#define MAXPOINTS 64 	//the maximum number of points allowed in the constellation

#include "debug.c"
#include "ecc.c" 
#include "setup.c"
#include "pll.c"

//raw packets, 1 buffer and 1 active
volatile char packet_raw[BUFFERS][PACKETLENGTH];

//coded packet buffers
volatile short packet_ecc[BUFFERS][PACKETLENGTH*2];

//set of symbols to be transmitted
volatile short packet_constellation[BUFFERS][PACKETLENGTH*8][2];


volatile unsigned char input_buffer = 0;
volatile unsigned char ecc_buffer = 0;
volatile unsigned char constellation_buffer = 0;
volatile unsigned char transmit_buffer = 0;

char status = 0;
/* 
each bit of status represents something that has to be done

bit  |	Task
0	There is a packet ready to be mapped to a constellation
1	There is a packet ready to be error coded
2	There is a free buffer for new data
3-7 	unused

pulse shaping is carried out on a per symbol basis inside the interrupt

priority is built into the todo, carry out the task on the least
significant set bit first
*/
