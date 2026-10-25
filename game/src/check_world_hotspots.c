#include "game.h"

void check_world_hotspots(void)
{
    long i;

    for (i = 0; i < 15; ++i)
        if ((door_open || areas[i]) && map_x + map_cursor_x >= labels[i].hot_x
                && map_x + map_cursor_x < labels[i].hot_x + labels[i].hot_w
                && map_y + map_cursor_y >= labels[i].hot_y
                && map_y + map_cursor_y < labels[i].hot_y + labels[i].hot_h)
            break;
    if (i < 15) {
        if (cur_label == -1) {
            cur_label = i;
            show_label(cur_label);
        } else if (labels[(short)cur_label].group != labels[i].group) {
            restore_label(cur_label);
            cur_label = i;
            show_label(cur_label);
        }
    } else if (cur_label != -1) {
        restore_label(cur_label);
        cur_label = -1;
    }
}
