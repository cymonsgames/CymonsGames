/*
    dungcrwl -- Beginner's RPG with PDCurses
    Uses some of the basic game mechanics from D&D
*/

#ifndef _XOPEN_SOURCE_EXTENDED
# define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <limits.h>

#ifdef WACS_S1
# define HAVE_WIDE 1
#else
# define HAVE_WIDE 0
#endif

#if HAVE_WIDE
# include <wchar.h>
#endif

#if defined(PDCURSES) && !defined(XCURSES)
# define HAVE_RESIZE 1
#else
# define HAVE_RESIZE 0
#endif

#ifdef A_COLOR
# define HAVE_COLOR 1
#else
# define HAVE_COLOR 0
#endif

#ifdef ACS_S3
# define ACSNUM 32
#else
# define ACSNUM 25
#endif

/* Game states */
#define GAME_MENU 0
#define GAME_INTRO 1
#define GAME_PLAYING 2
#define GAME_DONE 3
#define GAME_LOAD 4

/* Game constants */
#define ENCOUNTER_RATE 30
#define HEAL_STEPS 5
#define MAP_WIDTH 600            /* Width in columns */
#define MAP_HEIGHT 600           /* Height in rows */
#define MAP_DISPLAY_WIDTH 58    /* Displayable portion of map */
#define MAP_DISPLAY_HEIGHT 18
#define XP_MULTIPLIER 100

#define WALL 0
#define GRAVEL 1
#define WATER 2

WINDOW *win;           /* our main window for displaying stuff */
WINDOW *stats;         /* stats window */
WINDOW *helpwindow;    /* help window */
WINDOW *encounterwin;  /* shown when an encounter occurs */

int game_state;         /* stores current game state */
int width, height;      /* width and height of window area */
bool quit = FALSE;      /* quit game flag */
int walk_counter = 0;   /* used to determine when to heal the player 1 HP */
int mapTopX;
int mapTopY;

/*
    Used to construct our random dungeons
    using cellular automata
*/
typedef struct sgenParams {
 	int r1_cutoff, r2_cutoff;
 	int reps;
} generation_params;

int **grid;
int **grid2;

int fillprob = 40;
int r1_cutoff = 5, r2_cutoff = 2;
int size_x = MAP_WIDTH, size_y = MAP_HEIGHT;
generation_params *params;
generation_params *params_set;
int generations;


/*  The player data structure */
typedef struct splayer {
    char player_name[8];    /* Name of player (set by the user at start time) */
    int gold;               /* Amount of gold player has */
    int x;                  /* X-coordinate on the game map */
    int y;                  /* Y-coordinate on the game map */
    int hp;                 /* Hit Points */
    int xp;                 /* Experience Points */
    int xp_earned;          /* Counter for leveling up */
    int xp_needed;          /* XP Needed */
    int str;                /* Strength (used in damage calculations) */
    int def;                /* Defense (used when being hit) */
    int agi;                /* Agility */
    int level;              /* Player level */
    int weapon;             /* Current weapon, using an array right now to keep it simple */
    int race;               /* Player's race (Human, Elf, Gnome, etc.) */
    int armor;              /* Armor rating */
    int armor_class;        /* Armor Class */
    bool is_dead;           /* Used to tell if the player has died */
    char *avatar;           /* Holder for player's on-screen avatar */
    int slain;              /* Number of enemies vanquised in battle */
    int attack_bonus;       /* Base attack bonus */
    bool critical_hit;      /* If the player performed a critical strike in combat */
} PLAYER;

PLAYER player;

typedef struct srace {
    const char *race_name;
    int hp_modifier;
    int str_modifier;
    int max_str;
    int max_def;
    int start_hp;
    int max_agi;
    int min_str;
} RACE;

#define MAX_RACES 3
RACE races[MAX_RACES] = {
  {"Human",0,0,18,18,13,18,20},
  {"Elf",0,0,15,19,20,20,7},
  {"Giant",5,5,25,15,12,10,10}
};

typedef struct sweapon {
    const char *weapon_name;
    int basehit;
    int base_str;
} WEAPON;

