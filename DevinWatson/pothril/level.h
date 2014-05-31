/*  Game Level Map routines for Perils of Pothril */
#ifndef __LEVEL_H
#define __LEVEL_H

#include "tiles.h"
#include "coord.h"

#define LEVEL_RUINS 0
#define LEVEL_CITY 1
#define LEVEL_TEMPLE 2
#define LEVEL_DUNGEON 3
#define LEVEL_PRECREATE 4
#define LEVEL_ENDING 5

#define MAP_DISPLAY_WIDTH 58     /* Displayable portion of map */
#define MAP_DISPLAY_HEIGHT 18

#define MAX_MAP_WIDTH INT_MAX
#define MAX_MAP_HEIGHT INT_MAX
#define MAX_NUMLEVELS 100

/*static const char *letpairs0 = "ABOUSEITILETSTONLONUTHNO";*/

/*
    Used to construct our random dungeons
    using cellular automata
*/
typedef struct sgenParams {
 	int r1_cutoff, r2_cutoff;
 	int reps;
} generation_params;


typedef struct sGameLevel {
    char *level_name;           /* Name of the level */
    int level_id;               /* Unique map id for game */
    int width;                  /* Width of the map */
    int height;                 /* Height of level map */
    boolean has_shops;          /* Does the level contain shops */
    int level_type;             /* Type of level (ruins, city, temple, etc.) */
    int **grid;                 /* Grid data of map */
    COORD portal_up_loc;        /* Location of portal out */
    COORD portal_down_loc;
    COORD shop_loc;
    struct sGameLevel *next;
} LEVELMAP;

int numlevels;

LEVELMAP *cur_map;
int **grid2;    /* scratch grid used for automata generations */
LEVELMAP *game_world;

void init_world();
void world_destroy();
void generation(LEVELMAP *map, generation_params *params);
int add_level(char *level_name, int level_id, int width, int height, boolean has_shops, int level_type);
LEVELMAP* create_map(char *level_name, int level_id, int width, int height, boolean has_shops, int level_type);
LEVELMAP* get_map(int id);
boolean isPassable(LEVELMAP *map, int x, int y);
boolean isUsable(LEVELMAP *map, int x, int y);
boolean isLineClear(LEVELMAP *map, int x, int y);
void placeShop(LEVELMAP *map);
char *makeLevelName();
void buildMaps(int map_count);
void stripout(char *s,const char c);
int randMap(int min, int max);
void dumpMapList(FILE *f);
void placePortal(LEVELMAP *map);
boolean isUsable(LEVELMAP *map, int x, int y);

/* Used to randomly pick a tile element on the map */
static inline int mapPick(int fillprob) {
    int pick = rand()%100;
 	if (pick < fillprob) return WALL; else return GRAVEL;
}

#endif
