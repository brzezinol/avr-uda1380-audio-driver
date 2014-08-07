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

//uint8_t I2cRxRing[I2C_RX_RING_BUFFER_SIZE] = {0x55,0xAA,0x55};//to tylko dla trybu slave
	
void inituda(){
	USART_Log("Audio init\r");
	audiohw_init();
	USART_Log("Disable output\r");
	audiohw_enable_output(false);
	USART_Log("Set fs\r");
	audiohw_set_frequency(2);
	USART_Log("Rec vol\r");
	audiohw_set_recvol(0,0,AUDIO_GAIN_MIC);
	USART_Log("Master vol\r");
	audiohw_set_master_vol(0,0);
	USART_Log("Mixer vol\r");
	audiohw_set_mixer_vol(0,0);
	USART_Log("Set monitor\r");
	audiohw_set_monitor(true);
	USART_Log("Enable rec\r");
	audiohw_enable_recording(false);
	USART_Log("Mute false\r");
	audiohw_mute(false);
}
	
int main(void)
{
	/*
		Aliasy s¹ zdefiniowane w avr-common.h
		IIC_PORT to w zale¿noœci o procesora PORTD, PORTC etc.
		IIC_DDR to w zale¿noœci od procesora DDRD, DDRC etc.
		SDA_PIN i SCL_PIN te¿ s¹ zdefiniowane jak powy¿sze
	*/
	
	IIC_PORT = ((1<<SDA_PIN)|(1<<SCL_PIN)); 
	IIC_DDR = ((1<<SDA_PIN)|(1<<SCL_PIN)); 
	//opóŸnienie 500ms
	_delay_ms(500);
	//inicjalizacja USART
	USART_Init();
	//Log tekstowy
	USART_Log("Init i2c\r");
	//init TWI
	i2c_init_(0x45);                        //- F_TWI=100KHz
	
	//opóŸnienie 1s to do przetestowania jeszcze.
	for(int i=0;i<2;i++){
		_delay_ms(500);
	}
	
    while(1)
    {
		//init uda w petli co 5s
		inituda();
		for(int i=0;i<20;i++){
			_delay_ms(500);
		}
    }
}