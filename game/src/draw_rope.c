#include "game.h"

void draw_rope(long y)
{
    long x;
    long top;
    long rel_row;
    long i;

    if (rope_out) {
        x = (rope_x - pf_x) * 4;
        top = pf_y;
        for (i = rope_bottom; i >= rope_top; --i) {
            rel_row = i - top;
            if (rel_row > 0)
                port_call_sprite((long)compiled_sprites[9], x, rel_row * 4, plane_a_mem + y, line_table);
        }
    }
}
