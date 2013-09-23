#define LED_PORT GPIOB
#define STAT1_GR GPIO0
#define STAT1_RE GPIO1
#define STAT2_GR GPIO2
#define STAT2_RE GPIO10

#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/spi.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/cm3/nvic.h>


void clock_setup(void);
void usart_setup(void);
void ledpins_setup(void);
void spi1_setup(void);
void spi2_setup(void);
void dac_setup(void);
void function_timer_setup(int timer_clock, int period);
void transmit_timer_setup(int ksps);
