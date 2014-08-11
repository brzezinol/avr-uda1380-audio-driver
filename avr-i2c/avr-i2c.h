/*

*/

#ifndef AVRI2C_H
#define AVRI2C_H

#include "avr-common.h"

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>  
#include "rbuff.h"

/*
 *  I2C Bit Rate Calculator
 */
#if defined(F_TWI)
#define I2C_PS1_MIN_F_TWI   (F_CPU/526)
#define I2C_BR_PS1          (F_CPU/(2*F_TWI))-8
#define I2C_PS4_MIN_F_TWI   (F_CPU/2056)
#define I2C_BR_PS4          (F_CPU/(8*F_TWI))-2
#define I2C_PS16_MIN_F_TWI  (F_CPU/8176)
#define I2C_BR_PS16         (-(16*F_TWI - F_CPU)/(32*F_TWI))
#define I2C_PS64_MIN_F_TWI  (F_CPU/32656)
#define I2C_BR_PS64         (-(16*F_TWI - F_CPU)/(128*F_TWI))
#if F_CPU > (F_TWI * 16)
# if F_TWI > I2C_PS1_MIN_F_TWI //if greater then required min then use PS1 100000 > 30418
#  define I2C_BIT_RATE  I2C_BR_PS1 //72
#  define I2C_PRESCALER 0
# elif F_TWI > I2C_PS4_MIN_F_TWI
#  define I2C_BIT_RATE  I2C_BR_PS4
#  define I2C_PRESCALER 1
# elif F_TWI > I2C_PS16_MIN_F_TWI
#  define I2C_BIT_RATE  I2C_BR_PS16
#  define I2C_PRESCALER 2
# elif F_TWI > I2C_PS64_MIN_F_TWI
#  define I2C_BIT_RATE  I2C_BR_PS64
#  define I2C_PRESCALER 3
# else
#  error Invalid F_TWI value
# endif

# if 10 < I2C_BIT_RATE
#  define I2C_SET_BIT_RATE TWBR = (unsigned char)I2C_BIT_RATE; \
                           TWSR = I2C_PRESCALER
# else
#  error TWI clock freq[F_TWI] is too high for selected CPU clock freq[F_CPU]
# endif
#else
#error CPU freq must be at least 16x of TWI freq
#endif 
#endif //defined(F_TWI)
/*~~~~END I2C Bit Rate Calculator~~~~*/


/// Holds the status of the I2C state machine.
enum I2C_STATUS_ENUM
{
    I2C_OK = 0x00,               ///< Transaction completed succesfully
	I2C_NOACK = 0x01,			  ///< Transaction failed no ACK
    I2C_ERROR = 0x02,            ///< Transaction failed
    I2C_BUSY = 0x03,             ///< Transaction in progress
    I2C_TIMEOUT  = 0x04,  ///< Transaction failed
    I2C_LOST_ARB = 0x08,  ///< Transaction failed
    I2C_NO_RBUF  = 0x10,  ///< I2cRxBuff not initalized 
    I2C_CHKSUM_ERR = 0x20 ///< Transaction failed
};
/// Used for declaring a variable that holds the I2C Transaction status
typedef enum I2C_STATUS_ENUM I2CSTAT_T;

#define I2C_ADDRESS_IN_ARRAY 0xFF  ///< See i2c_read()
#define I2C_GEN_CALL_ADDRESS 0x00  ///< General Call Address

/// \brief [7]=128 [6]=64 [5]=32 [4]=16 [3]=8 [2]=4 [1]=2 [0]=invalid
#define I2C_RX_RING_BUFFER_BIT 5   ///< Valid defines: 1-7
#define I2C_RX_RING_BUFFER_SIZE (1<<I2C_RX_RING_BUFFER_BIT) //-Must be 2^n
#define I2C_DEV_ARRAY_SIZE 15      ///< Size of #I2cDevArray[]
#define SMBUS_ARA 0x0C             ///< SMBUS addressing mode
#define READ_I2C_DATA  0x01      //-XOR with 8-bit
#define WRITE_I2C_DATA 0xFE      //-AND with 8-bit
#define I2C_TIMEOUT_VALUE 0x80     ///< uses bitwise-AND

/*
 *  SFR's
 */
//#pragma GCC poison TWBCSR TWHSR

#if defined(TWDR) && defined(TWSR)
# define I2DAT TWDR                      /// SFR: I2C data
# define I2STAT (TWSR & 0xF8)            /// Macro: masks prescaler bits in SFR
#else
# error TWDR and TWSR must be defined
#endif

