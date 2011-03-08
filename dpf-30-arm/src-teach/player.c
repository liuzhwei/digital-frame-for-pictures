
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <linux/soundcard.h>
# include <pthread.h>

# include "mad.h"
# include "common.h"

//# define _40K	40960
# define N_4K	204800


static int decode(unsigned char const *, unsigned long);
int set_dsp(void);
void check_key(void);
void play_mp3(char *);
void *key_function(void *);
void *volme_display(void *arg);

int number;
int code_lenth = 0;
int lenth = 0;
int music_num = 0;
int second = 0; 
int open_flag = 0;




fb_info fb_inf;

int i = 0; 
int fddev;

int main(int argc, char *argv[])
{
	pthread_t tid, tid2;	
	int err;

	
	system("clear");
#if 1
	if (init_fb(&fb_inf) < 0){
		fprintf(stderr, "Error initial framebuffer\n");
		return 1;
	}
	//if (init_ft("mao.ttf",0) != 0){
	if (init_ft("123.ttf",0) != 0){
		fprintf(stderr, "Error initial mao.ttf\n");
		return 1;
	}
#endif
	srand((unsigned int)time(NULL));
	
	display_jpeg("mymusic.jpg",fb_inf);
	display_mp3button("mymusic3.jpg",fb_inf,148,258,535,698);
	//display_string (const char *buf, int x, int y, fb_info fb_inf,u32_t color);
	display_string ("♬ 喝下你藏好的毒",  fb_inf.w*150/1024,  fb_inf.h*200/768, fb_inf, 0x00f7f709);
	display_string ("♬ 山楂树之恋",  fb_inf.w*150/1024,  fb_inf.h*250/768, fb_inf, 0x000909f7);
	display_string ("♬ 怒放的生命",  fb_inf.w*150/1024,  fb_inf.h*300/768, fb_inf, 0x000909f7);
	display_string ("♬ 酸酸甜甜就是我",  fb_inf.w*150/1024,  fb_inf.h*350/768, fb_inf, 0x000909f7);

	err = pthread_create(&tid,NULL,key_function,NULL);
	err = pthread_create(&tid2,NULL,volme_display,NULL);
	if(err != 0)
	{
		perror("pthread_create tid");
		exit(1);
	}
	check_key();

	return 0;
}

/*
 * in this check_key function 
 * input number 1  -->  zanting
 * input number 0  -->  play
 * input number 2  -->  exit and return main picture
 * input number 3  -->  next music
 * input number 4  -->  privew music
 */
void check_key(void)
{
	while(1)
	{
		scanf("%d",&i);
		if(i == 2)
		{
			display_jpeg("mymusic.jpg",fb_inf);
			display_mp3button("mymusic3.jpg",fb_inf,543,653,535,698);
			sleep(1);
			break;
		}
		if(i == 0)
		{
			display_mp3button("mymusic.jpg",fb_inf,277,388,535,698);
			display_mp3button("mymusic3.jpg",fb_inf,148,258,535,698);
		}
		if(i == 1)
		{
			display_mp3button("mymusic.jpg",fb_inf,148,258,535,698);
			display_mp3button("mymusic3.jpg",fb_inf,277,388,535,698);
		}
		if(i == 4)
		{
			display_mp3button("mymusic.jpg",fb_inf,18,700,585,698); 	// all white button
			display_mp3button("mymusic3.jpg",fb_inf,18,131,585,698);

			second = 0;

			sleep(1);
			display_mp3button("mymusic.jpg",fb_inf,18,131,585,698);
			display_mp3button("mymusic3.jpg",fb_inf,148,258,585,698);
		}
		if(i == 3)
		{
			display_mp3button("mymusic.jpg",fb_inf,18,700,585,698);		// all white button
			display_mp3button("mymusic3.jpg",fb_inf,410,521,585,698);

			second = 0;

			sleep(1);
			display_mp3button("mymusic.jpg",fb_inf,410,521,585,698);
			display_mp3button("mymusic3.jpg",fb_inf,148,258,585,698);
		}
		if(i == 5)
		{
			display_mp3button("mymusic3.jpg",fb_inf,141,353,353,565);		// all white button
			i = 0;
		}
		if(i == 6)
		{
			display_mp3button("mymusic.jpg",fb_inf,141,353,353,565);		// all white button
			i = 0;
		}
	}
}

