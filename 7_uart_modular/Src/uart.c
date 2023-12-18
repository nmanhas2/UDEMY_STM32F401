/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 3: UART (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Creating a Library for USART2 to transmit data via USB, using printf to send full sentences

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////
#include "../Inc/uart.h"

#define GPIOAEN				(1U <<0)//need to enable GPIOA on the AHB1 bus
#define UART2EN				(1U <<17)//need to enable USART2 on the APB1 bus (Section 6.3.11 in Reference Manual)

#define SYS_FREQ			16000000//default system frequency (16MHz or cycles/per sec)
#define APB1_CLK			SYS_FREQ//divisor for the frequency should be 1 by default, so it would be the same as the sys clock

#define UART_BAUDRATE		115200

#define CR1_TE				(1U<<3)//need to enable TX on USART control register (Section 19.6.4 in Reference Manual)
#define CR1_UE				(1U<<13)//need to enable USART on USART control register

#define SR_TXE				(1U<<7)//need to check transmit status of USART status register

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_write(int ch);

//modifying putchar to send data over uart
int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

//Function to initialize USART2 tx
//Based on Table 9. in the Datasheet, TX = PA2 with AF07 as the alternate function selection
void uart2_tx_init()
{
	/*
	 * Step 1: Configure GPIOA, with PA2 set to alternate function mode on AF7 (USART2)
	 */
	RCC->AHB1ENR |= GPIOAEN; //enable clock access to GPIOA, GPIOA is on AHB1 bus

	//PA2 = MODER2 in GPIOA, alternate function mode = 10 on bits 5 and 4 (PA2) within MODER
	//Based on Section 8.4.1 in Reference Manual
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	//There's alternate function high and alternate function low registers for port x, the low
	//is for the first 8 port x bits (0-7), high is for the latter 8 (8-15). Each one contains
	//4 bits corresponding to each port x bit. Based on Section 8.4.9/8.4.10 in Reference Manual
	GPIOA->AFR[0] |= (1U<<8); //AF07 = 0111
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	/*
	 * Step 2: Configure USART2
	 */
	RCC->APB1ENR |= UART2EN; //enable clock access to USART2, this is on the APB1 Bus
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE); //configure USART2 to 115200Hz

	USART2->CR1 = CR1_TE; //configuring transfer direction (TX), based on Section 19.6.4 in Reference Manual
						  //want the rest of the register to be cleared, so it's intentionally "="
						  //this also makes the word length 1 start bit, 8 data bits, n stop bits
						  //with n = 0, since CR2 = 0 on bits 13:12, and even parity

	USART2->CR1 |= CR1_UE; //enable USART
}

//function to write to transmit data over USART
void uart2_write(int ch)
{
	//wait until transmit data register is empty with the status register
	//Section 19.6.1 in Reference Manual
	while(!(USART2->SR & SR_TXE));
	USART2->DR = (ch & 0xFF); //writing data value to the data register (8 bits), Section 19.6.2 in Reference Manual


}

//function for setting baudrate to the BRR register in the desired USART
//PeriphClk = peripheral clock frequency (APB1 in this case)
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

//function for computing the desired baudrate
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	//the reference manual explains it in Section 19.3.4, this assumes that OVER8 = 0

	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
