void set_constellation(int points);
void do_constellation(void);


//Lookup table for constellation co-ordinate
//constellation[n][0] is the I co-ordinate of the symbol n
//constellation[n][1] is the Q co-ordinate of the symbol n
char constellation[MAXPOINTS][2];
unsigned int current_size;
