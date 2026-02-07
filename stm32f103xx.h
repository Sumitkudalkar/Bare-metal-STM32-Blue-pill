/*
 * stm32f103xx.h
 *
 *  Created on: 08-Jan-2026
 *      Author: sumit
 */

#ifndef INC_STM32F103XX_H_
#define INC_STM32F103XX_H_
#include<stdint.h>

#define FLASH_REG_BASEADDR      0x40022000U
#define SRAM_BASEADDR           0x20000000U
#define ROM_BASEADDR            0x1FFFF000U
#define RCC_BASEADDR            0x40021000U

#define AHB_BASEADDR            0x40018000U
#define APB2_BASEADDR           0x40010000U
#define APB1_BASEADDR           0x40000000U

#define PORTA_BASEADDR          0x40010800U
#define PORTB_BASEADDR          0x40010C00U
#define PORTC_BASEADDR          0x40011000U
#define PORTD_BASEADDR          0x40011400U
#define PORTE_BASEADDR          0x40011800U
#define PORTF_BASEADDR          0x40011C00U
#define PORTG_BASEADDR          0x40012000U


#define I2C1_BASEADDR           0x40005400U
#define I2C2_BASEADDR           0x40005800U
#define SPI1_BASEADDR           0x40013000U
#define SPI2_BASEADDR           0x40003800U
#define USART1_BASEADDR         0x40013800U
#define USART2_BASEADDR         0x40004400U
#define USART3_BASEADDR         0x40004800U
#define EXTI_BASEADDR           0x40010400U
#define AFIO_BASEADDR           APB2_BASEADDR


#define NVIC_ISER0              (volatile uint32_t *)0xE000E100
#define NVIC_ISER1              (volatile uint32_t *)0xE000E104

#define NVIC_ICER0              (volatile uint32_t *)0XE000E180
#define NVIC_ICER1              (volatile uint32_t *)0XE000E184


#define NVIC_IPR_BASEADDR       (volatile uint32_t *)0xE000E400




typedef struct{
	volatile uint32_t CRL;  //offset values start from 0x00 and increments by 4
	volatile uint32_t CRH;      //0x04
	volatile uint32_t IDR;      //0x08
	volatile uint32_t ODR;       //so on
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR ;
}GPIO_REGDEF;   //structure for GPIO

//using the stucture for different ports

#define GPIOA               ((GPIO_REGDEF*)PORTA_BASEADDR)
#define GPIOB               ((GPIO_REGDEF*)PORTB_BASEADDR)
#define GPIOC               ((GPIO_REGDEF*)PORTC_BASEADDR)
#define GPIOD               ((GPIO_REGDEF*)PORTD_BASEADDR)
#define GPIOE               ((GPIO_REGDEF*)PORTE_BASEADDR)
#define GPIOF               ((GPIO_REGDEF*)PORTF_BASEADDR)
#define GPIOG               ((GPIO_REGDEF*)PORTG_BASEADDR)


//clock configuration for rcc registers

typedef struct{
	volatile uint32_t RCC_CR;
	volatile uint32_t RCC_CFGR;
	volatile uint32_t RCC_CIR;
	volatile uint32_t RCC_APB2RSTR;
	volatile uint32_t RCC_APB1RSTR;
	volatile uint32_t RCC_AHBENR;
	volatile uint32_t RCC_APB2ENR;
	volatile uint32_t RCC_APB1ENR;
	volatile uint32_t RCC_BDCR;
	volatile uint32_t RCC_CSR;
}RCC_REG;


typedef struct{
	uint32_t EXTI_IMR;
	uint32_t EXTI_EMR;
	uint32_t EXTI_RTSR;
	uint32_t EXTI_FTSR;
	uint32_t EXTI_SWIER;
	uint32_t EXTI_PR;
}EXTI_REG;

typedef struct{
	uint32_t AFIO_EVCR;
	uint32_t AFIO_MAPR;
	uint32_t AFIO_EXTICR1;
	uint32_t AFIO_EXTICR2;
	uint32_t AFIO_EXTICR3;
	uint32_t AFIO_EXTICR4;
	uint32_t AFIO_MAPR2;
}AFIO_REG;


