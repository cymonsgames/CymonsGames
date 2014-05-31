/*
    Perils of Pothril -- Beginner's RPG with C and Curses
    Uses some of the basic game mechanics from D&D and a few others
*/

#ifndef _XOPEN_SOURCE_EXTENDED
# define _XOPEN_SOURCE_EXTENDED 1
#endif

/*
#if defined(__WIN32__) || defined(__WINNT__) || defined(__NT__)
    #ifndef WIN32
        #define WIN32
    #endif
#endif
*/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <limits.h>
#include "coord.h"
#include "dice.h"
#include "level.h"
#include "treasure.h"
#include "races.h"
#include "weapons.h"
#include "player.h"
#include "monsters.h"
#include "items.h"
#include "shop.h"
#include "tiles.h"

/*
#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NONLS
    #define NOSERVICE
    #define NOKANJI
    #define NOMCX
#endif
*/

#ifdef WACS_S1
    #define HAVE_WIDE 1
#else
    #define HAVE_WIDE 0
#endif

#if HAVE_WIDE
    #include <wchar.h>
#endif

#if defined(PDCURSES) && !defined(XCURSES)
    #define HAVE_RESIZE 1
#else
    #define HAVE_RESIZE 0
#endif

#ifdef A_COLOR
    #define HAVE_COLOR 1
#else
    #define HAVE_COLOR 0
#endif

#ifdef ACS_S3
    #define ACSNUM 32
#else
    #define ACSNUM 25
#endif

/* Game states */
#define GAME_MENU 0
#define GAME_INTRO 1
#define GAME_PLAYING 2
#define GAME_DONE 3
#define GAME_LOAD 4

/* Game constants */
#define ENCOUNTER_RATE 45
#define HEAL_STEPS 8
#define XP_MULTIPLIER 100

/* Popup message durations */
#define DURATION_SHORT 500
#define DURATION_MEDIUM 1000
#define DURATION_LONG 1500

WINDOW *win;           /* our main window for displaying stuff */
WINDOW *stats;         /* stats window */
WINDOW *helpwindow;    /* help window */
WINDOW *encounterwin;  /* shown when an encounter occurs */
WINDOW *infowin;       /* information window popup */
WINDOW *charsheet;     /* character sheet */
WINDOW *inventorywin;  /* inventory display */

#if DEBUGLOG
    FILE *f_log;
#endif

int game_state;         /* stores current game state */
int width, height;      /* width and height of window area */
bool quit = FALSE;      /* quit game flag */
int walk_counter = 0;   /* used to determine when to heal the player 1 HP */
int encounter_count = 0;

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
void drawStatsArea();
void drawPlayArea();
int damageModifier(int str);
void gameShutdown();
void gameLoop();
void showHelp();
void doBattle();
long nextLevelXP(int newLevel);
void init_player();
int levelUp(long newXP);
int damageRoll(int dmg_cnt, int dmg_rnd);
int getStrengthModifier(int str);

void doShop() {
    wclear(win);
    wbkgd(win,COLOR_PAIR(6));
    box(win,ACS_VLINE,ACS_HLINE);
    wattrset(win,A_BOLD);
    mvwprintw(win,0,18,"%s Shop",cur_map->level_name);
    wattrset(win,A_NORMAL);
    wrefresh(win);
    wgetch(win);
    player.y++;
}

void endGame() {
    int key;

    wclear(win);
    wresize(win,height,width);
    wbkgd(win,COLOR_PAIR(4) | A_DIM);
    wclear(win);
    box(win,ACS_VLINE,ACS_HLINE);
    wattrset(win,A_BOLD);
    mvwprintw(win,3,25,"Thou Art Dead!");
    mvwprintw(win,7,25,"Press Any Key");
    wattrset(win,A_NORMAL);
    key = wgetch(win);

    player.is_dead = TRUE;
    game_state = GAME_DONE;
    quit = TRUE;
}

void init_player() {
    player.level = 1;
    player.is_dead = FALSE;
    player.avatar = 0x02;
    player.slain = 0;
    player.xp = 0;
    player.xp_earned = 0;
    player.xp_needed = nextLevelXP(player.level);
    player.attack_bonus = 0;
    player.armor = 10;
    player.has_chalice = FALSE;
    player.weapon = 0;
    player.gold = 0;
    player.status = STATUS_OK; /*STATUS_CONFUSED | STATUS_HUNGRY | STATUS_POISON;*/
    player.nutrition = 2000;
    walk_counter = 0;
}

char *formatLong(long num) {
    static int comma = '\0';
    static char retbuf[30];
    char *p = &retbuf[sizeof(retbuf)-1];
    int i = 0;

    if(comma == '\0') {
        struct lconv *lcp = localeconv();
        if(lcp != NULL) {
            if(lcp->thousands_sep != NULL &&
                *lcp->thousands_sep != '\0')
            comma = *lcp->thousands_sep;
            else comma = ',';
        }
    }

    *p = '\0';

    do {
        if(i%3 == 0 && i != 0)
            *--p = comma;
        *--p = '0' + num % 10;
        num /= 10;
        i++;
    } while(num != 0);

    return p;
}


void showInventory() {
    ITEM *elt = master_item_list;
    int yPos = 2;

    wclear(inventorywin);
    wbkgd(inventorywin,COLOR_PAIR(0));
    box(inventorywin,ACS_VLINE,ACS_HLINE);
    wattrset(inventorywin,A_BOLD);
    mvwprintw(inventorywin,0,27,"Inventory");
    wattrset(inventorywin,A_NORMAL);

    while (elt != NULL) {
        if (elt->qty > 0) {
            wattrset(inventorywin,COLOR_PAIR(elt->color));
            wattrset(inventorywin,A_NORMAL);
            mvwprintw(inventorywin,yPos,4,"%s: %i",elt->item_name, elt->qty);
            yPos++;
        }
        elt = elt->next;
    }

    wattrset(win,A_REVERSE);
    mvwprintw(inventorywin,23,27,"Press any key");
    wattrset(win,A_NORMAL);
    wgetch(inventorywin);
}

