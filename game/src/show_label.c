#include "game.h"

long label_slots[15];

void show_label(long label_idx)
{
    long dest;
    long src;

    if (label_idx == 15)
        return;
    dest = label_slots[label_idx] * 0x914;
    src = labels[label_idx].y * 704 + labels[label_idx].x;
    draw_label_save_under(room_gfx + dest, plane_b_buf0 + src, label_save_buf, labels[label_idx].w, labels[label_idx].h);
}
