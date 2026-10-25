#include "game.h"

char alt_repeat[21];
unsigned char alt_index[21];

void do_alternate_macro(long sprite_no)
{
    unsigned short image_no;

    alt_index[sprite_no] += 1;
    while ((image_no = *(alt_macro_lists[sprite_no] + alt_index[sprite_no])) == 0xffff) {
        if (!--alt_repeat[sprite_no]) {
            end_alternate_macro(sprite_no);
            return;
        }
        alt_index[sprite_no] = 0;
    }
    get_new_macro_image(sprite_no, alt_frame_base[sprite_no], image_no);
}
