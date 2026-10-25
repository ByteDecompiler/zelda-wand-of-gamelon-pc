#include "game.h"

void set_new_map(long spot_x, long spot_y, long spot_w, long spot_h)
{
    map_y = 0;
    map_x = 0;
    map_cursor_x = (short)spot_x + (short)spot_w / 2;
    if (map_cursor_x >= 0xc0) {
        map_x = map_cursor_x - 0xc0;
        map_cursor_x = 0xc0;
        if (map_x >= 0x140) {
            map_cursor_x += map_x - 0x140;
            map_x = 0x140;
        }
    }
    map_cursor_y = (short)spot_y + (short)spot_h / 2;
    if (map_cursor_y >= 0x78) {
        map_y = map_cursor_y - 0x78;
        map_cursor_y = 0x78;
        if (map_y >= 0xc8) {
            map_cursor_y += map_y - 0xc8;
            map_y = 0xc8;
        }
    }
    if (map_x % 2)
        --map_x;
    if (map_y % 2)
        --map_y;
    if (map_cursor_x % 2)
        --map_cursor_x;
    if (map_cursor_y % 2)
        --map_cursor_y;
    saved_map_cursor_x = map_cursor_x;
    saved_map_cursor_y = map_cursor_y;
    saved_map_x = map_x;
    saved_map_y = map_y;
}
