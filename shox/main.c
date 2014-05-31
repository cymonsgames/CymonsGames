/* Shox
 * by Joseph Larson ver 2008-apr-30
 * Initilazation comments taken from ballgame.c, a ncurses example program.
 */

#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

int dirs[6] = {0, 1, 0, -1, 0, 1};

void init () {
  srand (time(NULL));
  initscr (); /* Starts curses (initializes the data structures, starts
               *   with blank "curses screen", shows it.) */

  PDC_set_title("Shox by Cymon's Games");

  raw (); /* Lets you read chars as they are typed (no need to wait for <ENTER>)
           * Disables interrupts such as ^C, ^S
           * [cbreak() is like raw(), but ^C stops the program] */

  nodelay(stdscr,1); /* getch() doesn't even wait for one char -- returns ERR if
                      * the user didn't type anything yet */

  noecho(); /* Don't echo (show) characters as they are typed */

  curs_set(0); /* 0=don't show cursor */

  nonl(); /* [no newline] Without this, ENTER key generates ^M\n
           * With nonl(), ENTER is just ^M (13)
           * The curses value KEY_ENTER is for "cooked" (not raw) mode. */

  keypad(stdscr,0); /* Read arrow keys (enables the "keypad" where they live) */
}

void play () {
  int x, y, bx, by, c, d, in, bul, lvl, buf[LINES][COLS];
  /* note about LINES and COLS: Curses maintains these globals to track screen
   * size.
   * (1) LINES and COLS has no usable value before initscr().
   * (2) Some terminals can be resized on the fly. In this case getch() will
   *     return KEY_RESIZE. A good program will catch this and adjust
   *     accordingly. This program does not. */

  y = x = d = lvl = 0; bul = 10;  /* Set up variables for new game. */
  do {  /* Set up varaibles for new level */
    clear ();
    lvl++; bul += 15;
    mvprintw (LINES - 1, 37, "Round %d", lvl);
    for (by = 0; by < LINES; by++) for (bx = 0; bx < COLS; bx++)
      buf[by][bx] = 0; /* clear buffer */
    for (c = 0; c < 15; c++) {  /* Set up enemies. */
      do {
        by = 1 + rand () % (LINES - 3); bx = 1 + rand () % (COLS - 2);
      } while (buf[by][bx]);
      buf[by][bx] = 1;
      mvaddch (by, bx, 'X');
    } /* c will count targets */

    /* Draw last few more things on the screen. */
    mvaddch (y, x, 'O');
    mvprintw (LINES - 1, 0, "Bullets : %d ", bul); /* space after the d */
    mvprintw (LINES - 1, 60, "Press SPACE to fire.");
    refresh ();
    while ((bul > 0) && (c > 0)) { /* The main game loop */
      in = getch ();
      if (tolower(in) == 'q') return;
      if (in == ' ') { /* shoot a bullet */
        bul--;
        mvprintw (LINES - 1, 0, "Bullets : %d ", bul); /* space after again. */
        bx = x; by = y;
        do {
          bx += dirs[d + 1]; by += dirs[d + 2]; /* move the bullet */
          if (buf[by][bx]) { /* a hit */
            beep ();
            buf[by][bx] = 0;
            c--;
          }
          mvaddch (by, bx, '.');
          refresh ();
          napms (15);
          mvaddch (by, bx, ' ');
        } while  (!((bx==0) + (by==0) + (bx==COLS - 1) + (by==LINES - 2)));
        /* above line is for edge detection. */
      }
      mvaddch (y, x, ' ');
      x += dirs[d]; y += dirs[d + 1];
      if (((x == 0) + (y == 0) + (x == COLS - 1) + (y == LINES - 2)) == 2) {
        /* if we're at a corner change direction. */
        d++; d %= 4;
      }
      mvaddch (y, x, 'O');
      refresh ();
      napms (275 - 25 * lvl);
    } /* end of level loop if out of bullets or out of enemies. */
  } while ((c == 0) && (lvl < 10));
  if (c) mvprintw (10, 27, "Out of bullets. Game over.");
  else mvprintw (10, 15, "Congratulations! You have defeated every enemy!");
  refresh ();
}

int main () {
  init ();

  printw ("\tCymon's Games\n\thttp://WWW.CYMONSGAMES.COM\n\n"
  "   This game and its code is a 2008 Cymon's Games game.\n"
  "   If you have enjoyed this game or would just like to have a new game\n"
  "   each week please visit:\n"
  "   http://WWW.CYMONSGAMES.COM for C/C++ programming resources and programs\n"
  "   for everyone, beginners and advanced users alike.\n\n");
  printw ("\n\tPress ENTER to continue...\n");
  refresh ();
  while (getch ()==ERR);

  play ();
  mvprintw (15, 32, "Press any key...");
  while (getch ()!=ERR);
  nodelay(stdscr,0); getch ();
  endwin ();
  return 0;
}