void inspectCharacter() {
    int yLoc = 4;
    char buf[128];
    memset(buf,0,sizeof(buf));
    sprintf(buf,"XP Needed: %s",formatLong(player.xp_needed - player.xp_earned));

    wclear(charsheet);
    wbkgd(charsheet,COLOR_PAIR(1) | A_BOLD);
    box(charsheet,ACS_VLINE,ACS_HLINE);
    mvwprintw(charsheet,0,30,"[%s]",player.player_name);
    wattrset(charsheet,A_NORMAL);

    mvwprintw(charsheet,1,width-strlen(buf)-1,buf);
    memset(buf,0,sizeof(buf));
    sprintf(buf,"Monsters Slain: %s",formatLong(player.slain));
    mvwprintw(charsheet,2,width-strlen(buf)-1,buf);

    mvwprintw(charsheet,1,2,"%s - Level %i",races[player.race].race_name, player.level);

    mvwprintw(charsheet,2,2,"XP: %s",formatLong(player.xp));
    mvwprintw(charsheet,3,2,"HP: %i (%i)",player.hp,player.max_hp);

    mvwprintw(charsheet,5,2,"STR: %i", player.str);
    mvwprintw(charsheet,6,2,"DEX: %i", player.dex);
    mvwprintw(charsheet,7,2,"AGI: %i", player.agi);
    mvwprintw(charsheet,8,2,"CON: %i", player.con);
    mvwprintw(charsheet,9,2,"INT: %i", player.intel);
    mvwprintw(charsheet,10,2,"WIS: %i", player.wis);
    mvwprintw(charsheet,11,2,"CHA: %i", player.cha);

    mvwprintw(charsheet,yLoc,60,"Status:");
    yLoc++;
    if (player.status & STATUS_OK) {
        mvwprintw(charsheet,yLoc,63,"OK");
    } else {
        if (player.status & STATUS_BLIND) {
            wattrset(charsheet,A_DIM | A_UNDERLINE);
            mvwprintw(charsheet,yLoc,63,"Blind");
            wattrset(charsheet,A_NORMAL);
            yLoc++;
        }

        if (player.status & STATUS_CONFUSED) {
            mvwprintw(charsheet,yLoc,63,"desufnoC");
            yLoc++;
        }

        if (player.status & STATUS_POISON) {
            wattrset(charsheet,COLOR_PAIR(13) | A_BOLD);
            mvwprintw(charsheet,yLoc,63,"Poisoned");
            wattrset(charsheet,A_NORMAL);
            yLoc++;
        }

        if (player.status & STATUS_HUNGRY) {
            mvwprintw(charsheet,yLoc,63,"Hungry");
            yLoc++;
        }
    }

    wattrset(charsheet,A_BOLD);
    mvwprintw(charsheet,height-2,width/2 - 20,"Press any key...");
    wattrset(charsheet,A_NORMAL);
    wrefresh(charsheet);
    wgetch(charsheet);
}

/*
    Selects an enemy within a certain level
    of the character, so as to keep it from
    being a non-stop pummeling
*/
int pickEnemy() {
    int rnd;
    bool enemy_acceptable = FALSE;

    int spread = (int)(player.level - player.level * .1 + rollDie(1,player.level/5));

    #if DEBUGLOG
        fprintf(f_log,"Picking enemy -- Spread is %i -- ",spread);
    #endif

    do {
        rnd = rollDie(1,MAX_ENEMIES);
        if (rnd == MAX_ENEMIES) rnd = MAX_ENEMIES - 1;
        if (enemies[rnd].level <= (player.level + spread)) enemy_acceptable = TRUE;
    } while (enemy_acceptable == FALSE);

    #if DEBUGLOG
        fprintf(f_log,"selected %s (Level %i)\n",enemies[rnd].enemy_name, enemies[rnd].level);
    #endif

    return rnd;
}


/*
    Set up the enemy to fight the player
*/
void setupEnemy(ENEMY *nme, int *gold_amt) {
    int exp_modifier;
    int i;

    nme->hp = 0;
    for (i = 0; i < nme->level; i++) {
        nme->hp += rollDie(1,8);
    }

    /* Calculate various conditions here in advance */
    *gold_amt = 0;

    if (nme->carry > 0) {
        if (nme->carry == 100) {
            *gold_amt = rollDie(1,10);
        } else {
            if (rollDie(1,nme->carry) > rollDie(1,100)) {
                *gold_amt = rollDie(1,10);
            }
        }
    }

    if (nme->level == 1) {
        exp_modifier = nme->hp / 8;
    } else {
        exp_modifier = nme->hp / 6;
    }

    if (nme->level > 9) {
        exp_modifier *= 20;
    } else if (nme->level > 6) {
        exp_modifier *= 4;
    }

    nme->exp = nme->exp + exp_modifier;

    /* Enemy strength is based on level */
    nme->str = 10 + (nme->level - nme->level * .1 + rollDie(1,nme->level/5));
    /*nme->str = (nme->level + (nme->hp / 3));*/
}

void setEncounterRate() {
    encounter_count = rollDie(1,ENCOUNTER_RATE) + rollDie(1,ENCOUNTER_RATE) + 1;
}


