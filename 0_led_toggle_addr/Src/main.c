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

//RCC clock enable register, based on section 6.3.9 in reference manual
//RCC_AHB1ENR = 32 bit register
//
//*(volatile unsigned int *), the (volatile unsigned int *) part is
//just ype casting as a volatile int pointer, then the * at the start
//is to dereference. this is what allows access that register
#define RCC_AHB1ENR_OFFSET			(0x30UL)
#define RCC_AHB1ENR					(*(volatile unsigned int *)(RCC_BASE + RCC_AHB1ENR_OFFSET))

//GPIOAEN bit within RCC_AHB1ENR (section 6.3.9 in reference manual)
#define GPIOAEN						(1U<<0) //0b 0000 0000 0000 0000 0000 0000 0000 0001

//MODER = port mode register, ie. setting direction like input, output, etc
//based on Section 8.4.1 in reference manual.
//alternate function = setting it to an RX/TX to UART, SPI pin, etc.
//special function basically
#define MODER_OFFSET				(0x00UL)
#define GPIOA_MODER					(*(volatile unsigned int *)(GPIOA_BASE + MODER_OFFSET))


//output data register, based on section 8.4.6 in reference manual
#define ODR_OFFSET					(0x14UL)
#define GPIOA_ODR					(*(volatile unsigned int *)(GPIOA_BASE + ODR_OFFSET))

#define PIN5						(1U<<5) //PIN5 based on ODR, since we want that as an output
#define LED_PIN						PIN5

int main(){
	/* Enable clock access to GPIOA*/
	//don't just assign RCC_AHB1ENR = GPIOEN,
    //since that will overwrite everything else too
	RCC_AHB1ENR |= GPIOAEN;

	/* set PA5 as output*/
	//based on section 8.4.1 in reference manual, PA5 = MODER5, therefore
	//to set PA5 to an output, MODER5 (ie. bits 10 and 11) = 01
	GPIOA_MODER |= (1U << 10); //set MODER bit 10 to 1
	GPIOA_MODER &= ~(1U<<11); //set MODER bit 11 to 0

	while(1){
		/*Set PA5 high*/
		//GPIOA_ODR |= LED_PIN;

		/*toggle PA5*/
		GPIOA_ODR ^= LED_PIN; //'^' = toggle
		for(int i = 0; i < 100000; i++){

		}
	}
}
