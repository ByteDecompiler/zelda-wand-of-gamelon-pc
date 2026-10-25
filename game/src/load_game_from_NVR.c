#include "game.h"

char file_name[16];
char host_name[16];

long load_game_from_NVR(long save_slot)
{
    long handle;
    long result;

    create_file_and_host_names(save_slot);
    if ((handle = (long)nvr_open(file_name, host_name, 0x81)) == 0) {
        nvr_close_free((struct file_pos *)handle);
        return errno;
    }
    if ((result = nvr_read((struct file_pos *)handle, mem_space, 0x2000)) == -1) {
        nvr_close_free((struct file_pos *)handle);
        return errno;
    }
    if ((result = nvr_close_free((struct file_pos *)handle)) == -1)
        return errno;
    decode_mem_space();
    return 0;
}
