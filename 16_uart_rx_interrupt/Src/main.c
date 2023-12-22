/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 7: Interrupts (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Testing a Library for UART RX interrupt, using PA5 since that has the onboard LED
///	configured on it. The interrupt will trigger when '1' is pressed, and the LED will turn on

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

char key;

static void uart_callback(void);

int main(void){


	uart2_rx_interrupt();

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

//callback function to be called by the interrupt handler
static void uart_callback(void)
{
	//turn LED on/off depending on what was read
	if(key == '1')
	{
		GPIOA->ODR |= LED_PIN;
	}
	else
	{
		GPIOA->ODR &= ~LED_PIN;
	}
}

//handler for USART2 Interrupt
void USART2_IRQHandler(void)
{
	//check if RXNE is set
	if(USART2->SR & SR_RXNE)
	{
		key = USART2->DR; //read usart data register
		uart_callback();
	}
}


