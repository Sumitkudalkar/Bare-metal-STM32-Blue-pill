//uses polling to control the led state with a external button
/*
 * 002Led_button.c
 *
 *  Created on: 20-Jan-2026
 *      Author: sumit
 */
#include "stm32f103xx_gpio_driver.h"

void delay(void){
	for(volatile uint32_t i=0; i<500000; i++){
		//let it go
	}
}


int main(void){

	GPIO_HANDLER led;
	led.pGPIOx = GPIOA;
	led.GPIO_config.GPIO_pinnum = PIN_NUM_0;
	led.GPIO_config.GPIO_pinmode = GPIO_MODE_OUT_SPEED_2;
	led.GPIO_config.GPIO_CNFmode = GPIO_OUT_PUSHPULL;
	led.GPIO_config.GPIO_updown = GPIO_NOPUPD;


	GPIO_HANDLER button;
	button.pGPIOx = GPIOA;
	button.GPIO_config.GPIO_pinnum = PIN_NUM_2;
	button.GPIO_config.GPIO_pinmode = GPIO_MODE_IN;
	button.GPIO_config.GPIO_CNFmode = GPIO_IN_UPORDOWN;
	button.GPIO_config.GPIO_updown = GPIO_PULLUP;

	GPIO_PeriClkControl(GPIOA, ENABLE);
	GPIO_init(&led);
	GPIO_init(&button);


	while(1){
		if(GPIO_ReadfromInputPin(GPIOA, PIN_NUM_2) == 0){
			//means that the button is now pressed
			GPIO_WritetoOutputPin(GPIOA, PIN_NUM_0, SET);
			delay();
		}
		else{
			GPIO_WritetoOutputPin(GPIOA, PIN_NUM_0, RESET);
		}
	}


	return 0;
}

