/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 5: ADC (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for ADC, added interrupts for this version

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>
#define SR_EOC (1U<<1) //used to check end of conversion status
void pa1_adc_init(void);
uint32_t adc_read(void);
void start_conversion(void);
void start_continuous(void);
void pa1_adc_interrupt(void);
#endif /* INC_ADC_H_ */
