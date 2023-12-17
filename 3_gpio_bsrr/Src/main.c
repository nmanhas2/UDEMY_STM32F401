/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 2: General Purpose (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Utilizing the CMSIS header files to create a blinking LED project,

//	this is the same as project 1 and project 0

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////
#include "stm32f4xx.h"

#define GPIOAEN				(1U <<0)
#define PIN5				(1U<<5)
#define LED_PIN				PIN5

int main(void){
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);
	while(1){

		GPIOA->ODR ^= LED_PIN;
		for(int i = 0; i < 100000; i++){}
	}
}
