#ifndef TRICULE_H_INCLUDED
#define TRICULE_H_INCLUDED

#include <allegro.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "gfxwrapper.h"

/* make sure the math works or things will break, okay? */
#define XBIG 360
#define YBIG 270
#define XREG 9
#define YREG 9
#define XMINI 40
#define YMINI 30
#define MAXACTORS 10

extern struct regions {
    int tileset;
    int reflect;
    int numpaths;
    int maxlen;
    int minlen;
} regiondata[]; /* defined in main.c */
#define numregiondata 9
#define numregions 5
#define toxreg(c) (c / (XBIG / XREG))
#define toyreg(c) (c / (YBIG / YREG))

enum {EAST = 0, SOUTH, WEST, NORTH, MAXCOORD};
enum {HOR = 1, VERT};
extern int delta[5]; /* defined in main.c */
#define DX(c) (delta[(c % MAXCOORD) + 1])
#define DY(c) (delta[c % MAXCOORD])
#define REV(c) ((c + 2) % MAXCOORD)

enum { /* sprite frame defines */
feast0 = 0, feast1,  feast2,
fsouth0,    fsouth1, fsouth2,
fwest0,     fwest1,  fwest2,
fnorth0,    fnorth1, fnorth2,
numcharframes
};
enum { /* sprite definitions */
shansel, sgretel,
numsprites
};

enum { /* tile defines */
tdot_1 = 0, tdot_2, tdot_3, tdot_4,
tpath_1, tpath_2, tpath_3,
t1001, t0001, t0011,
t1000, t0000_1, t0000_2, t0000_3, t0010,
t1100, t0100, t0110,
tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2,
numtiles
};

extern struct playerstruct {
    int tilerange;
} playerdata[]; /* defined in main.c */
#define numplayers 2

/* game state data definitions: */
struct gfxstruct {
    BITMAP* blank;
    BITMAP* tiles[numregiondata * numtiles];
    BITMAP* sprites[numsprites][numcharframes];
    BITMAP* zoomview;
    BITMAP* mapview;
    int tilesize;
    int h; /* half tile */
    int tileoverlap;  /* Tiles overlap only to the north. */
    int spriteoverlap; /* Sprites overlap in all 4 directions. */
    int mapzoom;
    BITMAP* zmap; /* kinda abusing the bitmap here so I don't have to write stuff */
};

struct mapstruct {
    int map[XBIG][YBIG];
    int regs[XREG][YREG];
    int tiles[XBIG][YBIG];
};

struct coord {
    int x, y;
};

struct actorstruct {
    char active;
    int x, y;
    int tx, ty; /* tile coord */
    int rx, ry; /* region coord */
    int dir;
    int sprite;
    int frame;
    char isplayer;
    int haspower;
};

struct logicstruct {
    int actorcollision[XBIG][YBIG];
    int dots[XBIG][YBIG];
    int step;
    struct actorstruct actors[MAXACTORS];
    int playerindex[numplayers];
    int lastactor;
};

struct gamestruct {
    struct gfxstruct gfx;
    struct mapstruct map;
    struct logicstruct stage;
};

void qdebug(int x, int y, int pause, char *fmt, ...);
#endif // TRICULE_H_INCLUDED
