/*
 * avr_uda1380.c
 *
 * Created: 2014-07-22 21:46:14
 *  Author: pbrzezinski
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "avr-common.h"
#include "avr-i2c.h"
#include "rbuff.h"
#include "audio.h"
#include "USART.h"
#include "audiohw.h"

//uint8_t I2cRxRing[I2C_RX_RING_BUFFER_SIZE] = {0x55,0xAA,0x55};//to tylko dla trybu slave
	
void inituda(){
	#ifdef DEBUG
	USART_Log("Audio init\r");
	#endif
	audiohw_init();
	#ifdef DEBUG
	USART_Log("Disable output\r");
#endif
	audiohw_enable_output(false);
	#ifdef DEBUG
	USART_Log("Set fs\r");
	#endif
	audiohw_set_frequency(2);
	#ifdef DEBUG
	USART_Log("Rec vol\r");
	#endif
	audiohw_set_recvol(-16,-16,AUDIO_GAIN_LINEIN);
	#ifdef DEBUG
	USART_Log("Master vol\r");
	#endif
	audiohw_set_master_vol(128,128);
	#ifdef DEBUG
	USART_Log("Mixer vol\r");
	#endif
	audiohw_set_mixer_vol(128,128);
	#ifdef DEBUG
	USART_Log("Set monitor\r");
	#endif
	audiohw_set_monitor(true);
	#ifdef DEBUG
	USART_Log("Enable rec\r");
	#endif
	audiohw_enable_recording(false);
	#ifdef DEBUG
	USART_Log("treble(0)\r");
	#endif
	audiohw_set_treble(2);
	#ifdef DEBUG
	USART_Log("bass(8)\r");
	#endif
	audiohw_set_bass(6);
	#ifdef DEBUG
	USART_Log("Mute false\r");
	#endif
	audiohw_mute(false);
}


	
int main(void)
{
	DDRC = ((1<<SDA_PIN)|(1<<SCL_PIN)); 
	PORTC = ((1<<SDA_PIN)|(1<<SCL_PIN)); 
	_delay_ms(500);
	#ifdef DEBUG
	USART_Init();
	USART_Log("Init\r");
	#endif
	i2c_init_(0x45);                        //- F_TWI=100KHz
	
	_wait_s(2);
	
	inituda();
	
    while(1)
    {
		//demo();
    }
}