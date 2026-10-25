#include "game.h"

void mark_experience(long exp_id)
{
    long byte, bit;

    byte = exp_id / 8;
    bit = exp_id % 8;
    experiences[byte] |= 1 << bit;
}