void *volme_display(void *arg)
{
	int high = 0;
	char string[100];
	int  flag = 0, time = 0;
	FILE * lyric = NULL;
	char * lyric_name[] = {"./lyric1","./lyric2","./lyric3","./lyric4"};
	


	while(i != 2)
	{

	//	while(i == 0)
	//	{
			high = rand()%11;
			display_volme(fb_inf,0,high);
			high = rand()%11;
			display_volme(fb_inf,1,high);
			high = rand()%11;
			display_volme(fb_inf,2,high);
			high = rand()%11;
			display_volme(fb_inf,3,high);

			//usleep(100000 + rand()%80000);
			usleep(100000);

			display_mp3button("mymusic.jpg",fb_inf,635,925,200,410);
			display_mp3button("mymusic3.jpg",fb_inf,30,30+lenth+970*number/code_lenth,735,750);//display jindu bar
			lenth += 3;
			//if(lenth > 1000)
			//	lenth = 1000;

			/************** display rylic ************/

			if(open_flag == 2)
			{
				fclose(lyric);		
				lyric = NULL;
				open_flag = 0;
			}
			if(open_flag == 0)
			{
				lyric = fopen(lyric_name[music_num],"r");
				if(lyric == NULL)
				{
					perror("open lyric");
					exit(1);
				}
				open_flag = 1;			
				second = 0;
				flag = 0;
			}

			if (flag == 0)
			{
				fgets(string,100,lyric);
				time = (int)(string[0] - '0')*100 + (int)(string[1] - '0')*10 + (int)(string[2] - '0');
				flag = 1;
			}

			if(second == time)
			{
				flag =0;	
				display_mp3button("mymusic.jpg",fb_inf,400,1000,500,580);
				//display_string (string+3,  fb_inf.w*450/1024,  fb_inf.h*550/768, fb_inf, 0x000909f7);
				display_string (string+3,  fb_inf.w*450/1024,  fb_inf.h*550/768, fb_inf, 0x00000000);
			}
			second ++;
			//printf("second = %d\n",second);
	//	}
	}
	fclose(lyric);
	lyric = NULL;
	return NULL;
}

void *key_function(void *arg)
{
	char *music[4] = {"test1.mp3","test2.mp3","test3.mp3","test4.mp3"};

	while(1)
	{
		if(i == 2)
			break;
		if(i == 3)
		{
			music_num ++;
			if(music_num >= 4)
				music_num = 0;
			i = 0;
			display_mp3button("mymusic.jpg",fb_inf,400,1000,500,580);
			open_flag = 2;
		}
		if(i == 4)
		{
			music_num --;
			if(music_num < 0)
				music_num = 3;
			i = 0;
			display_mp3button("mymusic.jpg",fb_inf,400,1000,500,580);
			open_flag = 2;
		}
		switch(music_num)
		{
			case 0:
				display_string ("♬ 喝下你藏好的毒",  fb_inf.w*150/1024,  fb_inf.h*200/768, fb_inf, 0x00f7f709);
				display_string ("♬ 山楂树之恋",  fb_inf.w*150/1024,  fb_inf.h*250/768, fb_inf, 0x000909f7);
				display_string ("♬ 怒放的生命",  fb_inf.w*150/1024,  fb_inf.h*300/768, fb_inf, 0x000909f7);
				display_string ("♬ 酸酸甜甜就是我",  fb_inf.w*150/1024,  fb_inf.h*350/768, fb_inf, 0x000909f7);
				break;
			case 1:
				display_string ("♬ 喝下你藏好的毒",  fb_inf.w*150/1024,  fb_inf.h*200/768, fb_inf, 0x000909f7);
				display_string ("♬ 山楂树之恋",  fb_inf.w*150/1024,  fb_inf.h*250/768, fb_inf, 0x00f7f709);
				display_string ("♬ 怒放的生命",  fb_inf.w*150/1024,  fb_inf.h*300/768, fb_inf, 0x000909f7);
				display_string ("♬ 酸酸甜甜就是我",  fb_inf.w*150/1024,  fb_inf.h*350/768, fb_inf, 0x000909f7);
				break;
			case 2:
				display_string ("♬ 喝下你藏好的毒",  fb_inf.w*150/1024,  fb_inf.h*200/768, fb_inf, 0x000909f7);
				display_string ("♬ 山楂树之恋",  fb_inf.w*150/1024,  fb_inf.h*250/768, fb_inf, 0x000909f7);
				display_string ("♬ 怒放的生命",  fb_inf.w*150/1024,  fb_inf.h*300/768, fb_inf, 0x00f7f709);
				display_string ("♬ 酸酸甜甜就是我",  fb_inf.w*150/1024,  fb_inf.h*350/768, fb_inf, 0x000909f7);
				break;
			case 3:
				display_string ("♬ 喝下你藏好的毒",  fb_inf.w*150/1024,  fb_inf.h*200/768, fb_inf, 0x000909f7);
				display_string ("♬ 山楂树之恋",  fb_inf.w*150/1024,  fb_inf.h*250/768, fb_inf, 0x000909f7);
				display_string ("♬ 怒放的生命",  fb_inf.w*150/1024,  fb_inf.h*300/768, fb_inf, 0x000909f7);
				display_string ("♬ 酸酸甜甜就是我",  fb_inf.w*150/1024,  fb_inf.h*350/768, fb_inf, 0x00f7f709);
				break;
			default:
				break;
		}
	
		lenth = 0;
		display_mp3button("mymusic.jpg",fb_inf,30,1000,735,750);			//display jindu bar

		play_mp3(music[music_num]);
		if ( i == 0)
		{
			music_num ++;
			if(music_num >= 4)
				music_num = 0;
			open_flag = 2;
			display_mp3button("mymusic.jpg",fb_inf,400,1000,500,580);		//no_display lyric
		}
	}
	return NULL;
}

