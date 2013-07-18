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


volatile unsigned char input_buffer = 0;
volatile unsigned char ecc_buffer = 0;
volatile unsigned char constellation_buffer = 0;
volatile unsigned char transmit_buffer = 0;

char status = 0;
/* 
each bit of status represents something that has to be done

bit  |	Task
0	There is a packet ready to be transmitted
1	There is a packet ready to be mapped to a constellation
2	There is a packet ready to be error coded
3	There is a free buffer for new data
4	Currently transmitting
5	Currently Recieving
6-7	unused

pulse shaping is carried out on a per symbol basis inside the transmit interrupt

priority is built into the todo, carry out the task on the least
significant set bit first
*/

/*
When a task completes, it must set the next tasks flag and advance
it's own pointer. If its pointer now equals the one below, it must
reset its own flag.
*/
