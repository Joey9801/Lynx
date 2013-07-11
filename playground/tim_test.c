#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/timer.h>
#include <libopencm3/cm3/nvic.h>

void timer_setup(void);
void nvic_setup(void);
void usart_setup(void);
static void gpio_setup(void);

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


static void clock_setup(void){
	/*Operate at 168MHz*/
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
}

static void gpio_setup(void){
	//Enable port D gpio clock
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
	//set led pins to push/pull output
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
	gpio_set(GPIOD, GPIO12|GPIO14);
}

void nvic_setup(void){
	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 1);
}

void timer_setup(){
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);
	timer_reset(TIM2);
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	//timer_enable_update_event(TIM2);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
	timer_set_period(TIM2, 25000);
	timer_set_prescaler(TIM2, 1000);
	timer_enable_preload(TIM2);
	timer_enable_counter(TIM2);
	//TIM_CNT(TIM2) = 1; //Start counting at 1
	//TIM_PSC(TIM2) = 1500; //Arbitrary prescalar so we can see the blinking
	//TIM_ARR(TIM2) = 10000; //Timer end value
	//TIM_DIER(TIM2) |= TIM_DIER_UIE; //Enable interrupt on update
	//TIM_CR1(TIM2) |= (1<<4); //Downcounting mode
	//TIM_CR1(TIM2) |= TIM_CR1_CEN; //start the timer

}

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

void next_led(void){
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
	gpio_set(GPIOD, leds); //set the led's to be set
	gpio_clear(GPIOD, ~(leds|(~(GPIO12|GPIO13|GPIO14|GPIO15))));
	//unset the led's to be unset
}

int main(void)
{
	clock_setup();
	gpio_setup();
	nvic_setup();
	timer_setup();
	while(1){
		//while(!(TIM2_SR&TIM_SR_UIF));
		//tim2_isr();		
	}
}
