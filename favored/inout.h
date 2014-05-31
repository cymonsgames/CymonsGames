#ifndef __INOUT_H
#define __INOUT_H

#ifdef NCURSES
	#include <ncurses.h>
#else
	#include <curses.h>
#endif
#include <string.h>
#include <cstdlib>

#ifndef PDCURSES
#include <time.h>
#define napms(t) { struct timespec ts; ts.tv_sec = x / 1000000000; ts.tv_usec = x % 1000000000; nanosleep(&ts, NULL); }
#endif

void intro ();

void killedfavored(int, double);

void survival (double);
#endif
