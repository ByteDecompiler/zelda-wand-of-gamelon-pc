#include "game.h"

void clear_area_experiences(void)
{
    long i;

    if (area_has_exp)
        for (i = 95; i < 127; ++i)
            unmark_experience(i);
}
