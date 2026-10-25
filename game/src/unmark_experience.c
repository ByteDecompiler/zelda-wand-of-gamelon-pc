#include "game.h"

void unmark_experience(long exp_no)
{
    long byte, bit;

    byte = exp_no / 8;
    bit = exp_no % 8;
    experiences[byte] &= ~(1 << bit);
}
