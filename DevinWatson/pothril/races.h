#ifndef __RACES_H
#define __RACES_H

#define RACE_SIZE_FINE 8
#define RACE_SIZE_DIMINUTIVE 4
#define RACE_SIZE_TINY 2
#define RACE_SIZE_SMALL 1
#define RACE_SIZE_MEDIUM 0
#define RACE_SIZE_LARGE -1
#define RACE_SIZE_HUGE -2
#define RACE_SIZE_GARGANTUAN -4
#define RACE_SIZE_COLOSSAL -8

/*  Character race type */
typedef struct srace {
    const char *race_name;
    int hp_modifier;
    int str_modifier;
    int max_str;
    int max_dex;
    int start_hp;
    int max_agi;
    int min_str;
    short size_modifier;
} RACE;

#define MAX_RACES 3
RACE races[MAX_RACES] = {
  {"Human",0,0,18,18,11,18,20,RACE_SIZE_MEDIUM},
  {"Elf",0,0,15,19,20,20,7,RACE_SIZE_MEDIUM},
  {"Giant",5,5,25,15,12,10,10,RACE_SIZE_LARGE}
};

#endif
