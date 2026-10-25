#include "game.h"

long demo_playfield(long room, long track_no, long load_enemies, long unused, long zelda_start_x, long zelda_start_y, long x, long y,
                    unsigned char resume, unsigned char last_room)
{
    register long i;
    register long result;

    clear_area_experiences();
    room_number = room;
    clear_416x240(plane_a_buf0 + 32);
    for (i = 0; i < 8; ++i)
        *(long *)(plane_a_buf0 + i * 4) = 0;
    clear_416x240(plane_a_buf1);
    draw_target = plane_b_buf0;
    init_playfield_size(room);
    pf_plane_b_buffer = 0;
    pf_x_px = x * 4;
    pf_y_px = y * 4;
    if (pf_x_px > pf_x_px_max)
        pf_x_px = pf_x_px_max;
    if (pf_y_px > pf_y_px_max)
        pf_y_px = pf_y_px_max;
    pf_x = pf_x_px / 4;
    pf_y = pf_y_px / 4;
    init_playfield();
    plane_A_over_B();
    if ((result = drop_sprite(track_no, load_enemies, zelda_start_x, zelda_start_y, resume, last_room)) != -2)
        stop_music();
    pf_plane_b_buffer = 1;
    return result;
}
