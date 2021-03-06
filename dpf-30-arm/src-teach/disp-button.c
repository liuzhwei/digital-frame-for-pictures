
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

/* soft yet */

int display_button(fb_info fb_inf)
{
	fb_info jpeg_inf;
	int i, j;
	
	u8_t *buf24 = decode_jpeg("button_all.jpg", &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	for(i = 0; i < fb_inf.h; ++i){
		for (j = 0; j < fb_inf.w; ++j){
			fb_pixel(fb_inf, j, i/8+(fb_inf.h*670/768), buf32[j + i * fb_inf.w]);
		}
	}

	free(buf24);
	free(scale_buf);
	free(buf32);

	return 0;
}

