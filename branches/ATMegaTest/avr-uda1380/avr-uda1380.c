/*
 * avr_uda1380.c
 *
 * Created: 2014-07-22 21:46:14
 *  Author: pbrzezinski
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr-common.h"
#include "avr-i2c.h"
#include "rbuff.h"
#include "audio.h"
#include "USART.h"

uint8_t I2cRxRing[I2C_RX_RING_BUFFER_SIZE] = {0x55,0xAA,0x55};//to tylko dla trybu slave
	
int main(void)
{
	DDRC = ((1<<SDA_PIN)|(1<<SCL_PIN)); //port C  pins SDA_PIN,SCL_PIN as out
	PORTC = ((1<<SDA_PIN)|(1<<SCL_PIN)); // enable pullups on SDA_PIN,SCL_PIN pins
	USART_Init();
	USART_Log("Init i2c\r");
	i2c_init_(0x45);                        //- F_TWI=100KHz
	//ring_buff_init (&I2cRxBuff,I2cRxRing);  //- Slave-mode RX/TX
	USART_Log("Set i2c self address 0x5E\r");
	i2c_set_address_(0x5E);                 //- 8-bit address
	//i2c_enable_gc_();						//enable global call
	
    while(1)
    {
		USART_Log("Audio init\r");
        audiohw_init();
		USART_Log("Audio postinit\r");
		audiohw_postinit();
		USART_Log("Delay\r");
		USART_SendStr("5");
		_delay_ms(1000);
		USART_SendStr("4");
		_delay_ms(1000);
		USART_SendStr("3");
		_delay_ms(1000);
		USART_SendStr("2");
		_delay_ms(1000);
		USART_SendStr("1");
		_delay_ms(1000);
		USART_SendStr(MSG_CR);
    }
}