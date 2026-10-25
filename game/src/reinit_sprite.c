#include "game.h"

void reinit_sprite(long sprite_idx)
{
    register struct enemy *edata;

    edata = (struct enemy *)(enemy_data + (sprite_idx - 1) * 0xb0);
    macros[sprite_idx]->respawned = 1;
    macros[sprite_idx]->sleep_timer = 0;
    macros[sprite_idx]->wake_pending = 0;
    if (!edata->mode && edata->sleep > 0) {
        macros[sprite_idx]->sleep_timer = edata->sleep;
        sprite_asleep[sprite_idx] = 1;
        sprite_ptrs[sprite_idx] = 0;
        macros[sprite_idx]->in_wake_box = 0;
    } else if (edata->mode == 1) {
        sprite_asleep[sprite_idx] = 1;
        sprite_ptrs[sprite_idx] = 0;
    } else if (edata->mode == 2) {
        macros[sprite_idx]->sleep_timer = edata->sleep;
        sprite_asleep[sprite_idx] = 1;
        sprite_ptrs[sprite_idx] = 0;
    } else
        sprite_ptrs[sprite_idx] = (char *)1;
    main_init(sprite_idx, (struct macro_def *)edata);
}
