/**********************************************************

Software I2C Library for AVR Devices.

Copyright 2008-2012
eXtreme Electronics, India
www.eXtremeElectronics.co.in
**********************************************************/


#ifndef _I2CSOFT_H
#define _I2CSOFT_H

struct i2c_data2_struct_t
{
	uint8_t addr; ///< I2C address to write to
	uint8_t reg;  ///< Register of slave device to access
	uint8_t datah; ///< Data to write in register
	uint8_t datal; ///< Data to write in register
};
struct i2c_data4_struct_t
{
	uint8_t addr; ///< I2C address to write to
	uint8_t reg;  ///< Register of slave device to access
	uint8_t datah; ///< Data to write in register
	uint8_t datal; ///< Data to write in register
	uint8_t data2h; ///< Data to write in register
	uint8_t data2l; ///< Data to write in register
};
typedef struct i2c_data2_struct_t i2c_data2_t;
typedef struct i2c_data4_struct_t i2c_data4_t;

extern volatile uint8_t *I2cDataPtr;
/* 
I/O Configuration 
*/

#define SCLPORT	PORTC	//TAKE PORTD as SCL OUTPUT WRITE    atmega32 portc.0 i portc.1
#define SCLDDR	DDRC	//TAKE DDRB as SCL INPUT/OUTPUT configure

#define SDAPORT	PORTC	//TAKE PORTD as SDA OUTPUT WRITE
#define SDADDR	DDRC	//TAKE PORTD as SDA INPUT configure

#define SDAPIN	PINC	//TAKE PORTD TO READ DATA
#define SCLPIN	PINC	//TAKE PORTD TO READ DATA

#define SCL	PC0		//PORTD.0 PIN AS SCL PIN
#define SDA	PC1		//PORTD.1 PIN AS SDA PIN


#define SOFT_I2C_SDA_LOW	SDADDR|=((1<<SDA))
#define SOFT_I2C_SDA_HIGH	SDADDR&=(~(1<<SDA))

#define SOFT_I2C_SCL_LOW	SCLDDR|=((1<<SCL))
#define SOFT_I2C_SCL_HIGH	SCLDDR&=(~(1<<SCL))


/**********************************************************
SoftI2CInit()

Description:
	Initializes the Soft I2C Engine.
	Must be called before using any other lib functions.
	
Arguments:
	NONE
	
Returns:
	Nothing

**********************************************************/
void SoftI2CInit();	

/**********************************************************
SoftI2CStart()

Description:
	Generates a START(S) condition on the bus.
	NOTE: Can also be used for generating repeat start(Sr)
	condition too.
	
Arguments:
	NONE
	
Returns:
	Nothing

**********************************************************/
void SoftI2CStart();

/**********************************************************
SoftI2CStop()

Description:
	Generates a STOP(P) condition on the bus.
	NOTE: Can also be used for generating repeat start
	condition too.
	
Arguments:
	NONE
	
Returns:
	Nothing

**********************************************************/
void SoftI2CStop();

/**********************************************************
SoftI2CWriteByte()

Description:
	Sends a Byte to the slave.
	
Arguments:
	8 bit date to send to the slave.
	
Returns:
	non zero if slave acknowledge the data receipt.
	zero other wise.

**********************************************************/
uint8_t SoftI2CWriteByte(uint8_t data);

/**********************************************************
SoftI2CReadByte()

Description:
	Reads a byte from slave.
	
Arguments:
	1 if you want to acknowledge the receipt to slave.
	0 if you don't want to acknowledge the receipt to slave.
	
Returns:
	The 8 bit data read from the slave.

**********************************************************/
uint8_t SoftI2CReadByte(uint8_t ack);

uint8_t SoftI2CWritePacket(void *const data, const uint8_t n);

#endif 