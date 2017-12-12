#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define W 80
#define H 25
#define NONE          "\e[m"
#define RED           "\e[0;41m"
#define GREEN         "\e[0;42m"
#define BROWN         "\e[0;43m"
#define BLUE          "\e[0;44m"
#define PURPLE        "\e[0;45m"
#define CYAN          "\e[0;46m"
#define WHITE         "\e[0;47m"


char* color[] = {GREEN, BROWN, BLUE, PURPLE, CYAN, WHITE};

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;
	int x = 20;
	int y = 6;
	printf("\e[H\e[2J");
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; i++)
		{
			printf(color[j%6]);
			printf(" ");
		}
    }
	printf("\e[41m");
	for (int i = 0; i < 7; i++)				// I
		printf("\e[%d;%dH ", y+i, x);

	for (int i = 0; i < 7; i++)				// N
		printf("\e[%d;%dH ", y+i, x+3);
	for (int i = 0; i < 6; i++)
		printf("\e[%d;%dH ", y+i, x+3+i);
	for (int i = 0; i < 7; i++)
		printf("\e[%d;%dH ", y+i, x+3+6);
	
	for (int i = 0; i < 7; i++)
		printf("\e[%d;%dH ", y + i, x + 3 + 6 + 3);	// L
	for (int i = 0; i < 7; i++)
		printf("\e[%d;%dH ", y+6, x + 3 + 6 + 3 + i);

	for (int i = 0; i < 5; i++)							// O
		printf("\e[%d;%dH ", y+1+i, x + 3+ 6+3+6+3);
	for (int i = 0; i < 5; i++)
		printf("\e[%d;%dH ", y, x + 3+ 6+3+6+3+1+i);
	for (int i = 0; i < 5; i++)
		printf("\e[%d;%dH ", y+6, x + 3+ 6+3+6+3+1+i);
	for (int i = 0; i < 5; i++)
		printf("\e[%d;%dH ", y+1+i, x + 3+ 6+3+6+3+6);

	for (int i = 0; i < 7; i++)							//W
		printf("\e[%d;%dH ", y+i, x + 3 + 6 + 3 + 6 + 3 + 6 +3);
	for (int i = 0; i < 3; i++)
		printf("\e[%d;%dH ", y+5-i, x + 3+ 6+3+6+3+6+3+1+i);
	for (int i = 0; i < 2; i++)
		printf("\e[%d;%dH ", y+4+i, x + 3+ 6+3+6+3+6+3+4+i);
	for (int i = 0; i < 7; i++)
		printf("\e[%d;%dH ", y+i, x + 3 + 6 + 3 + 6 + 3 + 6 +3+6);
		
	printf("\e[16;50H\e[1;32;45mINLOW 0.0.3 LIGHT");
	printf("\e[25;1H\e[0m");
    return 0;
}
