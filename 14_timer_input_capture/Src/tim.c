/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 6: General Purpose Timers (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for Timer Driver, with input capture and output compare
/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"
#include "../Inc/tim.h"

#define TIM2EN				(1U<<0) //bit for enabling clock access to TIM2 in APB1 bus
#define TIM3EN				(1U<<1) //bit for enabling clock access to TIM3 in APB1 bus

#define CR1EN				(1U<<0) //bit for enabling timer in control register 1
#define OC_TOGGLE			((1U<<4) | (1U<<5)) //want to set the output compare to toggle
#define CCR_CC1E			(1U<<0)	//bit to enable input/output compare in CCER register

#define GPIOAEN				(1U<<0)	//used to enable GPIOA on the AHB1 bus

#define ARF5_TIM			(1U<<20) //used to enable AF01 alternate function mode
#define ARF6_TIM			(1U<<25) //used to enable AF02 alternate function mode

#define CCMR1_CC1S			(1U<<0) //used to enable input capture/compare, mapped on TI1

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

//function to configure TIM2 output compare on channel 1 (pa5)
void tim2_pa5_output_compare(void)
{
	RCC->AHB1ENR |= GPIOAEN; //enable clock access to GPIOA (PA5)

	//set PA5 to alternate function mode, based on section 8.4.2 in reference manual
	GPIOA->MODER |= (1U<<11);
	GPIOA->MODER &= ~(1U<<10);

	//set alternate function type to TIM2_CH1 (AF01), based on table 9 in datasheet

	GPIOA->AFR[0] |= ARF5_TIM; //based on section 8.4.9 in reference manual

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

	//set output compare toggle mode, based on section 12.4.7 in reference manual
	//first two bits from bits 6-4, set the output compare mode and we want the LED
	//to toggle, which means to set those bits to 011

	TIM2->CCMR1 = OC_TOGGLE;

	//enable tim2 ch1 in compare mode, this is because PA5 has LED2 on it, and we want it to toggle
	//directly based on the timer event, based on Table 9. in the datasheet

	TIM2->CCER |= CCR_CC1E;

	TIM2->CNT = 0; //reset timer count, section 13.4.10 in reference manual

	TIM2->CR1 |= CR1EN; //set the timer enable bit in control register 1 (section 12.4.1 in Reference Manual)
}

//function to configure TIM3 input capture on channel 1 (PA6)
void tim3_pa6_input_capture(void)
{
	RCC->AHB1ENR |= GPIOAEN; //enable clock access to GPIOA (PA6)

	//set PA6 to alternate function mode, based on section 8.4.2 in reference manual

	GPIOA->MODER |= (1U<<13);
	GPIOA->MODER &= ~(1U<<12);

	//set alternate function type to TIM3_CH1 (AF02), based on table 9 in datasheet

	GPIOA->AFR[0] |= ARF6_TIM; //based on section 8.4.9 in reference manual

	//enable clock access to TIM3, APB1 bus is where TIM3 can gain access
	//based on Figure 3 in Datasheet

	RCC->APB1ENR |= TIM3EN;

	//setting prescaler

	TIM3->PSC = 16000 - 1; //16Mhz/16K

	//set channel 1 in TIM3 to be set as an input capture, based on section 12.4.7 in reference manual

	TIM3->CCMR1 = CCMR1_CC1S;

	//enable capture on channel 1

	TIM3->CCER = CCR_CC1E; //based on section 13.4.9 in reference manual

	//enable the timer, based on section 12.4.1 in reference manual

	TIM3->CR1 = CR1EN;
}
