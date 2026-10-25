#include "game.h"

void draw_shield(long where)
{
    struct menu_icons *mi;
    long image;

    mi = &menu_icons;
    if (shield_level)
        image = mi->big_shield;
    else
        image = mi->small_shield;
    draw_rle_icon(where, SWORD_SHIELD_X, 0x7e, image);
}
