/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 9: I2C (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Testing a Library for I2C

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

#ifndef INC_I2C_H_
#define INC_I2C_H_

void i2c1_init(void);
void i2c1_byteread(char saddr, char maddr, char* data);

#endif /* INC_I2C_H_ */
