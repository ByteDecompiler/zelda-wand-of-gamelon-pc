#include "game.h"

void win_game(void)
{
    ss_abort(music_path);
    game_won = -1;
    play_background_animation(1);
    game_started = 0;
    door_open = 0;
    initial_item_quantity();
    initial_areas();
    selected_item = 99;
    set_life_hearts();
}
