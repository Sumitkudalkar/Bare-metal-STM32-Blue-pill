//hello world of embedded which toggles the LED on and off 


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

	GPIO_PeriClkControl(GPIOA, ENABLE);
	GPIO_init(&led);

	while(1){
		GPIO_ToggleOutputPin(GPIOA, PIN_NUM_0);
		delay();
	}

	return 0;
}
