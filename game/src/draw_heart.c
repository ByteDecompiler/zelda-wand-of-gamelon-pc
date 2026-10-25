#include "game.h"

long heart_x[11];
void (*compiled_sprites[24])(long x, long w, long y, long *lines);

void draw_heart(short kind, short heart_no, long y)
{
    port_call_sprite((long)compiled_sprites[kind], heart_x[heart_no] * 4, heart_w * 4, plane_a_mem + y, line_table);
}
