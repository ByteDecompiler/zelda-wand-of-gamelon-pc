#include "game.h"

short riding_box[4];
short riding_box_r[4];
long sprite_macros[21];
long check_riding_box(short *box, long riding, long sprite_no);
long move_type_5_or_6_1_block(long sprite_no, long dx);

void move_type_5(long sprite_no)
{
    long moved;
    short *box;
    long riding;
    register struct macro_def5 *def;
    long turned;
    long old_x;
    long old_y;
    unsigned short code;

    def = (struct macro_def5 *)(enemy_data + (sprite_no - 1) * 0xb0);
    turned = 0;
    restore_arkidai_path(sprite_no);
    old_x = sprite_pos[sprite_no][2];
    old_y = sprite_pos[sprite_no][3];
    macros[sprite_no]->state == 6 ? (box = riding_box) : (box = riding_box_r);
    if (riding = check_riding_box(box, 1, sprite_no))
        zelda_riding = 1;
    if (sprite_pos[sprite_no][2] == ((struct macro5 *)macros[sprite_no])->target_x) {
        turned = 1;
        if (macros[sprite_no]->state == 2) {
            sprite_draw_xs[sprite_no] = sprite_pos[sprite_no][2] - 9;
            macros[sprite_no]->state = 6;
            ((struct macro5 *)macros[sprite_no])->frame_offset = 3;
            ((struct macro5 *)macros[sprite_no])->target_x = def->lo;
        } else {
            sprite_draw_xs[sprite_no] = sprite_pos[sprite_no][2] - 8;
            macros[sprite_no]->state = 2;
            ((struct macro5 *)macros[sprite_no])->frame_offset = 0;
            ((struct macro5 *)macros[sprite_no])->target_x = def->hi;
        }
    }
    if (((struct macro5 *)macros[sprite_no])->target_x != sprite_pos[sprite_no][2]) {
        if (macros[sprite_no]->state == 6)
            moved = move_type_5_or_6_1_block(sprite_no, -1);
        else
            moved = move_type_5_or_6_1_block(sprite_no, 1);
        if (moved)
            ((struct macro5 *)macros[sprite_no])->target_x = sprite_pos[sprite_no][2];
    }
    if (riding) {
        if (block_ptrs[sprite_no]->track)
            jump_info.track = old_get_sprite_block(sprite_pos[sprite_no][2], sprite_pos[sprite_no][3] - 1);
        zelda_y += sprite_pos[sprite_no][3] - old_y;
        zelda_screen_y += sprite_pos[sprite_no][3] - old_y;
        zelda_sprite_y += sprite_pos[sprite_no][3] - old_y;
        zelda_moved = 1;
        if (turned) {
            zelda_x = sprite_pos[sprite_no][2] - (macros[sprite_no]->state == 6 ? -2 : 2);
            zelda_screen_x = zelda_x - pf_x;
            zelda_sprite_x = zelda_screen_x - (macros[sprite_no]->state == 6 ? 9 : 8);
            zelda_dir = macros[sprite_no]->state;
        } else {
            zelda_x += sprite_pos[sprite_no][2] - old_x;
            zelda_screen_x += sprite_pos[sprite_no][2] - old_x;
            zelda_sprite_x += sprite_pos[sprite_no][2] - old_x;
        }
    }
    if (macros[sprite_no]->state == 2)
        write_sprite_blocks(sprite_pos[sprite_no][2] - 8, sprite_pos[sprite_no][3] - 1, 12, &((char *)path_blocks)[sprite_no * 18], 3);
    else
        write_sprite_blocks(sprite_pos[sprite_no][2] - 3, sprite_pos[sprite_no][3] - 1, 12, &((char *)path_blocks)[sprite_no * 18], 3);
    if (old_get_sprite_block(sprite_pos[sprite_no][2], sprite_pos[sprite_no][3]) == 0x5d
            && ((struct macro5 *)macros[sprite_no])->kind != def->stop_kind) {
        if (def->stop_kind > 0) {
            sprite_macros[sprite_no] = macro_ptrs[(unsigned char)(def->stop_kind - 1)];
            (&anim_frame_idx)[sprite_no] = 0;
            ((struct macro5 *)macros[sprite_no])->loops = def->stop_loops;
            ((struct macro5 *)macros[sprite_no])->kind = def->stop_kind;
        }
    } else if (def->alt_delay) {
        if (--((struct macro5 *)macros[sprite_no])->alt_timer == 0) {
            ((struct macro5 *)macros[sprite_no])->alt_timer = def->alt_delay;
            if (def->alt_kind > 0 && def->alt_chance > 0
                    && ((struct macro5 *)macros[sprite_no])->kind != def->alt_kind
                    && ((struct macro5 *)macros[sprite_no])->kind != def->stop_kind
                    && (def->alt_chance == 10 || randomize() % (100 / def->alt_chance) < 10)) {
                sprite_macros[sprite_no] = macro_ptrs[(unsigned char)(def->alt_kind - 1)];
                (&anim_frame_idx)[sprite_no] = 0;
                ((struct macro5 *)macros[sprite_no])->loops = def->alt_loops;
                macros[sprite_no]->step_timer = def->speed;
                macros[sprite_no]->step_delay = def->speed;
                ((struct macro5 *)macros[sprite_no])->kind = def->alt_kind;
            }
        }
    }
    if (!turned && ((struct macro5 *)macros[sprite_no])->kind != def->stop_kind
            && ((struct macro5 *)macros[sprite_no])->kind != def->alt_kind)
        return;
    ++(&anim_frame_idx)[sprite_no];
    while ((code = *(unsigned short *)(sprite_macros[sprite_no] + (&anim_frame_idx)[sprite_no] * 2)) == 0xffff) {
        if (!--((struct macro5 *)macros[sprite_no])->loops) {
            ((struct macro5 *)macros[sprite_no])->loops = 0x100;
            sprite_macros[sprite_no] = macro_ptrs[(unsigned char)(def->kind - 1)];
            ((struct macro5 *)macros[sprite_no])->kind = def->kind;
        }
        (&anim_frame_idx)[sprite_no] = 0;
    }
    get_new_macro_image(sprite_no, ((struct macro5 *)macros[sprite_no])->frame_offset, code);
    if (sprite_frame_no[sprite_no] == 8 || sprite_frame_no[sprite_no] == 11)
        restore_arkidai_path(sprite_no);
}
