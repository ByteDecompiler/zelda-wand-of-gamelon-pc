#include "game.h"

char initial_area_tab[15];

void initial_areas(void)
{
    long i;

    for (i = 0; i < 15; ++i)
        areas[i] = initial_area_tab[i];
}
