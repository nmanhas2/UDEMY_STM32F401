/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 6: SYSTICK Timer Library (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for SYSTICK, added interrupts in this version

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////
#include "stm32f4xx.h"

//16MHz is default RC speed, 1ms = 0.001s
//Based on N (LOAD value) = delay(seconds) * SYSCLK(Hz), N = 16000
//Meaning 16 000 cycles happen in 1ms
//you can think of the LOAD register as a value register essentially
//Section 4.4.2 in Cortex-M4 Devices Generic User Guide
#define SYSTICK_LOAD_VAL			16000
#define ONE_SEC_LOAD				16000000 //same as above, but for 1 second delay

//need bits to enable the system timer, set the clock source, and read the count flag
//based on Section 4.4.1 in Cortex-M4 Devices Generic User Guide, all are located on the
//control and Status register (these are on the same register)
#define CTRL_ENABLE					(1U<<0)
#define CTRL_CLKSRC					(1U<<2)
#define CTRL_COUNTFLAG				(1U<<16)
#define CTRL_TICKINT				(1U<<1) //bit to enable systick interrupt in CTRL


//function to set a delay in ms using the internal system clock (SYSTICK)
void systickDelayMS(int delay)
{
	//specify start value to load into the current value register (CVR), which is what the counter is
	//set to when it's enabled initially, and when it reaches 0. essentially reloading it with the number
	//of clock cycles per ms
	//Based on Section 4.4.2 in Cortex-M4 Devices Generic User Guide
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	//clearing the current value in the current value register before we enable the systick
	//Section 4.4.3 in Cortex-M4 Devices Generic User Guide
	SysTick->VAL = 0;

	//enable systick and select clksource (processor clock = internal clock)
	//based on Section 4.4.1 in Cortex-M4 Devices Generic User Guide
	SysTick->CTRL = (CTRL_ENABLE | CTRL_CLKSRC);

	//start the delay
	for(int i = 0; i < delay; i++)
	{
		//wait until the COUNTFLAG is set,
		//based on Section 4.4.1 in Cortex-M4 Devices Generic User Guide
		while(!(SysTick->CTRL & CTRL_COUNTFLAG));
	}

	//reset the control register once the delay is done, since we don't need SysTick anymore
	SysTick->CTRL = 0;
}

//interrupt for systick, which will fire off every second
//similar setup to systickDelayMS()
void systick_1hz_interrupt(void)
{
	SysTick->LOAD = ONE_SEC_LOAD; //16M clock cycles per second

	SysTick->VAL = 0; //clearing current value in CVR

	//enable systick, select clksource (processor clock = internal clock), and enable interrupts
	//based on Section 4.4.1 in Cortex-M4 Devices Generic User Guide
	SysTick->CTRL = (CTRL_ENABLE | CTRL_CLKSRC | CTRL_TICKINT);


}
