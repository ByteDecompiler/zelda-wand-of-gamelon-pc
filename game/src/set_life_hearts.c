#include "game.h"

void set_life_hearts(void)
{
    max_hearts = 3;
    if (check_experience(0x9f))
        max_hearts += 1;
    if (check_experience(0xa0))
        max_hearts += 1;
    if (check_experience(0xa1))
        max_hearts += 1;
}
