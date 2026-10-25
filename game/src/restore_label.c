#include "game.h"

void restore_label(long label_id)
{
    long buf_offset;

    if (label_id == 15)
        return;
    buf_offset = labels[label_id].y * 704 + labels[label_id].x;
    restore_under_label(plane_b_buf0 + buf_offset, label_save_buf, labels[label_id].w, labels[label_id].h);
}
