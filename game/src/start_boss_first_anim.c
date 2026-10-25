#include "game.h"

void lantern_brightness();

void start_boss_first_anim(long room_no)
{
    if (boss_flag) {
        if (room_no == 28)
            game_won = 12;
        else if (room_no == 39)
            game_won = 21;
        else if (room_no == 45) {
            game_won = 25;
            blow_out_lantern();
            dim_lantern();
            lantern_brightness();
        } else if (room_no == 74)
            game_won = 43;
    }
}
