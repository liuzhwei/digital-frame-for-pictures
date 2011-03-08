

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "common.h"

#define DEBUG 0
#if DEBUG
#endif

int display_mp3button(char *filename, fb_info fb_inf,int x1, int x2, int y1, int y2)
{

	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;		
	for(i = fb_inf.h*y1/768; i < fb_inf.h*y2/768; ++i){
		for (j = fb_inf.w*x1/1024; j < fb_inf.w*x2/1024; ++j){
			fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
