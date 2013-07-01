/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "playground.h"
#include "leds.c"
#include "init.c"
#include "debug.c"
//#include <stdio.h>

/*
Accelerometer is on SPI1
CS -> PE3
*/

void tim2_isr(void){
	next_led(); //Toggle the leds
	timer_clear_flag(TIM2, TIM_SR_UIF);
	if (TIM_ARR(TIM2)<1000) updown = up;
	else if (TIM_ARR(TIM2)>20000) updown = down;
	if (updown == up) TIM_ARR(TIM2) += TIM_ARR(TIM2)/10;
	else TIM_ARR(TIM2) -= TIM_ARR(TIM2)/10;
	timer_set_counter(TIM2, 0);
	//TIM_SR(TIM2) &= ~TIM_SR_UIF; //remove the interrupt flag
	//TIM_CNT(TIM2) = 45000;
}



int main(void)
{
	clock_setup();
	gpio_setup();
	//nvic_setup();
	//timer_setup();
	usart_setup();
	int i = 0;
	int n = 0;
	while(1){
		char string[1];
		string[0] = '3';
		debug_send("\nhello world\n");
		debug_send(string);
		debug_send("The value of n is ");
		debug_send_int(n);
		//char string[6];
		//snprintf(string, "%d", n);
		//debug_send(string);
		next_led();
		n++;
		if(n==1000) n = 0;
		for(i=1; i<3000000; i++)
			__asm__ ("nop");
	}
}
