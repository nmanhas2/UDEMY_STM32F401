/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 6: General Purpose Timer Library (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Testing a Library for TIM3 Timer Driver input capture mode by setting CH1 to capture on every
/// rising edge. It will capture the input from the output compare on TIM2 CH1 by connecting a wire from
/// PA5 to PA6 (check Figure 19 in user manual to see pinout).

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

//USART2 happens to be accessible through the USB on this board
//Based on Section 6.8 in User Manual
//Based on the block diagram (Figure 3. in Datasheet), USART2 is on the APB1 Bus

#include <stdio.h> //need printf
#include <stdint.h> //need to include because we're using uint32_t
#include "../Inc/uart.h"
#include "stm32f4xx.h"
#include "../Inc/systick.h"
#include "../Inc/tim.h"
#include "../Inc/exti.h"

//GPIOAEN bit within RCC_AHB1ENR (section 6.3.9 in reference manual)
#define GPIOAEN						(1U<<0) //0b 0000 0000 0000 0000 0000 0000 0000 0001
#define PIN5						(1U<<5) //PIN5 based on ODR, since we want that as an output
#define LED_PIN						PIN5

static void exti_callback(void);

int main(void){

	pc13_exti_init();
	uart2_tx_init();

	/* Enable clock access to GPIOA*/
	//don't just assign RCC_AHB1ENR = GPIOEN,
    //since that will overwrite everything else too
	RCC->AHB1ENR |= GPIOAEN;

	/* set PA5 as output*/
	//based on section 8.4.1 in reference manual, PA5 = MODER5, therefore
	//to set PA5 to an output, MODER5 (ie. bits 10 and 11) = 01
	GPIOA->MODER |= (1U << 10); //set MODER bit 10 to 1
	GPIOA->MODER &= ~(1U<<11);	//set MODER bit 11 to 0

	while(1)
	{

	}
}

static void exti_callback(void)
{
	printf("BTN Pressed \n\r");
}

//interrupt request handler, open startup -> stm32f401retx.s, and go to the g_pfnVectors part of the code
//to see the appropriate name you would need for the interrupt handler depending on what you are using.
//then create a function similar to this.
void EXTI15_10_IRQHandler(void)
{
	//check if interrupt occurred
	if((EXTI->PR & LINE13) != 0)
	{
		//clear PR flag
		EXTI->PR |= LINE13;
		exti_callback();
	}
}