#define MAX_WEAPONS 3
WEAPON weapons[MAX_WEAPONS] = {
    {"Shortsword",6,10},
    {"Dagger",4,4},
    {"Broadsword",8,12}
};

/* Let's make some enemies to fight */
typedef struct senemy {
    const char *enemy_name;
    int level;
    int hp;
    int ac;
    int str;
    int def;
    int e_weapon;
    int max_gold;
    int exp;
    int armor;
} ENEMY;

#define MAX_ENEMIES 4
ENEMY enemies[MAX_ENEMIES] = {
  {"Goblin",1,15,8,10,12,1,2,10,4},
  {"Berserker",1,12,8,10,12,0,5,50,6},
  {"Orc",2,20,5,9,13,2,1,50,10},
  {"Evil Faerie",3,13,4,5,20,-1,0,50,2}
};

/* Structure used to hold menu commands and game states*/
typedef struct scommands {
	const char *text;
	int state;
} COMMAND;

/* Main menu structure */
#define MAX_OPTIONS 2
COMMAND command[MAX_OPTIONS] =
{
	{"New Game", GAME_INTRO},
	{"Quit", GAME_DONE}
};

void newGame();
void quitGame();
void drawStatsArea(void);
void drawPlayArea(void);
int statRoll(int);
void gameShutdown(void);
void gameLoop(void);
void updatePlayer(void);
void getPlayerInput(void);
int rollDie(int,int);
void showHelp(void);
void showEncounter(void);
void doBattle(void);
bool playerDidHit(ENEMY);
void clearPlay(void);
int levelUp(int);
void battleStats(ENEMY,int);
void enemyAttack(ENEMY,int);
void healPlayer(void);
void drawMap(void);
void buildMap(void);
void initMap(void);
int mapPick(void);
void generation(void);
int nextLevelXP(int);

/* Used to randomly pick a tile element on the map */
int mapPick() {
 	if(rand()%100 < fillprob) return WALL; else return GRAVEL;
}

/*
    Computes the next level's amount of XP needed
    and sticks it in the general "bucket"
*/
int nextLevelXP(int newLevel) {
    int i = newLevel;
    int sum = 0;

    while (i > 0) {
        sum += i;
        --i;
    }
    return (sum * XP_MULTIPLIER);
}

/*
    Checks to see if the player hit something
    Simulates a 1d20 roll with some modifiers
    If the player rolls a 20, they automatically hit
    If the player rolls 1, the automatically miss
*/
bool playerDidHit(ENEMY nme) {
    int attack_result = 0;
    int baseRoll = rollDie(1,20);
    player.critical_hit = FALSE;

    if (baseRoll == 1) return FALSE;
    if (baseRoll == 20) { player.critical_hit = TRUE; return TRUE;}
    attack_result = baseRoll + player.attack_bonus + player.str;
    if (attack_result > (nme.ac + nme.def)) return TRUE; else return FALSE;
}