void play_mp3(char *music_name)
{
  	struct stat stat;
  	void *fdm;
  	int fdmp3;
	//int number;
  
	fdmp3 = open(music_name,O_RDONLY);
	if(fdmp3 < 0)
	{
		printf("open mp3 file error\n");	
		exit(1);
	}

  	if (fstat(fdmp3, &stat) == -1 ||
      stat.st_size == 0)
  	{
 		printf("fstat error\n"); 
		exit(1);
  	}

	code_lenth = (int)stat.st_size/N_4K;

	fddev = set_dsp();

	for(number = 0; number< (int)stat.st_size/N_4K; number++)
	{
	  	fdm = mmap(0, N_4K, PROT_READ, MAP_SHARED, fdmp3, number*N_4K);
  		if (fdm == MAP_FAILED)
  		{
 			perror("mmap() error\n"); 
			exit(1);
  		}

  		decode(fdm, N_4K);

		lenth = 0;

  		if (munmap(fdm, N_4K) == -1)
  		{
 			printf("munmap() error\n"); 
			exit(1);
	  	}
	}


	close(fddev);
	return ;
}

int set_dsp(void)
{
	int arg;
	int status;
	int fdmix;

  	fddev = open("/dev/dsp",O_WRONLY);
  	if(fddev < 0)
  	{
 		perror("open /dev/dsp");
		exit(1);
  	}
	fdmix = open("/dev/mixer",O_WRONLY);
	if(fdmix < 0)
	{
		perror("open /dev/mixer");	
		exit(1);
	}

	arg = AFMT_S16_LE; 		//bit
	status = ioctl(fddev,SOUND_PCM_WRITE_BITS,&arg);
	if(status < 0)
	{
		perror("ioctl dsp/bit");
		exit(1);
	}
	//arg = 2;				//chanals
	arg = 2;				//chanals
	status = ioctl(fddev,SOUND_PCM_WRITE_CHANNELS,&arg);
	if(status < 0)
	{
		perror("ioctl dsp/chanals");
		exit(1);
	}
	arg = 44100;			//rat
	status = ioctl(fddev,SOUND_PCM_WRITE_RATE,&arg);
	if(status < 0)
	{
		perror("ioctl dsp/rat");
		exit(1);
	}
	arg = 50;				//volume
	arg = (arg << 8) + 50;
	status = ioctl(fdmix,SOUND_MIXER_WRITE_VOLUME,&arg);
	if(status < 0)
	{
		perror("ioctl dsp/volume");
		exit(1);
	}
	close(fdmix);

	return fddev;
}

struct buffer {
  unsigned char const *start;
  unsigned long length;
};

static
enum mad_flow input(void *data,
		    struct mad_stream *stream)
{
  struct buffer *buffer = data;

  if (!buffer->length)
    return MAD_FLOW_STOP;

  mad_stream_buffer(stream, buffer->start, buffer->length);

  buffer->length = 0;

  return MAD_FLOW_CONTINUE;
}

static inline
signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

static
enum mad_flow output(void *data,
		     struct mad_header const *header,
		     struct mad_pcm *pcm)
{
  unsigned int nchannels, nsamples;
  mad_fixed_t const *left_ch, *right_ch;

  signed int dat;
  signed int sample;

  /* pcm->samplerate contains the sampling frequency */

  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];

  while (nsamples--) {

  		if(i != 0)
		{
			while(i==1)
				usleep(5000);
			if(i ==2 || i==3 || i==4)
			{
    			return MAD_FLOW_STOP;
				break;
			}
		}


    	/* output sample(s) in 16-bit signed little-endian PCM */

    	sample = scale(*left_ch++);
		dat = (sample >> 0) & 0xff;
		write(fddev,&dat,1);
		dat = (sample >> 8) & 0xff;
		write(fddev,&dat,1);


#if 1
    	//if (nchannels == 2) {
      	sample = scale(*right_ch++);
		dat = (sample >> 0) & 0xff;
		write(fddev,&dat,1);
		dat = (sample >> 8) & 0xff;
		write(fddev,&dat,1);
    	//}

#endif
  }

  return MAD_FLOW_CONTINUE;
}

static
enum mad_flow error(void *data,
		    struct mad_stream *stream,
		    struct mad_frame *frame)
{
  return MAD_FLOW_CONTINUE;
}

static
int decode(unsigned char const *start, unsigned long length)
{
  struct buffer buffer;
  struct mad_decoder decoder;
  int result;

  /* initialize our private message structure */

  buffer.start  = start;
  buffer.length = length;

  /* configure input, output, and error functions */
  mad_decoder_init(&decoder, &buffer,
		   input, 0 /* header */, 0 /* filter */, output,
		   error, 0 /* message */);

  /* start decoding */

  result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

  /* release the decoder */

  mad_decoder_finish(&decoder);

  return result;
}
