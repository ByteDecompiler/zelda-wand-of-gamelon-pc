#include "game.h"

void get_sprite_block(long x, long y, struct block_info *info)
{
    long block_type;

    block_type = block_map[y * map_width + x];
    info->floor = 0;
    info->step = 0;
    info->solid = 0;
    info->left = 0;
    info->right = 0;
    info->deadly = 0;
    info->f12 = 0;
    info->f13 = 0;
    info->f14 = 0;
    info->f15 = 0;
    info->climb_a = 0;
    info->climb_b = 0;
    info->door = 0;
    info->ground = 0;
    info->f19 = 0;
    info->f1b = 0;
    info->f1c = 0;
    info->f1d = 0;
    info->track = 0;
    switch (block_type) {
    case 1:
        info->step = 1;
        info->floor = 1;
        break;
    case 57:
        info->solid = 1;
        break;
    case 37:
        info->solid = 1;
        info->left = 1;
        break;
    case 86:
        info->solid = 1;
        info->right = 1;
        break;
    case 82:
        info->deadly = 1;
        break;
    case 48:
        info->climb_a = 1;
        break;
    case 14:
        info->f12 = 1;
        info->floor = 1;
        break;
    case 31:
        info->f13 = 1;
        info->floor = 1;
        break;
    case 8:
        info->f14 = 1;
        info->floor = 1;
        break;
    case 5:
        info->f15 = 1;
        info->floor = 1;
        break;
    case 25:
        info->climb_b = 1;
        break;
    case 77:
    case 79:
    case 84:
    case 89:
    case 91:
    case 94:
        info->door = 1;
        break;
    case 12:
        if (info != &jump_info
                || old_get_sprite_block(x - 1, y - 1) != 0x35 && old_get_sprite_block(x - 1, y - 1) != 0x5d
                && old_get_sprite_block(x - 1, y - 1) != 0x14 && old_get_sprite_block(x - 1, y - 1) != 3
                && old_get_sprite_block(x - 1, y) != 0x35 && old_get_sprite_block(x - 1, y) != 0x5d
                && old_get_sprite_block(x - 1, y) != 0x14 && old_get_sprite_block(x - 1, y) != 3
                && old_get_sprite_block(x - 1, y + 1) != 0x35 && old_get_sprite_block(x - 1, y + 1) != 0x5d
                && old_get_sprite_block(x - 1, y + 1) != 0x14 && old_get_sprite_block(x - 1, y + 1) != 3
                && old_get_sprite_block(x + 1, y - 1) != 0x35 && old_get_sprite_block(x + 1, y - 1) != 0x5d
                && old_get_sprite_block(x + 1, y - 1) != 0x14 && old_get_sprite_block(x + 1, y - 1) != 3
                && old_get_sprite_block(x + 1, y) != 0x35 && old_get_sprite_block(x + 1, y) != 0x5d
                && old_get_sprite_block(x + 1, y) != 0x14 && old_get_sprite_block(x + 1, y) != 3
                && old_get_sprite_block(x + 1, y + 1) != 0x35 && old_get_sprite_block(x + 1, y + 1) != 0x5d
                && old_get_sprite_block(x + 1, y + 1) != 0x14 && old_get_sprite_block(x + 1, y + 1) != 3)
            info->track = 1;
        break;
    case 74:
        info->f19 = 1;
        info->step = 1;
        info->floor = 1;
        break;
    case 53:
        info->f1b = 1;
        break;
    case 93:
        info->f1c = 1;
        break;
    case 20:
        info->f1d = 1;
        break;
    case 28:
        info->step = 1;
        info->floor = 1;
        break;
    case 3:
        if (info == &jump_info) {
            info->step = 1;
            info->floor = 1;
        }
        break;
    default:
        info->ground = 1;
    }
}
