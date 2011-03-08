
//#ifndef __LIULIU_H__
//#define __LIULIU_H__

#include "common.h"


/* what i need h files */


/* ******************** disp_view.c ******************** */
/* display view windou */
extern int display_view(char *picture1,char *picture2, char *picture3, char *picture4, fb_info fb_inf);


/* ******************** disp_button.c ******************** */
/* display view windou */
extern int display_button(fb_info fb_inf);


/* ******************** disp_big.c ******************** */
/* display view windou with big picture */
extern int display_big(char *picture,fb_info fb_inf);


/* ******************** disp_jpegl.c ******************** */
/* display picture in some way */
extern int display_jpegl(char *picture,fb_info fb_inf);


