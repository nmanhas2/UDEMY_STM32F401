/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 5: ADC (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for ADC, added continuous conversion for this version

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>
void pa1_adc_init(void);
uint32_t adc_read(void);
void start_conversion(void);
void start_continuous(void);

#endif /* INC_ADC_H_ */
