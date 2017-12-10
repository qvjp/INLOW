#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

static void restoreTermios(void);
static void handleInput(void);

struct termios oldTermios;
int x = 15;
int y = 20;
int isFire = 0;
int nx = 30;
int ny = 3;
int isKilled = 0;
int v = 1;
int sumTime = 0;

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	tcgetattr(0, &oldTermios);
	atexit(restoreTermios);
	struct termios new_termios = oldTermios;
	new_termios.c_lflag &= ~(ECHO | ICANON);
	new_termios.c_cc[VMIN] = 0;
	tcsetattr(0, TCSAFLUSH, &new_termios);

	#ifndef __inlow__
	srand(time(NULL));
	#else
	srand(2);
	#endif

#ifndef __inlow__
setbuf(stdout, NULL);
#endif

	while(1)
	{
		sumTime > 10000 ? sumTime++ : sumTime--;
		struct timespec sleepTime;
		sleepTime.tv_sec = 0;
		sleepTime.tv_nsec = 27500000;
		nanosleep(&sleepTime, NULL);
		printf("\e[2J");
		
		printf("\e[41m");//红色
		printf("\e[%d;%dH ",ny,nx); //显示目标
		printf("\e[0m");//恢复
		
		if (!(sumTime % 10))
		{
			//printf("\e[%d;%dHx",ny,nx);
			v = nx < 80 && nx > 0 ? v : -v;
			nx += v;
			ny += 1;
			if (ny > 25)
			{
				printf("\e[2J");
				printf("\e[1;1H");
				exit(0);
			}
		}

		if (isFire == 1)
		{
			for (int i = y - 1; i > 1; i--)
			{
				printf("\e[36m");//蓝色
				printf("\e[%d;%dH|",i,x);
				printf("\e[0m");
				printf("\e[H");
				//fflush(stdout);

				// sleepTime.tv_sec = 0;
				// sleepTime.tv_nsec = 2750000;
				// nanosleep(&sleepTime, NULL);

			}	
			if (x == nx)
			{
				isKilled = 1;
				nx = rand() % 80;
				ny = rand() % 4;

			}
			isFire = 0;
		}

		// printf("\e[2J");
		printf("\e[42m");
		printf("\e[32m");
		printf("\e[%d;%dH*", y, x);
		printf("\e[%d;%dH*****",y+1, x-2);
		printf("\e[%d;%dH* *", y+2, x-1);
		printf("\e[0m");
		printf("\e[H");

		handleInput();
	}
	return 0;
}
static void handleInput(void)
{
	char key;
	if (read(0, &key, 1))
	{
		if (key == 'q')
		{
			printf("\e[2J");
			exit(0);	
		}
		switch (key)
		{
			case 'w': 
			case 'W': 
			{
				if (y > 2)
					y-=1;
				else
					y = y;
			}
				break;
			case 'a':
			case 'A':
			{
				if (x > 3)
					x-=1;
				else
					x = x;
			}
				break;
			case 's':
			case 'S':
			{
				if (y < 23)
				y+=1;
				else
				y = 23;
			}
				break;
			case 'd':
			case 'D':
			{
				if (x < 78)
				x+=1;
				else
				x = 78;
			}
				break;
			case ' ':
				isFire = 1;
		}
	}
}
static void restoreTermios(void)
{
	tcsetattr(0, TCSAFLUSH, &oldTermios);
}
