#include "game.h"

void erase_death_number(long y)
{
    draw_filled_rect(DEATH_NUMBER_X, 104, 131, 131, plane_a_mem + y, 1);
}
