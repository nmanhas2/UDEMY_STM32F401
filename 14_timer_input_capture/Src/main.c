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

//GPIOAEN bit within RCC_AHB1ENR (section 6.3.9 in reference manual)
#define GPIOAEN						(1U<<0) //0b 0000 0000 0000 0000 0000 0000 0000 0001
#define PIN5						(1U<<5) //PIN5 based on ODR, since we want that as an output
#define LED_PIN						PIN5

//capturing the count from the input capture, should increment by multiples of 1000
//use debug -> live expressions to capture this variable
int timestamp = 0;
int are = 0;
int main(void){
	tim2_pa5_output_compare();
	tim3_pa6_input_capture();
	while(1)
	{
		//wait until the input is captured on rising edge, based on section 13.4.5 in reference manual
		while(!(TIM3->SR & SR_CC1F));

		//read the value that was captured, based on section 13.4.13 in reference manual
		timestamp = TIM3->CCR1;
		are = TIM3->ARR;
	}
}

