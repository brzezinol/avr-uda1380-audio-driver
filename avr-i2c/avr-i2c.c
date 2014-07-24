/*
 * avr_i2c.c
 *
 * Created: 2014-07-22 16:33:45
 *  Author: pbrzezinski
 */ 

#include "avr-i2c.h"
#include <avr/interrupt.h>


/*
 *  Variable definitions
 */
volatile uint8_t * I2cDataPtr;
volatile I2CSTAT_T I2cStatus;
volatile uint8_t I2cNumByte;    //-Holds the # of bytes in the I2C data struct/array
volatile rbuff_t I2cTxBuff;     //-(slave-mode) Data to be sent on master request 
volatile rbuff_t I2cRxBuff;     //-(slave-mode) Data to be read from master

/*================================================================*
                              i2c_isr()
    An interrupt is requested whenever an event occurs,
 on the I2C bus, causing the I2C peripheral to change state.
 The state is held in the I2STAT SFR.
 *================================================================*/
ISR(TWI_vect)
{
    static uint8_t I2cNumRxByte;
    register uint8_t status = I2STAT;
    //////////////////////
    // Master Transmit  //
    //////////////////////
//-START CONDITION TRANSMITTED
//-REPEATED START CONDITION TRANSMITTED 
    if ((status == 0x08) || (status == 0x10))
    {
    //-clear start condition flag so another start condition is not requested
        i2c_no_start_();                        
    //-transmit address
        I2DAT = *I2cDataPtr;
    }
//-SLAVE ADDRESS + WRITE TRANSMITTED - ACK RECEIVED
    else if (status == 0x18)
    {//-Device has acknowledged that the master is talking to it so... send first 'data' byte
        ++I2cDataPtr;
        I2DAT = *I2cDataPtr;
        I2cNumByte -= 2;            //-Decrement the value after tx of byte 
    }
//-SLAVE ADDRESS + WRITE TRANSMITTED - NO ACK RECEIVED
    else if (status == 0x20)
    {
    //-Device is TOO busy to respond or is not present.
        i2c_stop_tx_();             //-no response.
        I2cStatus = I2C_ERROR;      //-status of operation is ERROR
    }
//-DATA BYTE - ACK RECEIVED
    else if (status == 0x28)
    {   //-first data byte was received so send next... if more
        if( I2cNumByte > 0)//-If not zero then send another byte
        {
            ++I2cDataPtr;
            I2DAT = *I2cDataPtr;    //-Send 'data byte' 2 through...  
            --I2cNumByte;     

        }//-send stop command since there are no more bytes to send
        else
        {
           I2cStatus = I2C_OK;
           i2c_stop_tx_();
        }
    }
//-DATA BYTE TRANSMITTED - NO ACK RECEIVED
    else if (status == 0x30)
    {
        I2cStatus = I2C_NOACK;      //-status of operation is noack
        i2c_stop_tx_();             //-no response.
    }
//-Arbitration lost
    else if (status == 0x38)
    { 
		I2cStatus = I2C_LOST_ARB;
        i2c_stop_tx_();     //-Release bus
        //I2CON &= 0x65;    //-Retry when bus is free (STA=1)
    }
    //////////////////////
    //  Master Receive  //
    //////////////////////
//-SLAVE ADDRESS + READ TRANSMITTED - ACK RECEIVED
    else if (status == 0x40)
    {
        if ( I2cNumByte > 1)
            i2c_ack_();     //-More bytes... go to case 50
        else
            i2c_nack_();    //-Nack... go to case 58
    }
//-SLAVE ADDRESS + READ TRANSMITTED - NO ACK RECEIVED
    else if (status == 0x48)
    {   //-Device may be too busy to respond or is not there
        i2c_stop_tx_();
        I2cStatus = I2C_NOACK;      //-status of operation is ERROR
    }
//-DATA BYTE RECEIVED - ACK TRANSMITTED
    else if (status == 0x50)
    {
        if( I2cNumByte > 2)//-If more then two receive another byte
        {
            *I2cDataPtr = I2DAT;    //-Load data buffer.
            ++I2cDataPtr;           //-Get ready for next byte
            --I2cNumByte;           //-
        }
        else//-Get ready to stop receiving
        {//-Part 1 of 2 of the stop sequence
            i2c_nack_();            //-Send Nack so next intrpt goes to case 58H
            *I2cDataPtr = I2DAT;    //-Load data buffer - second to last byte to receive
            ++I2cDataPtr;           //-Get ready for next byte
            --I2cNumByte;           //-
        }
    }
//-DATA BYTE RECEIVED - NO ACK TRANSMITTED
    else if (status == 0x58)
    {
        *I2cDataPtr = I2DAT;        //-Load data buffer.
        //-send stop command since there are no more bytes to send
        i2c_stop_tx_();
        
/// \todo add code to enable repeated starts
        I2cStatus = I2C_OK;         //-status of operation is OK
    }
    //////////////////////
    //   Slave Receive  //
    //////////////////////
//-OWN ADDRESS + WRITE RECEIVED - ACK Sent
    else if (status == 0x60)
    {
        if (I2cRxBuff.dptr)
        {
            i2c_ack_();
            I2cStatus = I2C_BUSY;    
        }
        else  //-ring buffer not initialized
        {
            i2c_nack_();
            I2cStatus = I2C_NO_RBUF;
        }
    }
//-Arbitration lost in SLA+R || Own SLA+W received - ACK RECEIVED
    else if (status == 0x68)//-lost arbitration with device trying to address this one
    {/// \todo write code to handle lost Arbitration 
    }
//-General call address received - ACK RECEIVED
    else if (status == 0x70)
    {//Get ready to Rx data(case-90).
        I2cStatus = I2C_BUSY;
        I2cNumRxByte = 0x00;
        if (I2cRxBuff.dptr)
            i2c_ack_();
        else  //-ring buffer not initialized
            i2c_nack_();            
    }
//-Arbitration lost in SLA+R to General call - ACK RECEIVED
    else if (status == 0x78)
    {}
//-DATA BYTE RECEIVED
    else if (status == 0x80)
    {
        *I2cRxBuff.tail = I2DAT;    //-Load data buffer.   
        i2c_ack_();
        //-Increment pointer
        if ((I2cRxBuff.tail-I2cRxBuff.dptr) & I2C_RX_RING_BUFFER_SIZE)
            I2cRxBuff.tail = I2cRxBuff.dptr;
        else
            ++(I2cRxBuff.tail);         
    }
//-DATA BYTE RECEIVED - Nack returned
    else if (status == 0x88) 
        i2c_ack_();  //-Re-enable reception
//-General call 'DATA' received - ACK RECEIVED
    else if (status == 0x90)
    {   //first time entering
        if ( I2cNumRxByte == 0x00 )
        {
            //-Hardware master (e.g keyboard)
            if (I2DAT & 0x01) 
            { 
                *I2cRxBuff.tail = (I2DAT>>1);//get address
                if ((I2cRxBuff.tail-I2cRxBuff.dptr) & I2C_RX_RING_BUFFER_SIZE)
                    I2cRxBuff.tail = I2cRxBuff.dptr;
                else
                    ++(I2cRxBuff.tail);
                i2c_ack_();
                I2cNumRxByte++;
            }
            else if (I2DAT == 0x04)  //-Re-read i2c-HW address
            {
                i2c_nack_();
                if (i2c_get_address)
                    i2c_get_address ();
            }
            else if (I2DAT == 0x06)  //-Reset then re-read i2c-HW address.
            {
            /// \todo Do a reset
            }                
        }
        else //-Two or more times through
        {
            *I2cRxBuff.tail = I2DAT;    //-Load data buffer.
            if ((I2cRxBuff.tail-I2cRxBuff.dptr) & I2C_RX_RING_BUFFER_SIZE)
                I2cRxBuff.tail = I2cRxBuff.dptr;
            else
                ++(I2cRxBuff.tail);
            i2c_ack_();
            I2cNumRxByte++;
        }
    }
//-General call 'DATA' received - Nack RECEIVED
    else if (status == 0x98)
    {
        i2c_ack_();
    //-Received General call but didn't ack because ring buffer wasn't initialized 
        I2cStatus = I2C_NO_RBUF; 
    }
//-Stop or reStart received while being addressed(talked to)
    else if (status == 0xA0)
    {
        i2c_ack_(); //-(re)enable slave reception
        I2cStatus = I2C_OK; 
    }
    //////////////////////
    //  Slave Transmit  //
    //////////////////////
//-OWN ADDRESS + read Requested - ACK RECEIVED
    else if (status == 0xA8)            
        i2c_nack_(); /// \todo finish slave transmit section
    ///TODO:set arbitration to OK
//-OWN ADDRESS + read Requested(Arbitration lost)- ACK RECEIVED
//      case 0xB0:
//             if (I2cTxBuff.head == I2cTxBuff.tail) //-Empty
//             {
//                 I2cTxBuff.head = I2cTxBuff.dptr;  //-Reset  
//                 I2DAT = 0xFF;
//                 i2c_nack();
//             }
//             else
//             {
//                 I2DAT = *I2cTxBuff.head;
//                 ++(I2cTxBuff.head);
//                 //-next byte...
//                 if (I2cTxBuff.head == I2cTxBuff.tail) //-Empty
//                     i2c_nack();  
//                 else
//                     i2c_ack();  //-More bytes... got to case B8h 
//             }
//      break;
//- Read Request conti - DATA BYTE ACK RECEIVED
//       case 0xB8:
//          I2DAT = *I2cTxBuff.head;
//             ++(I2cTxBuff.head);
//             //-next byte...
//             if (I2cTxBuff.head == I2cTxBuff.tail) //-Empty
//              i2c_nack();  
//          else
//              i2c_ack();  //-More bytes... I'll be back
//      break;
//-DATA BYTE RECEIVED - STOP TX REQUEST(nACK) FROM MASTER
    else if (status == 0xC0)
    {
        I2cStatus = I2C_OK;
        i2c_ack_(); //re-enable slave reception
    }
//-DATA BYTE RECEIVED - MASTER REQ FOR ANOTHER BYTE(ack), but not present [BUFFER OVERRUN]
    else if (status == 0xC8)
    {
        I2cStatus = I2C_ERROR;      //-status of operation is ERROR
        i2c_ack_(); //re-enable slave reception
    }
//-UNKNOWN STATE
    else
    {
        i2c_stop_tx_();             //-request to transmit stop condition
        I2cStatus = I2C_ERROR;      //-status of operation is ERROR
    }

    i2c_clr_intrpt_flag_();            //-Start next tx/rx seq.
}


#if defined(__DOXYGEN__)
/*================================================================*
                        i2c_get_address ()
                <General Call addressing feature>
 *================================================================*/
/// This function must be written by the USER when using General Call addressing
/// \post The SFR TWAR must be written to. 
/// \return USER definable
uint8_t i2c_get_address (){}
#endif
