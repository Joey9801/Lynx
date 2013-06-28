#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/spi.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/cm3/nvic.h>

char X, Y;
char accel_read(char axis);
char read_addr(char command);
void timer_setup(void);
void nvic_setup(void);

enum led {
	green	= 1<<12,
	orange 	= 1<<13,
	red 	= 1<<14,
	blue 	= 1<<15
};

static volatile enum led state = green;
void next_led(void);
void set_leds(int leds);

enum updown {
	up,
	down
};
static volatile enum updown updown = down;
