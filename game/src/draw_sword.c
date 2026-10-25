#include "game.h"

void draw_sword(long where)
{
    struct menu_icons *icons;
    long image;

    icons = &menu_icons;
    if (sword_level3)
        image = icons->sword3;
    else if (sword_level2)
        image = icons->sword2;
    else
        image = icons->sword1;
    draw_rle_icon(where, SWORD_SHIELD_X, 0x61, image);
}
