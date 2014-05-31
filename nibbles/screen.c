#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "config.h"

#include "misc.h"
#include "screen.h"

void endgraphics (void)
{
	endwin();
}

void startgraphics (WINDOW **w)
{
	// to put the nibbles-window in the center of the screen
	int		offsetx,offsety;
	int		loop,lx,ly;


	initscr();

	if ((COLS < WINDOWX) || (LINES < WINDOWY)) {
		endwin();
		fprintf(stderr,"A terminal with at least %d lines and %d columns is required.\n",WINDOWY,WINDOWX);
		exit(1);
	}

	offsetx = (COLS - WINDOWX) >> 1;
	offsety = (LINES - WINDOWY) >> 1;

	w[Main] = newwin(WINDOWY,WINDOWX,offsety,offsetx);
	w[Grid] = newwin(GRIDY,GRIDX*2,offsety+1,offsetx+1);
	w[Status] = newwin(1,WINDOWX-2,offsety+WINDOWY-2,offsetx+1);

	keypad(w[Grid], TRUE);
	nonl();
	cbreak();
	noecho();
	nodelay(w[Grid], TRUE);
	curs_set(FALSE);

	if (!has_colors()) {
		endwin();
		fprintf(stderr,"A terminal with color support is required.\n");
		exit(1);
	}

	start_color();

	// configure colors
	init_pair(WORMCOLORPAIR,WORMCOLOR,BACKGROUND);
	init_pair(LINECOLORPAIR,LINECOLOR,BACKGROUND);
	init_pair(STATUSCOLORPAIR,STATUSCOLOR,BACKGROUND);
	init_pair(DOTCOLORPAIR,DOTCOLOR,BACKGROUND);

	// draw pretty lines in w[Main]
	wattrset(w[Main],COLOR_PAIR(LINECOLORPAIR)|A_BOLD);
	mvwaddch(w[Main],0,0,ACS_ULCORNER);
	mvwaddch(w[Main],0,WINDOWX-1,ACS_URCORNER);
	mvwaddch(w[Main],WINDOWY-1,0,ACS_LLCORNER);
	mvwaddch(w[Main],WINDOWY-1,WINDOWX-1,ACS_LRCORNER);
	for (loop=1;loop<WINDOWY-1;loop++) {
		mvwaddch(w[Main],loop,0,ACS_VLINE);
		mvwaddch(w[Main],loop,WINDOWX-1,ACS_VLINE);
	}
	for (loop=1;loop<WINDOWX-1;loop++) {
		mvwaddch(w[Main],0,loop,ACS_HLINE);
		mvwaddch(w[Main],WINDOWY-3,loop,ACS_HLINE);
		mvwaddch(w[Main],WINDOWY-1,loop,ACS_HLINE);
	}
	mvwaddch(w[Main],WINDOWY-3,0,ACS_LTEE);
	mvwaddch(w[Main],WINDOWY-3,WINDOWX-1,ACS_RTEE);
	wrefresh(w[Main]);

	// clear the status screen
	wattrset(w[Status],COLOR_PAIR(STATUSCOLORPAIR)|A_BOLD);
	for (loop=0;loop<=GRIDX;loop++) {
		clearblock(w,loop,0);
	}
	wrefresh(w[Status]);

	// clear the grid screen
	wattrset(w[Grid],COLOR_PAIR(WORMCOLORPAIR)|A_BOLD);
	for (lx=0;lx<=GRIDX;lx++) {
		for (ly=0;ly<=GRIDY;ly++) {
			clearblock(w,lx,ly);
		}
	}
#ifdef PDCURSES
	PDC_set_title("Nibbles - Cymon's Games");
#endif
	wrefresh(w[Grid]);
}

void drawblock (WINDOW **w, int x, int y)
{
	mvwaddch(w[Grid],y,x*2,ACS_BLOCK);
	mvwaddch(w[Grid],y,x*2+1,ACS_BLOCK);
}

void clearblock (WINDOW **w, int x, int y)
{
	mvwaddch(w[Grid],y,x*2,' ');
	mvwaddch(w[Grid],y,x*2+1,' ');
}

void newdot (WINDOW **w, bool grid[GRIDX][GRIDY], int *x, int *y)
{
	do {
		*x = randint(0,GRIDX-1);
		*y = randint(0,GRIDY-1);
	} while (grid[*x][*y]);
	wattrset(w[Grid],COLOR_PAIR(DOTCOLORPAIR)|A_BOLD);
	drawblock(w,*x,*y);
	wattrset(w[Grid],COLOR_PAIR(WORMCOLORPAIR)|A_BOLD);
}
