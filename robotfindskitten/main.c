/*robotfindskitten v22July2008 source code.
 *
 * Modified from original code by Leonard Richardson leonardr@segfault.org 1997
 * By Joseph Larson for Cymon's Games 2008, http://www.cymonsgames.com
 */

#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define EMPTY -1
#define ROBOT 0
#define KITTEN 1
#define MAXMESSAGES 1000

void instructions();
void draw_robot();
void draw_kitten();
void process_input(int input, int** screen);
void play ();

typedef struct object
{
	int x;
	int y;
	int color;
	int character;
} OBJECT;

OBJECT robot;
OBJECT kitten;
OBJECT empty;
OBJECT bogus[MAXMESSAGES];
int bogus_messages[MAXMESSAGES];
char messages[MAXMESSAGES][81] =
{
    "This is not a kitten.",
    "That's just an old tin can.",
    "It's an altar to the horse god.",
    "A mere collection of pixels.",
    "A box of fumigation pellets.",
    "More grist for the mill.",
    "It's a square.",
    "Run away!  Run away!",
    "The rothe hits!  The rothe hits!",
    "This place is called Antarctica. There is no kitten here.",
    "It's a copy of \"Zen and The Art of Robot Maintenance\".",
    "\"Yes!\" says the bit.",
    "\"No!\" says the bit.",
    "A robot comedian. You feel amused.",
    "A forgotten telephone switchboard.",
    "It's a desperate plug for Cymon's Games, http://www.cymonsgames.com/",
    "The letters O and R.",
    "\"Blup, blup, blup\" says the mud pot.",
    "Another rabbit? That's three today!",
    "Thar's Mobius Dick, the convoluted whale. Arrr!",
    "This object here appears to be Louis Farrakhan's bow tie.",
    "...thingy???",
    "Pumpkin pie spice.",
    "Chewing gum and baling wire.",
    "It's the crusty exoskeleton of an arthropod!",
  };
int used_messages[MAXMESSAGES];
int numbogus;
int nummessages;

int main(int argc, char *argv[])
{
  int supplied_number; /*Number of bogus OBJECTs, supplied on command line. */
  int c, c2;
  int** screen;
  FILE *fp;
  char buffer[255];

  nummessages = 0;
  fp = fopen ("messages.txt", "r");
  if (fp == NULL) {
    nummessages = 25;
  }
  else
  {
    while (fgets (buffer, 255, fp)) {
      c = strlen(buffer);
      if (c > 1) {
        if (c > 79) c = 79;
        buffer[c-1] = 0;
        strcpy (messages[nummessages++], buffer);
      }
    }
  }

	if (argc == 1)
	{
	 numbogus = 20;
	} else {
	 supplied_number = atoi(argv[1]);
	 if (supplied_number < 0 || supplied_number > nummessages)
	 {
	  printw ("Error: run-time parameter must be between 0 and %d.", nummessages);
	  endwin (); exit(0);
	 } else {
	  numbogus = supplied_number;
	 }
	}
	/* Do general start-of-program stuff. */
	srand(time(NULL));
  initscr ();
  PDC_set_title("Robotfindskitten on Cymon's Games");
  raw (); noecho(); curs_set(0); nonl(); keypad(stdscr,1);
  start_color();
  init_pair(0, COLOR_WHITE & A_BOLD, 0); /* for text */
  init_pair(1, COLOR_WHITE, 0); /* for robot */
  for (c = 2; c < COLORS; c++) init_pair(c, c, COLOR_BLACK);

	empty.x = -1;
	empty.y = -1;
	empty.color = 0;
	empty.character = ' ';

	screen = (int**)malloc((COLS + 1) * sizeof (int*));
	for (c = 0; c <= COLS; c++) screen[c] = (int*)malloc((LINES + 1) * sizeof (int));

	/* Now we have the filling in of the various arrays.
	 * Create an array to represent the screen so that we can make sure all
	 * the OBJECTs go on different squares. */
	for (c = 0; c <= COLS; c++)
	{
		for (c2 = 0; c2 <= LINES; c2++)
		{
			screen[c][c2] = EMPTY;
		}
	}
	/* Create an array to ensure we don't get duplicate messages. */
	for (c = 0; c < nummessages; c++)
	{
		used_messages[c] = 0;
		bogus_messages[c] = 0;
		bogus[c] = empty;
	}

	/* Now we initialize the various game OBJECTs.
   * Assign a position to the player. */
	robot.x = rand() % COLS;
	robot.y = rand() % (LINES - 3) + 3;
	robot.character = '#';
	robot.color = 1;
	screen[robot.x][robot.y] = ROBOT;

	/* Assign the kitten a unique position. */
	do
	{
		kitten.x = rand() % COLS;
		kitten.y = rand() % (LINES - 3) + 3;
	} while (screen[kitten.x][kitten.y] != EMPTY);

	/* Assign the kitten a character and a color. */
	do
	{
		kitten.character = rand() % 254 + 1;
	} while (kitten.character == '#' || kitten.character == ' ');
	kitten.color = rand() % (COLORS - 2) + 2;
	screen[kitten.x][kitten.y] = KITTEN;

	/* Now, initialize non-kitten OBJECTs. */
	for (c = 0; c < numbogus; c++)
	{
		/* Assign a unique position. */
		do
		{
			bogus[c].x = rand() % COLS;
			bogus[c].y = rand() % (LINES - 3) + 3;
		} while (screen[bogus[c].x][bogus[c].y] != EMPTY);
		screen[bogus[c].x][bogus[c].y] = c+2;

		/* Assign a character. */
		do
		{
			bogus[c].character = rand() % 254 + 1;
		} while (bogus[c].character == '#' || bogus[c].character == ' ');
		bogus[c].color = rand() % (COLORS - 2) + 2;;

		/* Assign a unique message. */
		int index = 0;
		do
		{
			index = rand() % nummessages;
		} while (used_messages[index] != 0);
		bogus_messages[c] = index;
		used_messages[index] = 1;
	}

	/* Print instructions. */
	clear();
	instructions();
	clear();

	/* Put stuff on the screen. */
	attrset (COLOR_PAIR(0));
	move(0,0);
	attron (A_BOLD);
	printw ("robotfindskitten v22July2008");
	move(2,0);
	for (c = 0; c < COLS; c++) addch ('_');
	attroff (A_BOLD);
	move(kitten.y,kitten.x);
	draw_kitten();

	move (3,0);
	for (c = 0; c < numbogus; c++)
	{
		move(bogus[c].y,bogus[c].x);
		attrset (COLOR_PAIR(bogus[c].color));
		addch(bogus[c].character|A_ALTCHARSET);
	}

	move(robot.y,robot.x);
	draw_robot();
	int old_x = robot.x;
	int old_y = robot.y;

	/* Now the fun begins. */
	int input;
	input = getch();
	while (tolower(input) != 'q')
	{
		process_input(input, screen);
		/* Redraw robot, where avaliable */
		if (!(old_x == robot.x && old_y == robot.y))
		{
			move(old_y,old_x);
			addch(' ');
			move(robot.y,robot.x);
			draw_robot();
			screen[old_x][old_y] = EMPTY;
			screen[robot.x][robot.y] = ROBOT;
		old_x = robot.x;
			old_y = robot.y;
		}
		input = getch();
	}
	endwin ();
	return 0;
}

