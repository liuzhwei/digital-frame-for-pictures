
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "liuliu.h"


/* help function */

int main(int argc, char *argv[]) //help function
{
	fb_info fb_inf;
	char a;

	if (init_fb(&fb_inf) < 0){
		fprintf(stderr, "Error initial framebuffer\n");
		return 1;
	}

	display_jpeg("help2.jpg",fb_inf);
	while(1)
	{

		scanf("%c",&a);
		getchar();			//get the enter flag
		if (a == 'q') {
			break;
		}

	}

	munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);

	return 0;
}

