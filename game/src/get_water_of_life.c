#include "game.h"

void get_water_of_life(void)
{
    if (have_water) {
        water_used = 1;
        if (selected_item == 12)
            selected_item = 11;
    }
    damage_taken = 0;
}
