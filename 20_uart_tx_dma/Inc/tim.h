/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 6: General Purpose Timers (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for Timer Driver, with input capture and output compare
/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#ifndef INC_TIM_H_
#define INC_TIM_H_

void tim2_1hz_init(void);
void tim2_pa5_output_compare(void);
void tim3_pa6_input_capture(void);

#define SR_UIF	(1U<<0) //bit in timer status register to check if update flag has reset
#define SR_CC1F (1U<<1) //bit in timer status register to check if capture/compare flag is set

#endif /* INC_TIM_H_ */
