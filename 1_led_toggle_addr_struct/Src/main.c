/////////////////////////////////////////////////////////////////////////////

/// Using: STM32F401 NUCLEO Board

///	Following Section 1: (https://www.udemy.com/course/embedded-systems-bare-metal-programming/)

///	Purpose: Utilizing the Memory Addresses from the Reference Manual + Datasheet to construct

//	peripheral registers for GPIO, and testing this through blinking an LED

/// Author: Nubal Manhas

/////////////////////////////////////////////////////////////////////////////

//STM32F401 NUCLEO DEV Board
//Where is the led connected?
//Port: PortA
//LED: Pin 5
//Based on Section 6.4 in User Manual

//These are located in the peripheral block
//block. Section 5 in datasheet
//The peripherals need buses to access to the clock, and the
//bus contains the clock to various parts of the MCU
//For example ABP1 provides clock access to USART2, based
//on figure 3. in datasheet

//AHB (advanced high performance) buses are faster than the
//APB (advanced peripheral bus) buses. if GPIOA is on both
//buses for example, you can access it faster on AHB (ie. less
//clock cycles) compared to APB

//Based on figure 3. in the datasheet, GPIOA's clock is supplied
//through AHB1

//Each peripheral has an address, with each one having registers
//which are offsets of the peripheral address. this is where
//the boundary address comes into play (table 10 in datasheet)

//each hexadecimal digit = 4 binary digits, therefore
//we must write 8 digits in hex to achieve 32 bits (ie. 4*8)

#include <stdint.h>

//UL = unsigned long
#define PERIPH_BASE				(0x40000000UL) //base addr of peripherals (figure 15 in datasheet)

////based on table 10 in the datasheet, AHB1 starts on
//0x40020000, which happens to be where GPIOA also starts.
//since we know the base periph addr is 0x40000000, 0x00020000 is just
//an offset to add on to that base
#define AHB1PERIPH_OFFSET		(0x00020000UL) //offset to add to periph base for AHB1
#define AHB1PERIPH_BASE		(PERIPH_BASE + AHB1PERIPH_OFFSET) //base of AHB1

//offset would be 0, based on table 10 in datasheet, since
//AHB1 address starts on GPIOA
#define GPIOA_OFFSET			(0x0000UL)
#define GPIOA_BASE				(AHB1PERIPH_BASE + GPIOA_OFFSET)

//RCC = resetting clock control
//RCC on AHB1 = take AHB1PERIPH_BASE and add the offset like GPIOA
//in this case the offset is 0x3800, based on table 10 in datasheet
#define RCC_OFFSET				(0x3800UL)
#define RCC_BASE				(AHB1PERIPH_BASE + RCC_OFFSET)

//GPIOAEN bit within RCC_AHB1ENR (section 6.3.9 in reference manual)
#define GPIOAEN						(1U<<0) //0b 0000 0000 0000 0000 0000 0000 0000 0001

#define PIN5						(1U<<5) //PIN5 based on ODR, since we want that as an output
#define LED_PIN						PIN5

//using the same logic as the GPIOTypeDef, making a dummy
//to have a placeholder for the 12 registers that come before
//the AHB1ENR, which is what we want to use. section 6.3 in
//reference manual
typedef struct
{
	volatile uint32_t DUMMY[12];
	volatile uint32_t AHB1ENR; //RCC AHB1 peripheral clock register (offset: 0x30)
} RCC_TypeDef;

//Each GPIO register contains 10 registers, based on Section 8.4
//in reference manual
//uint32 sets them to occupy 4 bytes in memory (ie. 32 bits) which
//matches the reference manual. the offsets increase by 4 bytes
//in the order of the struct, for each register, according
//to the reference manual (section 8.4 in reference manual)
//this is why it is setup in this way. we're not using all of the
//members in the struct for this project, so just comment out and
//make a dummy to fill in the in between register values to not
//disrupt the order of the offset
//
//__IO = volatile
typedef struct
{
	volatile uint32_t MODER; //mode register, offset = 0x00
	//__IO uint32_t OTYPER; //output type register
	//__IO uint32_t OSPEEDR; //output speed
	//__IO uint32_t PUPDR; //pull up/pull down register
	//__IO uint32_t IDR; //input data register
	volatile uint32_t DUMMY[4];
	volatile uint32_t ODR; //output data register, offset = 0x14
	//__IO uint32_t BSRR; //bit setreset register
	//__IO uint32_t LCKR; //configuration lock register
	//__IO uint32_t AFR[2]; //alternate function high/low, array of size 2
} GPIO_TypeDef;

#define RCC							((RCC_TypeDef*)RCC_BASE)
#define GPIOA						((GPIO_TypeDef*)GPIOA_BASE)

int main(){
	/* Enable clock access to GPIOA*/
	//don't just assign RCC_AHB1ENR = GPIOEN,
    //since that will overwrite everything else too
	RCC->AHB1ENR |= GPIOAEN;

	/* set PA5 as output*/
	//based on section 8.4.1 in reference manual, PA5 = MODER5, therefore
	//to set PA5 to an output, MODER5 (ie. bits 10 and 11) = 01
	GPIOA->MODER |= (1U << 10); //set MODER bit 10 to 1
	GPIOA->MODER &= ~(1U<<11);	//set MODER bit 11 to 0

	while(1){
		/*toggle PA5*/
		GPIOA->ODR^= LED_PIN; //'^' = toggle
		//GPIOA_ODR ^= LED_PIN; //'^' = toggle
		for(int i = 0; i < 100000; i++){

		}
	}
}
