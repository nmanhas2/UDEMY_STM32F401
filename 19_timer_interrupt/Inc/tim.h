/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 6: General Purpose Timers (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for Timer Driver, added interrupts for this version
/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#ifndef INC_TIM_H_
#define INC_TIM_H_

void tim2_1hz_init(void);
void tim2_1hz_interrupt(void);
#define SR_UIF	(1U<<0) //bit in timer status register to check if the flag has reset

#endif /* INC_TIM_H_ */
