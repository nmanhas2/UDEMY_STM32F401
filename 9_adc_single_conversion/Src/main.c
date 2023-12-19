/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 5: ADC (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Testing ADC library for single conversion, using PA1 as an analog input to read

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

//USART2 happens to be accessible through the USB on this board
//Based on Section 6.8 in User Manual
//Based on the block diagram (Figure 3. in Datasheet), USART2 is on the APB1 Bus

#include <stdio.h> //need printf
#include <stdint.h> //need to include because we're using uint32_t
#include "../Inc/uart.h"
#include "stm32f4xx.h"
#include "../Inc/adc.h"

uint32_t sensor_value;

int main(void){
	uart2_tx_init();
	pa1_adc_init();
	start_conversion();
	while(1)
	{
		start_conversion();
		sensor_value = adc_read();
		printf("Sensor value: %d \n\r", (int)sensor_value);
	}
}