void popupInfo(char *msg, int msg_duration) {
    wresize(infowin,3,strlen(msg)+2);
    wclear(infowin);
    wbkgd(infowin,COLOR_PAIR(3) | A_BOLD);
    /*box(infowin,ACS_VLINE,ACS_HLINE);*/
    mvwprintw(infowin,1,1,msg);
    wrefresh(infowin);
    napms(msg_duration);
}

void makeTreasure() {
    int i = 0;
    int some_x;
    int some_y;
    int some_amt;

    #if DEBUGLOG
        fprintf(f_log,"in makeTreasure()...\n");
        fflush(f_log);
    #endif

    while (i < MAX_TREASURES) {
        some_amt = randMap(1,TREASURE_AMT_MAX);
        some_x = randMap(1,cur_map->width-2);
        some_y = randMap(1,cur_map->height-2);

        if (cur_map->grid[some_y][some_x] != WALL
                && cur_map->grid[some_y+1][some_x] != WALL
                && cur_map->grid[some_y][some_x+1] != WALL
                && cur_map->grid[some_y-1][some_x] != WALL
                && cur_map->grid[some_y][some_x+1] != WALL
                && !treasureAt(some_x,some_y) ) {

            addTreasure(some_x,some_y,some_amt);
            i++;
        }
    }

    #if DEBUGLOG
        dumpList(f_log);
    #endif
}

/*
    Computes the next level's amount of XP needed
    and sticks it in the general "bucket"
*/
long nextLevelXP(int newLevel) {
    int i = newLevel;
    int sum = 0;

    while (i > 0) {
        sum += i;
        --i;
    }

/*
    #if DEBUGLOG
        fprintf(f_log,"nextLevelXP: %i\n",sum * XP_MULTIPLIER);
    #endif
*/
    return (long)(sum * XP_MULTIPLIER);
}

/*
    Checks to see if the player hit something
    Simulates a 1d20 roll with some modifiers
    If the player rolls a 20, they automatically hit
    If the player rolls 1, they automatically miss
*/
bool playerDidHit(ENEMY nme) {
    int attack_result = 0;
    int baseRoll = rollDie(1,20);
    int modifiers = 0;
    player.critical_hit = FALSE;

    if (baseRoll == 1) return FALSE;
    if (baseRoll == 20) {
        int secondRoll = rollDie(1,20);
        if (secondRoll == 20) {
            player.critical_hit = TRUE;
            return TRUE;
        }
    }

    attack_result = baseRoll + player.attack_bonus + getStrengthModifier(player.str);

    if (nme.flags & NME_INVIS) {
        modifiers += 2;
    }

    if (attack_result > (nme.level + (-(nme.armor - 11)) + modifiers) ) return TRUE; else return FALSE;
}

void clearPlay() {
    wbkgd(win,COLOR_PAIR(0) | A_DIM);
    wclear(win);
    box(win,ACS_VLINE,ACS_HLINE);
}

