#include "game.h"

void wake_on_chance(long sprite_idx, struct enemy_def *wake_def)
{
    long roll;

    if (!macros[sprite_idx]->respawned) {
        if (!wake_def->chance || (roll = randomize() % (100 / wake_def->chance)) >= 10) {
            macros[sprite_idx]->sleep_timer = wake_def->sleep_time;
            return;
        }
    } else {
        if (!wake_def->chance2 || (roll = randomize() % (100 / wake_def->chance2)) >= 10) {
            macros[sprite_idx]->sleep_timer = wake_def->sleep_time2;
            return;
        }
    }
    sprite_asleep[sprite_idx] = 0;
    sprite_ptrs[sprite_idx] = (char *)1;
}
