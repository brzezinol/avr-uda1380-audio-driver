/*
 * UART.h
 *
 * Created: 2014-04-19 23:17:16
 *  Author: MeinFuhrer
 */ 
#include "avr-common.h"
#include <avr/io.h>

#ifndef USART_H_
#define USART_H_

#if defined (_AVR_IOM328P_H_)
  #define UBRRL UBRR0L
  #define UBRRH UBRR0H
  #define UCSRA UCSR0A
  #define UCSRB UCSR0B
  #define UCSRC UCSR0C
  #define UDRE UDRE0
  #define UDR UDR0
  #define RXCIE RXCIE0
  #define RXEN RXEN0
  #define TXEN TXEN0
  #define USBS USBS0
  #define UCSZ0 UCSZ00
  #define RXC RXC0
#endif


#ifndef USART_BAUDRATE
#define USART_BAUDRATE 19200
#endif

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#ifndef UART_USEINTERRUPT
#define UART_USEINTERRUPT 1
#endif

volatile unsigned char UART_value;  
volatile unsigned char UART_buffer[16];
extern volatile uint8_t UART_buffer_position;

void USART_Init(void);
void USART_SendByte(uint8_t u8Data);
void USART_SendStr(const char *str);
uint8_t USART_ReceiveByte();
void USART_Log(const char *str);

#endif /* USART_H_ */