/*
 *  Macros
 */
#define i2c_clr_intrpt_flag_() TWCR |= 0x80 ///< Clear flag
#define i2c_enable_()          TWCR |= 0x04 ///< Enable I2C module
#define i2c_disable_()         TWCR &= 0xF9 ///< Disable I2C module (use as regular I/O pins)
#define i2c_start_tx_()        TWCR |= 0xA5 ///< Sends the start command
#define i2c_no_start_()        TWCR &= 0x5D ///< Keep a repeated start from happening
#define i2c_stop_tx_()         TWCR |= 0x10 ///< Sends a stop command
#define i2c_ack_()             TWCR |= 0x40 ///< Sends an acknowldege
#define i2c_nack_()            TWCR &= 0xBF ///< Sends a Not acknowldeged
#define i2c_enable_intrpt_()   TWCR |= 0x01 ///< Enable I2C interrupts
#define i2c_disable_intrpt_()  TWCR &= 0xFE ///< Disable I2C interrupts
#define i2c_enable_gc_()       TWAR |= 0x01 ///< Enable general call addressing
#define i2c_disable_gc_()      TWAR &= 0xFE ///< Disable general call addressing

#define i2c_set_address_(x)    TWAR = (x & 0xFE) ///< Pass in the 8-bit HW slave address

/// Safe increment of i2c ring pointer
#define i2c_rx_ring_next_()                                               \
           if ((I2cRxBuff.head-I2cRxBuff.dptr) & I2C_RX_RING_BUFFER_SIZE) \
                I2cRxBuff.head = I2cRxBuff.dptr;                          \
           else                                                           \
                ++I2cRxBuff.head

#ifndef F_TWI
#error F_TWI needs to be defined prior to including avr-i2c.h
#endif
/**
 *  I2C initialization macro.\n
 * F_TWI needs to be defined first. For example: -DF_TWI=100000UL
 */
#define i2c_init_(cntrl)  TWCR=(cntrl & 0x45); I2C_SET_BIT_RATE 


/*
 *  Structure definitions for common I2C device access formats
 */ 
/// Used to write a data byte to the register of the slave device
struct i2c_data_struct_t
{
    uint8_t addr; ///< I2C address to write to
    uint8_t reg;  ///< Register of slave device to access
    uint8_t data; ///< Data to write in register
};
struct i2c_data2_struct_t
{
	uint8_t addr; ///< I2C address to write to
	uint8_t reg;  ///< Register of slave device to access
	uint8_t datah; ///< Data to write in register
	uint8_t datal; ///< Data to write in register
};
struct i2c_data4_struct_t
{
	uint8_t addr; ///< I2C address to write to
	uint8_t reg;  ///< Register of slave device to access
	uint8_t datah; ///< Data to write in register
	uint8_t datal; ///< Data to write in register
	uint8_t data2h; ///< Data to write in register
	uint8_t data2l; ///< Data to write in register
};


typedef struct i2c_data_struct_t i2c_data_t;
typedef struct i2c_data2_struct_t i2c_data2_t;
typedef struct i2c_data4_struct_t i2c_data4_t;

/*
 *  Global functions and variables
 */
/// Search for \b num of devices beginning at \b start
uint8_t i2c_dev_search (uint8_t start, uint8_t num) __attribute__ ((warn_unused_result));

/// Used by the ISR to update the hardware address
extern uint8_t i2c_get_address (void) __attribute__ ((weak));



/// A master write of \b n number of bytes from data in \b *Data
extern uint8_t i2c_write ( void *const data, const uint8_t n);

/// A master read from \b DeviceAddress of \b n number of bytes to \b *ReturnData 
extern uint8_t i2c_read ( uint8_t DeviceAddress, void *const ReturnData, const uint8_t n);

extern volatile uint8_t *I2cDataPtr;
/// Global variable holds the status of the I2C module
extern volatile I2CSTAT_T I2cStatus;
extern uint8_t I2cDevArray[I2C_DEV_ARRAY_SIZE];

/// Variable holds 8-bit I2C address 
extern volatile uint8_t I2cOwnAddress;          

// Pointer to the transmit ring buffer used by the I2C module 
extern volatile rbuff_t I2cTxBuff;
/// Pointer to the receive ring buffer used by the I2C module
extern volatile rbuff_t I2cRxBuff;

#endif /*------------------------- I2C_DAKARA_H -------------------------*/