void newGame() {
    int i;
    int key;
    int accepted;

    player.x = 2;
    player.y = 2;
    player.level = 1;
    player.race = 0;
    player.is_dead = FALSE;
    player.avatar = "@";
    player.slain = 0;
    player.xp = 0;
    player.xp_earned = 0;
    player.xp_needed = nextLevelXP(player.level);
    player.attack_bonus = 1;
    player.armor = 5;

    wclear(win);
    if (has_colors()) {
		init_pair(2, COLOR_WHITE, COLOR_BLUE);
		wbkgd(win, COLOR_PAIR(2) | A_BOLD);
	} else
        wbkgd(win, A_BOLD);

    wclear(win);
	box(win, ACS_VLINE, ACS_HLINE);
	wrefresh(win);

    echo();
    curs_set(1);
	mvwaddstr(win, 2, 2, "Character Name (8 letter max): ");
	wgetnstr(win, player.player_name, 8);

    if (player.player_name[0] == '\0') strcpy(player.player_name,"Mongo");

    curs_set(0);
    wrefresh(win);
    noecho();
    mvwaddstr(win,3,2,"Race: ");
    for (i = 0; i < MAX_RACES; i++) {
        mvwprintw(win,4+i,4,"[%i] - %s",i+1,races[i].race_name);
    }

    player.race = wgetch(win);
    while (player.race < 49 || player.race > 51) {
        player.race = wgetch(win);
    }

    /*
        Convert the ASCII character number to an index
        by shifting down 49 (makes 1 = 0, 2 = 1, etc.)
    */
    player.race -= 49;
    /* Set up basic player data */
    player.hp = (int)(races[player.race].start_hp + (player.level / 2));

    /*
        Do the stat rolls now.  If the player doesn't like the
        rolls with modifiers applied, they can do them over again
    */
    accepted = 0;
    while (accepted == 0) {
        player.str = statRoll(races[player.race].max_str);
        player.def = statRoll(races[player.race].max_def);
        player.agi = statRoll(races[player.race].max_agi);

        wclear(win);
        box(win, ACS_VLINE, ACS_HLINE);
        mvwprintw(win,2,2,"%s - %s", player.player_name,races[player.race].race_name);

        mvwprintw(win,4,15,"STR: %i",player.str);
        mvwprintw(win,5,15,"DEF: %i",player.def);
        mvwprintw(win,6,15,"AGI: %i",player.agi);

        wattrset(win,A_REVERSE);
        mvwprintw(win,10,15,"Enter/Space: Roll Again\tA - Accept Character");
        wattroff(win,A_REVERSE);

        flushinp();
        wrefresh(win);
        noecho();
        keypad(win, TRUE);
        raw();

        key = wgetch(win);

        switch(key) {
            case 10:
            case 13:
            case ' ':
            case KEY_ENTER:
                accepted = 0;
                break;
            case 'a':
            case 'A':
                accepted = 1;
                break;
        }

    }

    clearPlay();
    mvwprintw(win,4,20,"Generating Dungeon...");
    wrefresh(win);
    buildMap();

    /*
        Resize the main window so we can use it
        as the gameplay area.
    */
    resize_window(win,20,60);

    drawPlayArea();
    drawStatsArea();
    gameLoop();
}

void initMap() {
    int xi, yi;

	grid  = (int**)malloc(sizeof(int*) * MAP_HEIGHT);
	grid2 = (int**)malloc(sizeof(int*) * MAP_HEIGHT);

	for(yi=0; yi<size_y; yi++) {
		grid [yi] = (int*)malloc(sizeof(int) * MAP_WIDTH);
		grid2[yi] = (int*)malloc(sizeof(int) * MAP_WIDTH);
	}

	for(yi=1; yi<MAP_HEIGHT-1; yi++)
        for(xi=1; xi<MAP_WIDTH-1; xi++)
            grid[yi][xi] = mapPick();

	for(yi=0; yi<MAP_HEIGHT; yi++)
        for(xi=0; xi<MAP_WIDTH; xi++)
            grid2[yi][xi] = WALL;

	for(yi=0; yi<MAP_HEIGHT; yi++)
		grid[yi][0] = grid[yi][size_x-1] = WALL;
	for(xi=0; xi<MAP_WIDTH; xi++)
		grid[0][xi] = grid[size_y-1][xi] = WALL;

}

/*
    Construct a random dungeon
*/
void buildMap() {
    int ii;
    int jj;

    mapTopX = 0;
    mapTopY = 0;
    initMap();

    generations = 1;
    fillprob = 35;
    params = params_set = (generation_params*)malloc( sizeof(generation_params) * generations);

    params->r1_cutoff = 24;
    params->r2_cutoff = 5;
    params->reps = 1;
    params++;

    params->r1_cutoff = 20;
    params->r2_cutoff = 3;
    params->reps = 1;
    params++;

 	for(ii=0; ii<generations; ii++) {
 		params = &params_set[ii];
 		for(jj=0; jj<params->reps; jj++)
 			generation();
 	}

    free(params);
    free(params_set);
}

