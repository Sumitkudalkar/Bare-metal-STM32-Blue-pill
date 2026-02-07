/*
 * stm32f103xx_gpio_driver.c
 *
 *  Created on: 10-Jan-2026
 *      Author: sumit
 */
#include "stm32f103xx_gpio_driver.h"

void GPIO_PeriClkControl(GPIO_REGDEF *pGPIOx, uint8_t EnOrDi) {
	if (EnOrDi == ENABLE) {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		}
	} else if (EnOrDi == DISABLE) {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_DI();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_DI();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_DI();
		}
	}
}

void GPIO_init(GPIO_HANDLER *pGPIOhandle) {
	// we want to configure which port are we using + what mode and all
	uint32_t pin = pGPIOhandle->GPIO_config.GPIO_pinnum;
	uint32_t mode = pGPIOhandle->GPIO_config.GPIO_pinmode;

	if (mode <= GPIO_MODE_OUT_SPEED_50) {
		//not a interrupt
		if (pin < 8) { //crl register

			uint32_t shift = pin * 4; //first bit of mode
			uint32_t nextbit = shift + 1; //second bit of mode
			uint32_t cnfbit0 = nextbit + 1; //first bit of cnf
			uint32_t cnfbit1 = nextbit + 2; //second bit of cnf
			if (mode == GPIO_MODE_IN) {
				//right shift the crl register pin times and clear it 00 for input
				pGPIOhandle->pGPIOx->CRL &= ~((1 << shift) | (1 << nextbit)); //clears bit pin and pin+1
				//setting the cnf bits
				if (pGPIOhandle->GPIO_config.GPIO_CNFmode == GPIO_IN_ANALOG) {
					//reset both bits
					pGPIOhandle->pGPIOx->CRL &= ~((1 << cnfbit0)
							| (1 << cnfbit1));
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_IN_FLOATING) {
					pGPIOhandle->pGPIOx->CRL |= (1 << cnfbit0);
					pGPIOhandle->pGPIOx->CRL &= ~(1 << cnfbit1); //set bit0 and reset bit1
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_IN_UPORDOWN) {
					//set bit1 and reset bit0
					pGPIOhandle->pGPIOx->CRL |= (1 << cnfbit1);
					pGPIOhandle->pGPIOx->CRL &= ~(1 << cnfbit0);

					//whether up or down??
					if (pGPIOhandle->GPIO_config.GPIO_updown == GPIO_PULLUP) {
						//set the odr bit high
						pGPIOhandle->pGPIOx->ODR |= (1 << pin);
					} else {
						pGPIOhandle->pGPIOx->ODR &= ~(1 << pin);
					}
				} else {
					//reserved means all bit are high
					pGPIOhandle->pGPIOx->CRL |=
							((1 << cnfbit0) | (1 << cnfbit1));
				}
			} else if (mode == GPIO_MODE_OUT_SPEED_10) {
				//set the first bit and reset the next bit
				pGPIOhandle->pGPIOx->CRL |= (1 << shift);
				pGPIOhandle->pGPIOx->CRL &= ~(1 << nextbit);

				//setting up cnf bits
				if (pGPIOhandle->GPIO_config.GPIO_CNFmode == GPIO_OUT_PUSHPULL) {
					//reset both the bits
					pGPIOhandle->pGPIOx->CRL &= ~((1 << cnfbit0)
							| (1 << cnfbit1));
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_OPENDRAIN) {
					//reset bit1 and set bit0
					pGPIOhandle->pGPIOx->CRL &= ~(1 << cnfbit1);
					pGPIOhandle->pGPIOx->CRL |= (1 << cnfbit0);
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_AFPUSHPULL) {
					//set bit1 and reset bit 0
					pGPIOhandle->pGPIOx->CRL &= ~(1 << cnfbit0);
					pGPIOhandle->pGPIOx->CRL |= (1 << cnfbit1);

				} else {
					pGPIOhandle->pGPIOx->CRL |=
							((1 << cnfbit0) | (1 << cnfbit1));
				}

			} else if (mode == GPIO_MODE_OUT_SPEED_2) {
				//reset the first bit and set the next bit
				pGPIOhandle->pGPIOx->CRL &= ~(1 << shift);
				pGPIOhandle->pGPIOx->CRL |= (1 << nextbit);

				if (pGPIOhandle->GPIO_config.GPIO_CNFmode == GPIO_OUT_PUSHPULL) {
					//reset both the bits
					pGPIOhandle->pGPIOx->CRL &= ~((1 << cnfbit0)
							| (1 << cnfbit1));
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_OPENDRAIN) {
					//reset bit1 and set bit0
					pGPIOhandle->pGPIOx->CRL &= ~(1 << cnfbit1);
					pGPIOhandle->pGPIOx->CRL |= (1 << cnfbit0);
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_AFPUSHPULL) {
					//set bit1 and reset bit 0
					pGPIOhandle->pGPIOx->CRL &= ~(1 << cnfbit0);
					pGPIOhandle->pGPIOx->CRL |= (1 << cnfbit1);

				} else {
					pGPIOhandle->pGPIOx->CRL |=
							((1 << cnfbit0) | (1 << cnfbit1));
				}
			} else if (mode == GPIO_MODE_OUT_SPEED_50) {
				//set both the bits
				pGPIOhandle->pGPIOx->CRL |= ((1 << shift) | (1 << nextbit));

				if (pGPIOhandle->GPIO_config.GPIO_CNFmode == GPIO_OUT_PUSHPULL) {
					//reset both the bits
					pGPIOhandle->pGPIOx->CRL &= ~((1 << cnfbit0)
							| (1 << cnfbit1));
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_OPENDRAIN) {
					//reset bit1 and set bit0
					pGPIOhandle->pGPIOx->CRL &= ~(1 << cnfbit1);
					pGPIOhandle->pGPIOx->CRL |= (1 << cnfbit0);
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_AFPUSHPULL) {
					//set bit1 and reset bit 0
					pGPIOhandle->pGPIOx->CRL &= ~(1 << cnfbit0);
					pGPIOhandle->pGPIOx->CRL |= (1 << cnfbit1);

				} else {
					pGPIOhandle->pGPIOx->CRL |=
							((1 << cnfbit0) | (1 << cnfbit1));
				}
			}

		} else { //crh register
			uint32_t shift_bits = (pin % 8) * 4; // here pins are from 8 to 15 so scale it so that 8*4 =32 not possible
			uint32_t nextbits = shift_bits + 1;
			uint32_t cnf0 = nextbits + 1;
			uint32_t cnf1 = nextbits + 2;
			if (mode == GPIO_MODE_IN) {
				//right shift the crl register pin times and clear it 00 for input
				pGPIOhandle->pGPIOx->CRH &= ~((1 << shift_bits)
						| (1 << nextbits)); //clears bit pin and pin+1
				if (pGPIOhandle->GPIO_config.GPIO_CNFmode == GPIO_IN_ANALOG) {
					//reset both bits
					pGPIOhandle->pGPIOx->CRH &= ~((1 << cnf0) | (1 << cnf1));
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_IN_FLOATING) {
					pGPIOhandle->pGPIOx->CRH |= (1 << cnf0);
					pGPIOhandle->pGPIOx->CRH &= ~(1 << cnf1); //set bit0 and reset bit1
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_IN_UPORDOWN) {
					//set bit1 and reset bit0
					pGPIOhandle->pGPIOx->CRH |= (1 << cnf1);
					pGPIOhandle->pGPIOx->CRH &= ~(1 << cnf0);

					if (pGPIOhandle->GPIO_config.GPIO_updown == GPIO_PULLUP) {
						//set the odr bit high
						pGPIOhandle->pGPIOx->ODR |= (1 << pin);
					} else {
						pGPIOhandle->pGPIOx->ODR &= ~(1 << pin);
					}
				} else {
					//reserved means all bit are high
					pGPIOhandle->pGPIOx->CRH |= ((1 << cnf0) | (1 << cnf1));
				}
			} else if (mode == GPIO_MODE_OUT_SPEED_10) {
				//set the first bit and reset the next bit
				pGPIOhandle->pGPIOx->CRH |= (1 << shift_bits);
				pGPIOhandle->pGPIOx->CRH &= ~(1 << nextbits);

				if (pGPIOhandle->GPIO_config.GPIO_CNFmode == GPIO_OUT_PUSHPULL) {
					//reset both the bits
					pGPIOhandle->pGPIOx->CRH &= ~((1 << cnf0) | (1 << cnf1));
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_OPENDRAIN) {
					//reset bit1 and set bit0
					pGPIOhandle->pGPIOx->CRH &= ~(1 << cnf1);
					pGPIOhandle->pGPIOx->CRH |= (1 << cnf0);
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_AFPUSHPULL) {
					//set bit1 and reset bit 0
					pGPIOhandle->pGPIOx->CRH &= ~(1 << cnf0);
					pGPIOhandle->pGPIOx->CRH |= (1 << cnf1);

				} else {
					pGPIOhandle->pGPIOx->CRH |= ((1 << cnf0) | (1 << cnf1));
				}
			} else if (mode == GPIO_MODE_OUT_SPEED_2) {
				//reset the first bit and set the next bit
				pGPIOhandle->pGPIOx->CRH &= ~(1 << shift_bits);
				pGPIOhandle->pGPIOx->CRH |= (1 << nextbits);

				if (pGPIOhandle->GPIO_config.GPIO_CNFmode == GPIO_OUT_PUSHPULL) {
					//reset both the bits
					pGPIOhandle->pGPIOx->CRH &= ~((1 << cnf0) | (1 << cnf1));
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_OPENDRAIN) {
					//reset bit1 and set bit0
					pGPIOhandle->pGPIOx->CRH &= ~(1 << cnf1);
					pGPIOhandle->pGPIOx->CRH |= (1 << cnf0);
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_AFPUSHPULL) {
					//set bit1 and reset bit 0
					pGPIOhandle->pGPIOx->CRH &= ~(1 << cnf0);
					pGPIOhandle->pGPIOx->CRH |= (1 << cnf1);

				} else {
					pGPIOhandle->pGPIOx->CRH |= ((1 << cnf0) | (1 << cnf1));
				}
			} else if (mode == GPIO_MODE_OUT_SPEED_50) {
				//set both the bits
				pGPIOhandle->pGPIOx->CRH |=
						((1 << shift_bits) | (1 << nextbits));

				if (pGPIOhandle->GPIO_config.GPIO_CNFmode == GPIO_OUT_PUSHPULL) {
					//reset both the bits
					pGPIOhandle->pGPIOx->CRH &= ~((1 << cnf0) | (1 << cnf1));
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_OPENDRAIN) {
					//reset bit1 and set bit0
					pGPIOhandle->pGPIOx->CRH &= ~(1 << cnf1);
					pGPIOhandle->pGPIOx->CRH |= (1 << cnf0);
				} else if (pGPIOhandle->GPIO_config.GPIO_CNFmode
						== GPIO_OUT_AFPUSHPULL) {
					//set bit1 and reset bit 0
					pGPIOhandle->pGPIOx->CRH &= ~(1 << cnf0);
					pGPIOhandle->pGPIOx->CRH |= (1 << cnf1);

				} else {
					pGPIOhandle->pGPIOx->CRH |= ((1 << cnf0) | (1 << cnf1));
				}
			}

		}
	} else {
		//interrupt code goes here
		uint32_t bit0 = (pin % 4) * 4;
		uint32_t bit1 = bit0 + 1;
		uint32_t bit2 = bit1 + 1;
		uint32_t bit3 = bit2 + 1;

		//first clear all bits as per pin numbers
		if (pin <= 3) { //names of the registers changes rest everything is same as it is
			AFIO->AFIO_EXTICR1 &= ~((1 << bit0) | (1 << bit1) | (1 << bit2)
					| (1 << bit3));

		} else if (pin >= 4 && pin <= 7) {
			AFIO->AFIO_EXTICR2 &= ~((1 << bit0) | (1 << bit1) | (1 << bit2)
					| (1 << bit3));
		} else if (pin >= 8 && pin <= 11) {
			AFIO->AFIO_EXTICR3 &= ~((1 << bit0) | (1 << bit1) | (1 << bit2)
					| (1 << bit3));
		} else {
			AFIO->AFIO_EXTICR4 &= ~((1 << bit0) | (1 << bit1) | (1 << bit2)
					| (1 << bit3));
		}
		if (mode == GPIO_INTERRUPT_RT) {

			//once everything is cleared now see which port to make as interrupt

			if (pGPIOhandle->pGPIOx == GPIOA) {
				//nothing to do since port A needs all 4bits to be zero

				EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
				EXTI->EXTI_RTSR |= (1 << pin); //enables rising trigger for that EXTI line

			} else if (pGPIOhandle->pGPIOx == GPIOB) {
				if (pin <= 3) { //names of the registers changes rest everything is same as it is
					AFIO->AFIO_EXTICR1 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);

				} else if (pin >= 4 && pin <= 7) {
					AFIO->AFIO_EXTICR2 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);

				} else if (pin >= 8 && pin <= 11) {
					AFIO->AFIO_EXTICR3 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);

				} else {
					AFIO->AFIO_EXTICR4 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);

				}
			}

			else if (pGPIOhandle->pGPIOx == GPIOC) {
				if (pin <= 3) { //names of the registers changes rest everything is same as it is
					AFIO->AFIO_EXTICR1 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);

				} else if (pin >= 4 && pin <= 7) {
					AFIO->AFIO_EXTICR2 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);

				} else if (pin >= 8 && pin <= 11) {
					AFIO->AFIO_EXTICR3 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);

				} else {
					AFIO->AFIO_EXTICR4 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);

				}
			}

		} else if (mode == GPIO_INTERRUPT_FT) {
			if (pGPIOhandle->pGPIOx == GPIOA) {
				//nothing to do since port A needs all 4bits to be zero

				EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
				EXTI->EXTI_FTSR |= (1 << pin); //enables rising trigger for that EXTI line

			} else if (pGPIOhandle->pGPIOx == GPIOB) {
				if (pin <= 3) { //names of the registers changes rest everything is same as it is
					AFIO->AFIO_EXTICR1 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_FTSR |= (1 << pin);

				} else if (pin >= 4 && pin <= 7) {
					AFIO->AFIO_EXTICR2 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_FTSR |= (1 << pin);

				} else if (pin >= 8 && pin <= 11) {
					AFIO->AFIO_EXTICR3 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_FTSR |= (1 << pin);

				} else {
					AFIO->AFIO_EXTICR4 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_FTSR |= (1 << pin);

				}
			}

			else if (pGPIOhandle->pGPIOx == GPIOC) {
				if (pin <= 3) { //names of the registers changes rest everything is same as it is
					AFIO->AFIO_EXTICR1 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_FTSR |= (1 << pin);

				} else if (pin >= 4 && pin <= 7) {
					AFIO->AFIO_EXTICR2 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_FTSR |= (1 << pin);

				} else if (pin >= 8 && pin <= 11) {
					AFIO->AFIO_EXTICR3 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_FTSR |= (1 << pin);

				} else {
					AFIO->AFIO_EXTICR4 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_FTSR |= (1 << pin);

				}
			}

		} else if (mode == GPIO_INTERRUPT_RFT) {
			if (pGPIOhandle->pGPIOx == GPIOA) {
				//nothing to do since port A needs all 4bits to be zero

				EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
				EXTI->EXTI_RTSR |= (1 << pin); //enables rising trigger for that EXTI line
				EXTI->EXTI_FTSR |= (1 << pin);

			} else if (pGPIOhandle->pGPIOx == GPIOB) {
				if (pin <= 3) { //names of the registers changes rest everything is same as it is
					AFIO->AFIO_EXTICR1 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);
					EXTI->EXTI_FTSR |= (1 << pin);

				} else if (pin >= 4 && pin <= 7) {
					AFIO->AFIO_EXTICR2 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);
					EXTI->EXTI_FTSR |= (1 << pin);

				} else if (pin >= 8 && pin <= 11) {
					AFIO->AFIO_EXTICR3 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);
					EXTI->EXTI_FTSR |= (1 << pin);

				} else {
					AFIO->AFIO_EXTICR4 |= (1 << bit0);
					//0001 for port B
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);
					EXTI->EXTI_FTSR |= (1 << pin);

				}
			}

			else if (pGPIOhandle->pGPIOx == GPIOC) {
				if (pin <= 3) { //names of the registers changes rest everything is same as it is
					AFIO->AFIO_EXTICR1 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);
					EXTI->EXTI_FTSR |= (1 << pin);

				} else if (pin >= 4 && pin <= 7) {
					AFIO->AFIO_EXTICR2 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);
					EXTI->EXTI_FTSR |= (1 << pin);

				} else if (pin >= 8 && pin <= 11) {
					AFIO->AFIO_EXTICR3 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);
					EXTI->EXTI_FTSR |= (1 << pin);

				} else {
					AFIO->AFIO_EXTICR4 |= (1 << bit1);
					//0010 for port C
					EXTI->EXTI_IMR |= (1 << pin); //sets the bit of mask register to enable that specific EXTI line
					EXTI->EXTI_RTSR |= (1 << pin);
					EXTI->EXTI_FTSR |= (1 << pin);

				}
			}
		}

	}

}
void GPIO_deinit(GPIO_REGDEF *pGPIOx) {
	// we want to pass the base addr of the port first then set the reset register of rcc
	if (pGPIOx == GPIOA) {
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB) {
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC) {
		GPIOC_REG_RESET();
	}

}