void newGame() {
    int i;
    int key;
    int accepted;

    init_player();
    setEncounterRate();
    init_items();

    player.player_name = (char *)malloc(sizeof(char)*9);
    memset(player.player_name,0,9);
    player.race = 0;
    wclear(win);
    wresize(win,height,width);
    if (has_colors()) {
        init_pair(2, COLOR_WHITE, COLOR_BLUE);
        wbkgd(win, COLOR_PAIR(2) | A_BOLD);
    } else
        wbkgd(win, A_BOLD);
    wclear(win);
    box(win, ACS_VLINE, ACS_HLINE);
    wrefresh(win);

    echo();
    curs_set(2);
    mvwaddstr(win, 2, 2, "Character Name (8 letter max): ");
    wgetnstr(win, player.player_name, 8);

    if (strlen(player.player_name) == 0) strcpy(player.player_name,"Mongo");
    /*player.player_name[8] = 0;*/
    curs_set(0);
    wclear(win);
    box(win,ACS_VLINE,ACS_HLINE);
    wrefresh(win);
    noecho();
    mvwaddstr(win,1,2,"Race: ");
    for (i = 0; i < MAX_RACES; i++) {
        mvwprintw(win,2+i,4,"[%i] - %s",i+1,races[i].race_name);
    }

    player.race = wgetch(win);
    while (player.race < 49 || player.race > MAX_RACES + 48) {
        player.race = wgetch(win);
    }

    /*
        Convert the ASCII character number to an index
        by shifting down 49 (makes 1 = 0, 2 = 1, etc.)
    */
    player.race -= 49;

    /*
        Do the stat rolls now.  If the player doesn't like the
        rolls with modifiers applied, they can do them over again
    */
    accepted = 0;
    while (accepted == 0) {
        player.str = statRoll(races[player.race].max_str);
        player.dex = statRoll(races[player.race].max_dex);
        player.agi = statRoll(races[player.race].max_agi);
        player.con = statRoll(18);
        player.intel = statRoll(18);
        player.wis = statRoll(18);
        player.cha = statRoll(18);

        wclear(win);
        box(win, ACS_VLINE, ACS_HLINE);
        mvwprintw(win,2,2,"%s - %s", player.player_name,races[player.race].race_name);

        mvwprintw(win,4,15,"STR: %i",player.str);
        mvwprintw(win,5,15,"DEX: %i",player.dex);
        mvwprintw(win,6,15,"AGI: %i",player.agi);
        mvwprintw(win,7,15,"CON: %i",player.con);
        mvwprintw(win,8,15,"INT: %i",player.intel);
        mvwprintw(win,9,15,"WIS: %i",player.wis);
        mvwprintw(win,10,15,"CHA: %i",player.cha);

        wattrset(win,A_REVERSE | A_BOLD);
        mvwprintw(win,12,15,"Enter/Space: Roll Again\tA - Accept Character");
        wattrset(win,A_NORMAL);

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

    /* Set up player hit points */
    player.hp = (int)(races[player.race].start_hp + (player.level / 2) + 1);
    player.max_hp = (int)(races[player.race].start_hp + (player.level / 2) + 1);

    clearPlay();

    wclear(win);
    wresize(win,height,width);
    box(win,ACS_VLINE,ACS_HLINE);
    mvwprintw(win,4,27,"Building World...");
    wrefresh(win);
    cur_map = NULL;

    /*
        Start the world up
    */
    #if DEBUGLOG
        fprintf(f_log,"init_world() and buildMaps()...");
        fflush(f_log);
    #endif
    init_world();
    buildMaps(2);
    #if DEBUGLOG
        fprintf(f_log,"Done.\n");
        dumpMapList(f_log);
        fflush(f_log);
    #endif

    cur_map = get_map(0);
    #if DEBUGLOG
        fprintf(f_log,"Shop Location: (%i,%i)\n",cur_map->shop_loc.x,cur_map->shop_loc.y);
        fprintf(f_log,"Portal Up Location: (%i,%i)\n",cur_map->portal_up_loc.x,cur_map->portal_up_loc.y);
        fprintf(f_log,"Portal Down Location: (%i,%i)\n",cur_map->portal_down_loc.x,cur_map->portal_down_loc.y);
        fflush(f_log);
    #endif
    player.x = cur_map->width / 2;
    player.y = cur_map->height / 2;

    /*
        Once the map is built, we should check
        to see if the player is on an impassible
        If so, we pick a random placement and check
        This isn't efficient, but it will eventually
        find a point within the center of the map
        that isn't an impassible
    */
    while (!isPassable(cur_map,player.x,player.y)) {
        player.x = rollDie(1,cur_map->width / 2);
        player.y = rollDie(1,cur_map->height / 2);
    }

    #if DEBUGLOG
        fprintf(f_log,"player: (%i,%i)\n",player.x,player.y);
        fflush(f_log);
    #endif

    /*
        Initialize the treasure list
        get some random (X,Y) values and make sure
        they are on a passible
    */
    #if DEBUGLOG
        fprintf(f_log,"treasure_init()...");
        fflush(f_log);
    #endif
    treasure_init();
    #if DEBUGLOG
        fprintf(f_log,"done. count: %i\n",treasure_size);
        fflush(f_log);
    #endif

    makeTreasure();

    /*
        Resize the main window so we can use it
        as the gameplay area.
    */
    resize_window(win,20,60);
    game_state = GAME_PLAYING;
    drawPlayArea();
    drawStatsArea();
    gameLoop();
}

/* Gives a player back 1 HP */
void healPlayer() {
    if (player.hp < player.max_hp) player.hp++;
}

void showEncounter() {
    wclear(encounterwin);
    wbkgd(encounterwin,COLOR_PAIR(4) | A_BOLD);
    /*box(encounterwin,ACS_VLINE,ACS_HLINE);*/
    mvwprintw(encounterwin,1,1,"!!! ENCOUNTER !!!");
    wrefresh(encounterwin);
    napms(1250);
    doBattle();
}

void updatePlayer() {
    TREASURE *elt;
    if (encounter_count > 0) encounter_count--; else showEncounter();
    if (player.hp <= 0) player.is_dead = TRUE;

    /*
        Give the player 1 HP back after enough "game step time" has passed
    */
    if (walk_counter >= HEAL_STEPS && player.is_dead == FALSE) {
        walk_counter = 0;
        healPlayer();
        player.nutrition--;
    }

    if (player.nutrition < 150) {
        if (player.status == STATUS_OK) player.status = STATUS_HUNGRY; else player.status = player.status | STATUS_HUNGRY;
    }

    if (player.nutrition <= 0) player.is_dead = TRUE;

    elt = treasureAt(player.x,player.y);
    if ( (elt = treasureAt(player.x,player.y)) != NULL) {
        player.gold += elt->amt;
        /*
            Tell the player they got some gold
        */
        char buf[80];
        memset(buf,0,80);
        sprintf(buf,"Found %s gold",formatLong(elt->amt));
        popupInfo(buf,DURATION_MEDIUM);
        remove_treasureAt(elt->x,elt->y);
        if (treasure_size == 0) {
            popupInfo("You have all of the treasure!",DURATION_MEDIUM);
        }
    }

    if (cur_map->grid[player.y][player.x] == SHOP) {
        doShop();
    }
}

void getPlayerInput() {
    int key;
    int rc;
    int oldX = player.x;
    int oldY = player.y;
    int old_walk_counter = walk_counter;
    int old_encounter_count = encounter_count;

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
            player.x++;
            player.y--;
            break;
        case KEY_HOME:
            player.y--;
            player.x--;
            break;
        case KEY_F(2):
            inspectCharacter();
            /*
                This keeps the step heal and the encounter rates
                from prematurely firing
            */
            walk_counter = old_walk_counter;
            encounter_count++;
            break;
        case KEY_F(1):
            showHelp();
            walk_counter = old_walk_counter;
            encounter_count++;
            break;
        case KEY_NPAGE:
            player.x++;
            player.y++;
            break;
        case KEY_END:
            player.x--;
            player.y++;
            break;
        case KEY_UP:
            player.y--;
            walk_counter++;
            break;
        case KEY_DOWN:
            player.y++;
            walk_counter++;
            break;
        case KEY_RIGHT:
            player.x++;
            walk_counter++;
            break;
        case KEY_LEFT:
            player.x--;
            walk_counter++;
            break;
        case 'i':
        case 'I':
            showInventory();
            walk_counter = old_walk_counter;
            encounter_count++;
            break;
        case 'e':
        case 'E':
            rc = useItem(0,&player);
            if (rc == 2 || rc == -1) popupInfo("You don't have that",DURATION_MEDIUM); else popupInfo("You don't feel hungry anymore",DURATION_MEDIUM);
            break;
        case 'h':
        case 'H':
            useItem(1,&player);
            popupInfo("You feel much better",DURATION_MEDIUM);
            break;
        case 'd':
        case 'D':
            if (cur_map->grid[player.y][player.x] == GRAVEL) {
                cur_map->grid[player.y][player.x] = HOLE;
                popupInfo("Got nothin'",DURATION_SHORT);
            } else {
                popupInfo("There's already a hole here",DURATION_SHORT);
            }
            break;
        case 's':
        case 'S':
            endGame();
            break;
        case 'q':
        case 'Q':
            quit = TRUE;
            game_state = GAME_MENU;
            break;
        default:
            walk_counter = old_walk_counter;
            encounter_count++;
            break;
    }

    if (player.y < 0) player.y = 0;
    if (player.x < 0) player.x = 0;
    if (player.y > cur_map->height) player.y = cur_map->height;
    if (player.x > cur_map->width) player.y = cur_map->width;

    /*
        If the tile on the map at the new player coordinates
        is one of the impassible types, we reset the player.
    */
    if (!isPassable(cur_map,player.x,player.y)) {
        player.x = oldX;
        player.y = oldY;
        walk_counter = old_walk_counter;
        encounter_count = old_encounter_count;
    }
}