/*
    Computes a "generation" of a map
    using cellular automata
*/
void generation(void) {
    int xi, yi, ii, jj;

    for(yi=1; yi<MAP_HEIGHT-1; yi++)
        for(xi=1; xi<MAP_WIDTH-1; xi++) {
            int adjcount_r1 = 0, adjcount_r2 = 0;
            for(ii=-1; ii<=1; ii++)
                for(jj=-1; jj<=1; jj++) {
                    if(grid[yi+ii][xi+jj] != GRAVEL)
                        adjcount_r1++;
                }
                for(ii=yi-2; ii<=yi+2; ii++)
                    for(jj=xi-2; jj<=xi+2; jj++) {
                        if(abs(ii-yi)==2 && abs(jj-xi)==2)
                            continue;
                        if(ii<0 || jj<0 || ii>=size_y || jj>=size_x)
                            continue;
                        if(grid[ii][jj] != GRAVEL)
                            adjcount_r2++;
                    }
                    if(adjcount_r1 >= params->r1_cutoff || adjcount_r2 <= params->r2_cutoff)
                        grid2[yi][xi] = WALL;
                    else
                        grid2[yi][xi] = GRAVEL;
        }
        for(yi=1; yi<size_y-1; yi++)
            for(xi=1; xi<size_x-1; xi++)
                grid[yi][xi] = grid2[yi][xi];
}

void updatePlayer() {
    if (player.hp <= 0) player.is_dead = TRUE;

    if (rand()%100 == ENCOUNTER_RATE) showEncounter();
}

void getPlayerInput() {
    int key;

    int oldX = player.x;
    int oldY = player.y;

    flushinp();
    keypad(win, TRUE);
    raw();

    key = wgetch(win);

    switch(key) {
        case 10:
        case 13:
        case KEY_ENTER:
            break;
        case KEY_PPAGE:
        case KEY_HOME:
            break;
        case KEY_F(1):
            showHelp();
            break;
        case KEY_NPAGE:
        case KEY_END:
            break;

        case KEY_UP:
            if (player.y > 1) {
                player.y--;
                walk_counter++;
            } else {
                mapTopY--;
            }
            break;
        case KEY_DOWN:
            if (player.y < MAP_DISPLAY_HEIGHT - 1) {
                player.y++;
                walk_counter++;
            } else {
                mapTopY++;
            }
            break;
        case KEY_RIGHT:
            if (player.x < MAP_DISPLAY_WIDTH - 1) {
                ++player.x;
                walk_counter++;
            } else {
                mapTopX++;
            }
            break;
        case KEY_LEFT:
            if (player.x > 1) {
                player.x--;
            } else {
                mapTopX--;
            }
            break;
        case 'Q':
        case 'q':
            quit = TRUE;
            game_state = GAME_MENU;
            break;
    }

    if (mapTopY < 0) mapTopY = 0;
    if (mapTopX < 0) mapTopX = 0;

    if (mapTopY + MAP_DISPLAY_HEIGHT > MAP_HEIGHT) {
        mapTopY--;
    }

    if (mapTopX + MAP_DISPLAY_WIDTH > MAP_WIDTH) {
        mapTopX--;
    }

    /*
        Give the player 1 HP back
    */
    if (walk_counter >= HEAL_STEPS) {
        walk_counter = 0;
        healPlayer();
    }

    /*
        If the tile on the map at the new player coordinates
        is a WALL type, we reset the player.
    */
    if (grid[player.y - 1 + mapTopY][player.x - 1 + mapTopX] == WALL) {
        player.x = oldX;
        player.y = oldY;
    }

}

/* Gives a player back 1 HP */
void healPlayer() {
    int max_hp = (int)(races[player.race].start_hp + (player.level / 2));
    if (player.hp < max_hp) {
        player.hp++;
    }
}

void gameLoop() {
    while (game_state != GAME_DONE && quit != TRUE && player.is_dead == FALSE) {
        getPlayerInput();
        if (quit == FALSE) {
            updatePlayer();
            drawPlayArea();
            drawStatsArea();
        }
    }
}

int rollDie(int min, int max) {
    double r;
    double x;
    r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
	x = (r * max);
    return (int)x + min;
}

/*
    Stat rolls should be greater than 5
    for all stats according to D&D rules
*/
int statRoll(int max) {
    int val = 0;
    while (val <= 5) {
        val = rollDie(1,max);
    }
    return val;
}

