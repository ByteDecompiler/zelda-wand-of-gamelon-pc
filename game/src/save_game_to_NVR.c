#include "game.h"

char host_name[18];
char file_name[16];

#ifdef NEW_SAVE_CHECK

long save_game_to_NVR(long slot)
{
    register long nvr_file;
    long result;
    long size;

    create_file_and_host_names(slot);
    save_size = 0x41;
    result = nvr_avail();
    size = save_size + 0x22;
    if (result < size + 0x80) {
        nvr_file = (long)nvr_open(file_name, host_name, 0x81);
        if (!nvr_file)
            return 99;
        nvr_close_free((struct file_pos *)nvr_file);
    }
    nvr_delete(file_name);
    nvr_file = (long)nvr_create(file_name, host_name, save_host_info, size, 0x81);
    if (!nvr_file)
        return errno;
    encode_mem_space();
    result = nvr_write((struct file_pos *)nvr_file, mem_space, size);
    if (result == -1)
        return errno;
    result = nvr_close_free((struct file_pos *)nvr_file);
    if (result == -1)
        return errno;
    save_flags[slot - 1] = 0;
    return 0;
}

#else

long save_game_to_NVR(long slot)
{
    register long result;
    long nvr_file;
    long size;

    create_file_and_host_names(slot);
    save_size = 0x41;
    nvr_delete(file_name);
    nvr_file = nvr_avail();
    size = save_size + 0x22;
    if (nvr_file < size + 0x80)
        return 99;
    nvr_file = (long)nvr_create(file_name, host_name, save_host_info, size, 0x81);
    if (!nvr_file)
        return errno;
    encode_mem_space();
    result = nvr_write((struct file_pos *)nvr_file, mem_space, size);
    if (result == -1)
        return errno;
    result = nvr_close_free((struct file_pos *)nvr_file);
    if (result == -1)
        return errno;
    save_flags[slot - 1] = 0;
    return 0;
}

#endif
