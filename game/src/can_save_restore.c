#include "game.h"

long can_save_restore(char dir)
{
    long i;

    i = 1;
    if (mm_choice != 2 && mm_choice != 1)
        return 1;
    for (i = 1; mm_cursor + dir * i >= 0 && mm_cursor + dir * i <= mm_row_count; ++i)
        if (mm_choice == 2 && slot_used[mm_cursor + dir * i]
                || mm_choice == 1 && !save_flags[mm_cursor + dir * i]) {
            mm_cursor += dir * (i - 1);
            return 1;
        }
    return 0;
}
