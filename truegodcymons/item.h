#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED
#include "map.h"
#define MAXITEM 20
int numitems=4;


struct items{
    int x,y;
    char sym;
}item[MAXITEM];

void genitem(int i);
void useitem(items* it);
void destitem(items* it);
void putitems();
items* iteml[YSIZE][XSIZE];


#endif // ITEM_H_INCLUDED
