#ifndef GFXWRAPPER_H_INCLUDED
#define GFXWRAPPER_H_INCLUDED

#include <allegro.h>

#define XSCREEN 800
#define YSCREEN 600

int initgfx(int width, int height);
void initsound();

void showconsole();
void hideconsole();
void consolemsg(char *msg, ...);

void begintimer();
int wait();
int refresh (BITMAP *);

void deinitsound();
void deinitgfx();

int randint(int low, int high);
#endif // GFXWRAPPER_H_INCLUDED
