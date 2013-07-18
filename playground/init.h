#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/spi.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/cm3/nvic.h>

static void clock_setup(void);
static void gpio_setup(void);
static void usart_setup(void);
static void spi_setup(void);
void accel_setup(void);
