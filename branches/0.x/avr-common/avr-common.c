/*
 * Common.c
 *
 * Created: 2014-04-19 19:40:38
 *  Author: MeinFuhrer
 */ 

#include <avr/io.h>
#include "avr-common.h"

short* ByteToHexString(uint8_t b){
	char *str = (char *) malloc(sizeof(char) * 2);
	itoa(b,str,16);
	return str;
}