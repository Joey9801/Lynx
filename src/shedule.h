#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/f4/rcc.h>

void setup_timers(int ksps);

void transmit_timer_setup(int ksps);
void led_timer_setup(void);
void input_timeout_timer_setup(void);
void nvic_setup(void);

/*
Tim2 -> transmit things
Tim3 -> led updated
Tim4 -> input receive timeout
*/
