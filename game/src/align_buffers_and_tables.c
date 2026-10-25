#include "game.h"

void align_buffers_and_tables(void)
{
    macro_data = (char *)plane_a_extra;
    room_gfx = (long)macro_data + 0x800;
    tool_imagery_buf = room_gfx + 0x14000;
    palette_file = (char *)(room_gfx + ROOM_PALETTE_OFFSET);
    sfx_data = (long)palette_file + 0x800;
    ctl_tables = plane_a_data;
    line_table = (long *)(ctl_tables + 0x202);
    lct_a = (long *)((long)line_table + 0x3c0);
    lct_b = (long *)((long)lct_a + 0x3c0);
    sprite_palette = (struct sprite_palette *)((long)lct_b + 0xb40);
    menu_icons_buf = (long)sprite_palette + 0x208;
    proj_gfx_buf = menu_icons_buf + MENU_ICONS_SIZE;
    label_save_buf = menu_icons_buf + MENU_ICONS_SIZE;
    proj_cmap_base = proj_gfx_buf + 0x5000;
    proj_tables = proj_cmap_base + 0x570;
    zelda_imagery = proj_tables + 0xc00;
    zelda_cmap = zelda_imagery + 0x6000;
    sfx4_data = zelda_cmap + 0x570;
    block_map = (unsigned char *)plane_b_data;
    enemy_cmaps = (long)block_map + 0x4800;
    env_proj_buf = enemy_cmaps + 0x1050;
    sprite_buffer = env_proj_buf + 0x800;
    enemy_list = sprite_buffer + 0x1000;
    enemy_data = enemy_list + 32;
    path_blocks = enemy_list + 0x1000;
    path_blocks_r = path_blocks + 0x17a;
    frame_color_hi = path_blocks_r + 0x17a;
    frame_color_lo = frame_color_hi + 0x1f8;
    init_sprite_buffer_addresses();
    init_cmap_addresses();
    init_table_widths();
}
