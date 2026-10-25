#include "game.h"

unsigned char alt_index[21];
unsigned char alt_frame_base[21];

long get_image_number(long sprite_idx)
{
    unsigned short frame_bits;

    if (macros[sprite_idx]->alt_active) {
        frame_bits = *(unsigned short *)((long)alt_macro_lists[sprite_idx] + (long)alt_index[sprite_idx] * 2);
        return (frame_bits & 0x1f) + alt_frame_base[sprite_idx];
    }
    if (sprite_idx == 0)
        return hero_actions[hero_action].frames[anim_frame_idx];
    frame_bits = sprite_macros[sprite_idx]
        ? *(unsigned short *)(sprite_macros[sprite_idx] + (&anim_frame_idx)[sprite_idx] * 2) & 0x1f : 0;
    if (macros[sprite_idx]->kind == 2)
        return frame_bits + ((struct macro_b *)macros[sprite_idx])->frame_offset;
    if (macros[sprite_idx]->kind == 5)
        return frame_bits + ((struct macro5 *)macros[sprite_idx])->frame_offset;
    if (macros[sprite_idx]->kind == 7)
        return frame_bits + ((struct macro_b *)macros[sprite_idx])->alt2_macro;
    if (macros[sprite_idx]->kind == 8)
        return frame_bits + ((struct macro_b *)macros[sprite_idx])->alt2_macro;
    return sprite_actions[sprite_idx][(&hero_action)[sprite_idx]].frames[(&anim_frame_idx)[sprite_idx]];
}
