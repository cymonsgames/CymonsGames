#ifndef __MONSTERS_H
#define __MONSTERS_H

/* Enemy Flags (combined by using bitwise operators) */
#define NME_MEAN 1
#define NME_FLYING 2
#define NME_REGEN 4
#define NME_GREEDY 8
#define NME_INVIS 16
#define NME_CORRODES 32

#define NME_SIZE_FINE 8
#define NME_SIZE_DIMINUTIVE 4
#define NME_SIZE_TINY 2
#define NME_SIZE_SMALL 1
#define NME_SIZE_MEDIUM 0
#define NME_SIZE_LARGE -1
#define NME_SIZE_HUGE -2
#define NME_SIZE_GARGANTUAN -4
#define NME_SIZE_COLOSSAL -8

/* Let's make some enemies to fight */
typedef struct senemy {
    const char *enemy_name;
    short carry;
    short flags;
    short str;
    int exp;
    short level;
    int armor;
    short hp;
    short dmg_cnt;
    short dmg_rnd;
    short size_modifier;
} ENEMY;

#define MAX_ENEMIES 27
ENEMY enemies[MAX_ENEMIES] = {
    {"Aquator", 0, NME_MEAN | NME_CORRODES, 10, 20, 5, 2, 0, 0, 0, NME_SIZE_DIMINUTIVE},
    {"Bat", 0, NME_FLYING, 1, 1, 1, 16, 0, 1, 2, NME_SIZE_DIMINUTIVE},
    {"Centaur", 15, 10, 0, 25, 4, 4, 0, 1, 6, NME_SIZE_LARGE},
    {"Dragon", 100, NME_MEAN, 10, 6800, 10, -1, 0, 3, 10, NME_SIZE_GARGANTUAN},
    {"Emu", 0, NME_MEAN, 10, 2, 1, 7, 0, 1, 2, NME_SIZE_LARGE},
    {"Venus Flytrap", 0, NME_MEAN, 10, 80, 8, 3, 0, 0, 0, NME_SIZE_MEDIUM},
    {"Griffin", 20, NME_MEAN | NME_FLYING | NME_REGEN, 10, 2000, 13, 2, 0, 3, 5, NME_SIZE_LARGE},
    {"Hobgoblin", 0, NME_MEAN, 10, 3, 1, 5, 0, 1, 8, NME_SIZE_MEDIUM},
    {"Ice Monster", 0, NME_MEAN, 10, 15, 1, 9, 0, 1, 2, NME_SIZE_LARGE},
    {"Jabberwock", 70, 0, 10, 4000, 15, 6, 0, 2, 12, NME_SIZE_LARGE},
    {"Kestral", 0, NME_MEAN | NME_FLYING, 10, 1, 1, 7, 0, 1, 4, NME_SIZE_SMALL},
    {"Kobold", 15, NME_MEAN, 9, 5, 1, 15, 4, 1, 6, NME_SIZE_SMALL},
    {"Leprechaun", 100, NME_GREEDY, 10, 10, 3, 8, 0, 1, 2, NME_SIZE_SMALL},
    {"Medusa", 40, NME_MEAN, 10, 200, 8, 2, 0, 3, 4, NME_SIZE_LARGE},
    {"Nymph", 100, 0, 10, 37, 3, 9, 0, 0, 0, NME_SIZE_DIMINUTIVE},
    {"Orc", 15, NME_GREEDY, 10, 5, 1, 6, 0, 1, 8, NME_SIZE_MEDIUM},
    {"Phantom", 0, NME_INVIS, 10, 120, 8, 3, 0, 4, 4, NME_SIZE_MEDIUM},
    {"Quagga", 30, NME_MEAN, 10, 32, 3, 2, 0, 1, 4, NME_SIZE_LARGE},
    {"Rattlesnake", 0, NME_MEAN, 10, 9, 2, 3, 0, 1, 6, NME_SIZE_SMALL},
    {"Snake", 0, NME_MEAN, 10, 9, 2, 3, 0, 1, 6, NME_SIZE_SMALL},
    {"Troll", 50, NME_REGEN | NME_MEAN, 10, 120, 6, 4, 0, 1, 8, NME_SIZE_LARGE},
    {"Ur-Vile", 0, NME_MEAN, 10, 190, 7, -2, 0, 4, 6, NME_SIZE_MEDIUM},
    {"Vampire", 20, NME_REGEN | NME_MEAN, 10, 350, 8, 1, 0, 1, 10, NME_SIZE_MEDIUM},
    {"Wraith", 0, 0, 10, 55, 5, 4, 0, 1, 6, NME_SIZE_MEDIUM},
    {"Xeroc", 30, 0, 10, 100, 7, 7, 0, 3, 4, NME_SIZE_SMALL},
    {"Yeti", 30, 0, 10, 50, 4, 6, 0, 1, 6, NME_SIZE_LARGE},
    {"Zombie", 0, NME_MEAN, 12, 6, 2, 8, 0, 1, 6, NME_SIZE_MEDIUM}
};

#endif
