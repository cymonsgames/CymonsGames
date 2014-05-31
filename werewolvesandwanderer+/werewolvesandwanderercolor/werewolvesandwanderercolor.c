#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define numexits 5
#define numtreasure 6
#define nummonsters 4
#define nummerchants 3
#define numrooms 20
#define LINE 80

enum inventorytype {none, sword, amulet, axe, suit, numinventory};

struct treasuretype {
    char name[LINE];
    int value;
};

struct monstertype {
    char name[LINE];
    int strength;
};

struct exittype {
    char desc[LINE];
    int dest;
};

struct roomtype {
    int merchant;
    struct treasuretype treasure;
    struct monstertype monster;
    int foundstory;
};

struct gametype {
    int moves;
    int playerloc;
    int strength;
    int monster_tally;
    int wealth;
    int food;
    int inventory[numinventory];
    struct roomtype room[numrooms + 1];
    struct exittype exit[numexits];
    time_t date;
    char name[LINE];
};

struct expositiontype {
    int room;
    char description[LINE*20];
} exposition[] = {
    {6,
    "You clutch the faded letter you found in the\n"
    "old trunk left to you by your grandfather.\n"
    "Tho incomplete the letter spoke of a castle that\n"
    "was abandoned by its occupants who left behind a\n"
    "wealth of treasure in their haste.\n\n"
    "Had you but known about the sad story that left\n"
    "those cold walls cursed and the strange creatures\n"
    "that now roam those halls you might not have\n"
    "ventured here....\n\n"},
    {2,
    "Looking closer at one torn tapestry in particular\n"
    "you realize this tear isn't from wear or age.\n"
    "This tapestry was torn as if by a knife. You then\n"
    "notice the wall behind it is scratched too. 4 deep\n"
    "grooves run parallel to the tear. What creature\n"
    "could leave claw marks in solid stone?\n\n"},
    {4,
    "On the scribes table, written on parchment, are\n"
    "the minutes for the last meeting held in this\n"
    "hall. The notes are short as the discussion\n"
    "seemed to dwell on what they were going to do\n"
    "about the influx of new strange creatures that had\n"
    "begun to occupy the castle. Many options involved\n"
    "finding and either placating or killing some\n"
    "witch.\n\n"},
    {14,
    "Reading the open journal you learn that the queen's\n"
    "health was ailing and that an old witch had been\n"
    "blamed for her afliction. The kind ordered her\n"
    "eviction durring a particularly harsh thunderstorm.\n"
    "He thought by throwing her out her her malignant\n"
    "influence on his wife would cease. This did not\n"
    "happen and his wife became more and more ill and\n"
    "finally died.\n\n"
    "Her last days were not peaceful. The old woman's\n"
    "curse brought a reign of terror to the castle with\n"
    "many odd creatures taking residence within the\n"
    "castle eventually forcing the king and his surviving\n"
    "court to flee.\n\n"},
    {7,
    "The simple quarters, little more than a closet with\n"
    "a bed, is crammed with talismans and strange\n"
    "ingredients. This must have been the room of the\n"
    "witch before she left, or was forced to leave.\n\n"
    "Searching for some sign of what nefarious plot she\n"
    "was plodding instead you discover that every spell\n"
    "and talisman in her service was employed in\n"
    "protecting the castle from a wild dark wizard who\n"
    "sought to overrun the castle and help himself to\n"
    "the a talisman the king kept in his treasury.\n\n"
    "Judging from the state of the castle, it seems he\n"
    "achieved his end.\n\n"},
    {16,
    "The treasure remains largely in tact, a fact that\n"
    "you are grateful for, but you can't help noticing\n"
    "a conspicuously vacant place in the north wall.\n"
    "You wonder what once resided there, but there's no\n"
    "label or sign indicating what it was.\n\n"},
};
#define numexposition (sizeof exposition/sizeof exposition[0])

/* generic routines that should probably be put in an external file */
void colorinit () {
        int c;

    initscr ();

    scrollok(stdscr, 1);
    /*raw (); nodelay(stdscr,1); curs_set(1); keypad(stdscr,1);*/
    /*resize_term(25,80); */
#ifdef PDCURSES
    PDC_set_title("Cymon's Games - Werewolves and Wanderers");
#endif
    start_color();
    for (c = 0; c < COLORS; c++)
        init_pair(c, c, COLOR_BLACK);
}

void slowprintfc(char *fmt, ...) {
	int i, n;
	int col = COLOR_WHITE;
	int bold = 0;
	clock_t last;
	double pause = 0.001;
	char buf[4096];

	va_list ap;
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

    n = strlen(buf);
	last=clock();
	for(i = 0; i < n; i++) {
        if ((buf[i] == '~') && isdigit(buf[i+1])) {
            col = 0; bold = 0;
            while (isdigit(buf[i+1])) {
                i++;
                col = col * 10 + (buf[i] - '0');
            }
            if (tolower(buf[i+1]) == 'b') {
                bold = 1;
                i++;
            } else if (tolower(buf[i+1]) == '~')
                i++;
	    } else {
            addch(buf[i]|COLOR_PAIR(col)|(A_BOLD * bold));
            refresh();
            while ((float)(clock() - last) <(pause *(float)CLOCKS_PER_SEC));
            last=clock();
        }
	}
}

