#include "game.h"

void check_stick(long *event)
{
    long dx;
    long dy;

    if (music_paused) {
        retrieve_help_state(event, &pointer_buttons);
        return;
    }
    if (mkt_demo) {
        retrieve_mkt_demo(event, &pointer_buttons);
        return;
    }
    pt_coord(video_path, &pointer_buttons, &pointer_x, &pointer_y);
    pointer_buttons &= 3;
    pointer_x >>= 1;
    pointer_y >>= 1;
    dx = pointer_x - old_pointer_x;
    dy = pointer_y - old_pointer_y;
    *event = 0;
    if (dx > 0) {
        if (dy > 2)
            *event = 3;
        else if (dy < -1)
            *event = 1;
        else
            *event = 2;
    } else if (dx < 0) {
        if (dy > 2)
            *event = 5;
        else if (dy < -1)
            *event = 7;
        else
            *event = 6;
    } else if (dy > 0)
        *event = 4;
    else if (dy < 0)
        *event = 8;
    if (pointer_x > 0x16c || pointer_x < 0x14 || pointer_y > 0xdc || pointer_y < 0x14) {
        pointer_x = 0xc0;
        pointer_y = 0x78;
        pt_pos(video_path, pointer_x << 1, pointer_y << 1);
    }
    old_pointer_x = pointer_x;
    old_pointer_y = pointer_y;
}
