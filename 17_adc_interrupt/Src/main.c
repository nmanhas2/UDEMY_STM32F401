/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 7: ADC Interrupts (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Testing ADC interrupts library, essentially the same as project 10 but with interrupts

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
static void adc_callback(void);

int main(void){
	uart2_tx_init();
	pa1_adc_interrupt();
	start_continuous();
	while(1)
	{

	}
}

//callback for adc ISR
static void  adc_callback(void)
{	//adc_read() function waits for EOC, but the interrupt occurs when that happens, so no need
	//to wait. this is why that function is not being used
	sensor_value = ADC1->DR;
	printf("Sensor value: %d \n\r", (int)sensor_value);
}

//ISR for ADC interrupt
void ADC_IRQHandler(void)
{
	if(ADC1->SR & SR_EOC)
	{
		ADC1->SR &= ~SR_EOC; //clear EOC
		adc_callback();
	}
}
