#include "setup.h"

void clock_setup(void){
	/*Operate at 168MHz*/
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_48MHZ]);
}

void usart_setup(void){
	//setup the usart
	//the debug header is on USART1


	//below pasted from working playground
	
	//usart clock
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_USART1EN);
	//usart pins
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE,
						GPIO9|
						GPIO10);
	gpio_set_af(GPIOA, GPIO_AF7,		GPIO2|
						GPIO3);

	//configuring the usart
	usart_set_baudrate(USART2, 38400);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	//enabling the usart
	usart_enable(USART1);
}


void led_setup(void){
	//leds are all on port b
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN);
	//Set all as push/pull outputs
	gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, 
						STAT1_GR| 
						STAT1_RE| 
						STAT2_GR| 
						STAT2_RE);

}


//spi1 -> data input
//spi2 -> pll control
void spi_setup(void){

	//setup the chip select
	//may be able to use hardware instead of the hacky


	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, 
						GPIO13|
						GPIO14|
						GPIO15);
	gpio_set_af(GPIOA, GPIO_AF5, GPIO5|GPIO6|GPIO7);
	
	spi_init_master(SPI2, SPI_CR1_BAUDRATE_FPCLK_DIV_32,
			/* high or low for the peripheral device */
			SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
			/* CPHA: Clock phase: read on rising edge of clock */
			SPI_CR1_CPHA_CLK_TRANSITION_2,
			/* DFF: Date frame format (8 or 16 bit) */
			SPI_CR1_DFF_8BIT,
			/* Most or Least Sig Bit First */
			SPI_CR1_MSBFIRST);

	spi_enable_software_slave_management(SPI1);
	spi_set_nss_high(SPI1);


	//enable SPI1
	spi_enable(SPI1);
}



