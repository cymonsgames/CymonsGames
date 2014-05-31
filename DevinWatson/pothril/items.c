#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"

void itemMeatyBone(PLAYER *elt) {
    elt->nutrition += 50;

    if (elt->nutrition > 150 && (elt->status & STATUS_HUNGRY))
        elt->status = elt->status & ~(STATUS_HUNGRY);

    if (elt->status == 0) elt->status = STATUS_OK;
}

void itemWaterOfLife(PLAYER *elt) {
    if (elt->hp < elt->max_hp) {
        elt->hp = elt->max_hp;
    }
}

void init_items() {
    master_item_count = 0;
    master_item_list = NULL;

    addMasterItem("Meaty Bone",0,ITEM_TYPE_FOOD,USE_NORMAL,itemMeatyBone,10);
    addMasterItem("Water of Life",1,ITEM_TYPE_POTION,USE_COMBAT | USE_NORMAL,itemWaterOfLife,1);
}

int useItem(int id, PLAYER *play) {
    ITEM *elt;

    elt = master_item_list;

    while (elt != NULL) {
        if (elt->id == id && elt->qty > 0) {
            if (elt->function != NULL) {
                elt->function(play);
                elt->qty--;
                return 1;
            } else {
                if (elt->id == id && elt->qty == 0) return 2;
            }
        }
        elt = elt->next;
    }

    return -1;
}

void items_destroy() {
    ITEM *current;
    ITEM *temp;

    current = master_item_list;
    while (current != NULL) {
        temp = current->next;
        free(current->item_name);
        free(current);
        current = temp;
        master_item_count--;
    }

    /*free(master_item_list);*/

    master_item_list = NULL;
}

ITEM* createItem(char *name, int id, int item_type, int usage_flags, void *caller, int color) {
    ITEM *new_element;

    if ( (new_element = (ITEM *)malloc(sizeof(ITEM))) == NULL) return NULL;
    memset(new_element, 0, sizeof(ITEM));

    new_element->item_name = (char *)malloc(sizeof(char)*strlen(name)+1);
    memset(new_element->item_name, 0, strlen(name) + 1);

    strcpy(new_element->item_name,name);
    new_element->color = color;
    new_element->id = id;
    new_element->item_type = item_type;
    new_element->usage_flags = usage_flags;
    new_element->qty = 10;
    new_element->function = caller;
    new_element->next = NULL;

    return new_element;
}

/*
    Creates a new Master Item for the list
*/
int addMasterItem(char *name, int id, int item_type, int usage_flags, void *caller, int color) {
    ITEM *new_element;
    ITEM *current;

    new_element = createItem(name,id,item_type,usage_flags,caller,color);
    if (new_element == NULL) return 1;

    if (master_item_list == NULL) {
        master_item_list = new_element; /*createItem(name,id,item_type,usage_flags,caller,color); */
        if (master_item_list == NULL) return 1;
        master_item_count++;
        return 0;
    } else {
        current = master_item_list;
        while (current->next != NULL)
            current = current->next;
    }

    current->next = new_element;
    master_item_count++;
    return 0;
}

ITEM *getItemById(int id) {
    ITEM *elt;

    elt = master_item_list;
    if (elt->id == id) return elt;
    while (elt != NULL) {
        if (elt->id == id) return elt;
        elt = elt->next;
    }
    return NULL;
}

void dumpMasterList(FILE *f) {
    ITEM *elt;

    fflush(f);
    fprintf(f,"MASTER ITEM COUNT: %i\n",master_item_count);
    elt = master_item_list;
    while (elt != NULL) {
        fprintf(f,"ITEM %p - %s\tID: %i\tNext: %p\n",elt,elt->item_name,elt->id,elt->next);
        elt = elt->next;
    }
    fflush(f);
}
