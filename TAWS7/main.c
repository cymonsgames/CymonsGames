#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "curses.h"

int width, height;

void ResizeScreen(int w)
{
	width = w;
	height = w/2;
}

extern void Game_Loop(void);

int main (int args, char *argc[])
{
	int i, pdc=0;
	char input[64];

	ResizeScreen(40); // default
	for (i=0; i < width; i++)
		printf("_");
	printf("\n");
	for (i=0; i < height; i++)
		printf("|\n");
	printf("Welcome to TAWS Volume 5!\n");
	printf("Resizing the window to the size of the window is recommended.\n");
	while (1)
	{
		printf("Is the screen size above acceptable? ");
		fgets(input, 63, stdin);
		if (input[0] == 'y' || input[0] == 'Y')
		{
            pdc = 1;
            initscr();
            cbreak();
            noecho();
            keypad(stdscr, 1);
            nodelay(stdscr, TRUE);
            start_color();
            for (i=0; i <= 8; ++i)
                init_pair(i, i, COLOR_GREEN);
			Game_Loop();
			break;
		}
		else if (input[0] == 'x' || input[0] == 'X' || !strcmp(input, "exit"))
			break;
		else
		{
			memset(input, 0, sizeof(input));
			printf("Enter width in characters:");
			fgets(input, 63, stdin);
			ResizeScreen(atoi(input));
			memset(input, 0, sizeof(input));
			for (i=0; i < width; i++)
				printf("_");
			printf("\n");
			for (i=0; i < height; i++)
				printf("|\n");
		}
	}
	if (pdc)
 	   endwin();
	printf("Thanks for playing!\n");
	return 0;
}
