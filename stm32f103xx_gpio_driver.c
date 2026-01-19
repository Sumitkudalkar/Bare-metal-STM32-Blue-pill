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
	}

}
void GPIO_deinit(GPIO_REGDEF *pGPIOx) {
	// we want to pass the base addr of the port first then set the reset register of rcc
	if(pGPIOx == GPIOA){
		GPIOA_REG_RESET();
	}
	else if(pGPIOx == GPIOB){
		GPIOB_REG_RESET();
	}
	else if(pGPIOx == GPIOC){
		GPIOC_REG_RESET();
	}

}

//either 1 or 0 to the pin so minimum datatype is 1byte
//PA0 to PA15 means total 16 pin in 1 port hence 16 bit integer

uint8_t GPIO_ReadfromInputPin(GPIO_REGDEF *pGPIOx, uint8_t Pin_number) {
	uint8_t value = ((pGPIOx->IDR)>>Pin_number) & 1; //shift right pin number times and then and with 1 to know whether its 0 or 1
	return value;
}
uint16_t GPIO_ReadfromInputPort(GPIO_REGDEF *pGPIOx) {
	return (pGPIOx->IDR);
}

void GPIO_WritetoOutputPin(GPIO_REGDEF *pGPIOx, uint8_t Pin_number,
		uint8_t value) {
	if(value == 1){
		pGPIOx->ODR |= (1<<Pin_number);
	}
	else{
		pGPIOx->ODR &= ~(1<<Pin_number);
	}
}
void GPIO_WritetoOutputPort(GPIO_REGDEF *pGPIOx, uint16_t val){
	pGPIOx->ODR = val;
}
void GPIO_ToggleOutputPin(GPIO_REGDEF *pGPIOx, uint8_t Pin_number) {
	pGPIOx->ODR ^= (1<<Pin_number); // this will toggle the state of the pin
}

void GPIO_IRQconfig(uint8_t IRQ_number, uint8_t priority, uint8_t EnorDi) {

}
void GPIO_IRQhandling(uint8_t Pin_number) {

}

