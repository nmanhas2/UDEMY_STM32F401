/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 7: Interrupts (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for Interrupts
/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////
#include "../Inc/exti.h"

#define GPIOCEN			(1U<<2) //bit used to enable GPIOC on AHB1 Bus
#define SYSCFGEN		(1U<<14) //bit to enable SYSCFG on APB2 bus

//function to configure interrupt input on PC13
void pc13_exti_init(void)
{
	//disable global interrupt
	__disable_irq();

	//enable clock access for GPIOC
	RCC->AHB1ENR |= GPIOCEN;

	//Set PC13 as an input
	GPIOC->MODER &= ~(1U<<26);
	GPIOC->MODER &= ~(1U<<27);

	//enable clock access to SYSCFG, this is on APB2 bus, based on Section 6.3.12 in Reference Manual
	RCC->APB2ENR |= SYSCFGEN;

	//select PORTC for EXTI3, based on Section 7.2.6 in Reference Manual
	//for PORTC, this means setting the bits on the desired EXTI (in this case 13, because we want PC13)
	//to 0010, meaning that bit 5 should be set to one in the the SYSCFG_EXTICR4 register
	//For general information, every PORTyx is connected to a single EXTI, for example PORTC13 and PORTA13
	//will be on the same EXTI, so you can only have one of the two configured as an interrupt
	SYSCFG->EXTICR[3] |= (1U<<5);

	//unmask EXTI13, based on Section 10.3.1 in reference manual
	EXTI->IMR |= (1U<<13);

	//select falling edge trigger, based on section 10.3.4 in reference manual
	EXTI->FTSR |= (1U<<13);

	//enable EXTI10-15 line in NVIC, since we're interested in EXTI13 and EXTI10-15 are on the same line
	//based on table 38 in the reference manual
	//he used a function in the tutorial that's included in the core_cm4.h header, but you can also enable it
	//it in the NVIC ISER register, based on section 4.2.2 in Cortex-M4 Devices Generic User Guide.
	//there's 8(ISER0-7) different ISER registers, 32 bits each, and based on Table 38 as previously
	//mentioned the bit position for EXTI15_10 is 40. since ISER[0] will cover bits 1-32, and we need bit40,
	//use ISER[1] register and set bit 8
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	//NVIC->ISER[1] |= (1U<<7); //manual way of doing it

	//enable global interrupt
	__enable_irq();
}

