
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


/********  the file has soft yet ************/




int display_view(char *picture1, char *picture2, char *picture3, char *picture4, fb_info fb_inf)
{
	fb_info jpeg_inf;
	int i, j;

	/* display left_up with picture1 */
	u8_t *buf24_1 = decode_jpeg(picture1,&jpeg_inf);
	u8_t * scale_buf_1 = scale24(buf24_1, fb_inf, jpeg_inf);
	u32_t *buf32_1 = rgb24to32(scale_buf_1, fb_inf);

	for(i = 100; i < fb_inf.h-100; ++i,++i){
//	for(i = 0; i < fb_inf.h-(fb_inf.h*88/1024); ++i,++i){
		for (j = 100; j < fb_inf.w-50; ++j,++j){
//		for (j = 0; j < fb_inf.w; ++j,++j){
			fb_pixel(fb_inf, j/2, i/2-40, buf32_1[j + i * fb_inf.w]);
		}
	}

	free(buf24_1);
	free(scale_buf_1);
	free(buf32_1);

	/* display right_up with picture2 */
	u8_t *buf24_2 = decode_jpeg(picture2, &jpeg_inf);
	u8_t * scale_buf_2 = scale24(buf24_2, fb_inf, jpeg_inf);
	u32_t *buf32_2 = rgb24to32(scale_buf_2, fb_inf);

	for(i = 100; i < fb_inf.h-100; ++i,++i){
	//for(i = 0; i < fb_inf.h-(fb_inf.h*88/1024); ++i,++i){		//display for full screen
		for (j = 100; j < fb_inf.w-50; ++j,++j){
	//	for (j = 0; j < fb_inf.w; ++j,++j){					//display for full screen
			fb_pixel(fb_inf, (j+fb_inf.w)/2, i/2-40, buf32_2[j + i * fb_inf.w]);
		}
	}

	free(buf24_2);
	free(scale_buf_2);
	free(buf32_2);


	/* display lift_down with picture3 */
	u8_t *buf24_3 = decode_jpeg(picture3, &jpeg_inf);
	u8_t * scale_buf_3 = scale24(buf24_3, fb_inf, jpeg_inf);
	u32_t *buf32_3 = rgb24to32(scale_buf_3, fb_inf);

	for(i = 100; i < fb_inf.h-100; ++i,++i){
//	for(i = 0; i < fb_inf.h-(fb_inf.h*88/1024); ++i,++i){
		for (j = 100; j < fb_inf.w-50; ++j,++j){
	//	for (j = 0; j < fb_inf.w; ++j,++j){
			fb_pixel(fb_inf, j/2, (i+fb_inf.h)/2-(fb_inf.h*120/768), buf32_3[j + i * fb_inf.w]);
		}
	}

	free(buf24_3);
	free(scale_buf_3);
	free(buf32_3);


	/* display right_down with picture4 */
	u8_t *buf24_4 = decode_jpeg(picture4, &jpeg_inf);
	u8_t * scale_buf_4 = scale24(buf24_4, fb_inf, jpeg_inf);
	u32_t *buf32_4 = rgb24to32(scale_buf_4, fb_inf);

	for(i = 100; i < fb_inf.h-100; ++i,++i){
//	for(i = 0; i < fb_inf.h-(fb_inf.h*88/1024); ++i,++i){
		for (j = 100; j < fb_inf.w-50; ++j,++j){
	//	for (j = 0; j < fb_inf.w; ++j,++j){
			fb_pixel(fb_inf, (j+fb_inf.w)/2, (i+fb_inf.h)/2-(fb_inf.h*120/768), buf32_4[j + i * fb_inf.w]);
		}
	}

	free(buf24_4);
	free(scale_buf_4);
	free(buf32_4);

	return 0;
}
