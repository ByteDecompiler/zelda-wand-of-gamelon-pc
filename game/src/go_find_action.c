#include "game.h"

void go_find_action(long who, long id)
{
    long i;

    for (i = 0; i < 15; ++i)
        if (sprite_actions[who][i].id == id)
            break;
    hero_action_of[who] = i;
}