void gameLoop() {
    while ( (game_state == GAME_PLAYING) && (!quit) && (!player.is_dead)) {
        getPlayerInput();
        updatePlayer();
        drawPlayArea();
        drawStatsArea();
    }
    game_state = GAME_DONE;
}


void battleStats(ENEMY the_enemy, int round) {
    clearPlay();
    mvwprintw(win,1,1,"Battle - Round %i",round);

    /* If the enemy is invisible the player cannot identify them
        (at least until we get a means of identifying them in)
    */
    if (the_enemy.flags & NME_INVIS) {
        wattrset(win,A_REVERSE);
        mvwprintw(win,2,1,"???????");
        wattrset(win,A_NORMAL);
        mvwprintw(win,3,1,"Level: ??");
        mvwprintw(win,4,1,"HP: ??");
    } else {
        wattrset(win,A_REVERSE);
        mvwprintw(win,2,1,"%s",the_enemy.enemy_name);
        wattrset(win,A_NORMAL);
        mvwprintw(win,3,1,"Level: %i",the_enemy.level);
        mvwprintw(win,4,1,"HP: %i",the_enemy.hp);
        mvwprintw(win,5,1,"STR: %i",the_enemy.str);
    }
}

/* Performs an enemy attack on a player */
void enemyAttack(ENEMY the_enemy, int round) {
    int inRoll = 0;
    long damage = 0;
    long needed;

    battleStats(the_enemy,round);
    if (the_enemy.flags & NME_INVIS) {
        mvwprintw(win,7,3,"?????? attacks %s",player.player_name);
    } else {
        mvwprintw(win,7,3,"The %s attacks %s",the_enemy.enemy_name,player.player_name);
    }

    napms(500);
    inRoll = rollDie(1,20);

    if (inRoll == 1) {
        if (the_enemy.flags & NME_INVIS) {
            mvwprintw(win,8,5,"...?????? misses!",the_enemy.enemy_name);
        } else {
            mvwprintw(win,8,5,"...the %s misses!",the_enemy.enemy_name);
        }
    }

    needed = (20 - the_enemy.level) + (-(player.armor - 11));
    if (inRoll + the_enemy.str + getStrengthModifier(the_enemy.str) > needed) {

        damage = damageRoll(the_enemy.dmg_cnt, the_enemy.dmg_rnd) + damageModifier(the_enemy.str) + the_enemy.size_modifier;
        if (damage <= 0) damage = 1;
        if (the_enemy.flags & NME_INVIS) {
            mvwprintw(win,8,5,"...?????? hits %s for %s damage!",player.player_name,formatLong(damage));
        } else {
            mvwprintw(win,8,5,"...the %s hits %s for %s damage!",the_enemy.enemy_name,player.player_name,formatLong(damage));
        }

        player.hp -= damage;
    } else {
        if (the_enemy.flags & NME_INVIS) {
            mvwprintw(win,8,5,"...?????? misses!",the_enemy.enemy_name);
        } else {
            mvwprintw(win,8,5,"...the %s misses!",the_enemy.enemy_name);
        }
    }

    wrefresh(win);
    napms(850);
}

