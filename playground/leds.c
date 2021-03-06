#include <libopencm3/stm32/f4/gpio.h>

void next_led(void);
void set_leds(int leds);

enum led {
	green	= 1<<12,
	orange 	= 1<<13,
	red 	= 1<<14,
	blue 	= 1<<15
};

static volatile enum led state = green;

void next_led(void){
//can form circling pattern
	switch(state){
	case green:
		state = orange;
		break;
	case orange:
		state = red;
		break;
	case red:
		state = blue;
		break;
	case blue:
		state = green;
		break;
	}
	set_leds(state);
}

void set_leds(int leds){
	///gpio_set(GPIOD, leds); //set the led's to be set
	//gpio_clear(GPIOD, ~(leds|(~(GPIO12|GPIO13|GPIO14|GPIO15))));
	//unset the led's to be unset
	GPIO_DR(GPIOD) = leds;
}