#define RCC                     ((RCC_REG*)RCC_BASEADDR)
#define EXTI                    ((EXTI_REG*)EXTI_BASEADDR)
#define AFIO                    ((AFIO_REG*)AFIO_BASEADDR)


#define IRQ_NO_EXTI0                6
#define IRQ_NO_EXTI1				7
#define IRQ_NO_EXTI2 				8
#define IRQ_NO_EXTI3 				9
#define IRQ_NO_EXTI4 				10
#define IRQ_NO_EXTI9_5				23
#define IRQ_NO_EXTI10_15			40



#define GPIOA_PCLK_EN()       (RCC->RCC_APB2ENR |= (1<<2)) //enables the clk for GPIOA peripheral
#define GPIOB_PCLK_EN()       (RCC->RCC_APB2ENR |= (1<<3))
#define GPIOC_PCLK_EN()       (RCC->RCC_APB2ENR |= (1<<4))
#define GPIOD_PCLK_EN()       (RCC->RCC_APB2ENR |= (1<<5))
#define GPIOE_PCLK_EN()       (RCC->RCC_APB2ENR |= (1<<6))

//for i2c peripherals


#define I2C1_PCLK_EN()         (RCC->RCC_APB1ENR |= (1<<21))
#define I2C2_PCLK_EN()         (RCC->RCC_APB1ENR |= (1<<22))

//spi peripherals

#define SPI2_PCLK_EN()         (RCC->RCC_APB1ENR |= (1<<14))
#define SPI1_PCLK_EN()         (RCC->RCC_APB2ENR |= (1<<12))

//usart peripherals
#define USART2_PCLK_EN()         (RCC->RCC_APB1ENR |= (1<<17))
#define USART3_PCLK_EN()         (RCC->RCC_APB1ENR |= (1<<18))
#define USART1_PCLK_EN()         (RCC->RCC_APB2ENR |= (1<<14))

#define AFIO_PCLK_EN()           (RCC->RCC_APB2ENR |= (1<<0))

//disabling them now

#define GPIOA_PCLK_DI()       (RCC->RCC_APB2ENR &= ~(1<<2)) //disables the clk for GPIOA peripheral
#define GPIOB_PCLK_DI()       (RCC->RCC_APB2ENR &= ~(1<<3))
#define GPIOC_PCLK_DI()       (RCC->RCC_APB2ENR &= ~(1<<4))
#define GPIOD_PCLK_DI()       (RCC->RCC_APB2ENR &= ~(1<<5))
#define GPIOE_PCLK_DI()       (RCC->RCC_APB2ENR &= ~(1<<6))

#define I2C1_PCLK_DI()         (RCC->RCC_APB1ENR &= ~(1<<21))
#define I2C2_PCLK_DI()         (RCC->RCC_APB1ENR &= ~(1<<22))

#define SPI2_PCLK_DI()         (RCC->RCC_APB1ENR &= ~(1<<14))
#define SPI1_PCLK_DI()         (RCC->RCC_APB2ENR &= ~(1<<12))

#define USART2_PCLK_DI()         (RCC->RCC_APB1ENR &= ~(1<<17))
#define USART3_PCLK_DI()         (RCC->RCC_APB1ENR &= ~(1<<18))
#define USART1_PCLK_DI()         (RCC->RCC_APB2ENR &= ~(1<<14))

#define AFIO_PCLK_DI()           (RCC->RCC_APB2ENR &= ~(1<<0))

#define GPIOA_REG_RESET()        do{ (RCC->RCC_APB2RSTR |= (1<<2));  (RCC->RCC_APB2RSTR &= ~(1<<2)); } while(0)
#define GPIOB_REG_RESET()        do{ (RCC->RCC_APB2RSTR |= (1<<3));  (RCC->RCC_APB2RSTR &= ~(1<<3)); } while(0)
#define GPIOC_REG_RESET()        do{ (RCC->RCC_APB2RSTR |= (1<<4));  (RCC->RCC_APB2RSTR &= ~(1<<4)); } while(0)




#define ENABLE                     1
#define DISABLE                    0
#define SET                        ENABLE
#define RESET                      DISABLE


#endif /* INC_STM32F103XX_H_ */
