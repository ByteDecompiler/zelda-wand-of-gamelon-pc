#include "game.h"

void reset_pcl(struct pcl *pcl, char ctrl, char *buf, long cnt)
{
    pcl->ctrl = ctrl;
    pcl->buf = (long)buf;
    pcl->cnt = cnt;
}
