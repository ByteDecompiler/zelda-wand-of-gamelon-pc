#include "game.h"

long check_if_was_on_screen(long sprite_index)
{
    return sprite_xs[sprite_index] >= -8 && sprite_xs[sprite_index] + 18 < 0x68 && sprite_ys[sprite_index] >= 0 && sprite_ys[sprite_index] + 14 < 0x3c;
}
