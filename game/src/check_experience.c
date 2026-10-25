#include "game.h"

long check_experience(long exp_no)
{
    long byte, bit;

    byte = exp_no / 8;
    bit = exp_no % 8;
    return experiences[byte] & (1 << bit);
}
