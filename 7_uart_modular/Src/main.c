/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 3: UART (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Utilizing USART2 to transmit data via USB, this time using printf to send full sentences

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////
#include <stdio.h> //need printf
#include <stdint.h> //need to include because we're using uint32_t
#include "../Inc/uart.h"
#include "stm32f4xx.h"


//USART2 happens to be accessible through the USB on this board
//Based on Section 6.8 in User Manual

//Based on the block diagram (Figure 3. in Datasheet), USART2 is on the APB1 Bus

int main(void){
	uart2_tx_init();

	while(1){
		printf("Hello from STM32F2401\n\r");
	}
}

