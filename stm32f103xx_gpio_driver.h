/*
 * stm32f103xx_gpio_driver.h
 *
 *  Created on: 10-Jan-2026
 *      Author: sumit
 */

#ifndef INC_STM32F103XX_GPIO_DRIVER_H_
#define INC_STM32F103XX_GPIO_DRIVER_H_

#include "stm32f103xx.h"

//GPIO based macros
#define GPIO_IN_ANALOG             0
#define GPIO_IN_FLOATING           1
#define GPIO_IN_UPORDOWN           2
#define GPIO_IN_RESERVED           3

#define GPIO_OUT_PUSHPULL          0
#define GPIO_OUT_OPENDRAIN         1
#define GPIO_OUT_AFPUSHPULL        2
#define GPIO_OUT_AFOPENDRAIN       3

#define GPIO_MODE_IN               0  //modes
#define GPIO_MODE_OUT_SPEED_10     1
#define GPIO_MODE_OUT_SPEED_2      2
#define GPIO_MODE_OUT_SPEED_50     3
//if mode is more than 3 then gpio is acting as interrupt else normally
#define GPIO_INTERRUPT_RT          4
#define GPIO_INTERRUPT_FT          5
#define GPIO_INTERRUPT_RFT         6 //till here modes

#define GPIO_PULLUP                0
#define GPIO_PULLDOWN              1
#define GPIO_NOPUPD                2

#define PIN_NUM_0                  0
#define PIN_NUM_1                  1
#define PIN_NUM_2                  2
#define PIN_NUM_3                  3
#define PIN_NUM_4                  4
#define PIN_NUM_5                  5
#define PIN_NUM_6                  6
#define PIN_NUM_7                  7
#define PIN_NUM_8                  8
#define PIN_NUM_9                  9
#define PIN_NUM_10                 10
#define PIN_NUM_11                 11
#define PIN_NUM_12                 12
#define PIN_NUM_13                 13
#define PIN_NUM_14                 14
#define PIN_NUM_15                 15


#define NVIC_PRIORITY_0            0
#define NVIC_PRIORITY_1            1
#define NVIC_PRIORITY_2            2
#define NVIC_PRIORITY_3            3
#define NVIC_PRIORITY_4            4
#define NVIC_PRIORITY_5            5
#define NVIC_PRIORITY_6            6
#define NVIC_PRIORITY_7            7
#define NVIC_PRIORITY_8            8
#define NVIC_PRIORITY_9            9
#define NVIC_PRIORITY_10           10
#define NVIC_PRIORITY_11           11
#define NVIC_PRIORITY_12           12
#define NVIC_PRIORITY_13           13
#define NVIC_PRIORITY_14           14
#define NVIC_PRIORITY_15           15

typedef struct{
	uint32_t GPIO_pinnum;
	uint32_t GPIO_pinmode;
	uint32_t GPIO_CNFmode;
	uint32_t GPIO_updown;

}GPIO_PIN_CONFIG;


typedef struct{
	GPIO_REGDEF *pGPIOx;
	GPIO_PIN_CONFIG GPIO_config;
}GPIO_HANDLER;


void GPIO_PeriClkControl(GPIO_REGDEF *pGPIOx, uint8_t EnOrDi);


void GPIO_init(GPIO_HANDLER *pGPIOhandle); // we want to configure which port are we using + what mode and all
void GPIO_deinit(GPIO_REGDEF *pGPIOx); // we want to pass the base addr of the port first then set the reset register of rcc



//either 1 or 0 to the pin so minimum datatype is 1byte
//PA0 to PA15 means total 16 pin in 1 port hence 16 bit integer

uint8_t GPIO_ReadfromInputPin(GPIO_REGDEF *pGPIOx, uint8_t Pin_number);
uint16_t GPIO_ReadfromInputPort(GPIO_REGDEF *pGPIOx);

void GPIO_WritetoOutputPin(GPIO_REGDEF *pGPIOx, uint8_t Pin_number, uint8_t value);
void GPIO_WritetoOutputPort(GPIO_REGDEF *pGPIOx, uint16_t val);
void GPIO_ToggleOutputPin(GPIO_REGDEF *pGPIOx, uint8_t Pin_number);

void GPIO_IRQconfig(uint8_t IRQ_number, uint8_t EnorDi);
void GPIO_IRQpriority(uint8_t IRQ_number, uint8_t priority);
void GPIO_IRQhandling(uint8_t Pin_number);

#endif /* INC_STM32F103XX_GPIO_DRIVER_H_ */