void showEncounter() {
    wclear(encounterwin);
    wbkgd(encounterwin,COLOR_PAIR(4) | A_BOLD);
    box(encounterwin,ACS_VLINE,ACS_HLINE);
    mvwprintw(encounterwin,1,1,"!!! ENCOUNTER !!!");
    wrefresh(encounterwin);
    napms(1500);
    doBattle();
}

void clearPlay() {
    wbkgd(win,COLOR_PAIR(0) | A_DIM);
    wclear(win);
    box(win,ACS_VLINE,ACS_HLINE);
}

/*
    Checks to see if the player has leveled up
    and acts accordingly. This has been modified
    now to use 3.5 Edition D&D rules
    See nextLevelXP() for the calculation
*/
int levelUp(int newXP) {
    int newXPNeeded;
    player.xp += newXP;
    player.xp_earned += newXP;
    newXPNeeded = nextLevelXP(player.level);

    if (player.xp_earned >= newXPNeeded) {
        player.xp_earned = player.xp_earned - newXPNeeded;
        player.xp_needed = nextLevelXP(player.level + 1);
        player.level++;
        player.str++;
        player.def++;
        player.attack_bonus++;
        player.hp = (int)(races[player.race].start_hp + (player.level / 2));
        return 1;
    } else return 0;
}

void doBattle() {
    int round = 1;
    int nme_idx;
    int gold_amt = 0;
    int expGiven = 0;
    int key;
    int damage = 0;
    int canFlee = 0;
    bool didFlee = FALSE;
    ENEMY the_enemy;

    /* Pick an enemy randomly from the list */
    nme_idx = rollDie(0,MAX_ENEMIES);
    if (nme_idx == MAX_ENEMIES) nme_idx = MAX_ENEMIES - 1;

    /*
        Keep a temporary copy so we can kill
        it without hurting the prototypes
    */
    the_enemy = enemies[nme_idx];

    /*
        If the enemy has an e_weapon of -1
        we give them one randomly
    */
    if (the_enemy.e_weapon == -1) {
        nme_idx = rollDie(0,MAX_WEAPONS);
        if (nme_idx == MAX_WEAPONS) nme_idx = MAX_WEAPONS - 1;
        the_enemy.e_weapon = nme_idx;
    }

    /* Calculate victory condition values */
    expGiven = rollDie(1,the_enemy.exp);
    if (the_enemy.max_gold != 0) gold_amt = rollDie(1,the_enemy.max_gold);

    while (the_enemy.hp > 0 && player.is_dead == FALSE && player.hp > 0 && didFlee == FALSE) {
        flushinp();
        nodelay(win,FALSE);
        battleStats(the_enemy,round);

        wattrset(win,A_BOLD);
        mvwprintw(win,7,12,"[A]ttack");
        mvwprintw(win,8,12,"[F]lee");
        wattrset(win,A_NORMAL);

        noecho();
        keypad(win, TRUE);
        raw();

        key = wgetch(win);
        flushinp();

        switch(key) {
            case 'F':
            case 'f':
                canFlee = rollDie(1,6) + rollDie(1,6) + rollDie(1,6);
                if (player.agi > canFlee) {
                    battleStats(the_enemy,round);
                    mvwprintw(win,10,3,"%s flees from the battle!",player.player_name);
                    wrefresh(win);
                    napms(500);
                    didFlee = TRUE;
                    walk_counter = 0;
                } else {
                    battleStats(the_enemy,round);
                    mvwprintw(win,10,3,"%s attempts to flee but fails!",player.player_name);
                    wrefresh(win);
                    napms(1000);
                    enemyAttack(the_enemy,round);
                    round++;
                }
                break;
            case 'A':
            case 'a':
                /* keypad(win,FALSE); */
                battleStats(the_enemy,round);
                mvwprintw(win,10,3,"%s attacks the %s...",player.player_name,the_enemy.enemy_name,weapons[player.weapon].weapon_name);
                /* napms(750); */
                if (playerDidHit(the_enemy) == TRUE) {
                    damage = (player.str + weapons[player.weapon].basehit + player.attack_bonus + rollDie(1,weapons[player.weapon].base_str)) - (the_enemy.def + the_enemy.armor);
                    if (damage <= 0) {
                        mvwprintw(win,11,5,"...%s's strike glances off the %s!",player.player_name,the_enemy.enemy_name);
                    } else {
                        mvwprintw(win,11,5,"...%s hits the %s for %i damage!",player.player_name,the_enemy.enemy_name,damage);
                        the_enemy.hp -= damage;
                    }
                } else {
                    mvwprintw(win,11,5,"...%s misses!",player.player_name);
                }
                wrefresh(win);
                napms(750);
                if (the_enemy.hp > 0) {
                    enemyAttack(the_enemy,round);
                    round++;
                }
                break;
            case 'k':
            case 'K':
                the_enemy.hp = 0;
                break;
            default:
                wattrset(win,A_REVERSE);
                mvwprintw(win,10,10,"Invalid Option. Try again.");
                wattrset(win,A_NORMAL);
                wrefresh(win);
                napms(500);
                break;
        }
        if (player.hp < 0) player.hp = 0;
        if (player.hp == 0) player.is_dead = TRUE;
        drawStatsArea();
    }

    flushinp();
    if (player.is_dead == TRUE) {
        wbkgd(win,COLOR_PAIR(4) | A_DIM);
        wclear(win);
        box(win,ACS_VLINE,ACS_HLINE);
        mvwprintw(win,10,20,"Thou art dead!");
        mvwprintw(win,12,20,"Press any key...");
        wrefresh(win);
        wgetch(win);
        game_state = GAME_DONE;
    } else if (didFlee == FALSE) {
        /* wclear(encounterwin); */
        if (player.gold + gold_amt >= INT_MAX) player.gold = INT_MAX; else player.gold += gold_amt;
        player.slain++;
        walk_counter = 0;
        wbkgd(win,COLOR_PAIR(1) | A_DIM);
        wclear(win);
        box(win,ACS_VLINE,ACS_HLINE);

        mvwprintw(win,3,10,"Victory!");
        mvwprintw(win,5,5,"%s hast slain the %s!",player.player_name,the_enemy.enemy_name);
        mvwprintw(win,6,5,"%s found %i gold on the %s",player.player_name,gold_amt,the_enemy.enemy_name);
        if (expGiven > 0)
            mvwprintw(win,7,5,"%s gained %i XP from the battle",player.player_name,expGiven);

        if (levelUp(expGiven) == 1) {
            wattrset(win,A_BOLD);
            mvwprintw(win, 9, 5,"%s has gained a level!",player.player_name);
            wattrset(win,A_NORMAL);
        }
        wattrset(win,A_REVERSE);
        mvwprintw(win,13,10,"Press any key to continue thine journey...");
        wattrset(win,A_NORMAL);
        wrefresh(win);
        drawStatsArea();
        wgetch(win);
    }
}

