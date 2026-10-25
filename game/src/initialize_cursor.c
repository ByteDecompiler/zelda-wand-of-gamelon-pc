#include "game.h"

void initialize_cursor(void)
{
    gc_def(video_path, 0, 0, 16, 16, 0, cursor_data);
    gc_col(video_path, 0x80008080);
    gc_org(video_path, 0, 0);
    gc_pos(video_path, 0, 0);
    gc_blnk(video_path, 16);
    gc_hide(video_path);
    pt_org(video_path, 0, 0);
    pt_pos(video_path, 0, 0);
}
