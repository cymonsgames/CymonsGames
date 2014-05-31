#ifndef __TREASURE_H
#define __TREASURE_H


/* Dynamic list of treasure scattered
   throughout the game world */
typedef struct sTreasureLoc {
    int x;
    int y;
    int amt;
    struct sTreasureLoc *next;
} TREASURE;

TREASURE *treasure_list;

int treasure_size;
void treasure_init();
void treasure_destroy();
int addTreasure(int x, int y, int amt);
TREASURE* createTreasure(int x, int y, int amt);
TREASURE* treasureAt(int x, int y);
int remove_treasureAt(int x, int y);
void dumpList(FILE *f);

#define MAX_TREASURES 25
#define TREASURE_AMT_MAX 35

#endif
