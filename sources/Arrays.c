
#include "Arrays.h"
#include "Global_def.h"

void set_bit(bit_field *bitfield, int pos, int value) {
    if (value)
        bitfield->data |= (1 << pos);
    else
        bitfield->data &= ~(1 << pos);
}



