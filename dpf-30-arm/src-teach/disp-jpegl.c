
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

int display_jpegl(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;//, t, a;		

//	for(t = 0; t < 1024; t++)
//	{
		for(i = 0; i < fb_inf.h; ++i){
			for (j = 0; j < fb_inf.w; ++j){
				if(i < fb_inf.h/2 && j < fb_inf.w/2)
				fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
			}
		}
//		usleep(10);
//	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
