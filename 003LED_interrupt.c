//controlling state of led using interrupts with external button

/*
 * 003LED_button.c
 *
 *  Created on: 06-Feb-2026
 *      Author: sumit
 */

//make led high connected to port b pin 5 falling edge using button on PB4
#include "stm32f103xx_gpio_driver.h"

void delay(void){
	for(volatile uint32_t i=0; i<500000; i++){
		//let it go
	}
}

int main(void) {

	GPIO_HANDLER button_interrupt;
	button_interrupt.pGPIOx = GPIOB;
	button_interrupt.GPIO_config.GPIO_pinnum = PIN_NUM_4;
	button_interrupt.GPIO_config.GPIO_pinmode = GPIO_INTERRUPT_FT;
	button_interrupt.GPIO_config.GPIO_CNFmode = GPIO_IN_UPORDOWN;
	button_interrupt.GPIO_config.GPIO_updown = GPIO_PULLUP; //beacuse i want transtion on falling edge

	GPIO_HANDLER led_interrupt;
	led_interrupt.pGPIOx = GPIOB;
	led_interrupt.GPIO_config.GPIO_pinnum = PIN_NUM_5;
	led_interrupt.GPIO_config.GPIO_pinmode = GPIO_MODE_OUT_SPEED_2;
	led_interrupt.GPIO_config.GPIO_CNFmode = GPIO_OUT_PUSHPULL;
	led_interrupt.GPIO_config.GPIO_updown = GPIO_NOPUPD;

	GPIO_PeriClkControl(GPIOB, ENABLE); //enable gpiob clock
	RCC->RCC_APB2ENR |= (1 << 0); //enables the AFIO EN bit because for interrupts we will be accessing AFIO registers

	GPIO_init(&button_interrupt);
	GPIO_init(&led_interrupt);

	GPIO_IRQconfig(IRQ_NO_EXTI4, ENABLE);
	GPIO_IRQpriority(IRQ_NO_EXTI4, NVIC_PRIORITY_5);

	while (1) {
		//let cpu wait for interrupt
		GPIO_WritetoOutputPin(GPIOB, PIN_NUM_5, RESET); //keep led low rest of time
		delay(); //fake delay
	}
	return 0;
}

void EXTI4_IRQHandler() {
		GPIO_WritetoOutputPin(GPIOB, PIN_NUM_5, SET);
		GPIO_IRQhandling(PIN_NUM_4);
	}
