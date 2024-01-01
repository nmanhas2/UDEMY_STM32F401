/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 8: DMA (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Testing a Library for UART DMA transmit, sending a message once through USART2

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

static void dma_callback(void);

int main(void){

	char message[35] = "Hello from Stm32 DMA Transmitter \n\r";
	uart2_tx_init();
	dma1_stream6_init((uint32_t) &message[0], (uint32_t) &USART2->DR, 35);

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
static void dma_callback(void)
{
	//turn LED on
	GPIOA->ODR |= LED_PIN;

}

//handler for DMA1 Stream6 Interrupt, each stream has its own handler
//using transfer complete interrupt
void DMA1_Stream6_IRQHandler(void)
{
	//check for transfer complete flag, based on section 9.5.2 in reference manual, it can be cleared
	//within the HIFCR register, based on section 9.5.4
	if(DMA1->HISR & (HISR_TCIF6))
	{
		//clearing the flag makes it so that it doesn't keep transmitting data, so it'll transmit once
		DMA1->HIFCR |= HIFCR_CTCIF6;
	}

	dma_callback();
}


