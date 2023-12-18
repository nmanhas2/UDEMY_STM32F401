/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 3: UART (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for USART2 to transmit data via USB, using printf to send full sentences

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdint.h> //need to include because we're using uint32_t
#include "stm32f4xx.h"

void uart2_tx_init(void);
char uart2_read(void);
void uart2_rxtx_init(void);

#endif /* INC_UART_H_ */
