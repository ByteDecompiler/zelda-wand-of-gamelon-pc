#include "game.h"

void init_type_4(long sprite_no, struct macro_def *def)
{
    register struct macro_def *def_reg;

    def_reg = def;
    macros[sprite_no]->lo = def_reg->start_state;
    macros[sprite_no]->hi = 1;
    if (macros[sprite_no]->lo & macros[sprite_no]->hi)
        macros[sprite_no]->state = 6;
    else
        macros[sprite_no]->state = 2;
    init_1_or_4(sprite_no);
    macros[sprite_no]->hi <<= 1;
}
