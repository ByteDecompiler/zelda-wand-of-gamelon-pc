#include "game.h"

void get_collision_result(long high_bit, long low_bit, unsigned short *mask, long unused, long down, long flip,
                          long *hit, long *pos)
{
    long i;

    *pos = 0;
    *hit = 0;
    if (down) {
        if (flip) {
            for (i = high_bit; i >= low_bit; --i)
                if (port_be16(mask) & bit_masks[15 - i]) {
                    *hit = 1;
                    *pos = 15 - i;
                    return;
                }
        } else {
            for (i = high_bit; i >= low_bit; --i)
                if (port_be16(mask) & bit_masks[i]) {
                    *hit = 1;
                    *pos = 15 - i;
                    return;
                }
        }
    } else {
        if (flip) {
            for (i = low_bit; i <= high_bit; ++i)
                if (port_be16(mask) & bit_masks[15 - i]) {
                    *hit = 1;
                    *pos = 15 - i;
                    return;
                }
        } else {
            for (i = low_bit; i <= high_bit; ++i)
                if (port_be16(mask) & bit_masks[i]) {
                    *hit = 1;
                    *pos = 15 - i;
                    return;
                }
        }
    }
}