void doBattle() {
    int round = 1;
    int nme_idx;
    int gold_amt = 0;
    int expGiven = 0;
    int key;
    int damage = 0;
    int canFlee = 0;
    /*int i;*/
    short nme_max_hp;

    bool didFlee = FALSE;
    ENEMY the_enemy;

    /* Pick an enemy randomly from the list */
    /*nme_idx = rollDie(0,MAX_ENEMIES);*/
    nme_idx = pickEnemy();

    /*
        Keep a temporary copy so we can kill
        it without hurting the prototypes
    */
    the_enemy = enemies[nme_idx];

    setupEnemy(&the_enemy,&gold_amt);
    expGiven = the_enemy.exp;
    nme_max_hp = the_enemy.hp;

    while (the_enemy.hp > 0 && player.is_dead == FALSE && player.hp > 0 && didFlee == FALSE) {
        flushinp();
        nodelay(win,FALSE);
        battleStats(the_enemy,round);

        wattrset(win,A_BOLD);
        mvwprintw(win,5,13,"Options");
        mvwhline(win,6,12,ACS_HLINE,10);
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
                    mvwprintw(win,7,3,"%s flees from the battle!",player.player_name);
                    wrefresh(win);
                    napms(500);
                    didFlee = TRUE;
                    walk_counter = 0;
                    setEncounterRate();
                } else {
                    battleStats(the_enemy,round);
                    mvwprintw(win,7,3,"%s attempts to flee but fails!",player.player_name);
                    wrefresh(win);
                    napms(1000);
                    enemyAttack(the_enemy,round);
                    round++;
                }
                break;
            case 'A':
            case 'a':
                battleStats(the_enemy,round);
                if (the_enemy.flags & NME_INVIS) {
                    mvwprintw(win,7,3,"%s attacks ??????",player.player_name);
                } else {
                    mvwprintw(win,7,3,"%s attacks the %s...",player.player_name,the_enemy.enemy_name);
                }

                if (playerDidHit(the_enemy) == TRUE) {

                    damage = damageRoll(weapons[player.weapon].wield_dmg_cnt,weapons[player.weapon].wield_dmg_rnd) + races[player.race].size_modifier + damageModifier(player.str);
                    if (damage <= 0) damage = 1;
                    if (player.critical_hit == TRUE) {
                        damage *= 2;
                    }

                    if (the_enemy.flags & NME_INVIS) {
                        mvwprintw(win,8,5,"...%s hits ?????? for %i damage!",player.player_name,damage);
                    } else {
                        mvwprintw(win,8,5,"...%s hits the %s for %i damage!",player.player_name,the_enemy.enemy_name,damage);
                    }

                    if (player.critical_hit == TRUE) {
                        wattrset(win,A_BOLD | COLOR_PAIR(4));
                        mvwprintw(win,10,5,"!!! CRITICAL HIT !!!");
                        wattrset(win,A_NORMAL);
                        player.critical_hit = FALSE;
                    }

                    the_enemy.hp -= damage;

                    if (the_enemy.hp > 0 && (the_enemy.flags & NME_REGEN) && (round % 3)) {
                        /*
                            If the enemy isn't dead from this attack, and it can regenerate HP,
                            we give it back some
                        */
                        the_enemy.hp += (nme_max_hp * 0.1);
                        wattrset(win,A_BOLD);
                        mvwprintw(win,10,3,"The monster regains some life!");
                        wattrset(win,A_NORMAL);
                    }
                } else {
                    mvwprintw(win,8,5,"...%s misses!",player.player_name);
                }
                wrefresh(win);
                napms(850);
                if (the_enemy.hp > 0) {
                    enemyAttack(the_enemy,round);
                    round++;
                }
                break;
            /* The k (KILL) command is only for testing purposes */
            case 'k':
            case 'K':
                the_enemy.hp = 0;
                break;
            default:
                wattrset(win,COLOR_PAIR(4) | A_BOLD);
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
        endGame();
    } else if (didFlee == FALSE) {
        if (player.gold + gold_amt >= INT_MAX) player.gold = INT_MAX; else player.gold += gold_amt;
        player.slain++;
        walk_counter = 0;
        wbkgd(win,COLOR_PAIR(1) | A_DIM);
        wclear(win);
        box(win,ACS_VLINE,ACS_HLINE);

        wattrset(win,A_BOLD);
        mvwprintw(win,1,17,"Victory!");
        wattrset(win,A_NORMAL);
        if (the_enemy.flags & NME_INVIS) {
            mvwprintw(win,5,5,"%s hast slain ??????",player.player_name);
            mvwprintw(win,6,5,"%s found %s gold",player.player_name,formatLong(gold_amt));
        } else {
            mvwprintw(win,5,5,"%s hast slain the %s!",player.player_name,the_enemy.enemy_name);
            mvwprintw(win,6,5,"%s found %s gold on the %s",player.player_name,formatLong(gold_amt),the_enemy.enemy_name);
        }
        if (expGiven > 0)
            mvwprintw(win,7,5,"%s gained %s XP from the battle",player.player_name,formatLong(expGiven));

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
        setEncounterRate();
        wgetch(win);
    }
}

/*
    Checks to see if the player has leveled up
    and acts accordingly. This has been modified
    now to use 3.5 Edition D&D rules
    See nextLevelXP() for the calculation
*/
int levelUp(long newXP) {
    long newXPNeeded;
    player.xp += newXP;
    player.xp_earned += newXP;
    newXPNeeded = nextLevelXP(player.level);

    if (player.xp_earned >= newXPNeeded) {
        player.xp_earned = player.xp_earned - newXPNeeded;
        player.xp_needed = nextLevelXP(player.level + 1);
        player.level++;
        if (player.str < races[player.race].max_str) player.str++;
        player.dex++;
        player.attack_bonus++;
        player.hp = (int)(races[player.race].start_hp + (player.level / 2) + 1);
        player.max_hp = (int)(races[player.race].start_hp + (player.level / 2) + 1);
        return 1;
    } else return 0;
}


int getStrengthModifier(int str) {
    int mod = 4;

    if (str < 8) return str - 7;
    if (str < 31) mod--;
    if (str < 21) mod--;
    if (str < 19) mod--;
    if (str < 17) mod--;
    return mod;
}

