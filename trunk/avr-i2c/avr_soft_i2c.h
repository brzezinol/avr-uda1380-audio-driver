/*
 * avr_soft_i2c.h
 *
 * Created: 2014-07-30 13:55:16
 *  Author: pbrzezinski
 */ 


#ifndef AVR_SOFT_I2C_H_
#define AVR_SOFT_I2C_H_

#include "avr-common.h"

/// A master write of \b n number of bytes from data in \b *Data
extern uint8_t i2c_write ( void *const data, const uint8_t n);
/// A master read from \b DeviceAddress of \b n number of bytes to \b *ReturnData
extern uint8_t i2c_read ( uint8_t DeviceAddress, void *const ReturnData, const uint8_t n);


#endif /* AVR_SOFT_I2C_H_ */