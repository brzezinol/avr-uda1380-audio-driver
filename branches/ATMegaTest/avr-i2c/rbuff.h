/**  
 *  \file   rbuff.h  
 *  \brief  Ring buffer   
 */
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
/*  
 *  Enumerations and Structure declarations
 */
/// Ring buffer pointer structure
struct rb_fifo_struct_type
{
    uint8_t * dptr;  ///< Always points to start of buffer
    uint8_t * head;  ///< Manipulated by Application FW (i.e you)
    uint8_t * tail;  ///< Manipulated by ISR
};
/// Ring buffer pointer
typedef struct rb_fifo_struct_type rbuff_t;

/*
 *  Global functions and variables
 */

/*================================================================*
                        ring_buff_init ()
 *================================================================*/
/// \param[in] *rb \b rbuff_t pointer to Initialize
/// \param[out] *ring \b *rb points to this buffer array location 
inline void ring_buff_init (volatile rbuff_t *const rb, uint8_t *const ring)
{
    //-Point to array buffer
    rb->dptr = ring;  
    rb->head = ring;
    rb->tail = ring;
}

#endif // RING_BUFFER_H