/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 6: General Purpose Timers (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for Timer Driver, added interrupts for this version
/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"
#include "../Inc/tim.h"

#define TIM2EN				(1U<<0)
#define CR1EN				(1U<<0)
#define DIER_UIE			(1U<<0) //bit to enable update interrupts
//function to configure TIM2 for 1Hz
void tim2_1hz_init(void)
{
	//enable clock access to TIM2, APB1 bus is where TIM2 can gain access
	//based on Figure 3 in Datasheet
	RCC->APB1ENR |= TIM2EN;

	//16MHz is the default CLK speed, we want this to be 1Hz, so a prescaler is needed
	//based on section 13.4.11 in Reference Manual. this number is then multiplied by the
	//period assigned to the auto reload register (ARR), based on section 13.4.12
	//which means CLK / (PSC * ARR) = desired delay
	//for this purpose 1600*10000 = 16M, CLK speed = 16Mhz, 16MHz/16M = 1Hz
	TIM2->PSC = 1600 - 1; // '-1' because it's 0 based
	TIM2->ARR = 10000 - 1;

	TIM2->CNT = 0; //reset timer count, section 13.4.10 in reference manual

	TIM2->CR1 |= CR1EN; //set the timer enable bit in control register 1 (section 12.4.1 in Reference Manual)
}

//function to configure update interrupt for TIM2 at 1Hz
void tim2_1hz_interrupt(void)
{
	//enable clock access to TIM2, APB1 bus is where TIM2 can gain access
	//based on Figure 3 in Datasheet
	RCC->APB1ENR |= TIM2EN;

	//16MHz is the default CLK speed, we want this to be 1Hz, so a prescaler is needed
	//based on section 13.4.11 in Reference Manual. this number is then multiplied by the
	//period assigned to the auto reload register (ARR), based on section 13.4.12
	//which means CLK / (PSC * ARR) = desired delay
	//for this purpose 1600*10000 = 16M, CLK speed = 16Mhz, 16MHz/16M = 1Hz
	TIM2->PSC = 1600 - 1; // '-1' because it's 0 based
	TIM2->ARR = 10000 - 1;

	TIM2->CNT = 0; //reset timer count, section 13.4.10 in reference manual

	TIM2->CR1 |= CR1EN; //set the timer enable bit in control register 1 (section 12.4.1 in Reference Manual)

	TIM2->DIER |= DIER_UIE; //enabling update interrupt, section 13.4.4 in reference manual

	NVIC_EnableIRQ(TIM2_IRQn); //enable TIM2 interrupt in NVIC
}
