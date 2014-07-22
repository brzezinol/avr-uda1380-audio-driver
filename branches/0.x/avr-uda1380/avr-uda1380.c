/*
 * avr_uda1380.c
 *
 * Created: 2014-07-22 21:46:14
 *  Author: pbrzezinski
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr-i2c.h"
#include "rbuff.h"
#include "audio.h"

uint8_t I2cRxRing[I2C_RX_RING_BUFFER_SIZE] = {0x55,0xAA,0x55};

int main(void)
{
	DDRC |= 0b00110000; //port C  pins 4,5 as out
	PORTC |= 0b00110000; // enable pullups on 4,5 pins
	i2c_init_(0x45);                        //- F_TWI=100KHz
	//ring_buff_init (&I2cRxBuff,I2cRxRing);  //- Slave-mode RX/TX
	i2c_set_address_(0x5E);                 //- 8-bit address
	i2c_enable_gc_();
	
	audiohw_init();
	
	audiohw_postinit();
	
	sei();   
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}