#ifndef __WEAPONS_H
#define __WEAPONS_H

typedef struct sweapon {
    const char *weapon_name;
    short wield_dmg_cnt;
    short wield_dmg_rnd;
} WEAPON;

#define MAX_WEAPONS 5
WEAPON weapons[MAX_WEAPONS] = {
    {"Shortsword",1,3},
    {"Mace",2,4},
    {"Longsword",3,4},
    {"Dagger",1,4},
    {"Two-Handed Sword",2,6}
};


#endif
