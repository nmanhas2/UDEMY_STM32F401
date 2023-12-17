/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 2: General Purpose (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Utilizing IDR to read the value of B1 (PC13 -> found this in Section 6.5 in User Manual)

///	as an input, then turn the LED on or off using similar BSRR code in project 3, based on the input

///	that was read

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"

#define GPIOAEN				(1U <<0)//need to enable GPIOA on the AHB1 bus
#define PIN5				(1U<<5)
#define LED_PIN				PIN5	//using LED2, which corresponds to PIN5

#define GPIOCEN				(1U<<2)//enabling GPIOC on the AHB1 bus, Section 6.3.9 in Reference Manual
#define PIN13				(1U<<13)//B1 (push-button 1) is on PC13
#define BTN_PIN				PIN13

int main(void){
	//enabling GPIOA on the AHB1 bus, which accessed through the RCC for the clocks
	//GPIOA is the first bit of this 32bit register, so simply set that to 1, do similar
	//for GPIOC
	//(Section 6.3.9 in Reference Manual)
	RCC->AHB1ENR |= GPIOAEN;
	RCC->AHB1ENR |= GPIOCEN;

	//setting MODER5 to output
	//Section 8.4.1 in Reference Manual to see port types
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	//setting MODER13 as input
	//Section 8.4.1 in Reference Manual to see port types
	GPIOC->MODER &= ~(1U<<26);
	GPIOC->MODER &= ~(1U<<27);

	while(1){
		//check to see if button (ie. PC13) is high, using IDR
		//based on Section 8.4.5 in Reference Manual
		//this button happens to be active low (ie. initially high)
		if(GPIOC->IDR & BTN_PIN){
			//Based on Section 8.4.7 in Reference Manual, BSRR has 16 bits reserved for
			//setting a bit corresponding to each of the ODR bits, with the remaining 16
			//bits reserved for resetting. To toggle you must set, then reset.

			//turn LED on
			GPIOA->BSRR = LED_PIN;
		} else{
			//turn LED off
			GPIOA->BSRR = (1U<<21);
		}
	}
}
