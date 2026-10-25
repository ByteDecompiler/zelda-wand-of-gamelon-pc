#include "game.h"

void set_mkt_demo_off(void)
{
    mkt_demo = 0;
    sattenuation(127);
    play_overworld_music();
}
