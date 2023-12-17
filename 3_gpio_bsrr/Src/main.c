//STM32F401
//essentially the same as the project 0 and project 1, just implementing the CMSIS header files
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
