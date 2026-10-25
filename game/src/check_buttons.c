#include "game.h"

long check_buttons(void)
{
    long buttons;
    long x;
    long y;

    pt_coord(video_path, &buttons, &x, &y);
    if (buttons & 3) {
        ss_abort(anim_path);
        while (buttons & 3)
            pt_coord(video_path, &buttons, &x, &y);
        attenuation(0xff);
        return 1;
    }
    return 0;
}
