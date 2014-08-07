/*
 * UART.c
 *
 * Created: 2014-04-19 23:17:03
 *  Author: MeinFuhrer
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <string.h>
#include "avr-common.h"
#include "USART.h"

/* This variable is volatile so both main and RX interrupt can use it.
It could also be a uint8_t type */
extern volatile unsigned char UART_value;  
extern volatile unsigned char UART_buffer[16];
volatile uint8_t UART_buffer_position = 0; 


void USART_Init(void){
   // Set baud rate
   UBRRL = BAUD_PRESCALE;// Load lower 8-bits into the low byte of the UBRR register
   UBRRH = (BAUD_PRESCALE >> 8); 
	 /* Load upper 8-bits into the high byte of the UBRR register
    Default frame format is 8 data bits, no parity, 1 stop bit
  to change use UCSRC, see AVR datasheet*/ 

  // Enable receiver and transmitter and receive complete interrupt 
  UCSRB = ((1<<TXEN)|(1<<RXEN) | (UART_USEINTERRUPT<<RXCIE));
  UCSRC = (0<<USBS)|(3<<UCSZ0);//|(0<<UMSEL)|(1<<UPM0)|(1<<UPM1)|(0<<USBS); //Async, PArity odd , 1 stop bit
  sei();
}

void USART_SendByte(uint8_t u8Data){
	// Wait until last byte has been transmitted
	while((UCSRA &(1<<UDRE)) == 0);

	// Transmit data
	UDR = u8Data;
}

void USART_SendStr(const char *str){
	while(*str) {
		USART_SendByte(*str);		//Advance though string till end
		str++;
	}
	return;
}

void USART_Log(const char *str){
	USART_SendStr("TRACE: ");
	USART_SendStr(str);
}

// not being used but here for completeness
// Wait until a byte has been received and return received data
uint8_t USART_ReceiveByte(){
	while((UCSRA &(1<<RXC)) == 0);
	return UDR;
}

void USART_clearFrame(void){
	for(UART_buffer_position=0;UART_buffer_position<16;UART_buffer_position++)
		UART_buffer[UART_buffer_position] = 0;
	UART_buffer_position = 0;
}

