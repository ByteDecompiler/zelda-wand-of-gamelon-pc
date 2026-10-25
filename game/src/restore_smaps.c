#include "game.h"

long smap_saved[2];
extern struct smap *smaps[100000];

void restore_smaps(void)
{
    long i;

    for (i = 0; i < 2; ++i) {
        smaps[i]->data = smap_saved[i];
        smaps[i]->size = 0x900;
        smaps[i]->w = 0x12;
        smaps[i]->pos = 0;
    }
}
