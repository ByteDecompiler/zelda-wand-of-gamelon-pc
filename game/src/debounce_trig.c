#include "game.h"

void debounce_trig(void)
{
    long buttons, x, y;

    while (pt_coord(video_path, &buttons, &x, &y), buttons &= 3)
        port_idle();
    pt_coord(video_path, &buttons, &pointer_x, &pointer_y);
    pointer_x >>= 1;
    pointer_y >>= 1;
    old_pointer_x = pointer_x;
    old_pointer_y = pointer_y;
}
