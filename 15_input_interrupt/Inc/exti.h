/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 7: Interrupts (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for Interrupts, just general input interrupts for now
/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#ifndef INC_EXTI_H_
#define INC_EXTI_H_

#include "stm32f4xx.h"
#define  LINE13 (1U<<13) //used to read EXTI_PR for bit 13, section 10.3.6 in reference manual
void pc13_exti_init(void);

#endif /* INC_EXTI_H_ */
