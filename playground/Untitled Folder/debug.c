void debug_send(const char *data)
{
    while (*data)
    {
        usart_send_blocking(USART2, *data);
        data++;
    }
}

void debug_send_int(int number){
	int length = len(number);
	char string[length+1];
	int i;
	for(i=0; i<length; i++) {
		string[length-i] = number%10 + '0';
		number = number/10;
	}
	debug_send(string);
}

int len(int n){
	if(n<10) return 1;
	return 1 + len(n/10);
}