int damageRoll(int dmg_cnt, int dmg_rnd) {
    int i;
    int sum;

    sum = 0;
    for (i = 0; i < dmg_cnt; i++) {
        sum += rollDie(1,dmg_rnd);
    }
    return sum;
}

int damageModifier(int str) {
    int mod = 6;

    if (str < 8) return str - 7;
    if (str < 31) mod--;
    if (str < 22) mod--;
    if (str < 20) mod--;
    if (str < 18) mod--;
    if (str < 17) mod--;
    if (str < 16) mod--;
    return mod;
}

void drawMap(int centerY, int centerX) {
    int XIndex, YIndex;

    int mapTopX = centerX - MAP_DISPLAY_WIDTH / 2;
    int mapTopY = centerY - MAP_DISPLAY_HEIGHT / 2;

    if (mapTopX < 0) mapTopX = 0;
    if (mapTopY < 0) mapTopY = 0;
    if (mapTopX + MAP_DISPLAY_WIDTH > cur_map->width) mapTopX = cur_map->width - MAP_DISPLAY_WIDTH;
    if (mapTopY + MAP_DISPLAY_HEIGHT > cur_map->height) mapTopY = cur_map->height - MAP_DISPLAY_HEIGHT;

    for (YIndex = 0; YIndex < MAP_DISPLAY_HEIGHT; YIndex++) {
        for (XIndex = 0; XIndex < MAP_DISPLAY_WIDTH; XIndex++) {
            if ((YIndex+mapTopY == player.y) && (XIndex+mapTopX == player.x)) {
                if (player.status & STATUS_POISON) wattrset(win,COLOR_PAIR(11) | A_BOLD); else wattrset(win,COLOR_PAIR(10) | A_BOLD);
                mvwaddrawch(win,YIndex+1,XIndex+1,player.avatar);
                wattrset(win,A_NORMAL);
            } else if (treasureAt(XIndex+mapTopX,YIndex+mapTopY)) {
                wattrset(win,COLOR_PAIR(10) | A_BOLD);
                mvwaddrawch(win,YIndex+1,XIndex+1,ACS_LANTERN);
                wattrset(win,A_NORMAL);
            } else {
                switch (cur_map->grid[YIndex+mapTopY][XIndex+mapTopX]) {
                    case WALL:
                        wattrset(win,COLOR_PAIR(4));
                        mvwaddrawch(win,YIndex+1,XIndex+1,ACS_BOARD);
                        wattrset(win,A_NORMAL);
                        break;
                    case GRAVEL:
                        wattrset(win,COLOR_PAIR(11) | A_DIM);
                        /*mvwprintw(win,YIndex+1,XIndex+1,".");*/
                        mvwaddrawch(win,YIndex+1,XIndex+1,0xF9);
                        wattrset(win,A_NORMAL);
                        break;
                    case OUTERWALL:
                        wattrset(win,COLOR_PAIR(12) | A_DIM);
                        mvwaddrawch(win,YIndex+1,XIndex+1,ACS_BOARD);
                        wattrset(win,A_NORMAL);
                        break;
                    case HOLE:
                        wattrset(win,COLOR_PAIR(0));
                        mvwprintw(win,YIndex+1,XIndex+1," ");
                        wattrset(win,A_NORMAL);
                        break;
                    case SHOP:
                        wattrset(win,COLOR_PAIR(12) | A_REVERSE);
                        mvwprintw(win,YIndex+1,XIndex+1,"S");
                        wattrset(win,A_NORMAL);
                        break;
                    case WATER:
                        wattrset(win,COLOR_PAIR(14) | A_BOLD);
                        mvwaddrawch(win,YIndex+1,XIndex+1,0xF7);
                        wattrset(win,A_NORMAL);
                        break;
                    case PORTAL:
                        wattrset(win,COLOR_PAIR(14) | A_BOLD);
                        mvwaddrawch(win,YIndex+1,XIndex+1,0xF4);
                        wattrset(win,A_NORMAL);
                        break;
                }
            }
        }
    }
}

void drawPlayArea() {
    clearPlay();
    drawMap(player.y, player.x);
    wrefresh(win);
}

void drawStatsArea() {
    int xPos = 1;
    wclear(stats);
    wbkgd(stats,COLOR_PAIR(0) | A_DIM);
    box(stats,ACS_VLINE,ACS_HLINE);
    wattrset(stats,A_REVERSE);
    mvwprintw(stats, 1, 1, player.player_name);
    wattrset(stats,A_NORMAL);

    mvwprintw(stats, 2, 1, "%s", races[player.race].race_name);
    mvwprintw(stats, 3, 1, "%s", weapons[player.weapon].weapon_name);
    mvwprintw(stats, 4, 1, "%s gp", formatLong(player.gold));
    mvwprintw(stats, 6, 1, "HP: %i (%i)", player.hp,player.max_hp);
    mvwprintw(stats, 7, 1, "XP: %s", formatLong(player.xp));
    mvwprintw(stats, 8, 1, "(%i,%i)",player.x,player.y);

    wattrset(stats,A_REVERSE | COLOR_PAIR(2));
    mvwprintw(stats, height - 2, 1,"%s",cur_map->level_name);
    wattrset(stats,A_NORMAL);

    if (player.status == STATUS_OK) {
        wattrset(stats,COLOR_PAIR(11) | A_BOLD);
        mvwprintw(stats,9,xPos,"OK");
        wattrset(stats,A_NORMAL);
        xPos++;
    } else {
        if (player.status & STATUS_POISON) {
            wattrset(stats,COLOR_PAIR(11) | A_BOLD);
            mvwprintw(stats,9,xPos,"P");
            wattrset(stats,A_NORMAL);
            xPos++;
        }

        if (player.status & STATUS_HUNGRY) {
            wattrset(stats,COLOR_PAIR(10) | A_BOLD);
            mvwprintw(stats,9,xPos,"H");
            wattrset(stats,A_NORMAL);
            xPos++;
        }

        if (player.status & STATUS_CONFUSED) {
            wattrset(stats,COLOR_PAIR(14) | A_BOLD);
            mvwaddrawch(stats,9,xPos,0xA8);
            wattrset(stats,A_NORMAL);
            xPos++;
        }
    }

    wrefresh(stats);
}