/* Performs an enemy attack on a player */
void enemyAttack(ENEMY the_enemy, int round) {
    int inRoll = 0;
    int attack_result = 0;
    int damage = 0;

    battleStats(the_enemy,round);
    mvwprintw(win,10,3,"The %s attacks %s with a %s...",the_enemy.enemy_name,player.player_name,weapons[the_enemy.e_weapon].weapon_name);
    napms(500);
    inRoll = rollDie(1,20);

    if (inRoll == 1) mvwprintw(win,11,5,"...the %s misses!",the_enemy.enemy_name);

    attack_result = inRoll + the_enemy.str;
    if (inRoll == 20 || attack_result > (player.def + player.armor)) {

        damage = (the_enemy.str + weapons[the_enemy.e_weapon].basehit + rollDie(1,weapons[the_enemy.e_weapon].base_str)) - (player.def + player.armor);
        if (damage <= 0) {
            mvwprintw(win,11,5,"...the strike glances off %s's armor",player.player_name);
        } else {
            mvwprintw(win,11,5,"...the %s deals %i damage to %s!",the_enemy.enemy_name,damage,player.player_name);
            player.hp -= damage;
        }
    } else {
        mvwprintw(win,11,5,"...the %s misses!",the_enemy.enemy_name);
    }

    wrefresh(win);
    napms(500);
}

