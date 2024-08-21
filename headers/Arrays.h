
#ifndef MY_ARREYS_H
#define MY_ARREYS_H

#include <stdio.h>
#include <stdlib.h>


#define BIT_FIELD_SIZE 14

/* Structure representing a 14-bit bitfield*/
typedef struct {
    unsigned int data : BIT_FIELD_SIZE;
} bit_field;


void print_bits(bit_field *array, int size) ;

/**
 * Set a specific bit in a bitfield.
 *
 * @param bitfield Pointer to the bitfield structure.
 * @param pos      Position of the bit to set (0-indexed).
 * @param value    Value to set the bit to (0 or 1).
 * @return         None.
 */
void set_bit(bit_field *bitfield, int pos, int value);






#endif /* MY_ARREYS_H */

