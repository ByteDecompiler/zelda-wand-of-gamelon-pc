#include "game.h"

long check_if_on_screen(long sprite_idx)
{
    if (sprite_ptrs[sprite_idx] && room_number >= 0x43 && room_number <= 0x4b && !item_counts[2]
            && ((char *)(enemy_data + (sprite_idx - 1) * 0xb0 + 0x50))[0] == 4)
        return 0;
    return sprite_pos[sprite_idx][2] - 9 >= pf_x - 8 && sprite_pos[sprite_idx][2] + 10 < pf_x + 0x68
        && sprite_pos[sprite_idx][3] - 14 >= pf_y && sprite_pos[sprite_idx][3] + 0 < pf_y + 60;
}
