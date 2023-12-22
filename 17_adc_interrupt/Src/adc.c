/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 5: ADC (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for ADC, added interrupts for this version

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////
//This microcontroller has 16 external channels on a single 12 bit ADC
//A temperature sensor is internally connected to ADC_IN16 input channel as well
//We will be initializing PA1 as ADC for our purposes
//Based on Section 3.29 in Datasheet

#include "stm32f4xx.h"
#include "../Inc/adc.h"

#define GPIOAEN			(1U<<0) //enable GPIOA clock access on RCC for PA1
#define ADC1EN			(1U<<8) //enable clock access to ADC1 on AHB2
#define ADC_CH1			(1U<<0) //enable channel l in the sequence
#define CR2_ADON		(1U<<0) //enable ADC in the ADC_CR2 register
#define CR2_SWSTART		(1U<<30) //used to start the conversion of channels
#define CR2_CONT		(1U<<1) //used to enable continuous conversion
#define CR1_EOCIE		(1U<<5) //used to enable end of conversion interrupt

//function to enable ADC on PA1
void pa1_adc_init(void)
{
	RCC->AHB1ENR |= GPIOAEN; //enable GPIOA clock access

	//initialize PA1 port mode as analog, bits = 11 (Section 8.4.1 in Reference Manual)
	GPIOA->MODER |= (1U<<2);
	GPIOA->MODER |= (1U<<3);

	RCC->APB2ENR |= ADC1EN; //ADC1 is on AHB2 Bus, so need to enable clock access there
							//Based on Section 7.3.10 in Reference Manual & Figure 3. in Datasheet

	//Have to indicate where the conversion starts (ie. what channel), in this case there's only one
	//channel so we want it to start on channel 1, which will correspond to SQ1 within the ADC_SQR3
	//register (Section 11.12.11 in Reference Manual). This register is where you would sequence the
	//order of the channels, if you want channel 5 start to start, then channel 2 to follow up after
	//for example. The number you put in the sequence is in binary, so for channel 5 you would put 0101
	//for example.
	ADC1->SQR3 = ADC_CH1; //setting this to '=' since we only want channel 1

	//need to set the length of the sequence, basically how many samples you're taking (or conversions
	//as it says in Section 11.12.9). Only using 1 channel, so 1 conversion is needed.
	ADC1->SQR1 |= 0x00; //0000 on bits 20-23 means 1 conversion, we're not using the rest of SQR1 so "= 0"

	ADC1->CR2 |= CR2_ADON; //enable ADC, based on Section 11.12.3 in Reference Manual
}

//function to enable ADC on PA1
void pa1_adc_interrupt(void)
{
	RCC->AHB1ENR |= GPIOAEN; //enable GPIOA clock access

	//initialize PA1 port mode as analog, bits = 11 (Section 8.4.1 in Reference Manual)
	GPIOA->MODER |= (1U<<2);
	GPIOA->MODER |= (1U<<3);

	RCC->APB2ENR |= ADC1EN; //ADC1 is on AHB2 Bus, so need to enable clock access there
							//Based on Section 7.3.10 in Reference Manual & Figure 3. in Datasheet

	ADC1->CR1 |= CR1_EOCIE; //enable end of conversion interrupt, based on  section 11.12.2 in reference manual

	NVIC_EnableIRQ(ADC_IRQn); //enable ADC interrupt in NVIC

	//Have to indicate where the conversion starts (ie. what channel), in this case there's only one
	//channel so we want it to start on channel 1, which will correspond to SQ1 within the ADC_SQR3
	//register (Section 11.12.11 in Reference Manual). This register is where you would sequence the
	//order of the channels, if you want channel 5 start to start, then channel 2 to follow up after
	//for example. The number you put in the sequence is in binary, so for channel 5 you would put 0101
	//for example.
	ADC1->SQR3 = ADC_CH1; //setting this to '=' since we only want channel 1

	//need to set the length of the sequence, basically how many samples you're taking (or conversions
	//as it says in Section 11.12.9). Only using 1 channel, so 1 conversion is needed.
	ADC1->SQR1 |= 0x00; //0000 on bits 20-23 means 1 conversion, we're not using the rest of SQR1 so "= 0"

	ADC1->CR2 |= CR2_ADON; //enable ADC, based on Section 11.12.3 in Reference Manual
}

//function to start the single conversion of the channels using software
void start_conversion(void)
{
	ADC1->CR2 |= CR2_SWSTART; //start adc conversion
}

//function to start the continuous conversion of the channels using software
void start_continuous(void)
{
	ADC1->CR2 |= CR2_CONT; //enable continuous conversion, based on Section 11.12.3 in Reference Manual
	ADC1->CR2 |= CR2_SWSTART; //start adc conversion
}

//function to wait until the conversion is complete, and return value contained in data register if not
uint32_t adc_read(void)
{
	while(!(ADC1->SR & SR_EOC)); //wait for completion, based on Section 11.12.1 in Reference Manual
	return (ADC1->DR); //return data read from data register, based on Section 11.12.14 in Reference Manual

}

