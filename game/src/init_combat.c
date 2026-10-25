#include "game.h"

char attack_state[21];

void init_combat(void)
{
    long i;

    for (i = 1; i < 21; ++i) {
        attack_state[i] = 0;
        sprite_killed[i] = 0;
        sprite_damage[i] = 0;
        attack_damage[i] = 0;
        knockback[i] = 0;
    }
    attack_state[0] = 0;
    sprite_killed[0] = 0;
    attack_damage[0] = 0;
    knockback[0] = 0;
    freeze_timer = 0;
    init_enemy_projectiles();
}