void battleStats(ENEMY the_enemy,int round) {
    clearPlay();
    mvwprintw(win,1,1,"Battle - Round %i",round);
    wattrset(win,A_REVERSE);
    mvwprintw(win,2,1,"%s",the_enemy.enemy_name);
    wattrset(win,A_NORMAL);
    mvwprintw(win,3,1,"Level: %i",the_enemy.level);
    mvwprintw(win,4,1,"HP: %i",the_enemy.hp);
}

void drawPlayArea() {
    clearPlay();
    drawMap();
    wattron(win,COLOR_PAIR(10) | A_BOLD);
    mvwprintw(win,player.y,player.x,player.avatar);
    wattroff(win,COLOR_PAIR(10)| A_BOLD);
    wrefresh(win);
}

void drawMap() {
    int XIndex, YIndex;

    for (YIndex = 0; YIndex < MAP_DISPLAY_HEIGHT; YIndex++) {
        for (XIndex = 0; XIndex < MAP_DISPLAY_WIDTH; XIndex++) {
            if (YIndex + mapTopY < MAP_HEIGHT || XIndex+mapTopX < MAP_WIDTH) {
                switch(grid[YIndex+mapTopY][XIndex+mapTopX]) {
                    case WALL:
                        wattrset(win,COLOR_PAIR(4));
                        mvwaddrawch(win,YIndex+1,XIndex+1,ACS_BOARD);
                        wattroff(win,COLOR_PAIR(4));
                        break;
                    case GRAVEL:
                        wattrset(win,COLOR_PAIR(11) | A_NORMAL);
                        mvwprintw(win,YIndex+1,XIndex+1,".");
                        wattroff(win,COLOR_PAIR(11) | A_NORMAL);
                        break;
                }
            }
        }
    }
}

void drawStatsArea() {
    wclear(stats);
    wbkgd(stats,COLOR_PAIR(0) | A_DIM);
    box(stats,ACS_VLINE,ACS_HLINE);
    wattrset(stats,A_BOLD);
    mvwprintw(stats, 1, 1, player.player_name);
    wattrset(stats,A_NORMAL);

    mvwprintw(stats, 2, 1, "%s", races[player.race].race_name);
    mvwprintw(stats, 3, 1, "Gold: %i", player.gold);
    mvwprintw(stats, 4, 1, "Level: %i", player.level);
    mvwprintw(stats, 5, 1, "HP: %i", player.hp);
    mvwprintw(stats, 6, 1, "XP: %i", player.xp);
    mvwprintw(stats, 7, 1, "STR: %i", player.str);
    mvwprintw(stats, 8, 1, "DEF: %i", player.def);
    mvwprintw(stats, 9, 1, "AGI: %i", player.agi);

    mvwhline(stats,16,1,ACS_HLINE,18);
    mvwprintw(stats, 17, 1, "Slain: %i",player.slain);
    mvwprintw(stats, 18, 1, "XP Needed: %i", player.xp_needed - player.xp_earned);
    wrefresh(stats);
}

void showHelp() {
    flushinp();
    wclear(helpwindow);
    wbkgd(helpwindow,COLOR_PAIR(2) | A_DIM);
    wborder(helpwindow,0,0,0,0,0,0,0,0);
    wattrset(helpwindow,A_BOLD);
    mvwprintw(helpwindow,0,2,"Help");
    mvwprintw(helpwindow,2,2,"Q - Quit");
    mvwprintw(helpwindow,3,2,"F1 - Help (this screen)");
    mvwprintw(helpwindow,4,2,"Left Arrow - Move Left");
    mvwprintw(helpwindow,5,2,"Right Arrow - Move Right");
    mvwprintw(helpwindow,6,2,"Down Arrow - Move Down");
    mvwprintw(helpwindow,7,2,"Up Arrow - Move Up");
    wattrset(helpwindow,A_REVERSE);
    mvwprintw(helpwindow,18,2,"Press any key to resume");
    wattrset(helpwindow,A_NORMAL);
    wrefresh(helpwindow);
    wgetch(helpwindow);
}

void quitGame() {
    quit = TRUE;
}

