#include "game.h"

void hero_hit(long damage)
{
    damage_taken += damage;
    if (max_hearts * 2 <= damage_taken) {
        if (!(cheats & 1)) {
            damage_taken = max_hearts * 2;
            kill_sprite(0);
        } else
            damage_taken = 0;
    }
}
