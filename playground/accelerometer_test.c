#include "init.c"
#include "debug.c"

char read_addr(char addr);

int main(void){
	clock_setup();
	usart_setup();
	debug_send("spi_setup()\n");
	spi_setup();
	debug_send("accel_setup()\n");
	accel_setup();
	char X;
	char Y;
	char Z;
	int i = 0;
	while(1){
		X = read_addr(0x29);
		Y = read_addr(0x2B);
		Z = read_addr(0x2D);
		if(X<128) X +=128;
		else X -=128;
		//if(Y<128) X +=128;
		//else Y -=128;
		//if(Z<128) X +=128;
		//else Z -=128;
		debug_send_int(X);
		debug_send(",");
		debug_send_int(Y);
		debug_send(",");
		debug_send_int(Z);
		debug_send("\n");
		for(i=0; i<65534; i++) __asm__("NOP");
	}
}

char read_addr(char addr){
	char read;
	addr |= 1<<7;			//set the 'read' bit high

	gpio_clear(GPIOE, GPIO3);
	spi_send(SPI1, addr);
	read = spi_read(SPI1);
	spi_send(SPI1, 0);
	while(SPI_SR(SPI1)&SPI_SR_BSY);
	read = spi_read(SPI1);
	gpio_set(GPIOE, GPIO3);

	return read;
}
