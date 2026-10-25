#include "game.h"

void set_ov_help_off(void)
{
    attenuation(0xff);
    abort_rtrs();
    set_help_off();
    pointer_buttons = 2;
    pointer_x = 0xc0;
    pointer_y = 0x78;
    pt_pos(video_path, pointer_x << 1, pointer_y << 1);
    old_pointer_x = pointer_x;
    old_pointer_y = pointer_y;
    help_ended = 1;
    if (game_started)
        sattenuation(0x7f);
    else
        sattenuation(0);
    play_overworld_music();
}
