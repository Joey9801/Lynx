#DEFINE ever (;;)

#DEFINE PACKETLENGTH 50	//Number of bytes in each transmit
#DEFINE BUFFERS 2	//Number of buffer sets

//raw packets, 1 buffer and 1 active
volatile char packet_raw[BUFFERS][PACKETLENGTH];

//coded packet. Buffer as before
volatile short packet_ecc[BUFFERS][PACKETLENGTH*2];


volatile char input_buffer = 0;
volatile char coding_buffer = 0;
volatile char transmit_buffer = 0;

char status = 0;
/* 
each bit of status represents something that has to be done

bit  |	Task
0	There are packets not mapped to constellations
1	There are packets to be coded
2	There is space for more raw to be read in
3-7 	unused

pulse shaping is carried out on a per symbol basis inside the interrupt



priority is built into the todo, carry out the task on the least
significant set bit first
*/
