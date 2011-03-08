
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "common.h"


int display_volme(fb_info fb_inf, int lie, int num)
{
	int x1, x2, y1, y2;

	x1 = fb_inf.w*645/1024 + lie*fb_inf.w*71/1024;
	x2 = x1 +fb_inf.w*71/1024;
	y2 = fb_inf.h*426/768;
	y1 = y2 - num*17*fb_inf.h/768;

	display_mp3button("mymusic3.jpg", fb_inf, x1, x2, y1, y2);

	return 0;
}


