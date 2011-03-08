
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "liuliu.h"

fb_info fb_inf;

void view_one(char a, char *p[],int t)
{

	if(a == 'a') {
		display_big(p[t*4], fb_inf);	
	}
	if(a == 'b') {
		display_big(p[t*4+1], fb_inf);	
	}
	if(a == 'c') {
		display_big(p[t*4+2], fb_inf);	
	}
	if(a == 'd') {
		display_big(p[t*4+3], fb_inf);	
	}
}

/* view function */
int main(int argc, char *argv[]) //view function
{
	int i = 0, flag = 1;
	int close = 0;
	char a, w;
	char *pict[] = {"1.jpg","2.jpg","3.jpg","4.jpg","5.jpg","6.jpg","7.jpg","8.jpg"};

	if (init_fb(&fb_inf) < 0){
		fprintf(stderr, "Error initial framebuffer\n");
		return 1;
	}

//	display_button(fb_inf);
	display_jpeg("vie_back1.jpg",fb_inf);
	//display_mp3button("vie_back1.jpg",fb_inf);
	display_view(pict[0], pict[1], pict[2], pict[3], fb_inf);

	while(1)
	{

		scanf("%c",&a);
		getchar();			//get the enter flag

		if ( a == 'n' || a == 'l' || a == 'q') {
			w = a;	
			flag = 0;
		} else {
			flag = 1;
			close ++;
		}

		if (w == 'n' && flag == 0 && close%2 == 0) {
			i++; i = i%2;		// %2 = together 2 pages
			display_jpeg("vie_back1.jpg",fb_inf);
			display_mp3button("vie_back2.jpg",fb_inf,27,166);
			display_view(pict[i*4], pict[i*4+1], pict[i*4+2], pict[i*4+3], fb_inf);
		}
		if (w == 'l' && flag == 0 && close%2 == 0) {
			i++; i = i%2;
			display_jpeg("vie_back1.jpg",fb_inf);
			display_mp3button("vie_back2.jpg",fb_inf,276,443);
			display_view(pict[i*4], pict[i*4+1], pict[i*4+2], pict[i*4+3], fb_inf);
		
		}

		if (w == 'q') {
			display_jpeg("vie_back1.jpg",fb_inf);
			display_mp3button("vie_back2.jpg",fb_inf,552,720);
		//	sleep(1);
			break;
		}

		if(flag == 1 && close%2 == 1) {
			view_one(a,pict,i);
		}
		if(flag == 1 && close%2 == 0) {
			display_view(pict[i*4], pict[i*4+1], pict[i*4+2], pict[i*4+3], fb_inf);
		}
	}

	munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);

	return 0;
}

