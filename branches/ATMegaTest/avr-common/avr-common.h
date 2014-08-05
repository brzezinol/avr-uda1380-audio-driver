/*
 * Common.h
 *
 * Created: 2014-04-19 19:40:54
 *  Author: MeinFuhrer
 */ 
#include <stdint.h>

#ifndef COMMON_H_
#define COMMON_H_

#if defined (_AVR_IOM328P_H_)
#define SDA_PIN 0x04 //port C  pins 4,5 as out
#define SCL_PIN 0x05
#elif defined (_AVR_IOM32_H_)
#define SDA_PIN 0x00
#define SCL_PIN 0x01
#endif

//master define cpu oscilator
#ifndef F_CPU
  #define F_CPU 16000000UL
#endif

//master twi speed definition
#ifndef F_TWI
  #define F_TWI 100000UL
#endif

//audiohw
#ifndef HAVE_UDA1380
  #define HAVE_UDA1380
#endif

#ifndef HAVE_RECORDING
  #define HAVE_RECORDING
#endif

#ifndef AUDIO_GAIN_LINEIN
  #define AUDIO_GAIN_LINEIN       0
#endif

#ifndef AUDIO_GAIN_MIC
  #define AUDIO_GAIN_MIC          1
#endif

//uart
#ifndef USART_BAUDRATE
 #define USART_BAUDRATE 57600
#endif

#ifndef UART_USEINTERRUPT
 #define UART_USEINTERRUPT 1
#endif

////uart codes
#define MSG_CR "\r\0"
//extern const char MSG_init[6] = {"INIT\r\0"};
//extern const char MSG_ok[4] = {"OK\r\0"};
//extern const char MSG_ovf[5] = {"OVF\r\0"};
//extern const char MSG_ukn[6] = {"UKNV\r\0"};
//extern const char MSG_addr[4] = {"ADR\0"};
//extern const char MSG_loc[4] = {"LOC\0"};
//extern const char MSG_rmt[4] = {"RMT\0"};
//extern const char MSG_rx[6] = {"DANE\r\0"};
//extern const char MSG_i2cerr[8] = {"I2CERR\r\0"};
//extern const char CMD_GET_REG_1FFC[7] = {"GET1FFC"}; // 0x1FFC
//extern const char CMD_LVL_ON[5] = {"LVLON"};
//extern const char CMD_LVL_OFF[6] = {"LVLOFF"};


#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))

#define SET_PINPORT_AS_OUT(ADDRES,PIN) (ADDRES |= (1<<PIN))
#define SET_PINPORT_AS_IN(ADDRES,PIN) (ADDRES &= ~(1<<PIN))

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
/* return number of elements in array a */
#define ARRAYLEN(a) (sizeof(a)/sizeof((a)[0]))

extern short* ByteToHexString(uint8_t b);

#endif /* COMMON_H_ */