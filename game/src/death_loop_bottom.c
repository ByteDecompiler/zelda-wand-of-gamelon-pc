#include "game.h"

void death_loop_bottom(void)
{
    draw_filled_rect(DEATH_ERASE_X, 88, DEATH_ERASE_X_END, 144, plane_a_mem + death_y, 1);
    saved_zelda_y = zelda_sprite_y;
    saved_zelda_x = zelda_sprite_x;
    erase_death_number(death_y);
    deaths_shown = deaths;
}
