#include "game.h"

void retrieve_mkt_demo(long *x, long *y)
{
    if (get_input()) {
        *x = *y = 0;
        mkt_demo_end = 1;
    } else {
        mkt_demo_byte = *mkt_demo_ptr++;
        *x = mkt_demo_byte & 0x1f;
        *y = mkt_demo_byte >> 5;
    }
}
