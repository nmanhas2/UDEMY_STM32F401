/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 9: I2C (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Testing a Library for I2C

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"

#define GPIOBEN 				(1U<<1)

#define I2C1EN					(1U<<21)

#define I2C_100KHZ				80 //0101 0000 = 80 in decimal

#define SD_MODE_MAX_RISE_TIME	17

#define I2C_CR1_PE				(1U<<0)

#define I2C_SR2_BUSY			(1U<<1)

#define I2C_CR1_START			(1U<<8)

#define I2C_SR1_SB				(1U<<0)

#define I2C_SR1_ADDR			(1U<<1)

#define I2C_SR1_TXE				(1U<<7)

#define I2C_CR1_ACK				(1U<<10)

#define I2C_CR1_STOP			(1U<<9)

#define I2C_SR1_RXNE			(1U<<6)

#define I2C_SR1_BTF				(1U<<2)
void i2c1_init(void)
{
	//Enable clock access to GPIOB

	RCC->AHB1ENR |= GPIOBEN;

	//Set PB8 as I2C_SCL, and PB9 for I2C_SDA, based on Table 9 in Datasheet, so Alternate Function
	//mode is needed

	GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |= (1U<<17);

	GPIOB->MODER &= ~(1U<<18);
	GPIOB->MODER |= (1U<<19);

	//I2C requires Open Drain and Pullups

	//open drain
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);

	//pullup
	GPIOB->PUPDR |= (1U<<16);
	GPIOB->PUPDR &= ~(1U<<17);

	GPIOB->PUPDR |= (1U<<18);
	GPIOB->PUPDR &= ~(1U<<19);

	//Enable clock acccess to I2C1 (APB1 Bus in the Datasheet Block Diagram)

	RCC->APB1ENR |= I2C1EN;

	//Reset I2C
	I2C1->CR1 |= (1U<<15);

	//Come out of reset
	I2C1->CR1 &= ~(1U<<15);

	//Set peripheral clock frequency 16MHz (APB1 bus)
	// 001000 = 16MHz
	I2C1->CR2 = (1U<<4);

	//Set I2C to standard mode  at 100KHz clock
	I2C1->CCR = I2C_100KHZ;

	//Set rise time
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	//Enable I2C1 Module
	I2C1->CR1 |= I2C_CR1_PE;


}

/*
 * Function to read a byte from a slave
 *
 * saddr = slave address
 * maddr = memory address
 * data = storing what was read
 */
void i2c1_byteread(char saddr, char maddr, char* data)
{
	volatile int tmp;

	/*
	 * Transmit slave address
	 */

	//wait until bus is not busy
	while(I2C1->SR2 & (I2C_SR2_BUSY));

	//generate start
	I2C1->CR1 |= I2C_CR1_START;

	//wait for start condition to be generated
	while(!(I2C1->SR1 & (I2C_SR1_SB)));

	//transmit slave address + write
	//slave address is 7 bits, but the register is 8
	//bits, hence why it is shifted by 1 bit. bit 0 is
	//for the read/write operation
	I2C1->DR = saddr << 1;

	//wait for addr flag to be set
	while(!(I2C1->SR1 & (I2C_SR1_ADDR)));

	//clear addr flag, reading I2C_SR2 after reading I2C_SR1
	//clears the ADDR flag (18.6.7 in Ref Manual)
	tmp = I2C1->SR2;

	/*
	 * Now we can start reading from the memory location
	 * inside the slave
	 */

	//send memory address
	I2C1->DR = maddr;

	//wait until transmitter is empty
	while(!(I2C1->SR1 & (I2C_SR1_TXE)));

	//generate restart
	I2C1->CR1 |= I2C_CR1_START;

	//wait for start condition to be generated
	while(!(I2C1->SR1 & (I2C_SR1_SB)));

	//transmit slave address + read
	I2C1->DR = saddr << 1 | 1;

	//wait until addr flag is set
	while(!(I2C1->SR1 & (I2C_SR1_ADDR)));

	//disable ack
	I2C1->CR1 &= ~I2C_CR1_ACK;

	//clear addr flag
	tmp = I2C1->SR2;

	//generate stop after data is received
	I2C1->CR1 |= I2C_CR1_STOP;

	//wait until RXNE flag is set (ie. there is data to read)
	while(!(I2C1->SR1 * I2C_SR1_RXNE));

	//read data from DR
	*data++ = I2C1->DR;
}

