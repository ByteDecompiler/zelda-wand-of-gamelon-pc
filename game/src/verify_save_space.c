#include "game.h"

char file_name[16];
char host_name[18];

void verify_save_space(void)
{
    long i;
    long nvr_file;
    long avail;
    long size;

    slot_used[3] = 1;
    slot_used[2] = 1;
    slot_used[1] = 1;
    slot_used[0] = 1;
    save_flags[3] = 0;
    save_flags[2] = 0;
    save_flags[1] = 0;
    save_flags[0] = 0;
    for (i = 0; i < 3; ++i) {
        create_file_and_host_names(i + 1);
        if (!(nvr_file = (long)nvr_open(file_name, host_name, 0x81))) {
            save_flags[i] = 1;
            slot_used[i] = 0;
        }
        nvr_close_free((struct file_pos *)nvr_file);
    }
    size = 0x63;
    avail = nvr_avail();
    if (avail > (size + 0x80) * 3) {
        slot_used[2] = 1;
        slot_used[1] = 1;
        slot_used[0] = 1;
    } else if (save_flags[0] || save_flags[1] || save_flags[2]) {
        if (save_flags[0] == 1 && save_flags[1] == 1 && save_flags[2] == 1) {
            if (avail >= (size + 0x80) * 2) {
                slot_used[1] = 1;
                slot_used[0] = 1;
            } else if (avail >= size + 0x80)
                slot_used[0] = 1;
            else
                mm_continue = 1;
        } else if (!save_flags[0] && !save_flags[1] || !save_flags[0] && !save_flags[2]
                   || !save_flags[1] && !save_flags[2]) {
            if (avail >= size + 0x80) {
                slot_used[2] = 1;
                slot_used[1] = 1;
                slot_used[0] = 1;
            }
        } else if (avail >= (size + 0x80) * 2) {
            slot_used[2] = 1;
            slot_used[1] = 1;
            slot_used[0] = 1;
        } else if (avail >= size + 0x80) {
            if (!save_flags[0])
                slot_used[1] = 1;
            else
                slot_used[0] = 1;
        }
    }
}
