#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80

enum Direction
{
	UP,
	LEFT,
	DOWN,
	RIGHT,
	NONE
};

struct SnakeSegment
{
	int row;
	int col;
	enum Direction direction;
	struct SnakeSegment* next;
};

struct Food
{
	int row;
	int col;
};

struct SnakeSegment* snakeHead;
struct SnakeSegment* snakeTail;
struct Food food;
struct termios oldTermios;

static bool checkCollision(void);
static void checkFood(void);
static void drawScreen(void);
static int gameOver(int);
static void handleInput(void);
static void initializeWorld(void);
static void move(struct SnakeSegment* snake);
static void restoreTermios(void);

int main(int argc, char* argv[])
{
	if (argc >= 2)
	{
		unsigned int seed = strtoul(argv[1], NULL, 10);
		srand(seed);
	}
	else
	{
		#ifndef __inlow__
			srand(time(NULL));
		#else
			srand(2);
		#endif
	}

#ifndef __inlow__
	setbuf(stdout, NULL);
#endif

	tcgetattr(0, &oldTermios);
	atexit(restoreTermios);
	struct termios new_termios = oldTermios;
	new_termios.c_lflag &= ~(ECHO | ICANON);
	new_termios.c_cc[VMIN] = 0;
	tcsetattr(0, TCSAFLUSH, &new_termios);

	initializeWorld();

	while(true)
	{
		drawScreen();
		struct timespec sleepTime;
		sleepTime.tv_sec = 0;
		sleepTime.tv_nsec = 175000000;
		nanosleep(&sleepTime, NULL);
		handleInput();
		move(snakeHead);

		if (checkCollision())
		{
			puts("\e[2JYou loose");
			return 0;
		}
	}
}

static bool checkCollision(void)
{
	struct SnakeSegment* current = snakeHead;
	while (current)
	{
		if (current->row < 0 || current->row >= HEIGHT || current->col < 0 || current->col >= WIDTH)
				return true;
		struct SnakeSegment* other = current->next;
		while(other)
		{
			if (current->row == other->row && current->col == other->col)
				return true;
			other = other->next;
		}
		current = current->next;
	}
	return false;
}

static void checkFood(void)
{
	if (food.row == snakeHead->row && food.col == snakeHead->col)
	{
		struct SnakeSegment* newSegment = malloc(sizeof(struct SnakeSegment));
		newSegment->row = snakeTail->row;
		newSegment->col = snakeTail->col;

		newSegment->direction = NONE;
		newSegment->next = NULL;

		snakeTail->next = newSegment;
		snakeTail = newSegment;

		food.row = rand() % HEIGHT;
		food.col = rand() % WIDTH;
	}
}

static void drawScreen(void)
{
	printf("\e[2J");
	struct SnakeSegment* current = snakeHead;
	while (current)
	{
		if (current->row >= 0 && current->row < HEIGHT && current->col >= 0 && current->col < WIDTH)
				printf("\e[%d;%uH0", current->row, current->col);
		current = current->next;
	}
	printf("\e[%d;%dHX", food.row, food.col);
	printf("\e[H");
}

static void handleInput(void)
{
	char key;
	if (read(0, &key, 1))
	{
		if (key == 'q')
			gameOver(0);
		enum Direction newDirection = snakeHead->direction;
		switch (key)
		{
			case 'w': 
			case 'W': 
					newDirection = UP;
					break;
			case 'a':
			case 'A':
					newDirection = LEFT;
					break;
			case 's':
			case 'S':
					newDirection = DOWN;
					break;
			case 'd':
			case 'D':
					newDirection = RIGHT;
					break;
		}
		if ((snakeHead->direction + 2) % 4 != newDirection)
				snakeHead->direction = newDirection;
	}
}

static void initializeWorld(void)
{
	snakeHead = malloc(sizeof(struct SnakeSegment));
	snakeHead->row = 20;
	snakeHead->col = 10;
	snakeHead->direction = UP;

	snakeTail = snakeHead;
	for (int i = 0; i < 5; i++)
	{
		struct SnakeSegment* next = malloc(sizeof(struct SnakeSegment));
		snakeTail->next = next;

		next->row = 20;
		next->col = 11 + i;
		next->direction = LEFT;
		next->next = NULL;
		snakeTail = next;
	}
	food.row = rand() % HEIGHT;
	food.col = rand() % WIDTH;
}

static void move(struct SnakeSegment* snake)
{
	switch (snake->direction)
	{
		case UP:
				snake->row--;
				break;
		case LEFT:
				snake->col--;
				break;
		case DOWN:
				snake->row++;
				break;
		case RIGHT:
				snake->col++;
				break;
		case NONE:
				break;
	}
	if (snake == snakeHead)
	{
		checkFood();
	}
	if (snake->next)
	{
		move(snake->next);
		snake->next->direction = snake->direction;
	}
}

static void restoreTermios(void)
{
	tcsetattr(0, TCSAFLUSH, &oldTermios);
}

static int gameOver(int i)
{
	printf("\e[2J");
	exit(i);
}
