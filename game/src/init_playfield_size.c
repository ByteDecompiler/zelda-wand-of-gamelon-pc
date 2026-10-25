#include "game.h"

void init_playfield_size(long room)
{
    pf_extra_screens = 0;
    pf_x_px_max = 0;
    pf_y_px_max = 0;
    pf_scrolls_horiz = 1;
    map_width = 0x60;
    map_height = 60;
    line_step = room_widths[room] * 0x180;
    line_count = room_heights[room] * 0xf0;
    if (line_step > 0x180) {
        pf_extra_screens = line_step / 0x180 - 1;
        pf_x_px_max = pf_extra_screens * 0x180;
        map_width = line_step / 4;
        pf_scrolls_horiz = 1;
    } else if (line_count > 0xf0) {
        pf_extra_screens = line_count / 0xf0 - 1;
        pf_y_px_max = pf_extra_screens * 0xf0;
        map_height = line_count / 4;
        pf_scrolls_horiz = 0;
    }
    map_width_px = line_step / 4;
    map_height_px = line_count / 4;
}
