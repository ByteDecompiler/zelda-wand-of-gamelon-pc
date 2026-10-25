#include "game.h"

void back_on_overworld(void)
{
    fade_up_plane_AB();
    wait_for_line_event();
    move_map();
    wait_for_line_event();
}
