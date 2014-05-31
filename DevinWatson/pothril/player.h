/*#pragma ONCE*/
#ifndef __PLAYER_H
#define __PLAYER_H

/*  The player data structure */
typedef struct splayer {
    char *player_name;    /* Name of player (set by the player at start time) */
    int gold;               /* Amount of gold player has */
    int x;                  /* X-coordinate on the game map */
    int y;                  /* Y-coordinate on the game map */
    int hp;                 /* Hit Points */
    int max_hp;
    int nutrition;
    long xp;                 /* Experience Points */
    long xp_earned;          /* Counter for leveling up */
    long xp_needed;          /* XP Needed */
    int str;                /* Strength (used in damage calculations) */
    int agi;                /* Agility */
    int con;                /* Constitution */
    int intel;              /* Intelligence */
    int wis;                /* Wisdom */
    int cha;                /* Charisma */
    int dex;                /* Dexterity */
    int level;              /* Player level */
    int weapon;             /* Current weapon, using an array right now to keep it simple */
    int race;               /* Player's race (Human, Elf, Gnome, etc.) */
    int armor;              /* Armor rating */
    int is_dead;           /* Used to tell if the player has died */
    char avatar;            /* Holder for player's on-screen avatar */
    int slain;              /* Number of enemies vanquised in battle */
    int attack_bonus;       /* Base attack bonus */
    int critical_hit;      /* If the player performed a critical strike in combat */
    int status;             /* For status ailments and conditions */
    int has_chalice;
} PLAYER;

PLAYER player;

/* Player status ailments and conditions */
#define STATUS_OK 1
#define STATUS_POISON 2
#define STATUS_CONFUSED 4
#define STATUS_BLIND 8
#define STATUS_HUNGRY 16

#endif
