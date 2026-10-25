#include "game.h"

char attack_state[21];

void check_enemy_combat(void)
{
    char *enemy_state;
    char *box_dir;
    struct enemy *edata;
    short *box;
    long ddx;
    long ddy;
    long sx;
    long sy;
    long tx;
    long ty;
    long zelda_x_save;
    long zelda_y_save;
    long combat;
    long i;
    long vx;
    long dist;
    long vy;
    struct port_be_shorts *cparams_be;
#define cparams (cparams_be->v)

    if ((unsigned char)freeze_timer > 0)
        --freeze_timer;
    for (i = 1; i < 21; ++i) {
        if (!i)
            continue;
        edata = (struct enemy *)(enemy_data + (i - 1) * 0xb0);
        if (sprite_gone[i]) {
            if (edata->item > 0)
                leave_item(edata->item, (&zelda_sprite_x)[i], (&zelda_sprite_y)[i]);
            if (edata->kind == 3)
                check_for_restart(i);
            else if (edata->kind == 1 || edata->kind == 2)
                mark_experience(edata->experience);
            sprite_gone[i] = 0;
        }
        if (!sprite_ptrs[i])
            continue;
        enemy_state = (char *)(enemy_data + (i - 1) * 0xb0 + 0x50);
        box_dir = &enemy_state[1];
        box = (short *)&enemy_state[4];
        cparams_be = (struct port_be_shorts *)&enemy_state[0x1c];
        zelda_x_save = zelda_sprite_x;
        zelda_y_save = zelda_sprite_y;
        combat = (unsigned char)in_combat;
        switch ((long)(unsigned char)*enemy_state) {
        case 1:
            break;
        case 2:
            if (cparams[12] < (&damage_taken)[i]) {
                kill_sprite(i);
                break;
            }
            if (cparams[5] == 999) {
                dist = zelda_sprite_x + pf_x - (&zelda_sprite_x)[i];
                if (!check_combat_box(&box[0], (unsigned char)*box_dir, i, macros[i]->state) || dist < 0 && dist > 5 - cparams[2]
                        || dist >= 0 && dist < cparams[2] - 5) {
                    if ((unsigned char)attack_state[i] > 0) {
                        if (macros[i]->alt_active)
                            end_alternate_macro(i);
                        attack_state[i] = 0;
                    }
                } else if (!attack_state[i]) {
                    attack_state[i] = 1;
                    cparams[31] = 0;
                    cparams[32] = cparams[9];
                    cparams[33] = 0;
                }
            } else if (!check_combat_box(&box[0], (unsigned char)*box_dir, i, macros[i]->state)) {
                if ((unsigned char)attack_state[i] > 0 && !jump_rising && !jump_falling) {
                    if (macros[i]->alt_active)
                        end_alternate_macro(i);
                    attack_state[i] = 0;
                }
            } else if (!attack_state[i]) {
                attack_state[i] = 1;
                cparams[31] = 0;
                cparams[32] = cparams[9];
                cparams[33] = 0;
            }
            switch ((unsigned char)attack_state[i]) {
            case 0:
                break;
            case 1:
                if (cparams[32] > 0) {
                    --cparams[32];
                    break;
                }
                cparams[29] = (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x ? 6 : 2;
                attack_state[i] = 2;
                cparams[31] = cparams[1];
                cparams[32] = cparams[10];
                cparams[33] = cparams[11];
                set_alternate_macro(i, cparams[13], cparams[19], cparams[29], cparams[16]);
                do_alternate_macro(i);
                break;
            case 2:
                if (macros[i]->alt_active)
                    break;
                if (cparams[5] < 998)
                    init_projectile(sprite_gfx[i] + 1,
                                    cparams[29] == 6 ? (&zelda_sprite_x)[i] + (10 - cparams[2]) : (&zelda_sprite_x)[i] + cparams[2],
                                    (&zelda_sprite_y)[i] + cparams[3], cparams[29] == 6 ? -cparams[4] : cparams[4], -cparams[5], cparams[6], 1, 0, 0, cparams[7],
                                    cparams[8]);
                else {
                    sx = cparams[29] == 6 ? (&zelda_sprite_x)[i] + (10 - cparams[2]) : (&zelda_sprite_x)[i] + cparams[2];
                    tx = zelda_sprite_x + pf_x + 5;
                    sy = (&zelda_sprite_y)[i] + cparams[3];
                    ty = zelda_sprite_y + pf_y + 6;
                    ddx = (tx - sx) * 8;
                    ddy = (ty - sy) * 8;
                    if (cparams[4] > 0) {
                        if (!(dist = (ddx > 0 ? ddx : -ddx) / cparams[4]))
                            dist = 1;
                        vx = cparams[4];
                        vy = ddy / dist - cparams[6] * (dist + 1) / 2;
                        if (vy > vx) {
                            if (cparams[5] == 998)
                                vx = vx * cparams[4] / vy;
                            vy = cparams[4];
                        }
                        if (vy < -vx) {
                            if (cparams[5] == 998)
                                vx = vx * -cparams[4] / vy;
                            vy = -cparams[4];
                        }
                        init_projectile(sprite_gfx[i] + 1,
                                        cparams[29] == 6 ? (&zelda_sprite_x)[i] + (10 - cparams[2]) : (&zelda_sprite_x)[i] + cparams[2],
                                        (&zelda_sprite_y)[i] + cparams[3], cparams[29] == 6 ? -vx : vx, vy, cparams[6], 1, 0, 0, cparams[7], cparams[8]);
                    } else
                        attack_state[i] = 0;
                }
                if (attack_state[i] == 2) {
                    set_alternate_macro(i, cparams[14], cparams[20], cparams[29], cparams[17]);
                    do_alternate_macro(i);
                    attack_state[i] = 3;
                    if (--cparams[31] > 0)
                        cparams[32] = cparams[10];
                    else
                        cparams[32] = 0;
                }
                break;
            case 3:
                if (cparams[32] > 0) {
                    --cparams[32];
                    break;
                }
                if (macros[i]->alt_active)
                    break;
                if (cparams[31] > 0) {
                    attack_state[i] = 2;
                    set_alternate_macro(i, cparams[13], cparams[19], cparams[29], cparams[16]);
                    do_alternate_macro(i);
                    break;
                }
                attack_state[i] = 4;
                cparams[33] = cparams[11];
                set_alternate_macro(i, cparams[15], cparams[21], cparams[29], cparams[18]);
                do_alternate_macro(i);
                break;
            case 4:
                if (cparams[33] > 0) {
                    --cparams[33];
                    break;
                }
                if (macros[i]->alt_active)
                    break;
                cparams[29] = (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x ? 6 : 2;
                attack_state[i] = 2;
                cparams[31] = cparams[1];
                set_alternate_macro(i, cparams[13], cparams[19], cparams[29], cparams[16]);
                do_alternate_macro(i);
                break;
            }
            break;
        case 3:
            if (cparams[3] < (&damage_taken)[i]) {
                kill_sprite(i);
                break;
            }
            if (!check_combat_box(&box[0], (unsigned char)*box_dir, i, macros[i]->state)) {
                if ((unsigned char)attack_state[i] > 0 && !jump_rising && !jump_falling) {
                    randomize();
                    set_walker(i, random_seed % ((short)macros[i]->hi - (short)macros[i]->lo)
                                      + (short)macros[i]->lo);
                    if (macros[i]->alt_active)
                        end_alternate_macro(i);
                    attack_state[i] = 0;
                }
            } else if (!attack_state[i]) {
                attack_state[i] = 1;
                cparams[32] = cparams[1];
            }
            if (attack_state[i] == 1) {
                if (cparams[32] > 0) {
                    --cparams[32];
                    break;
                }
                cparams[29] = (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x ? 6 : 2;
                attack_state[i] = 2;
                if (cparams[29] == 6)
                    set_walker(i, get_abs_x((unsigned char)*box_dir, box[0], (&zelda_sprite_x)[i], cparams[29]));
                else
                    set_walker(i, get_abs_x((unsigned char)*box_dir, box[2], (&zelda_sprite_x)[i], cparams[29]));
            } else if (attack_state[i] == 2) {
                if (macros[i]->alt_active)
                    break;
                attack_damage[i] = 0;
                if (cparams[32] > 0) {
                    --cparams[32];
                    if ((&zelda_sprite_x)[i] != (&saved_zelda_x)[i] || (&zelda_sprite_y)[i] != (&saved_zelda_y)[i])
                        move_1_block(i, cparams[29] == 6 ? 1 : -1, 1);
                    move_1_block(i, cparams[29] == 6 ? 1 : -1, 1);
                    if ((&zelda_sprite_x)[i] == (&saved_zelda_x)[i] && (&zelda_sprite_y)[i] == (&saved_zelda_y)[i]
                            && !macros[i]->alt_active && cparams[32] > 0)
                        cparams[32] = 1;
                    break;
                }
                cparams[29] = (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x ? 6 : 2;
                if (cparams[29] == 6)
                    set_walker(i, get_abs_x((unsigned char)*box_dir, box[0], (&zelda_sprite_x)[i], cparams[29]));
                else
                    set_walker(i, get_abs_x((unsigned char)*box_dir, box[2], (&zelda_sprite_x)[i], cparams[29]));
                if (check_combat_box(&box[4], (unsigned char)box_dir[1], i, cparams[29])) {
                    attack_damage[i] = ((char *)cparams)[1];
                    knockback[i] = (cparams[29] == 6 ? -1 : 1) * cparams[7];
                    cparams[32] = cparams[2];
                    set_alternate_macro(i, cparams[4], cparams[6], cparams[29], cparams[5]);
                    do_alternate_macro(i);
                }
            }
            break;
        case 4:
            if (cparams[2] < (&damage_taken)[i]) {
                kill_sprite(i);
                break;
            }
            if (freeze_timer) {
                attack_damage[i] = 0;
                break;
            }
            if (bread_timer) {
                in_combat = 0;
                zelda_sprite_x = bread_x - (pf_x + 5);
                zelda_sprite_y = bread_y - (pf_y - 3);
                if (box_dir[1])
                    zelda_sprite_y += box[5] - 14;
            }
            switch ((unsigned char)attack_state[i]) {
            case 0:
                attack_state[i] = 1;
                cparams[31] = cparams[0];
                break;
            case 1:
                if (cparams[1] > 0) {
                    if (check_combat_box(&box[0], (unsigned char)*box_dir, i, (unsigned char)macros[i]->alt_active) || bread_timer) {
                        if (cparams[31] > 0)
                            --cparams[31];
                        if (!cparams[31])
                            attack_state[i] = 2;
                    } else if (!jump_rising && !jump_falling)
                        cparams[31] = cparams[0];
                } else {
                    if (cparams[31] > 0)
                        --cparams[31];
                    if (!cparams[31])
                        attack_state[i] = 2;
                }
                if (attack_state[i] == 2) {
                    cparams[26] = zelda_sprite_y + pf_y + 14;
                    cparams[29] = (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x ? 6 : 2;
                    cparams[32] = cparams[14];
                    cparams[33] = 0;
                    set_alternate_macro(i, cparams[4], cparams[6], cparams[29], cparams[5]);
                    do_alternate_macro(i);
                    if (!cparams[7])
                        attack_damage[i] = 1;
                }
                break;
            case 2:
                if (in_combat) {
                    if (cparams[15] > 0) {
                        attack_state[i] = 4;
                        set_alternate_macro(i, cparams[16], cparams[18], cparams[29], cparams[17]);
                        do_alternate_macro(i);
                    } else
                        check_for_restart(i);
                }
                if (cparams[3] > 1) {
                    if (check_combat_box(&box[4], (unsigned char)box_dir[1], i, cparams[29])) {
                        if (cparams[7] > 0) {
                            attack_state[i] = 3;
                            cparams[31] = 0;
                            attack_damage[i] = 1;
                            if (cparams[7] == 1) {
                                randomize();
                                if (!cparams[11] || random_seed % 100 < 50)
                                    set_alternate_macro(i, cparams[8], cparams[10], cparams[29], cparams[9]);
                                else
                                    set_alternate_macro(i, cparams[11], cparams[13], cparams[29], cparams[12]);
                            } else {
                                cparams[28] = (cparams[19] + cparams[33]) / 8;
                                cparams[33] = (cparams[19] + cparams[33]) % 8;
                                if (cparams[29] == 6) {
                                    sprite_pos[i][2] -= cparams[28];
                                    (&zelda_sprite_x)[i] -= cparams[28];
                                } else {
                                    sprite_pos[i][2] += cparams[28];
                                    (&zelda_sprite_x)[i] += cparams[28];
                                }
                                randomize();
                                if (cparams[29] == 2)
                                    cparams[27] = (&zelda_sprite_x)[i] + cparams[20] + random_seed % (cparams[21] + 1);
                                else
                                    cparams[27] = (&zelda_sprite_x)[i] - (cparams[20] + random_seed % (cparams[21] + 1));
                                set_alternate_macro(i, cparams[11], cparams[13], cparams[29], cparams[12]);
                            }
                            do_alternate_macro(i);
                        } else if (cparams[15] > 0) {
                            attack_state[i] = 4;
                            set_alternate_macro(i, cparams[16], cparams[18], cparams[29], cparams[17]);
                            do_alternate_macro(i);
                        } else
                            check_for_restart(i);
                        break;
                    }
                    if (((&zelda_sprite_x)[i] < zelda_sprite_x + pf_x - 48 || (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x + 48)
                            && (cparams[29] == 6 && (&zelda_sprite_x)[i] < zelda_sprite_x + pf_x
                                || cparams[29] == 2 && (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x)) {
                        cparams[29] = (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x ? 6 : 2;
                        set_alternate_macro(i, cparams[4], cparams[6], cparams[29], cparams[5]);
                        do_alternate_macro(i);
                    }
                    if (!macros[i]->alt_active) {
                        set_alternate_macro(i, cparams[4], cparams[6], cparams[29], cparams[5]);
                        do_alternate_macro(i);
                    }
                    cparams[28] = (cparams[19] + cparams[33]) / 8;
                    cparams[33] = (cparams[19] + cparams[33]) % 8;
                    if (cparams[29] == 6) {
                        sprite_pos[i][2] -= cparams[28];
                        (&zelda_sprite_x)[i] -= cparams[28];
                    } else {
                        sprite_pos[i][2] += cparams[28];
                        (&zelda_sprite_x)[i] += cparams[28];
                    }
                    if (!jump_rising && !jump_falling)
                        cparams[26] = zelda_sprite_y + pf_y + 14;
                    if (box_dir[1] && (&zelda_sprite_y)[i] + box[7] + 7 < cparams[26] || !box_dir[1] && box[7] < cparams[26]) {
                        sprite_pos[i][3] += cparams[28];
                        (&zelda_sprite_y)[i] += cparams[28];
                        if (cparams[3] == 3) {
                            set_alternate_macro(i, cparams[4], cparams[6], cparams[29], cparams[5]);
                            do_alternate_macro(i);
                        }
                    } else if (box_dir[1] && (&zelda_sprite_y)[i] + box[5] + 7 > cparams[26] || !box_dir[1] && box[5] > cparams[26]) {
                        sprite_pos[i][3] -= cparams[28];
                        (&zelda_sprite_y)[i] -= cparams[28];
                    }
                } else if (cparams[3] == 1) {
                    if (!cparams[7]) {
                        if (macros[i]->alt_active)
                            break;
                        if (cparams[15] > 0) {
                            attack_state[i] = 4;
                            set_alternate_macro(i, cparams[16], cparams[18], cparams[29], cparams[17]);
                            do_alternate_macro(i);
                        } else
                            check_for_restart(i);
                        break;
                    }
                    if (check_combat_box(&box[4], (unsigned char)box_dir[1], i, cparams[29])) {
                        attack_state[i] = 3;
                        cparams[31] = 0;
                        attack_damage[i] = 1;
                        if (cparams[7] == 1) {
                            randomize();
                            if (!cparams[11] || random_seed % 2)
                                set_alternate_macro(i, cparams[8], cparams[10], cparams[29], cparams[9]);
                            else
                                set_alternate_macro(i, cparams[11], cparams[13], cparams[29], cparams[12]);
                        } else {
                            randomize();
                            if (cparams[29] == 2)
                                cparams[27] = (&zelda_sprite_x)[i] + cparams[20] + random_seed % (cparams[21] + 1);
                            else
                                cparams[27] = (&zelda_sprite_x)[i] - (cparams[20] + random_seed % (cparams[21] + 1));
                            set_alternate_macro(i, cparams[11], cparams[13], cparams[29], cparams[12]);
                        }
                        do_alternate_macro(i);
                        break;
                    }
                    if (macros[i]->alt_active)
                        break;
                    if (cparams[15] > 0) {
                        attack_state[i] = 4;
                        set_alternate_macro(i, cparams[16], cparams[18], cparams[29], cparams[17]);
                        do_alternate_macro(i);
                    } else
                        check_for_restart(i);
                }
                break;
            case 3:
                if (cparams[7] == 1) {
                    if (macros[i]->alt_active)
                        break;
                    if (cparams[32] > 0) {
                        attack_state[i] = 2;
                        --cparams[32];
                        cparams[26] = zelda_sprite_y + pf_y + 14;
                        cparams[29] = (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x ? 6 : 2;
                        set_alternate_macro(i, cparams[4], cparams[6], cparams[29], cparams[5]);
                        do_alternate_macro(i);
                        break;
                    }
                    if (cparams[15] > 0) {
                        attack_state[i] = 4;
                        set_alternate_macro(i, cparams[16], cparams[18], cparams[29], cparams[17]);
                        do_alternate_macro(i);
                    } else
                        check_for_restart(i);
                    break;
                }
                cparams[28] = (cparams[19] + cparams[33]) / 8;
                cparams[33] = (cparams[19] + cparams[33]) % 8;
                if (!check_combat_box(&box[4], (unsigned char)box_dir[1], i, cparams[29])) {
                    cparams[31] = 0;
                    if (!cparams[32]) {
                        if (cparams[15] > 0) {
                            attack_state[i] = 4;
                            set_alternate_macro(i, cparams[16], cparams[18], cparams[29], cparams[17]);
                            do_alternate_macro(i);
                        } else
                            check_for_restart(i);
                        break;
                    }
                    if (cparams[28] < (&zelda_sprite_y)[i]
                            && (cparams[29] == 6 && (&zelda_sprite_x)[i] < zelda_sprite_x + pf_x - 9
                                || cparams[29] == 2 && (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x + 9)) {
                        sprite_pos[i][3] -= cparams[28];
                        (&zelda_sprite_y)[i] -= cparams[28];
                    }
                }
                if (!macros[i]->alt_active) {
                    if (!cparams[31])
                        set_alternate_macro(i, cparams[8], cparams[10], cparams[29], cparams[9]);
                    else
                        set_alternate_macro(i, cparams[11], cparams[13], cparams[29], cparams[12]);
                    do_alternate_macro(i);
                    cparams[31] = !cparams[31];
                }
                if (cparams[29] == 6) {
                    sprite_pos[i][2] -= cparams[28];
                    (&zelda_sprite_x)[i] -= cparams[28];
                } else {
                    sprite_pos[i][2] += cparams[28];
                    (&zelda_sprite_x)[i] += cparams[28];
                }
                if (cparams[32] > 0 && (cparams[29] == 2 && cparams[27] < (&zelda_sprite_x)[i] || cparams[29] == 6 && cparams[27] > (&zelda_sprite_x)[i])) {
                    attack_state[i] = 2;
                    --cparams[32];
                    cparams[26] = zelda_sprite_y + pf_y + 14;
                    cparams[29] = (&zelda_sprite_x)[i] > zelda_sprite_x + pf_x ? 6 : 2;
                    set_alternate_macro(i, cparams[4], cparams[6], cparams[29], cparams[5]);
                    do_alternate_macro(i);
                }
                break;
            case 4:
                if (cparams[15] == 1) {
                    if (!macros[i]->alt_active)
                        check_for_restart(i);
                    break;
                }
                if (cparams[15] > 1) {
                    if (!macros[i]->alt_active) {
                        set_alternate_macro(i, cparams[16], cparams[18], cparams[29], cparams[17]);
                        do_alternate_macro(i);
                    }
                    cparams[28] = (cparams[19] + cparams[33]) / 8;
                    cparams[33] = (cparams[19] + cparams[33]) % 8;
                    if (cparams[29] == 6) {
                        sprite_pos[i][2] -= cparams[28];
                        (&zelda_sprite_x)[i] -= cparams[28];
                    } else {
                        sprite_pos[i][2] += cparams[28];
                        (&zelda_sprite_x)[i] += cparams[28];
                    }
                    if ((&zelda_sprite_x)[i] < pf_x - 9 || (&zelda_sprite_x)[i] > pf_x + 87)
                        check_for_restart(i);
                }
                break;
            }
            break;
        case 5:
            if (cparams[3] < (&damage_taken)[i]) {
                kill_sprite(i);
                break;
            }
            if (!check_combat_box(&box[0], (unsigned char)*box_dir, i, macros[i]->state)) {
                cparams[32] = cparams[1];
                attack_damage[i] = 0;
                break;
            }
            if (cparams[32] > 0) {
                --cparams[32];
                break;
            }
            if (macros[i]->alt_active)
                break;
            set_alternate_macro(i, cparams[4], cparams[6], macros[i]->state, cparams[5]);
            do_alternate_macro(i);
            cparams[32] = cparams[2];
            knockback[i] = (macros[i]->state == 6 ? -1 : 1) * cparams[7];
            attack_damage[i] = ((char *)cparams)[1];
            break;
        case 6:
            if (cparams[0] < (&damage_taken)[i])
                kill_sprite_noflash(i);
            break;
        case 7:
            if (cparams[0] < (&damage_taken)[i]) {
                kill_sprite(i);
                if (edata->kind == 1 || edata->kind == 2)
                    mark_experience(edata->experience);
                if (!cparams[1])
                    restore_sprite_blocks(sprite_pos[i][2] - 9, sprite_pos[i][3] - 1, 18,
                                          &((char *)path_blocks)[i * 18]);
                else {
                    restore_vert_blocks(sprite_pos[i][2] - 2, sprite_pos[i][3] - 13, 14, &((char *)path_blocks)[i * 18]);
                    restore_vert_blocks(sprite_pos[i][2] + 2, sprite_pos[i][3] - 13, 14, &((char *)path_blocks_r)[i * 18]);
                }
            }
            break;
        }
        zelda_sprite_x = zelda_x_save;
        zelda_sprite_y = zelda_y_save;
        in_combat = combat;
    }
}