void showHelp() {
    flushinp();
    wclear(helpwindow);
    wbkgd(helpwindow,COLOR_PAIR(2) | A_DIM);
    wborder(helpwindow,0,0,0,0,0,0,0,0);
    wattrset(helpwindow,A_BOLD);
    mvwprintw(helpwindow,0,2,"System Help");
    wattrset(helpwindow,A_NORMAL);

    mvwprintw(helpwindow,2,2,"Q  - Quit");
    mvwprintw(helpwindow,3,2,"D  - Dig a hole");
    mvwprintw(helpwindow,4,2,"I  - Inventory");
    mvwprintw(helpwindow,5,2,"F2 - Inspect Character");

    wattrset(helpwindow,A_REVERSE | A_BOLD);
    mvwprintw(helpwindow,height - 2,2,"Press any key to resume");
    wattrset(helpwindow,A_NORMAL);
    wrefresh(helpwindow);
    wgetch(helpwindow);
}

void display_menu(int sel_option) {
	int lmarg = (COLS - 14) / 2,
		tmarg = (LINES - (MAX_OPTIONS + 2)) / 2;
    int i;

    wclear(win);
    box(win,ACS_VLINE,ACS_HLINE);
    wattrset(win,A_BOLD);
    mvwaddstr(win,tmarg - 6, lmarg - 10, "Perils of Pothril - A Beginner's RPG");
    wattrset(win,A_NORMAL);

    for (i = 0; i < MAX_OPTIONS; i++) {
        if (i == sel_option) {
            wattrset(win,COLOR_PAIR(11) | A_BOLD);
            mvwaddrawch(win,tmarg + i, lmarg - 2,0x1A);
            /*mvwaddrawch(win,tmarg + i, lmarg + strlen(command[i].text),);*/
            wattrset(win,A_REVERSE | COLOR_PAIR(0));
            mvwaddstr(win,tmarg + i, lmarg, command[i].text);
            wattrset(win,A_NORMAL);
        } else mvwaddstr(win, tmarg + i, lmarg, command[i].text);
    }

	wrefresh(win);
}

/*  Initializes PDCurses and game startup */
int initGame() {
    int c;
    init_genrand((unsigned int) time(NULL));
    srand(time(NULL));
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

    /* Special color pairs used for drawing player */
    init_pair(10,COLOR_YELLOW,COLOR_BLACK);
    init_pair(11,COLOR_GREEN,COLOR_BLACK);
    init_pair(12,COLOR_WHITE,COLOR_BLACK);
    init_pair(13,COLOR_GREEN,COLOR_BLACK);
    init_pair(14,COLOR_BLUE,COLOR_BLACK);
    /*init_pair(15,COLOR_YELLOW,COLOR_BLACK);*/

	width  = 80;
	height = 20;

    clear();
    refresh();

    #if DEBUGLOG
        f_log = fopen("debug.log","w");
        if (f_log == NULL) return 1;
        fprintf(f_log,"Game startup - initGame()\n");
        fflush(f_log);
    #endif

    /* Resize the terminal window */
    resize_term(height,width);

	win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
	helpwindow = newwin(0,0,0,0);
	stats = newwin(20,20,0,60); /* create the character stats window */
	encounterwin = newwin(3,19,10,20);
    infowin = newwin(3,35,10,20);
    inventorywin = newwin(height,width,0,0);
    charsheet = newwin(height, width, 0, 0);

	if (win == NULL || stats == NULL || helpwindow == NULL
        || infowin == NULL || charsheet == NULL || inventorywin == NULL) {
		endwin();
		return 1;
	}

    savetty();
    resize_term(0,0);
    game_state = GAME_MENU;
	return 0;
}

void gameShutdown() {
    #if DEBUGLOG
        fclose(f_log);
    #endif
    wresize(win,height,width);
    wclear(win);
    box(win,ACS_VLINE,ACS_HLINE);
    mvwprintw(win,10,20,"Shutting down...");
    wrefresh(win);
    free(player.player_name);
    cur_map = NULL;
    world_destroy();
    items_destroy();
    treasure_destroy();
	endwin();
}

/*
    Draws the main menu
*/
void showMenu() {
    int key, selOption = 0;
	display_menu(selOption);

	while (quit == FALSE && game_state == GAME_MENU) {
		noecho();
		keypad(stdscr, TRUE);
		raw();

		key = getch();

		switch(key) {
            case 10:
            case 13:
            case KEY_ENTER:
                game_state = command[selOption].state;
                break;
            case KEY_UP:
                if (selOption > 0) selOption--;
                display_menu(selOption);
                break;
            case KEY_DOWN:
                if (selOption < MAX_OPTIONS - 1) selOption++;
                display_menu(selOption);
                break;
        #ifdef KEY_RESIZE
            case KEY_RESIZE:
        #ifdef PDCURSES
                resize_term(0, 0);
        #endif
                erase();
                /*display_menu(old_option, new_option);*/
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
    PDC_set_title("Perils of Pothril");

	if (initGame())
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