/*
 * Function to read multiple bytes from I2C slave
 *
 * n = number of bytes
 */
void i2c1_burst_read(char saddr, char maddr, int n, char* data)
{
	volatile int tmp;

	//wait until bus not busy
	while(I2C1->SR2 & (I2C_SR2_BUSY));

	//generate start
	I2C1->CR1 |= I2C_CR1_START;

	//wait for start condition to be generated
	while(!(I2C1->SR1 & (I2C_SR1_SB)));

	//transmit slave address + write
	//slave address is 7 bits, but the register is 8
	//bits, hence why it is shifted by 1 bit. bit 0 is
	//for the read/write operation
	I2C1->DR = saddr << 1;

	//wait for addr flag to be set
	while(!(I2C1->SR1 & (I2C_SR1_ADDR)));

	//clear addr flag, reading I2C_SR2 after reading I2C_SR1
	//clears the ADDR flag (18.6.7 in Ref Manual)
	tmp = I2C1->SR2;

	//wait until transmitter is empty
	while(!(I2C1->SR1 & (I2C_SR1_TXE)));

	//send memory address
	I2C1->DR = maddr;

	//wait until transmitter is empty
	while(!(I2C1->SR1 & (I2C_SR1_TXE)));

	//generate restart
	I2C1->CR1 |= I2C_CR1_START;

	//wait for start condition to be generated
	while(!(I2C1->SR1 & (I2C_SR1_SB)));

	//transmit slave address + read
	I2C1->DR = saddr << 1 | 1;

	//wait for addr flag to be set
	while(!(I2C1->SR1 & (I2C_SR1_ADDR)));

	//clear addr flag, reading I2C_SR2 after reading I2C_SR1
	//clears the ADDR flag (18.6.7 in Ref Manual)
	tmp = I2C1->SR2;

	//disable ack
	I2C1->CR1 |= I2C_CR1_ACK;

	while (n > 0U)
	{
		//check if n = 1 (1 byte)
		if(n == 1U)
		{
			//disable ack
			I2C1->CR1 &= ~I2C_CR1_ACK;

			//generate stop after data is received
			I2C1->CR1 |= I2C_CR1_STOP;

			//wait until RXNE flag is set (ie. there is data to read)
			while(!(I2C1->SR1 * I2C_SR1_RXNE));

			//read data from DR
			*data++ = I2C1->DR;

			break;
		}
		else
		{
			//wait until RXNE flag is set (ie. there is data to read)
			while(!(I2C1->SR1 * I2C_SR1_RXNE));

			//read data from DR
			*data++ = I2C1->DR;

			n--;
		}
	}
}

/*
 * Function to write multiple bytes to a slave
 */
void i2c1_burst_write(char saddr, char maddr, int n, char* data)
{
	volatile int tmp;

	//wait until bus not busy
	while(I2C1->SR2 & (I2C_SR2_BUSY));

	//generate start
	I2C1->CR1 |= I2C_CR1_START;

	//wait for start condition to be generated
	while(!(I2C1->SR1 & (I2C_SR1_SB)));

	//transmit slave address + write
	//slave address is 7 bits, but the register is 8
	//bits, hence why it is shifted by 1 bit. bit 0 is
	//for the read/write operation
	I2C1->DR = saddr << 1;

	//wait for addr flag to be set
	while(!(I2C1->SR1 & (I2C_SR1_ADDR)));

	//clear addr flag, reading I2C_SR2 after reading I2C_SR1
	//clears the ADDR flag (18.6.7 in Ref Manual)
	tmp = I2C1->SR2;

	//wait until transmitter is empty
	while(!(I2C1->SR1 & (I2C_SR1_TXE)));

	//send memory address
	I2C1->DR = maddr;

	for(int i = 0; i < n; i++)
	{
		//wait until TXE flag is set (ie. there is data to read)
		while(!(I2C1->SR1 * I2C_SR1_TXE));

		//transmit memory address
		I2C1->DR = *data++;
	}

	while(!(I2C1->SR1 & I2C_SR1_BTF));

	//generate stop after data is transmitted
	I2C1->CR1 |= I2C_CR1_STOP;
}
