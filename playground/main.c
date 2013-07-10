#include "init.c"
#include "debug.c"
#include "leds.c"

char read_addr(char addr);

int main(void){
	clock_setup();
	usart_setup();
	debug_send("spi_setup()\n");
	spi_setup();
	debug_send("accel_setup()\n");
	accel_setup();
	debug_send("gpio_setup()\n");
	gipo_setup();
	int X;
	int i = 0;
	while(1){
		X = read_addr(0x29);
		debug_send_int(X);
		debug_send("\n");
		for(i=0; i<65000; i++) __asm__("NOP");
	}
}

char read_addr(char addr){
	int read;
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
