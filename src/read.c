//this file deals with reading in the data.
//the stm is set up as a slave on spi1
//When a packet is filled with raw data, new_packets is increased by 1
//When the last buffer is filled, buffers_full is set to true



void spi1_isr(void){
	packet_raw[input_buffer][read_ptr] = spi_read(SPI1);
	read_ptr++;
	if(read_ptr==PACKETLENGTH){
		read_ptr = 0;
		input_buffer++;
		if(input_buffer==BUFFERS)
			input_buffer=0;
		if(input_buffer==transmit_buffer){
			buffers_full = true;
			//buffers full pin goes high
			nvic_enable_irq(NVIC_SPI1_IRQ);
		}
	}
}
