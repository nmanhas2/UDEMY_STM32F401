/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 3: UART (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Utilizing USART2 to receive data via Tera Term (or other software) on USART2, and turning LED2 on/off based
/// on what is received

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

//USART2 happens to be accessible through the USB on this board
//Based on Section 6.8 in User Manual
//Based on the block diagram (Figure 3. in Datasheet), USART2 is on the APB1 Bus

#include <stdio.h> //need printf
#include <stdint.h> //need to include because we're using uint32_t
#include "../Inc/uart.h"
#include "stm32f4xx.h"

#define GPIOAEN				(1U<<0) //need to enable GPIOA on the AHB1ENR
#define PIN5				(1U<<5)
#define LED_PIN				PIN5	 //using LED2, which corresponds to PIN5

unsigned char key;



int main(void){
	uart2_rxtx_init();

	//enabling GPIOA on the AHB1 bus, which accessed through the RCC for the clocks
	//GPIOA is the first bit of this 32bit register, so simply set that to 1
	//(Section 6.3.9 in Reference Manual)
	RCC->AHB1ENR |= GPIOAEN;

	//setting MODER5 to output
	//Section 8.4.1 in Reference Manual to see port types
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	while(1)
	{
		key = uart2_read();
		if(key == '1')
		{
			GPIOA->ODR |= LED_PIN;
		}
		else
		{
			GPIOA->ODR &= ~(LED_PIN);
		}
	}
}