/* Given the keyboard input, interprets it. */
void process_input(int input, int** screen)
{
	int check_x = robot.x;
	int check_y = robot.y;
	int c;

	switch (input)
	{
		case KEY_UP: /* up */
			check_y--;
			break;
		case KEY_DOWN: /* down */
			check_y++;
			break;
		case KEY_LEFT: /* left */
			check_x--;
			break;
		case KEY_RIGHT: /* right */
			check_x++;
			break;
		case 0:
			break;
		default: /* invalid command */
			move(1,0);
			attrset (COLOR_PAIR(0));
			printw("Invalid command: Use direction keys or Press 'Q'."); clrtoeol ();
			return;
	}

	/* Check for going off the edge of the screen. */
	if (check_y < 3 || check_y > LINES - 1 || check_x < 0 || check_x > COLS - 1)
	{
		return;
	}

	/* Check for collision */
	if (screen[check_x][check_y] != EMPTY)
	{
		switch (screen[check_x][check_y])
		{
			case ROBOT:
				/* We didn't move. */
				break;
			case KITTEN: /* Found it! */
				move(1,0); clrtoeol ();

				/* The grand cinema scene. */
				for (c = 0; c <=3; c++)
				{
					move(1, COLS / 2 - 5 + c);
					addch(' ');
					move(1,COLS / 2 + 4 - c);
					addch(' ');
					move(1,COLS / 2 - 4 + c);
					if (input == KEY_LEFT || input == KEY_UP)
						draw_kitten();
					else
						draw_robot();
					move(1,COLS / 2 + 3 - c);
					if (input == KEY_LEFT || input == KEY_UP)
						draw_robot();
					else
						draw_kitten();
          refresh (); napms (600);
				}
				/* They're in love! */
				move(0,COLS / 2 - 1);
				attrset (COLOR_PAIR(COLOR_RED)|A_BOLD);
				addch(3|A_ALTCHARSET);
				addch(3|A_ALTCHARSET);
				move(1,0);
				refresh(); napms (1200);
				attrset (COLOR_PAIR(0)); attron (A_BOLD);
				printw ("You found kitten! Way to go, robot!");
				attroff (A_BOLD);
				getch ();
				endwin (); exit(0);
				break;
			default:
				move(1,0);
				attron (A_BOLD);
				printw (messages[bogus_messages[screen[check_x][check_y]-2]]); clrtoeol ();
				attroff (A_BOLD);
				break;
		}
		move(2,0);
		return;
	}
	/* Otherwise, move the robot. */
	robot.x = check_x;
	robot.y = check_y;
}

void instructions()
{
	attrset (COLOR_PAIR(0)|A_BOLD);
	move(0,0);
	printw ("robotfindskitten v22July2008\n"
    "Originally by the illustrious Leonard Richardson 1997\n"
    "Written especially for the Nerth Pork robotfindskitten contest\n"
    "ReWritten in PDCurses by Joseph Larson\n"
    "Especially for Cymon's Games 2008 HTTP://WWW.CYMONSGAMES.COM\n\n"
    "   In this game, you are robot (");
	draw_robot();
	attrset (COLOR_PAIR(0)|A_BOLD);
	printw ("). Your job is to find kitten. This task\n"
    "is complicated by the existance of various things which are not kitten.\n"
    "Robot must touch items to determine if they are kitten or not. The game\n"
    "ends when robotfindskitten. Alternatively, you may end the game by hitting\n"
    "the Esc key. See the documentation for more information.\n\n"
    "   Press any key to start.\n");
	getch();

}

void draw_robot() /* Draws robot at current position */
{
	attrset (COLOR_PAIR(robot.color));
	addch(robot.character);
}

void draw_kitten() /* Draws kitten at current position */
{
	attrset (COLOR_PAIR(kitten.color));
	addch(kitten.character|A_ALTCHARSET);
}
