/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 6: SYSTICK Timer Library (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for SYSTICK, added interrupts in this version

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

void systickDelayMS(int delay);
void systick_1hz_interrupt(void);
#endif /* INC_SYSTICK_H_ */
