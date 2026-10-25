#include "game.h"

void quit_game(long code)
{
    if (disc_errors == 3)
        fade_out_plane_A();
    abort_rtrs();
    wait_for_line_event();
    wait_for_line_event();
    wait_for_line_event();
    gc_hide(video_path);
    exit(code);
}
