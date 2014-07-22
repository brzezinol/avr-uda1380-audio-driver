/* Copyright (c) 2009 John Myers
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.
 
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 
*/
#include "avr-i2c.h"

extern volatile uint8_t I2cNumByte; 

/*================================================================*
                          i2c_read()
    Function to read (as master) from another I2C device.   
 *================================================================*/
/// \b DeviceAddress is an \b 8-bit number.
/// \param[in] DeviceAddress Device to read from or \b I2C_ADDRESS_IN_ARRAY to 
///  allow the first struct member or array index zero of \c ReturnData to be the address.
/// \param[out] *ReturnData destination
/// \param[in] n number of bytes to write
/// \return I2cStatus
uint8_t i2c_read ( uint8_t DeviceAddress, void *const ReturnData, const uint8_t n)
{
    register uint8_t TimeOut = 0x00;
    
    I2cDataPtr = (uint8_t *)ReturnData; //-Point to where the data should go
    I2cNumByte = n;                     //-Set number of bytes to receive.
  
    //-if 0xFF then use ReturnData for address
    if (DeviceAddress == I2C_ADDRESS_IN_ARRAY)
        --I2cNumByte; //-Don't count the address for number of bytes to load.
    else
       *I2cDataPtr = DeviceAddress;
  
    *I2cDataPtr |= READ_I2C_DATA;   //-Force read
    I2cStatus = I2C_BUSY;           //-Set status of I2C bus 
    i2c_start_tx_();  

//-wait until I2C is no longer busy.
    while (I2cStatus == I2C_BUSY)
    {
        _delay_ms (8);
        if (TimeOut++ & I2C_TIMEOUT_VALUE)
        {
            I2cStatus |= I2C_TIMEOUT;
            break;
        }
    }
//-Return status
    return I2cStatus;
}
