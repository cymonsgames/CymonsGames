#ifndef __DICE_H
#define __DICE_H

#include "mt19937int.h"

static inline int rollDie(int min, int max) {
    int random_value;

    random_value = (int) (((float)genrand_int32() / (float)0xFFFFFFFF)*(max));
    return random_value + min;
}

/*
    Stat rolls should be greater than 5
    for all stats according to D&D rules
*/
static inline int statRoll(int max) {
    int val = 0;
    while (val <= 5) {
        val = rollDie(1,max);
    }
    return val;
}


#endif
