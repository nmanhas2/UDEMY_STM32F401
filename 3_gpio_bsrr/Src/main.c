/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 2: General Purpose (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Utilizing BSRR to toggle the LED instead of ODR, this is the same as project 2

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"

#define GPIOAEN				(1U <<0)//need to enable GPIOA on the AHB1ENR
#define PIN5				(1U<<5)
#define LED_PIN				PIN5	//using LED2, which corresponds to PIN5

int main(void){
	//enabling GPIOA on the AHB1 bus, which accessed through the RCC for the clocks
	//GPIOA is the first bit of this 32bit register, so simply set that to 1
	//(Section 6.3.9 in Reference Manual)
	RCC->AHB1ENR |= GPIOAEN;

	//setting MODER5 to output
	//Section 8.4.1 in Reference Manual to see port types
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);
	while(1){

		//Based on Section 8.4.7 in Reference Manual, BSRR has 16 bits reserved for
		//setting a bit corresponding to each of the ODR bits, with the remaining 16
		//bits reserved for resetting. To toggle you must set, then reset.
		GPIOA->BSRR = LED_PIN;
		for(int i = 0; i < 100000; i++){}

		GPIOA->BSRR = (1U<<21);
		for(int i = 0; i < 100000; i++){}
	}
}
