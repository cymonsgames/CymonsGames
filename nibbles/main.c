#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#include "config.h"
#include "misc.h"
#include "screen.h"

#ifdef PDCURSES
#define usleep(x) napms((x)/1000)
#endif

int quit (int sig)
{
	endgraphics();
	exit(0);
}

int main (int argc, char *argv[])
{
	// The tree windows (Main,Grid,Status in that order) used by ncurses
	WINDOW	*w [3];

	// The grid, to make it easy to tell where the worm is
	bool grid [GRIDX] [GRIDY];

	// The worm history
	short int	hx [HISTORYSIZE];
	short int	hy [HISTORYSIZE];

	// Worm variables
	int			length = STARTLENGTH; // the length of the worm
	int			extralength = STARTELENGTH; // how much the worm is going to grow
	int			curx,cury; // current position of the worm
	int			dir = RIGHT; // direction the worm is facing
	int			lastdir = RIGHT; // last direction
	int			score = 0;
	bool		paused = false;


	int			dotx,doty; // position of the dot (worm food)

	int			tick; // current tick

	// User input
	int			ch;

	// Initialize ncurses and the windows
	startgraphics(w);

	// Seed the random number generator
	randomize();

	// clear the grid
	memset(grid,0,sizeof(grid));

	// Initialize and draw the worm
	curx = (GRIDX+length) >> 1;
	cury = GRIDY >> 1;
	for (tick=0;tick<length;tick++) {
		hx[tick] = curx - (length - tick) + 1;
		hy[tick] = cury;
		grid[curx-(length-tick)+1][cury] = true;
		drawblock(w,curx-(length-tick)+1,cury);
	}

	// Initialize and draw the dot (worm food)
	newdot(w,grid,&dotx,&doty);

	// Main loop
	while (1) {
		lastdir = dir;

		if (paused) {
			ch = wgetch(w[Grid]);
			while (ch != ERR) {
				switch (ch) {
					case 'p':
						paused = false;
						mvwprintw(w[Status],0,WINDOWX - 2 /* because of the border lines */ - 7 /* strlen("PAUSED ") */, "      ");
						break;
					case 'q':
						endgraphics();
						printf("Quitter! Your score was %d.\n",score);
						exit(0);
						break;
				}
				ch = wgetch(w[Grid]);
			}
			mvwprintw(w[Status],0,0,"Score:  %d ",score);
			mvwprintw(w[Status],0,WINDOWX - 2 /* because of the border lines */ - 7 /* strlen("PAUSED ") */, "Paused");
			wrefresh(w[Status]);
			usleep(TICKSPEED);
			continue;
		}

		// catch user input
		ch = wgetch(w[Grid]);
		while (ch != ERR) {
			switch (ch) {
				case KEY_LEFT:
				case 'h': // accept vi-style directions as well
					if ((lastdir == UP) || (lastdir == DOWN)) {
						dir = LEFT;
					}
					break;
				case KEY_RIGHT:
				case 'l':
					if ((lastdir == UP) || (lastdir == DOWN)) {
						dir = RIGHT;
					}
					break;
				case KEY_UP:
				case 'k':
					if ((lastdir == LEFT) || (lastdir == RIGHT)) {
						dir = UP;
					}
					break;
				case KEY_DOWN:
				case 'j':
					if ((lastdir == LEFT) || (lastdir == RIGHT)) {
						dir = DOWN;
					}
					break;
				case 'p':
					paused = true;
					break;
				case 'q':
					endgraphics();
					printf("Quitter! Your score was %d.\n",score);
					exit(0);
					break;
			}
			ch = wgetch(w[Grid]);
		}

		// modify the new position based on the direction
		switch (dir) {
			case LEFT:	curx--; break;
			case RIGHT:	curx++; break;
			case UP:	cury--; break;
			case DOWN:	cury++; break;
		}

		// check whether or not the worm has crashed in a wall
		if ((curx<0) || (curx>=GRIDX) || (cury<0) || (cury>=GRIDY)) {
			endgraphics();
			printf("You crashed in a wall. Your score was %d.\n",score);
			exit(0);
		}

		// check whether or not the worm has crashed with itself
		if (grid[curx][cury]) {
			endgraphics();
			printf("You crashed with yourself. Your score was %d.\n",score);
			exit(0);
		}

		// the worm is still on the grid, and it seems healty :)

		drawblock(w,curx,cury); // let's draw the new block
		grid[curx][cury] = true; // and update the grid

		// update the history array
		hx[tick] = curx;
		hy[tick] = cury;

		// has the worm eaten food?
		if ((dotx == curx) && (doty == cury)) {
			score++;
			extralength += WORMGROWTH;
			newdot(w,grid,&dotx,&doty);
		}

		// if the worm still has some growing to do..
		if (extralength > 0) {
			extralength--;
			length++;
		} else {
			// otherwise, clear the last block
			clearblock(w,hx[tick-length],hy[tick-length]);
			grid[hx[tick-length]][hy[tick-length]] = 0;
		}

		wrefresh(w[Grid]);

		// finally, update the status line
		mvwprintw(w[Status],0,0,"Score:  %d",score);
		mvwprintw(w[Status],0,GRIDX - 2 /* because of the border lines */ - 7 /* strlen("PAUSED ") */,"       ");
		wrefresh(w[Status]);

		usleep(TICKSPEED);
		tick++;

		// if the history array is full, copy the length of the worm
		// to the beginning of the array and decrease the tick counter
		if (tick == HISTORYSIZE) {
			memcpy(hx, hx+HISTORYSIZE-length, length * sizeof(short int));
			memcpy(hy, hy+HISTORYSIZE-length, length * sizeof(short int));
			tick = length;
		}

		// ready for next tick!
	}
	endgraphics();
}
