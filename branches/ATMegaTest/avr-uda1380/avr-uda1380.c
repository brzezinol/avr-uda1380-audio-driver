/*
 * avr_uda1380.c
 *
 * Created: 2014-07-22 21:46:14
 *  Author: pbrzezinski
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include "avr-common.h"
//#include "avr-i2c.h"
#include "avr_soft_i2c.h"
#include "rbuff.h"
#include "audio.h"
#include "USART.h"

//uint8_t I2cRxRing[I2C_RX_RING_BUFFER_SIZE] = {0x55,0xAA,0x55};//to tylko dla trybu slave
	
void inituda(){
	//USART_Log("Audio init\r");
	audiohw_init();
	//USART_Log("Disable output\r");
	audiohw_enable_output(false);
	//USART_Log("Set fs\r");
	audiohw_set_frequency(2);
	//USART_Log("Rec vol\r");
	audiohw_set_recvol(0,0,AUDIO_GAIN_MIC);
	//USART_Log("Master vol\r");
	audiohw_set_master_vol(0,0);
	//USART_Log("Mixer vol\r");
	audiohw_set_mixer_vol(0,0);
	//USART_Log("Set monitor\r");
	audiohw_set_monitor(true);
	//USART_Log("Enable rec\r");
	audiohw_enable_recording(true);
	//USART_Log("Mute false\r");
	audiohw_mute(false);
}
	
int main(void)
{
	DDRC = ((1<<SDA_PIN)|(1<<SCL_PIN)); 
	PORTC = ((1<<SDA_PIN)|(1<<SCL_PIN)); 
	_delay_ms(500);
	//USART_Init();
	//USART_Log("Init i2c\r");
	//i2c_init_(0x45);                        //- F_TWI=100KHz
	SoftI2CInit();	
	inituda();
	
    while(1)
    {
		//_delay_ms(500);
		//uint8_t cmd = 0;
		//cmd = USART_ReceiveByte();
		//if(cmd != 0)
		//{
			//USART_Log("Byte recv\r");
			//USART_SendByte(cmd);
		//}
			
    }
}