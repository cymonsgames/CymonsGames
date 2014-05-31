#ifndef __ITEMS_H
#define __ITEMS_H

#include "player.h"

#define ITEM_TYPE_FOOD 0
#define ITEM_TYPE_POTION 1
#define ITEM_TYPE_RING 2
#define ITEM_TYPE_SCROLL 4
#define ITEM_TYPE_UNKNOWN 5

#define USE_NORMAL 1
#define USE_COMBAT 2

typedef struct sItem {
    char *item_name;
    int id;
    int item_type;
    int usage_flags;
    short qty;
    int color;
    void (*function)(PLAYER *);
    struct sItem *next;
} ITEM;

int inventory_count;
int master_item_count;

ITEM *master_item_list;

void init_items();
void items_destroy();
int addMasterItem(char *name, int id, int item_type, int usage_flags, void *caller, int color);
ITEM* createItem(char *name, int id, int item_type, int usage_flags, void *caller, int color);
ITEM* getItemById(int id);
void dumpMasterList(FILE *f);
int useItem(int id, PLAYER *play);

#endif
