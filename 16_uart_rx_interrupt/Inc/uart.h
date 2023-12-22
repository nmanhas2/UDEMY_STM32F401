/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 3: UART (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for USART2 to transmit data via USB, using printf to send full sentences
/// added interrupts for  this version

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdint.h> //need to include because we're using uint32_t
#include "stm32f4xx.h"

#define SR_TXE				(1U<<7)//need to check empty status of transmit in USART status register
#define SR_RXNE				(1U<<5)//need to check not empty status of receiver in USART SR

void uart2_rx_interrupt(void);
void uart2_tx_init(void);
char uart2_read(void);
void uart2_rxtx_init(void);

#endif /* INC_UART_H_ */
