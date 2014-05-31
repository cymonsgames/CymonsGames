#ifndef MAPGEN_H_INCLUDED
#define MAPGEN_H_INCLUDED

#include "tricule.h"

void generatemap(struct mapstruct *mapdata);
void tilesdebugoutput (int t[XBIG][YBIG], int xx, int yy, int pause, char *fmt, ...);

#endif // MAPGEN_H_INCLUDED
