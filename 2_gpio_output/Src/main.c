/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 2: General Purpose (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Utilizing the CMSIS header files to create a blinking LED project,

//	this is the same as project 1 and project 0

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////
#include "stm32f4xx.h" //including header file from CMSIS

#define GPIOAEN				(1U <<0) //need to enable GPIOA on the AHB1ENR
#define PIN5				(1U<<5)
#define LED_PIN				PIN5	 //using LED2, which corresponds to PIN5

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

		//Based on Section 8.4.6 in Reference Manual, 16 bits are reserved for
		//writing/reading to each of the outputs. In these, the LED is being
		//toggled on PA5.
		GPIOA->ODR ^= LED_PIN;
		for(int i = 0; i < 100000; i++){}
	}
}