char * ngets (char *s, int n) { /* a safer version of gets */
    if (s == NULL) {
        s = calloc(255, sizeof(char));
        n = 255;
    }
    attrset(COLOR_PAIR(1)|A_BOLD);
    do {
        getnstr(s, n);
        if (s[strlen(s) - 1] == '\n')
            s[strlen(s) - 1] = 0;
    } while (s[0] == 0);
    return s;
}

int scanpair(char *verb, char *noun) {
/* verb and noun must be char arrays of at least 32 characters. */
/* get_commands returns the number of commands received. */
    char buf[LINE];
    char* tok;

    verb[0] = 0; noun[0] = 0;
    ngets(buf, sizeof buf);
    tok = strtok (buf," ");
	if (tok == NULL)
		return 0;
    sprintf(verb, tok);
    tok = strtok (NULL, " \0");
    if (tok != NULL) {
        sprintf(noun, tok);
        while (tok != NULL)
            tok = strtok (NULL, " \n");
        return 2;
    }
    return 1;
}

void strtolower(char *s) {
    int i, n;

    n = strlen(s);
    for(i = 0; i < n; i++)
        s[i] = tolower(s[i]);
}

int randint(int low, int high) {
    /* returns a value x such that low <= x <= high */
	int x, range = high - low + 1;
	if (range < 0) range = -range;
	do {
        x = rand();
	} while (x > RAND_MAX -(RAND_MAX % 9));
	return low + x % range;
}
/* end generic routines */

#define START 6
#define END 11

void help(char *topic) {
    if (topic)
        strtolower(topic);
    if (!strncmp(topic, "co", 2)) {
        slowprintfc("~6WEREWOLVES AND WANDERER COMMANDS\n\n"
            "~1bGO ~7- Travel to a different room. ~1bGO ~7then ~1bEAST~7,\n"
            "     ~1bGO EAST~7, or just ~1bEAST ~7are all the same.\n"
            "~1bLOOK ~7- Get a description of the room\n"
            "~1bINVENTORY ~7- See your stats.\n"
            "~1bGET ~7or ~1bPICK UP ~7- Add the treasure in the room\n"
            "     to your wealth total.\n"
            "~1bTRADE ~7or ~1bBUY ~7- Purchase items from the merchant\n"
            "     if available. Typing ~1bTRADE ~7then ~1bAXE ~7or\n"
            "     ~1bBUY AXE ~7are the same.\n"
            "~1bEAT ~7- Consume food to increase your strength.\n"
            "~1bFIGHT ~7- Engage monster in the room to combat.\n"
            "~1bRUN ~7or ~1bFLEE ~7- Attempt to escape from monsters\n"
            "     in the room. ~1bRUN ~7then ~1bEAST ~7or ~1bRUN EAST~7\n"
            "     both have the same effect.\n"
            "~1bMAGIC ~7- If you have the amulet you can summon\n"
            "     its power to teleport you within the castle.\n"
            "~1bEXAMINE ~7- In certain rooms you may learn more\n"
            "     about the story.\n"
            "~1bHIGH SCORE ~7- look at the high score table.\n"
            "~1bQUIT ~7- End your game.\n\n");
    } else {
        slowprintfc("~6WEREWOLVES AND WANDERER\n"
            "Original game by Tim Hartnell.\n"
            "Remake by Joseph Larson\n\n"
            "~7In this adventure you are the wanderer and\n"
            "werewolves and other strange creatures inhabit\n"
            "an ancient, abandoned, stone castle where you\n"
            "are seeking untold fortune.\n\n"
            "This is a basic text-adventure that seeks to\n"
            "simplify the experience by giving you the\n"
            "available exits and commands without needing\n"
            "to hunt around for them, tho it will also\n"
            "accept a few other commands as well as\n"
            "occasional two word commands.\n\n"
            "The ~1bEXAMINE ~7command is the one exception. Typing\n"
            "~1bEXAMINE ~7in a room may give you extra expository\n"
            "information about that room, tho the game may not\n"
            "always tell you when you can use it.\n\n"
            "Type ~1bHELP COMMANDS ~7for a list of all commands.\n\n");
    }
    slowprintfc("Press ~2bENTER ~7to continue\n");
    getch();
}

void clearexits(struct exittype *e) {
    int c;

    for(c = 0; c < numexits; c++)
        memset(&e[c], 0, sizeof e[c]);
}

