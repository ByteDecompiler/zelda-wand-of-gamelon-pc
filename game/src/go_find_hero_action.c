#include "game.h"

void go_find_hero_action(char action)
{
    long i;

    for (i = 0; i < 15; ++i)
        if (hero_actions[i].id == action)
            break;
    hero_action = i;
}