//either 1 or 0 to the pin so minimum datatype is 1byte
//PA0 to PA15 means total 16 pin in 1 port hence 16 bit integer

uint8_t GPIO_ReadfromInputPin(GPIO_REGDEF *pGPIOx, uint8_t Pin_number) {
	uint8_t value = ((pGPIOx->IDR) >> Pin_number) & 1; //shift right pin number times and then and with 1 to know whether its 0 or 1
	return value;
}
uint16_t GPIO_ReadfromInputPort(GPIO_REGDEF *pGPIOx) {
	return (pGPIOx->IDR);
}

void GPIO_WritetoOutputPin(GPIO_REGDEF *pGPIOx, uint8_t Pin_number,
		uint8_t value) {
	if (value == 1) {
		pGPIOx->ODR |= (1 << Pin_number);
	} else {
		pGPIOx->ODR &= ~(1 << Pin_number);
	}
}
void GPIO_WritetoOutputPort(GPIO_REGDEF *pGPIOx, uint16_t val) {
	pGPIOx->ODR = val;
}
void GPIO_ToggleOutputPin(GPIO_REGDEF *pGPIOx, uint8_t Pin_number) {
	pGPIOx->ODR ^= (1 << Pin_number); // this will toggle the state of the pin
}

void GPIO_IRQconfig(uint8_t IRQ_number, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (IRQ_number <= 31) {
			//NVIC_ISER0 register
			*NVIC_ISER0 |= (1 << IRQ_number);
		} else if (IRQ_number > 31 && IRQ_number <= 63) {
			//NVIC_ISER1 register
			IRQ_number = IRQ_number % 32;
			*NVIC_ISER1 |= (1 << IRQ_number);
		}
		//last IRQ number is 59 so no need to go to NVIC_ISER2

	} else {
		//assuming it is to disable the interrupt
		if (IRQ_number <= 31) {
			//NVIC_ICER0 register
			*NVIC_ICER0 |= (1 << IRQ_number);
		} else if (IRQ_number > 31 && IRQ_number <= 63) {
			//NVIC_ICER1 register
			IRQ_number = IRQ_number % 32;
			*NVIC_ICER1 |= (1 << IRQ_number);
		}
		//last IRQ number is 59 so no need to go to NVIC_ICER2
	}
}

void GPIO_IRQpriority(uint8_t IRQ_number, uint8_t priority) {
	uint8_t IPRx = IRQ_number / 4;
	uint8_t PRIx = IRQ_number % 4;
//need to clear the bits before giving it priority
	*(NVIC_IPR_BASEADDR + IPRx) &= ~((0xFF << PRIx * 8)); //0xff will clear that specific 8 bit space and after that we are alloting priority to that space

	*(NVIC_IPR_BASEADDR + IPRx) |= ((priority << PRIx * 8) << 4); //correct addr of IPR reg and then since they consider only 4 MSB we need to shift the lsb to msb side

}

void GPIO_IRQhandling(uint8_t Pin_number) {
	//disable the pending bit of exti_pr by writing 1 to it
	if((EXTI->EXTI_PR & (1 << Pin_number)) > 0){
		EXTI->EXTI_PR = (1 << Pin_number); //clearing the pending bit by setting it ,here cant perform bitwise or beacause other pending pins will also get reset due to this hence we use = operator
	}
}