int roomdescription(struct gametype *game) {
    slowprintfc("\n");
    switch(game->playerloc) {
    case 1:
        slowprintfc("You are in the hallway.\n");
        if (randint(0, 9) >= 4)
            slowprintfc("From the dust on the ground you can tell no one has\n"
                "walked here for a long, long time.\n");
        slowprintfc("There is a door to the south.\n"
            "Through the windows to the north you can see a secret\n"
            "herb garden.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "west"); game->exit[0].dest = 6;
        snprintf(game->exit[1].desc, LINE, "south"); game->exit[1].dest = 2;
        break;
    case 2:
        slowprintfc("This is the audience chamber.\n");
        if (randint(0, 9) >= 4)
            slowprintfc("The faded tapestries on the wall only hint at the\n"
                "splendor which this room once had.\n");
        slowprintfc("There is a window to the west. By looking to the right\n"
            "through it you can, see the entrance to the castle.\n"
            "You think you might ~1bEXAMINE ~7the tapestries closer.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "north"); game->exit[0].dest = 1;
        snprintf(game->exit[1].desc, LINE, "east"); game->exit[1].dest = 20;
        snprintf(game->exit[2].desc, LINE, "south"); game->exit[2].dest = 3;
        break;
    case 3:
        slowprintfc("You are in the southern wing of the great\n"
            "hall, an L-shaped room that wraps around the\n"
            "audience chamber. There are two doors in this room.\n"
            "The wood panels are warped and faded.\n");
        if (randint(0, 9) >= 4)
            slowprintfc("As you stand there, you hear a mouse scamper\n"
                "along the floor behind you. You whirl\n"
                "around...but see nothing.\n");
        slowprintfc("Around the corner is the dark northern wing\n"
            "of the hall.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "north"); game->exit[0].dest = 2;
        snprintf(game->exit[1].desc, LINE, "east"); game->exit[1].dest = 5;
        snprintf(game->exit[2].desc, LINE, "corner"); game->exit[2].dest = 20;
        break;
    case 4:
        slowprintfc("This is the monarch's private meeting room.\n"
            "There is a single exit to the south.\n");
        if (randint(0, 9) >= 4)
            slowprintfc("The echo of ancient plotting and wrangling hangs\n"
                "heavy in the musty air.\n"
                "You see a scribes table which you could ~1bEXAMINE ~7.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "south"); game->exit[0].dest = 5;
        break;
    case 5:
        slowprintfc("This inner hallway contains a door to the north,\n"
            "and one to the west, and a circular stairwell\n"
            "passes through the room.\n");
        if (randint(0, 9) >= 6)
            slowprintfc("The room is small and unfriendly.\n");
        slowprintfc("You can see an ornamental lake through the\n"
            "windows to the south.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "north"); game->exit[0].dest = 4;
        snprintf(game->exit[1].desc, LINE, "west"); game->exit[1].dest = 3;
        snprintf(game->exit[2].desc, LINE, "up"); game->exit[2].dest = 15;
        snprintf(game->exit[3].desc, LINE, "down"); game->exit[3].dest = 13;
        break;
    case 6:
        slowprintfc("You are at the entrance to a forbidding-looking\n"
            "stone castle. You are facing east.\n"
            "You begin to doubt the wisdom of following the faded\n"
            "letter you found here. Wonder if you shouldn't\n"
            "~1bEXAMINE ~7 the letter once more.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "east"); game->exit[0].dest = 1;
        break;
    case 7:
        slowprintfc("This is the castle's kitchen. Through windows in\n"
            "the north wall you can see a secret herb garden.\n"
            "It has been many years since meals were\n"
            "prepared for the monarch and the court\n"
            "in this kitchen.\n");
        if (randint(0, 9) >= 4)
            slowprintfc("A rat scurries across the floor and draws\n"
                "your attention to the servant's quarters\n"
                "in the corner. If you ~1bEXAMINE ~7 them you\n"
                "may learn something.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "south"); game->exit[0].dest = 8;
        break;
    case 8:
        slowprintfc("You are in the store room, amidst spices,\n"
            "vegetables, and vast sacks of flour and\n"
            "other provisions.\n"
            "The air is thick with spice and curry fumes.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "north"); game->exit[0].dest = 7;
        snprintf(game->exit[1].desc, LINE, "south"); game->exit[1].dest = 10;
        break;
    case 9:
        slowprintfc("You enter the lift.\nIt descends slowly. . .         \n");
        game->playerloc = 10;
        /* no break. */
    case 10:
        slowprintfc("You are in the rear vestibule.\n"
            "There are windows to the south from which you\n"
            "can see the ornamental lake.\n"
            "There is an exit to the east and one to the north.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "north"); game->exit[0].dest = 8;
        snprintf(game->exit[1].desc, LINE, "east"); game->exit[1].dest = 11;
        break;
    case 11:
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "east"); game->exit[0].dest = 10;
        break;
    case 12:
        slowprintfc("You are in the dank, dark, dungeon.\n"
            "There is a single exit, a small hole in the\n"
            "wall to the west.\n");
        if (randint(0, 9) >= 4)
            slowprintfc("...a hollow, dry chuckle is heard\n"
                "from the guard room...\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "west"); game->exit[0].dest = 13;
        break;
    case 13:
        slowprintfc("You are in the prison guardroom, in the\n"
            "basement of the castle, the stairwell\n"
            "ends in this room. There is one other\n"
            "exit, a small hole in the east wall.\n"
            "The air is damp and unpleasant... a chill wind\n"
            "rushes into the room from the gap in the stone\n"
            "at the top of the walls.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "east"); game->exit[0].dest = 12;
        snprintf(game->exit[1].desc, LINE, "up"); game->exit[1].dest = 5;
        break;
    case 14:
        slowprintfc("You are in the master bedroom on the upper\n"
            "level of the castle.\n"
            "Looking down from the window to the west you\n"
            "can see the entrance to the castle, while the\n"
            "secret herb garden is visible below the north\n"
            "window. There are doors to the east and\n"
            "to the south.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "south"); game->exit[0].dest = 15;
        snprintf(game->exit[1].desc, LINE, "east"); game->exit[1].dest = 17;
        break;
    case 15:
        slowprintfc("This is the L-shaped upper hallway.\n");
        if (randint(0, 9) >= 4)
            slowprintfc("...a moth flutters across near the ceiling...\n");
        slowprintfc("To the north is a door, and there is a\n"
            "stairwell in the hall as well. You can see\n"
            "the lake through the south windows.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "north"); game->exit[0].dest = 14;
        snprintf(game->exit[1].desc, LINE, "down"); game->exit[1].dest = 5;
        break;
    case 16:
        slowprintfc("This room was used as the castle treasure in\n"
            "bygone years...\n");
        if (randint(0, 9) >= 4)
            slowprintfc("...A spider scampers down the wall.....\n");
        slowprintfc("There are no windows, just exits. You can\n"
            "~1bEXAMINE ~7 the treasure, or just help yourself.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "north"); game->exit[0].dest = 17;
        snprintf(game->exit[1].desc, LINE, "east"); game->exit[1].dest = 19;
        break;
    case 17:
        slowprintfc("Ooooh... You are in the chambermaids' bedroom.\n"
            "Faint perfume still hangs in the air...\n"
            "There is an exit to the west and a door\n"
            "to the south.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "north"); game->exit[0].dest = 18;
        snprintf(game->exit[1].desc, LINE, "south"); game->exit[1].dest = 16;
        snprintf(game->exit[2].desc, LINE, "west"); game->exit[2].dest = 14;
        break;
    case 18:
        slowprintfc("This tiny room on the upper level is the\n"
            "dressing chamber. there is a window to the\n"
            "north with a view of the herb garden down\n"
            "below. A door leaves to the south.\n");
        if (randint(0, 9) >= 4)
            slowprintfc("You catch a glimpse of yourself in the mirror\n"
            "hanging on the wall and are shocked at your\n"
            "disheveled appearance.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "south"); game->exit[0].dest = 17;
        break;
    case 19:
        slowprintfc("This small upper room actually resides on the\n"
            "outside of the castle.\n"
            "You can see the lake through the southern\n"
            "windows. To the north is the doors to a small lift.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "lift"); game->exit[0].dest = 9;
        snprintf(game->exit[1].desc, LINE, "west"); game->exit[1].dest = 16;
        break;
    case 20:
        slowprintfc("You are in the northern wing of the L-shaped\n"
            "great hall. It is dark and gloomy here but\n"
            "there is some light to the south.\n");
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, "west"); game->exit[0].dest = 2;
        snprintf(game->exit[1].desc, LINE, "south"); game->exit[1].dest = 3;
        break;
    default:
        slowprintfc("The game is broken.\nQUIT now\n");
        clearexits(game->exit);
        break;
/*
    case 0:
        slowprintfc();
        if (randint(0, 9) => 4)
            slowprintfc();
        slowprintfc();
        clearexits(game->exit);
        snprintf(game->exit[0].desc, LINE, ""); game->exit[0].dest = 0;
        snprintf(game->exit[1].desc, LINE, ""); game->exit[0].dest = 0;
        snprintf(game->exit[2].desc, LINE, ""); game->exit[0].dest = 0;
        break;
 */
    }
    return 1;
}

int go_exit(struct gametype *game, char *go) {
    int c;

    for(c = 0; c < numexits; c++)
        if (!strncmp(go, game->exit[c].desc, 2)) {
            game->playerloc = game->exit[c].dest;
            game->strength -= 5;
            return game->exit[c].dest;
        }
    return 0;
}

void examine(struct gametype *game) {
    int c;

    slowprintfc("\n");
    for(c = 0; c < numexposition; c++)
        if (exposition[c].room == game->playerloc) {
            slowprintfc(exposition[c].description);
            game->room[game->playerloc].foundstory = 1;

            return;
        }
    slowprintfc("Nothing here to examine.\n");
    return;
}

void roomobjects(struct gametype *game) {
    int r = game->playerloc;

    if (game->room[r].treasure.value)
        slowprintfc("You see ~6b%s ~7here worth ~6b$%d~7.\n",
            game->room[r].treasure.name, game->room[r].treasure.value);
    if (game->room[r].merchant)
        slowprintfc("You see a ~3strangely clothed little man ~7here.\n");
    if (game->room[r].monster.strength)
        slowprintfc("~4bDANGER! ~7There is a ~4b%s ~7here!\n(Danger level ~4b%d~7.)\n",
            game->room[r].monster.name, game->room[r].monster.strength);
}

void playerstats(struct gametype *game) {
    int c, first = 1;

    slowprintfc("Strength:~3b%d  ~7Wealth:~3b%d  ~7Food:~3b%d\n",
        game->strength, game->wealth, game->food);
    for(c = 0; c < numinventory; c++)
        if (game->inventory[c]) {
            if (first) {
                slowprintfc("You are carrying ");
                first = 0;
            } else
                slowprintfc(", ");
            switch(c) {
            case sword: slowprintfc("~3ba sword"); break;
            case axe: slowprintfc("~3ban axe"); break;
            case suit: slowprintfc("~3barmor"); break;
            case amulet: slowprintfc("~3ban amulet"); break;
            default: slowprintfc("~3bgrue spray(tm)"); break;
            }
        }
    if (!first)
        slowprintfc("\n");

}

void exits(struct gametype *game) {
    int c, first = 1;

    slowprintfc("You can exit the room: ");
    for(c = 0; c < numexits; c++)
        if (game->exit[c].desc[0]) {
            if (first)
                first = 0;
            else
                slowprintfc(", ");
            slowprintfc("~5b%s", game->exit[c].desc);
        }
    slowprintfc("\n");
}

void look(struct gametype *game) {
    if (game->playerloc == END)
        return;
    roomdescription(game);
    roomobjects(game);
    playerstats(game);
    exits(game);
}

void trade(struct gametype *game, char* item) {
    char input[32];

    if (!item || !item[0]) {
        slowprintfc("The little man speaks in a raspy, yet sing-song\n"
            "voice as he says:\n"
            "\"Little trinkets have I. If you buy them they will\n"
            "help you. Buy. Buy!\n"
            "Then he produces some items and lays them on\n"
            "the ground before you, touching each and\n"
            "telling the price.\n"
            "Type the name of the item you wish to buy:\n"
            "~1bFOOD ~7- ~3b$2 ~7per unit\n~1bAXE - ~3b$15\n~1bSWORD ~7- ~3b$20\n"
            "~1bSUIT OF ARMOR ~7- ~3b$50\n~1bMAGIC AMULET ~7- ~3b$150\n"
            "~7or ~1bRETURN ~7to your adventure.\n"
            "~2bYou have ~3b$%d~2b, what will you buy?\n~7>", game->wealth);
        ngets(input, sizeof input);
        item = input;
    }
	if (!strncmp(item, "ax", 2)) {
	    if (game->inventory[axe])
	        slowprintfc("You already have the ~3baxe~7.\n");
        else if (game->wealth < 15)
            slowprintfc("You don't have enough for the ~3baxe~7.\n");
        else {
            slowprintfc("You buy the ~3baxe~7.\n");
            game->inventory[axe] = 1;
            game->wealth -= 15;
        }
	} else if (!strncmp(item, "sw", 2)) {
	    if (game->inventory[sword])
	        slowprintfc("You already have the ~3bsword~7.\n");
        else if (game->wealth < 20)
            slowprintfc("You don't have enough for the ~3bsword~7.\n");
        else {
            slowprintfc("You buy the ~3bsword~7.\n");
            game->inventory[sword] = 1;
            game->wealth -= 20;
        }
	} else if ((!strncmp(item, "ma", 2)) ||(!strncmp(item, "am", 2))) {
	    if (game->inventory[amulet])
	        slowprintfc("You already have the ~3bmagic amulet~7.\n");
        else if (game->wealth < 150)
            slowprintfc("You don't have enough for the ~3bmagic amulet~7.\n");
        else {
            slowprintfc("You buy the ~3bmagic amulet~7.\n");
            game->inventory[amulet] = 1;
            game->wealth -= 150;
        }
	} else if ((!strncmp(item, "su", 2)) ||(!strncmp(item, "ar", 2))) {
	    if (game->inventory[suit])
	        slowprintfc("You already have the ~3bsuit of armor~7.\n");
        else if (game->wealth < 50)
            slowprintfc("You don't have enough for the ~3bsuit of armor~7.\n");
        else {
            slowprintfc("You buy the ~3bsuit of armor~7.\n");
            game->inventory[suit] = 1;
            game->wealth -= 50;
        }
	} else if (!strncmp(item, "fo", 2)) {
        int num;

        slowprintfc("~2bHow much ~3bfood ~2bdo you want to buy?\n~7>");
        scanw("%d", &num);
        if (game->wealth < 2 * num) {
            slowprintfc("You can't afford that much ~3bfood~7.\n");
        } else {
            slowprintfc("You buy ~3b%d ~7units of ~3bfood~7.\n", num);
            game->wealth -= 2 * num;
            game->food += num;
        }
	} else if (!strncmp(item, "re", 2)) {
        slowprintfc("You thank the little man and turn away.\n");
	} else {
        slowprintfc("The old man is puzzled by your request\n"
            "for ~1b%s~7.\n", item);
	}
}

void combat(struct gametype *game) {
    int odds = game->room[game->playerloc].monster.strength;
    int weapon = 0;

    if (game->inventory[suit]) {
        odds = 3 * (odds / 4);
    }
    if (game->inventory[sword] && game->inventory[axe]) {
        char input[32];

        while (!weapon) {
            slowprintfc("Will you weild the ~1bAXE ~7or the ~1bSWORD~7?\n>");
            ngets(input, sizeof input);
            strtolower(input);
            if (!strncmp(input, "sw", 2))
                weapon = sword;
            else if (!strncmp(input, "ax", 2))
                weapon = axe;
            else
                slowprintfc("Invalid. Type ~1bSWORD ~7or ~1bAXE~7.\n");
        }
    } else if (game->inventory[sword])
        weapon = sword;
    else if (game->inventory[axe])
        weapon = axe;
    if (!weapon) {
        slowprintfc("You bravely raise your ~3bfists ~7against the\n~4b%s~7.\n",
            game->room[game->playerloc].monster.name);
        odds += odds / 5;
    }
    if (weapon == sword) {
        slowprintfc("You wield your ~3bsword ~7against the\n~4b%s~7.\n",
            game->room[game->playerloc].monster.name);
        odds = 4 * (odds / 5);
    }
    if (weapon == axe) {
        slowprintfc("You brandish your ~3baxe ~7against the\n~4b%s~7.\n",
            game->room[game->playerloc].monster.name);
        odds = 3 * (odds / 4);
    }
    do {
        if (randint(1,10) <= 5) {
            slowprintfc("~2bYour attack lands a blow on the\n%s.\n",
               game->room[game->playerloc].monster.name);
            odds = (int)(5 * (float)odds / 6);
        } else
            slowprintfc("Your clumsy strike misses.\n");
        if (randint(1,10) <= 5) {
            slowprintfc("~4bThe %s strikes you.\n",
                game->room[game->playerloc].monster.name);
            if (game->inventory[suit])
                game->strength -= 2;
            else
                game->strength -= 5;
        } else
            slowprintfc("You narrowly avoid the creature's attack.\n");
    } while (randint(0,100) > 35);

    if (randint(0, 15) > odds) {
        slowprintfc("~2bA lucky blow in your favor and you finish off\n"
            "the %s!\n", game->room[game->playerloc].monster.name);
            game->monster_tally++;
    } else {
        slowprintfc("~4bThe %s defeats you and\nmoves on, leaving you "
            "wounded on the ground.\n",
            game->room[game->playerloc].monster.name);
        game->strength /= 2;
    }
    slowprintfc("Your strength is now ~3b%d~7.\n", game->strength);
    game->room[game->playerloc].monster.strength = 0;
}

int gamesortwealth(const void *a, const void *b) {
    struct gametype *aa = (struct gametype *)a;
    struct gametype *bb = (struct gametype *)b;

    if (aa->wealth < bb->wealth)
        return 1;
    else if (aa->wealth > bb->wealth)
        return -1;
    return 0;
}

int gamesortmoves(const void *a, const void *b) {
    struct gametype *aa = (struct gametype *)a;
    struct gametype *bb = (struct gametype *)b;

    if (aa->moves == 0)
        return 1;
    if (bb->moves == 0)
        return -1;
    if (aa->moves > bb->moves)
        return 1;
    else if (aa->moves < bb->moves)
        return -1;
    return 0;
}

int gamesortmk(const void *a, const void *b) {
    struct gametype *aa = (struct gametype *)a;
    struct gametype *bb = (struct gametype *)b;

    if (aa->monster_tally < bb->monster_tally)
        return 1;
    else if (aa->monster_tally > bb->monster_tally)
        return -1;
    return 0;
}

void gameout(struct gametype *game) {
    char *datestr = asctime(localtime(&game->date));
    datestr[11] = 0;
    if (game->name[0])
        slowprintfc("%s- %s %s ~7with ~3b$%d ~7and slayed ~6~%d ~7in ~5~%d ~7moves.\n",
                   datestr, game->name, (game->strength > 0)?"~2escaped":"~4bdied",
                   game->wealth, game->monster_tally, game->moves);
}

void high_scores(struct gametype *game) {
    int c;
    FILE* sf;
    struct gametype wealthiest[4], fastest[4], killer[4];

    memset (&wealthiest, 0, sizeof wealthiest);
    memset (&fastest, 0, sizeof fastest);
    memset (&killer, 0, sizeof killer);
    sf = fopen("scores.dat", "r+b");
    if (sf != NULL) {
        fread(wealthiest, sizeof wealthiest[0], 3, sf);
        fread(killer, sizeof killer[0], 3, sf);
        fread(fastest, sizeof fastest[0], 3, sf);
        fclose(sf);
    } else {
        slowprintfc("~4bNo high scores yet.\n");
    }
    if (game != NULL) {
        game->date=time(NULL);
        if ((game->wealth >= wealthiest[2].wealth)
            || (game->monster_tally >= killer[2].monster_tally)
            || (game->moves < fastest[2].moves)
            || (fastest[2].moves == 0)) {
            slowprintfc("~2bWhat is your name, adventurer?\n~7>");
            ngets(game->name, sizeof game->name);
            if (!game->name[0])
                sprintf(game->name, "Wanderer");
        }
        memmove(&wealthiest[3], game, sizeof *game);
        memmove(&killer[3], game, sizeof *game);
        if (game->strength > 0)
            memmove(&fastest[3], game, sizeof *game);
        qsort(wealthiest, 4, sizeof wealthiest[0], gamesortwealth);
        qsort(killer, 4, sizeof killer[0], gamesortmk);
        qsort(fastest, 4, sizeof fastest[0], gamesortmoves);
        sf = fopen("scores.dat", "w+b");
        fwrite(wealthiest, sizeof wealthiest[0], 3, sf);
        fwrite(killer, sizeof killer[0], 3, sf);
        fwrite(fastest, sizeof fastest[0], 3, sf);
        fclose(sf);
    }
    if (wealthiest[0].wealth > 0) {
        slowprintfc("\n~3bWealthiest Adventurers:\n");
        for (c = 0; c < 3; c++)
            gameout(&wealthiest[c]);
    }
    if (killer[0].monster_tally > 0) {
        slowprintfc("\n~6~Bravest Adventurers:\n");
        for (c = 0; c < 3; c++)
            gameout(&killer[c]);
    }
    if (fastest[0].moves > 0) {
        slowprintfc("\n~5Fastest Adventurers:\n");
        for (c = 0; c < 3; c++)
            gameout(&fastest[c]);
    }
    slowprintfc("\n");
}

int getcommands(struct gametype *game) {
   	char v[32], n[32];
   	int c;
   	int hasmonster = !!game->room[game->playerloc].monster.strength;
   	int hastreasure = !!game->room[game->playerloc].treasure.value;
   	int hasmerchant = game->room[game->playerloc].merchant;

    if (game->strength <= 15) {
        slowprintfc("Your strength is failing. You should EAT.\n");
    }
    slowprintfc("Possible commands: ");
    if (hasmonster) {
        slowprintfc("~1bFIGHT~7, ~1bRUN");
    } else {
        slowprintfc("~1bLOOK~7, ~1bGO");
        if (hastreasure)
            slowprintfc(", ~1bPICK UP");
        if (hasmerchant)
            slowprintfc(", ~1bTRADE");
        if (game->food > 0)
            slowprintfc(", ~1bEAT");
    }
    if (game->inventory[amulet])
        slowprintfc(", ~1bMAGIC");
    slowprintfc(" or ~1bHELP\n~2bWhat do you want to do?\n~7>");
	do
        scanpair(v, n);
    while (!v[0]);
	strtolower(v); strtolower(n);
	if (!strncmp(v, "fi", 2)) {
	    if (!hasmonster) {
	        slowprintfc("There's nothing here to fight.\n");
	        return -1;
	    }
        combat(game);
	} else if ((!strncmp(v, "ru", 2)) ||(!strncmp(v, "fl", 2))) {
	    if (!hasmonster) {
	        slowprintfc("There's nothing here to run from.\n");
	        return -1;
	    }
	    if (randint(1, 10) <= 7) {
	        if (!n[0]) {
                slowprintfc("~2bRun which way?\n!7>");
                ngets(n, sizeof n);
	        }
            strtolower(n);
            if (!go_exit(game, n)) {
                slowprintfc("In your panic you run into the wall.\n");
                return -1;
            } else
                slowprintfc("Dodging the creature you flee %s.\n", n);
            look(game);
	    } else {
	        slowprintfc("The %s blocks your exit and attacks.\n",
                game->room[game->playerloc].monster.name);
            combat(game);
	    }
	} else if ((!strncmp(v, "pi", 2)) ||(!strncmp(v, "ge", 2))) {
	    if (hasmonster) {
	        slowprintfc("Before you can act the ~4b%s ~7attacks.\n",
                game->room[game->playerloc].monster.name);
            combat(game);
	    } else if (hastreasure) {
	        slowprintfc("You add the ~6b%s ~7to your wealth.\n",
                game->room[game->playerloc].treasure.name);
            game->wealth+=game->room[game->playerloc].treasure.value;
            game->room[game->playerloc].treasure.value = 0;
	    } else {
	        slowprintfc("There is nothing here to get.\n");
	        return -1;
	    }
	} else if ((!strncmp(v, "tr", 2)) || (!strncmp(v, "bu", 2))) {
	    if (hasmonster) {
            slowprintfc("Before you can act the ~4b%s ~7attacks.\n",
                game->room[game->playerloc].monster.name);
            combat(game);
	    } else {
	        trade(game, n);
	    }
	} else if (!strncmp(v, "ma", 2)) {
	    if (game->inventory[amulet]) {
	        slowprintfc("You feel a strange power tingle over\n"
                "your whole body and you are wisked away.\n");
            do
                game->playerloc = randint(1, numrooms);
            while ((game->playerloc == START) || (game->playerloc == END));
            game->strength -= 20;
            look(game);
	    } else {
	        slowprintfc("Invalid command '%s'.\n", v);
	        return -1;
	    }
	} else if (!strncmp(v, "eat", 3)) { /* so as to not confuse with "east" */
	    if (hasmonster) {
            slowprintfc("Before you can act the ~4b%s ~7attacks.\n",
               game->room[game->playerloc].monster.name);
            combat(game);
	    } else if (!game->food) {
	        slowprintfc("You don't have any food.\n");
	    } else {
            slowprintfc("You have ~3b%d ~7units of food.\n"
                "~2bHow much do you want to eat?\n~7>", game->food);
            scanw("%d", &c);
            if (c > game->food)
                slowprintfc("You don't have that much food.\n");
            else if (c) {
                game->strength += 5 * c;
                game->food -= c;
                slowprintfc("You feel much better after eating something.\n");
                if (game->strength > 105) {
                    slowprintfc("...but you feel you may have over eaten.\n");
                    game->strength = 105;
                }
                slowprintfc("You now have ~3b%d ~7strength.\n", game->strength);
            } else
                slowprintfc("Command canceled.\n");
	    }
	} else if (!strncmp(v, "lo", 2)) {
	    look(game);
	} else if ((!strncmp(v, "qu", 2)) ||(!strncmp(v, "exit", 4))) {
	    exit(0);
	} else if (!strncmp(v, "ex", 2)) {
	    if (hasmonster) {
            slowprintfc("Before you can act the ~4b%s ~7attacks.\n",
                game->room[game->playerloc].monster.name);
            combat(game);
	    } else
            examine(game);
	} else if ((!strncmp(v, "he", 2)) ||(!strncmp(v, "?", 1))) {
	    help(n);
	    look(game);
	    return -1;
	} else if (!strncmp(v, "inv", 3)) {
	    playerstats(game);
	} else if (!strncmp(v, "hi", 2)) {
	    high_scores(NULL);
	    return -1;
	} else if (!strncmp(v, "go", 2)) {
	    if (hasmonster) {
	        slowprintfc("Before you can act the ~4b%s ~7attacks.\n",
                game->room[game->playerloc].monster.name);
            combat(game);
	    } else {
	        if (!n[0]) {
	            exits(game);
                slowprintfc("~2bWhich exit?\n~7>");
                ngets(n, sizeof n);
                strtolower(n);
            }
            if (!go_exit(game, n)) {
                slowprintfc("Invalid exit ~5b%s~7.\n", n);
                return -1;
            }
            look(game);
	    }
	} else {
	    if (hasmonster) {
	        slowprintfc("Before you can act the ~4b%s ~7attacks.\n",
                game->room[game->playerloc].monster.name);
            combat(game);
	    } else if (!go_exit(game, v)) {
	        slowprintfc("Invalid command ~1b%s~7.\n", v);
	        return -1;
	    }
	    look(game);
	}
	return 0;
}

int gameloop(struct gametype *game) {
    if (0 <= getcommands(game))
        game->moves++;
	if (game->playerloc == END || game->strength <= 0)
        return 0;
    return 1;
}

int again() {
  char input[LINE];

  slowprintfc("\n~2bDo you want to play again?\n~7>");
  ngets(input, sizeof input);
  if (tolower(input[0]) == 'y') return 1;
  else return 0;
}

void score(struct gametype *game) {
    int c, story = 0;

    if (game->strength > 0)
        slowprintfc("You escaped");
    else
        slowprintfc("You died");
    slowprintfc(" having aquired ~6b$%d ~7and slayed ~6b%d ~7monster%c\n"
                "in ~6b%d ~7moves.\n",
        game->wealth,
        game->monster_tally, (game->monster_tally == 1)?' ':'s',
        game->moves);
    for (c = 0; c < numrooms + 1; c++)
        if (game->room[c].foundstory)
            story++;
    slowprintfc("You found ~3b%d ~7out of ~3~%d ~7story entries.\n", story, numexposition);
    high_scores(game);
}

void init(struct gametype *game) {
    int c, r = 0;

    srand(time(NULL));
    game->playerloc = START;
    game->strength = 50;
    game->monster_tally = 0;
    game->wealth = 25;
    game->food = 0;
    game->moves = 0;
    /* memset, a quick way to clear arrays... */
    memset(game->inventory, 0, sizeof game->inventory);
    memset(game->room, 0, sizeof game->room);

    for(c = 0; c < nummerchants; c++) {
        if (c == 0)
            r = 5;
        else do {
            r = randint(1, numrooms);
        } while (game->room[r].merchant || (r == START) || (r == END));
        game->room[r].merchant = 1;
    }
    for(c = 0; c < nummonsters; c++) {
        do {
            r = randint(1, numrooms);
        } while (game->room[r].merchant || game->room[r].monster.strength
                  || (r == START) || (r == END));
        switch(c) {
        case 0:
            snprintf(game->room[r].monster.name, LINE, "ferocious werewolf");
            game->room[r].monster.strength = 5;
            break;
        case 1:
            snprintf(game->room[r].monster.name, LINE, "fanatical fleshgorger");
            game->room[r].monster.strength = 10;
            break;
        case 2:
            snprintf(game->room[r].monster.name, LINE, "malevolent maldemer");
            game->room[r].monster.strength = 15;
            break;
        case 3:
            snprintf(game->room[r].monster.name, LINE, "devastating ice-dragon");
            game->room[r].monster.strength = 20;
            snprintf(game->room[r].treasure.name, LINE, "gold and gems");
            game->room[r].treasure.value = 900;
            break;
        default: /* shouldn't need, but just in case */
            snprintf(game->room[r].monster.name, LINE, "monster");
            game->room[r].monster.strength = 5;
        }
    }

    for(c = 0; c < numtreasure; c++) {
        if (c == 0) {
            r = 4;
        } else if (c == 1) {
            r = 16;
        } else do {
            r = randint(1, numrooms);
        } while (game->room[r].treasure.value || (r == START) || (r == END));
        switch(c) {
        case 0:
            snprintf(game->room[r].treasure.name, LINE, "diamonds");
            game->room[r].treasure.value = 169;
            break;
        case 1:
            snprintf(game->room[r].treasure.name, LINE, "jewels");
            game->room[r].treasure.value = 500;
            break;
        case 2:
            snprintf(game->room[r].treasure.name, LINE, "emerald");
            game->room[r].treasure.value = 274;
            break;
        default:
            snprintf(game->room[r].treasure.name, LINE, "treasure");
            game->room[r].treasure.value = randint(10, 110);
            break;
        }
    }
}

int main() {
    struct gametype game; /* storage for all game data */

    colorinit ();

    slowprintfc("~6WEREWOLVES AND WANDERER\n"
        "Original game by Tim Hartnell.\n"
        "Remake by Joseph Larson\n");


    do {
        init(&game);
        look(&game);
        while (gameloop(&game)) ;
        if (game.strength > 0)
            slowprintfc("\nCongratulations you made it out alive! You turn\n"
                "from the the castle and it's strange adventure\n"
                "and return home with your riches and your life,\n"
                "grateful to have both.\n");
        else
            slowprintfc("\nYour adventure leaves you without any strength.\n"
                       "You colapse on the ground as the world fades\n"
                       "around you.\n"
                       "(TIP: Eating will keep your strength up.)\n");
        score(&game);
    } while (again());
    endwin();
    return 0;
}
