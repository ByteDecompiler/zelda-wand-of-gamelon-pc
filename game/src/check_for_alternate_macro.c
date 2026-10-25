#include "game.h"

long check_for_alternate_macro(long sprite_idx, long state)
{
    if (macros[sprite_idx]->kind == 3) {
        if (!((struct macro_alt *)macros[sprite_idx])->b2a)
            return 0;
        if (!((struct macro_alt *)macros[sprite_idx])->alt3_chance)
            return 0;
        if (((struct macro_alt *)macros[sprite_idx])->alt3_chance < 10
                && randomize() % (100 / ((struct macro_alt *)macros[sprite_idx])->alt3_chance) >= 10)
            return 0;
        if (((struct macro_alt *)macros[sprite_idx])->alt3_dir != 10)
            state = ((struct macro_alt *)macros[sprite_idx])->alt3_dir;
        if (set_alternate_macro(sprite_idx, ((struct macro_alt *)macros[sprite_idx])->b2a, ((struct macro_alt *)macros[sprite_idx])->b2b,
                                state, ((struct macro_alt *)macros[sprite_idx])->alt3_speed))
            return 0;
        do_alternate_macro(sprite_idx);
        return 1;
    } else if (macros[sprite_idx]->kind == 2) {
        if (!((struct macro_alt *)macros[sprite_idx])->alt2_macro)
            return 0;
        if (!((struct macro_alt *)macros[sprite_idx])->alt2_chance)
            return 0;
        if (((struct macro_alt *)macros[sprite_idx])->alt2_chance < 10
                && randomize() % (100 / ((struct macro_alt *)macros[sprite_idx])->alt2_chance) >= 10)
            return 0;
        if (((struct macro_alt *)macros[sprite_idx])->b2a != 10)
            state = ((struct macro_alt *)macros[sprite_idx])->b2a;
        if (set_alternate_macro(sprite_idx, ((struct macro_alt *)macros[sprite_idx])->alt2_macro, ((struct macro_alt *)macros[sprite_idx])->alt2_repeat,
                                state, ((struct macro_alt *)macros[sprite_idx])->b2b))
            return 0;
        do_alternate_macro(sprite_idx);
        return 1;
    }
}
