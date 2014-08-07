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
#include <util/atomic.h>

#define I2C_ACK  0x18
#define I2C_NACK 0x20
#define I2C_START 0x08
#define I2C_REP_START 0x10

/// \brief Holds a list of device address's that are online.
uint8_t I2cDevArray[I2C_DEV_ARRAY_SIZE];

/*===============================================================*
                         i2c_dev_search () 
 *===============================================================*/
/// Searches for devices that will acknowldege and then
/// places the address of the device in I2cDevArray[].
/// At most I2C_DEV_ARRAY_SIZE can be placed in I2cDevArray[].
/// \param[in] start 8-bit address to start at
/// \param[in] num Number of addresses to try
/// \return non-zero on error
uint8_t i2c_dev_search (uint8_t start, uint8_t num)
{
    register uint8_t i,j=0x00,TimeOut=0xFF;
    uint8_t cntrl = TWCR;
    start >>= 1;
    
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    for (i=start; i<(num + start); ++i)
    {
        TWCR = 0xA4;             //-i2c_start_tx_() - ISR;
        
        while ( !(TWCR&0x80) )  //-wait for start
        {
            if (!(TimeOut--))
                return 1;
        }
        TimeOut=0xFF;
        
        if ((I2STAT==I2C_START)||(I2STAT==I2C_REP_START))
            I2DAT = i<<1;       //-Load address [Write]
        else    
            return 1;           //-Error
            
        i2c_clr_intrpt_flag_(); //-Send address
        
        while ( !(TWCR&0x80) ) //-wait for transmit complete
        {
            if (!(TimeOut--))
                return 1;
        } 
        TimeOut=0xFF;

        if (I2STAT==I2C_ACK)
            I2cDevArray[j++]=i; //-Store address               
        else if (I2STAT!=I2C_NACK) 
            return 1;           //-Error
    }
    TWCR = cntrl & 0b01010101; //-Restore

  }/* End Atomic block */
    
    return 0;           
}

