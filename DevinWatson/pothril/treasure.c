#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treasure.h"

void treasure_init() {
    treasure_size = 0;
    treasure_list = NULL;
}

void treasure_destroy() {
    TREASURE *current;
    TREASURE *temp;

    current = treasure_list;
    while (current != NULL) {
        temp = current->next;
        free(current);
        current = temp;
        treasure_size--;
    }
    /*free(treasure_list);*/
    treasure_list = NULL;
}

TREASURE* createTreasure(int x, int y, int amt) {
    TREASURE *new_element;

    if ( (new_element = (TREASURE *)malloc(sizeof(TREASURE))) == NULL) return NULL;
    memset(new_element,0, sizeof(TREASURE));

    new_element->x = x;
    new_element->y = y;
    new_element->amt = amt;
    new_element->next = NULL;

    return new_element;
}

int addTreasure(int x, int y, int amt) {
    TREASURE *new_element;
    TREASURE *current;

    new_element = createTreasure(x,y,amt);

    if (treasure_list == NULL) {
        treasure_list = createTreasure(x,y,amt);
        if (treasure_list == NULL) return 1;
        treasure_size++;
        return 0;
    } else {
        current = treasure_list;
        while (current->next != NULL)
            current = current->next;
    }

    current->next = new_element;
    treasure_size++;
    return 0;
}

TREASURE* treasureAt(int x, int y) {
    TREASURE *elt;

    if (treasure_size == 0) return NULL;

    elt = treasure_list;
    while (elt != NULL) {
        if (elt->x == x && elt->y == y) return elt;
        elt = elt->next;
    }

    return NULL;
}

int remove_treasureAt(int x, int y) {
    TREASURE *pBefore=NULL;
    TREASURE *current;

    if (treasure_size == 0) return -1;

    current = treasure_list;

	while (current != NULL) {
		if (current->x == x && current->y == y) {
			if(pBefore) pBefore->next = current->next;
			free(current);
            treasure_size--;
			return 1;
		}
		pBefore = current;
		current = current->next;
	}

    return 0;
}

void dumpList(FILE *f) {
    TREASURE *elt;

    fflush(f);
    elt = treasure_list;
    fprintf(f,"\n---[TREASURE LIST]---\n");
    while (elt != NULL) {
        fprintf(f,"TREASURE %p - (%i,%i) - %i\tNext: %p\n",elt,elt->x, elt->y, elt->amt,elt->next);
        elt = elt->next;
    }
    fflush(f);
}