void display_menu(int old_option, int new_option) {
	int lmarg = (COLS - 14) / 2,
		tmarg = (LINES - (MAX_OPTIONS + 2)) / 2;
    int i;

    wclear(win);
    box(win,ACS_VLINE,ACS_HLINE);
    wattrset(win,A_BOLD);
    mvwaddstr(win,tmarg - 2, lmarg - 5, "Dungcrwl - A Beginner's RPG");
    wattrset(win,A_NORMAL);

	if (old_option == -1) {
		for (i = 0; i < MAX_OPTIONS; i++) mvwaddstr(win,tmarg + i, lmarg, command[i].text);
	} else mvwaddstr(win,tmarg + old_option, lmarg, command[old_option].text);

	wattrset(win,A_REVERSE);
	mvwaddstr(win,tmarg + new_option, lmarg, command[new_option].text);
	wattrset(win,A_NORMAL);
	wrefresh(win);
}

/*  Initializes PDCurses and game startup */
int initGame(int argc, char *argv[]) {
    int c;

    srand (time(NULL));
	initscr();
    raw();
    nodelay(stdscr,1);
    noecho();
    curs_set(0);
    nonl();

	if (has_colors()) start_color();

    for (c = 0; c < COLORS; c++) {
        if (c == COLOR_WHITE) init_pair(c,COLOR_BLACK,c);
        else init_pair(c,COLOR_WHITE,c);
    }

    /* Special color pair used for drawing player */
    init_pair(10,COLOR_YELLOW,COLOR_BLACK);
    init_pair(11,COLOR_GREEN,COLOR_BLACK);

	width  = 80;
	height = 20;

    clear();
    refresh();

    /* Resize the terminal window */
    resize_term(height,width);

	win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
	helpwindow = newwin(0,0,0,0);
	stats = newwin(20,20,0,60); /* create the character stats window */
	encounterwin = newwin(3,19,10,20);

	if (win == NULL || stats == NULL || helpwindow == NULL) {
		endwin();
		return 1;
	}

    savetty();
    resize_term(0,0);
    game_state = GAME_MENU;
	return 0;
}

void gameShutdown() {
	free(grid);
	free(grid2);
	endwin();
}

/*
    Draws the main menu
*/
void showMenu() {
    int key, old_option = -1, new_option = 0;
	display_menu(old_option, new_option);

	while (quit == FALSE && game_state == GAME_MENU) {
		noecho();
		keypad(stdscr, TRUE);
		raw();

		key = getch();

		switch(key) {
            case 10:
            case 13:
            case KEY_ENTER:
                old_option = -1;
                erase();
                wrefresh(win);
                game_state = command[new_option].state;
                erase();
                display_menu(old_option, new_option);
                break;

            case KEY_PPAGE:
            case KEY_HOME:
                old_option = new_option;
                new_option = 0;
                display_menu(old_option, new_option);
                break;

            case KEY_NPAGE:
            case KEY_END:
                old_option = new_option;
                new_option = MAX_OPTIONS - 1;
                display_menu(old_option, new_option);
                break;

            case KEY_UP:
                old_option = new_option;
                new_option = (new_option == 0) ?
                    new_option : new_option - 1;
                display_menu(old_option, new_option);
                break;

            case KEY_DOWN:
                old_option = new_option;
                new_option = (new_option == MAX_OPTIONS - 1) ?
                    new_option : new_option + 1;
                display_menu(old_option, new_option);
                break;
        #ifdef KEY_RESIZE
            case KEY_RESIZE:
        #ifdef PDCURSES
                resize_term(0, 0);
        #endif
                old_option = -1;
                erase();
                display_menu(old_option, new_option);
                break;
    #endif
            case 'Q':
            case 'q':
                quit = TRUE;
                game_state = GAME_DONE;
                break;
		}
	}
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL,"");
    PDC_set_title("Dungcrwl");

	if (initGame(argc, argv))
		return 1;

    while (quit == FALSE) {
        switch(game_state) {
            case GAME_MENU:
                showMenu();
                break;
            case GAME_INTRO:
                newGame();
                break;
            case GAME_PLAYING:
                gameLoop();
                break;
            case GAME_DONE:
                quit = TRUE;
                break;
        }
    }

    gameShutdown();

	return 0;
}
