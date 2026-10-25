#include "game.h"

void draw_initial_death_screen(long y)
{
    draw_outlined_box(plane_a_mem + y, DEATH_BOX_X, 0x50, 0x120, DEATH_BOX_Y_END, 1, 3);
    zelda_sprite_x = DEATH_LINK_X;
    saved_zelda_x = DEATH_LINK_X;
    zelda_sprite_y = 0x16;
    saved_zelda_y = 0x16;
    draw_rle_sprite(plane_a_mem + y, 0, 0x1a0);
    font_draw(DEATH_TEXT_X, 0x68, death_text, plane_a_mem + y);
    draw_death_number(y);
}
