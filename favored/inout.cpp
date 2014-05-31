#include "inout.h"

void introanim (){
  int bunx, buny, turn, px, py, xx, yy;
  nodelay(stdscr,1);
  attrset (COLOR_PAIR(COLOR_WHITE) | A_BOLD);
  mvprintw(1,5,"                 .___________. __    __   _______ ");
  mvprintw(2,5,"                 |           ||  |  |  | |   ____|");
  mvprintw(3,5,"                 `---|  |----`|  |__|  | |  |__   ");
  mvprintw(4,5,"                     |  |     |   __   | |   __|  ");
  mvprintw(5,5,"                     |  |     |  |  |  | |  |____ ");
  mvprintw(6,5,"                     |__|     |__|  |__| |_______|");
  mvprintw(7,5," _______    ___   ____    ____  ______   .______    _______  _______  ");
  mvprintw(8,5,"|   ____|  /   \\  \\   \\  /   / /  __  \\  |   _  \\  |   ____||       \\ ");
  mvprintw(9,5,"|  |__    /  ^  \\  \\   \\/   / |  |  |  | |  |_)  | |  |__   |  .--.  |");
  mvprintw(10,5,"|   __|  /  /_\\  \\  \\      /  |  |  |  | |      /  |   __|  |  |  |  |");
  mvprintw(11,5,"|  |    /  _____  \\  \\    /   |  `--'  | |  |\\  \\  |  |____ |  '--'  |");
  mvprintw(12,5,"|__|   /__/     \\__\\  \\__/     \\______/  |__| \\__\\ |_______||_______/ ");
  attrset (COLOR_PAIR(COLOR_GREEN) | A_BOLD);
  for (yy = 2 * LINES / 3; yy < LINES; yy++)
    for (xx = 0; xx < COLS; xx++) mvaddch(yy, xx, '#'); // Grass
  if (getch() != ERR) { nodelay(stdscr,0); return; }
  int buncol = rand() % COLORS;
  px = bunx = 0; py = buny = 2 * LINES / 3 - 1; turn = 0;
  while ((bunx <= 3 * COLS / 4) || (px <= COLS / 2)) { // We're animating with math!
    if (bunx <= 3 * COLS / 4) {
      attrset (COLOR_PAIR(buncol) | A_BOLD);
      if ((turn % 3) == 0) {
        mvaddch(buny--, bunx++, ' ');
        mvaddch(buny, bunx, 'd'); // up
      }
      if ((turn % 3) == 1) {
        mvaddch(buny++, bunx++, ' ');
        mvaddch(buny, bunx, 'd'); // down
      }
    }
    if ((px <= COLS / 2) && (turn > 30)) {
      attrset (COLOR_PAIR(COLOR_WHITE));
      if (turn > 5 * COLS / 8) {// bunny head start
        mvaddch (py, px++, ' ');
        mvaddch (py, px, '@');
      }
    }
    refresh (); napms(100);
    if (getch() != ERR) { nodelay(stdscr,0); return; }
    turn++;
  }
  mvaddch(buny, bunx, ' ');
  attrset (COLOR_PAIR(buncol) | A_BOLD);
  mvaddch(py, bunx, 'b');
  attrset (COLOR_PAIR(COLOR_WHITE));
  mvaddch(py, px, '@');
  attrset (COLOR_PAIR(COLOR_GREEN) | A_BOLD);
  for (xx = 3 * COLS / 8; xx < 5 * COLS / 8; xx++)  // The ground breaks
    for (yy = 2 * LINES / 3; yy < LINES; yy++) {
      mvaddch (yy, xx, 'X');
    }
  refresh (); napms(300);
  attrset (COLOR_PAIR(COLOR_YELLOW) | A_BOLD);
  mvaddch (py-1, px, '?'); // Huh?
  if (getch() != ERR) { nodelay(stdscr,0); return; }
  attrset (COLOR_PAIR(COLOR_GREEN) | A_BOLD);
  for (xx = 3 * COLS / 8; xx < 5 * COLS / 8; xx++)  // The ground breaks more
    for (yy = 2 * LINES / 3; yy < LINES; yy++) {
      mvaddch (yy, xx, ':');
    }
  refresh (); napms(300);
  if (getch() != ERR) { nodelay(stdscr,0); return; }
  attrset (COLOR_PAIR(COLOR_GREEN) | A_BOLD);
  for (xx = 3 * COLS / 8; xx < 5 * COLS / 8; xx++)  // The ground breaks more
    for (yy = 2 * LINES / 3; yy < LINES; yy++) {
      mvaddch (yy, xx, '.');
    }
  refresh (); napms(300);
  if (getch() != ERR) { nodelay(stdscr,0); return; }
  for (xx = 3 * COLS / 8; xx < 5 * COLS / 8; xx++)  // The ground is gone
    for (yy = 2 * LINES / 3; yy < LINES; yy++) {
      mvaddch (yy, xx, ' ');
    }
  refresh (); napms(300);
  if (getch() != ERR) { nodelay(stdscr,0); return; }
  refresh (); napms(300);
  if (getch() != ERR) { nodelay(stdscr,0); return; }
  refresh (); napms(300);
  if (getch() != ERR) { nodelay(stdscr,0); return; }
  attrset (COLOR_PAIR(COLOR_WHITE));
  mvaddch (py - 1, px, ' ');
  for (yy = 2 * LINES / 3 - 1; yy < LINES; yy ++) { // fallin...
    mvaddch (yy, px, '@');
    refresh (); napms(50);
    if (getch() != ERR) { nodelay(stdscr,0); return; }
    mvaddch (yy, px, ' ');
  }
  refresh();
  nodelay(stdscr,0);
}

void intro () {
  introanim ();
  WINDOW * introwin;
  introwin = newwin (LINES - 4, COLS - 4, 2, 2);
  wclear (introwin);
  wattrset (introwin, COLOR_PAIR(COLOR_GREEN));
  box(introwin, 0, 0);
  wattrset (introwin, COLOR_PAIR(COLOR_WHITE));
  mvwprintw(introwin, 1, 2, "The Favored, a 7DRL by Joe Larson");
  mvwprintw(introwin, 3, 2, "You are a new adventurer who was leveling up in the fields around your");
  mvwprintw(introwin, 4, 2, "starting village by killing rodents and other small animals when a rabbit");
  mvwprintw(introwin, 5, 2, "you were chasing ran across a clearing, stopping only a few yards away.");
  mvwprintw(introwin, 6, 2, "As you closed the distance to the creature the ground beneath you");
  mvwprintw(introwin, 7, 2, "suddenly gave way and you fell into darkness. Did that rabbit set a trap?");
  mvwprintw(introwin, 9, 2, "Fortunately the rabbit's plan seems to have been only half-baked. Soon");
  mvwprintw(introwin, 10, 2, "as you awoke you found yourself in a huge rabbit warren, but none of the");
  mvwprintw(introwin, 11, 2, "other rabbits seem intent on harming you. You try to climb out, but the");
  mvwprintw(introwin, 12, 2, "ceiling of the cave you're in is too far away. Occasionally you see spots");
  mvwprintw(introwin, 13, 2, "where you could get up if you had some rope or something.");
  mvwprintw(introwin, 15, 2, "Unknown to our adventurer this is the warren of the favored wherein on");
  mvwprintw(introwin, 16, 2, "each level lives a rabbit favored of the rabbit gods. Killing this");
  mvwprintw(introwin, 17, 2, "hallowed creature will bring down the wrath of an angry deity.");
  wattrset (introwin, COLOR_PAIR(COLOR_RED)|A_BOLD);
  mvwprintw(introwin, 19, 2, "Press any key...");
  wrefresh (introwin); wgetch (introwin);
  wclear (introwin);
  wattrset (introwin, COLOR_PAIR(COLOR_GREEN));
  box(introwin, 0, 0);
  wattrset (introwin, COLOR_PAIR(COLOR_WHITE));
  mvwprintw(introwin, 1, 2, "PLAYING THE GAME");
  mvwprintw(introwin, 2, 2, "You control your character using the keypad or the following keys:");
  mvwprintw(introwin, 4, 2, "\tQ W E\tY K U\t7 8 9");
  mvwprintw(introwin, 5, 2, "\t \\|/\t \\|/\t \\|/");
  mvwprintw(introwin, 6, 2, "\tA-@-D\tH-@-L\t4-@-8");
  mvwprintw(introwin, 7, 2, "\t /|\\\t /|\\\t /|\\");
  mvwprintw(introwin, 8, 2, "\tZ X C\tB J N\t1 2 3");
  mvwprintw(introwin, 10, 2, "ESC quits. Press any other key to wait where you are.");
  mvwprintw(introwin, 12, 2, "Move into a rabbit to attack them. You can only carry so many rabbits at");
  mvwprintw(introwin, 13, 2, "a time. Move into the exit to drop the dead bunnies you've collected");
  mvwprintw(introwin, 14, 2, "there. When you drop enough corpses on the exit it will change");
  mvwprintw(introwin, 15, 2, "into a '<'. Then just walk into it to go to the next level, but keep in");
  mvwprintw(introwin, 16, 2, "mind that when you go up a level you can't go back.");
  wattrset (introwin, COLOR_PAIR(COLOR_RED)|A_BOLD);
  mvwprintw(introwin,18, 2, "Press any key to begin...");
  wrefresh(introwin); wgetch (introwin);
  delwin (introwin);
}

void killedfavored(int favored_color, double odds) {
  WINDOW *animwin;
  int width, yy, xx;

  animwin = newwin (0,0,0,0);
  wclear(animwin);
  nodelay(animwin,1);
  wattrset (animwin, COLOR_PAIR (COLORS));
  for (xx = 0; xx < COLS; xx++) mvwaddch (animwin, LINES - 1, xx, ' ');
  mvwprintw(animwin, LINES - 1, 0, "Probability: %.2f out of 100 against",(1 - odds) * 100);
  wattrset (animwin, COLOR_PAIR (COLOR_WHITE));
  mvwaddch (animwin, LINES - 2, COLS / 2 + 1, '@');
  wattrset (animwin, COLOR_PAIR (favored_color) | A_BOLD);
  mvwaddch (animwin, LINES - 2, COLS / 2 - 1, 'd');

  wattrset (animwin, COLOR_PAIR(COLOR_WHITE) | A_BOLD);
  wrefresh (animwin); napms(1000);
  if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }
  xx = COLS / 2 - 25 / 2;
  mvwprintw(animwin, 1,xx," _____ _");
  mvwprintw(animwin, 2,xx,"|_   _| |__   ___  _   _");
  mvwprintw(animwin, 3,xx,"  | | | '_ \\ / _ \\| | | |");
  mvwprintw(animwin, 4,xx,"  | | | | | | (_) | |_| |");
  mvwprintw(animwin, 5,xx,"  |_| |_| |_|\\___/ \\__,_|");
  wrefresh (animwin); napms(1500);
  if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }
  xx = COLS / 2 - (42 / 2);
  mvwprintw(animwin, 7,xx," ____  _           _ _   ");
  mvwprintw(animwin, 8,xx,"/ ___|| |__   __ _| | |_ ");
  mvwprintw(animwin, 9,xx,"\\___ \\| '_ \\ / _` | | __|");
  mvwprintw(animwin, 10,xx," ___) | | | | (_| | | |_ ");
  mvwprintw(animwin, 11,xx,"|____/|_| |_|\\__,_|_|\\__|");
  wrefresh (animwin); napms(500);
  if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }
  xx += 27;
  mvwprintw(animwin, 7,xx," _   _       _");
  mvwprintw(animwin, 8,xx,"| \\ | | ___ | |_");
  mvwprintw(animwin, 9,xx,"|  \\| |/ _ \\| __|");
  mvwprintw(animwin, 10,xx,"| |\\  | (_) | |_");
  mvwprintw(animwin, 11,xx,"|_| \\_|\\___/ \\__|");
  wrefresh (animwin); napms(800);
  if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }
  xx = COLS / 2 - (26 / 2);
  mvwprintw(animwin, 13,xx," _____                _");
  mvwprintw(animwin, 14,xx,"|_   _|__  _   _  ___| |__");
  mvwprintw(animwin, 15,xx,"  | |/ _ \\| | | |/ __| '_ \\");
  mvwprintw(animwin, 16,xx,"  | | (_) | |_| | (__| | | |");
  mvwprintw(animwin, 17,xx,"  |_|\\___/ \\__,_|\\___|_| |_|");
  wrefresh (animwin); napms(1500);
  if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }

  wclear (animwin);
  wattrset (animwin, COLOR_PAIR (COLORS));
  for (xx = 0; xx < COLS; xx++) mvwaddch (animwin, LINES - 1, xx, ' ');
  mvwprintw(animwin, LINES - 1, 0, "Probability: %.2f out of 100 against",(1 - odds) * 100);
  wattrset (animwin, COLOR_PAIR (COLOR_WHITE));
  mvwaddch (animwin, LINES - 2, COLS / 2 + 1, '@');
  wattrset (animwin, COLOR_PAIR (favored_color) | A_BOLD);
  mvwaddch (animwin, LINES - 2, COLS / 2 - 1, 'd');

  wattrset (animwin, COLOR_PAIR(COLOR_WHITE) | A_BOLD);
  mvwprintw(animwin, 1,5,"                 .___________. __    __   _______ ");
  mvwprintw(animwin, 2,5,"                 |           ||  |  |  | |   ____|");
  mvwprintw(animwin, 3,5,"                 `---|  |----`|  |__|  | |  |__   ");
  mvwprintw(animwin, 4,5,"                     |  |     |   __   | |   __|  ");
  mvwprintw(animwin, 5,5,"                     |  |     |  |  |  | |  |____ ");
  mvwprintw(animwin, 6,5,"                     |__|     |__|  |__| |_______|");
  wrefresh (animwin); napms(100);
  if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }
  mvwprintw(animwin, 7,5," _______    ___   ____    ____  ______   .______    _______  _______  ");
  mvwprintw(animwin, 8,5,"|   ____|  /   \\  \\   \\  /   / /  __  \\  |   _  \\  |   ____||       \\ ");
  mvwprintw(animwin, 9,5,"|  |__    /  ^  \\  \\   \\/   / |  |  |  | |  |_)  | |  |__   |  .--.  |");
  mvwprintw(animwin, 10,5,"|   __|  /  /_\\  \\  \\      /  |  |  |  | |      /  |   __|  |  |  |  |");
  mvwprintw(animwin, 11,5,"|  |    /  _____  \\  \\    /   |  `--'  | |  |\\  \\  |  |____ |  '--'  |");
  mvwprintw(animwin, 12,5,"|__|   /__/     \\__\\  \\__/     \\______/  |__| \\__\\ |_______||_______/ ");
  wrefresh (animwin); napms(3000);
  if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }

  // Lightning Crashes
  int lighttime[8]={200, 50, 200, 50, 400, 50, 200, 0};
  for (int t = 0; t < 8; t++) {
    xx = COLS / 2 + 2; width = 6;
    wattrset (animwin, COLOR_PAIR (COLOR_YELLOW));
    for (yy = 0; yy < LINES - 2; yy++) {
      if (!(yy % (LINES / 10))) xx--;
      wmove (animwin, yy, xx);
      for (int c = 0; c < width; c++) waddch (animwin, '#');
      if (!(yy % (LINES / 5))) width--;
      if (!(yy % (LINES / 3))) {
        xx += width;
        wmove (animwin, yy, xx);
        for (int c = 0; c < width; c++) waddch (animwin, '#');
      }
    }
    wrefresh (animwin); napms(lighttime[t++]);
    if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }
    wclear (animwin);
    wattrset (animwin, COLOR_PAIR (COLORS));
    for (xx = 0; xx < COLS; xx++) mvwaddch (animwin, LINES - 1, xx, ' ');
  mvwprintw(animwin, LINES - 1, 0, "Probability: %.2f out of 100 against",(1 - odds) * 100);
    wattrset (animwin, COLOR_PAIR (COLOR_WHITE));
    mvwaddch (animwin, LINES - 2, COLS / 2 + 1, '@');
    wattrset (animwin, COLOR_PAIR (favored_color) | A_BOLD);
    mvwaddch (animwin, LINES - 2, COLS / 2 - 1, 'd');
    wrefresh (animwin); napms(lighttime[t]);
    if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }
  }
  mvwaddch (animwin, LINES - 2, COLS / 2 + 1, ' ');
  wattrset (animwin, COLOR_PAIR (favored_color) | A_BOLD);
  xx = COLS / 2 - 1; yy = LINES - 2;
  wrefresh (animwin); napms(3000);
  if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }
  while (xx < COLS) { // hop away.
    if (xx < COLS) {
      attrset (COLOR_PAIR(favored_color) | A_BOLD);
      mvwaddch(animwin, yy--, xx++, ' ');
      mvwaddch(animwin, yy, xx, 'd'); // up
    }
    wrefresh (animwin); napms(100);
    if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }
    mvwaddch(animwin, yy++, xx++, ' ');
    mvwaddch(animwin, yy, xx, 'd'); // down
    wrefresh (animwin); napms(500);
    if (wgetch(animwin) != ERR) { nodelay(animwin,0); delwin (animwin); return; }
  }
  nodelay(animwin,0); delwin (animwin);
}

void survival (double odds) {
  WINDOW * extrowin;
  extrowin = newwin (LINES - 4, COLS - 4, 2, 2);
  wclear (extrowin);
  wattrset (extrowin, COLOR_PAIR(COLOR_GREEN));
  box(extrowin, 0, 0);
  wattrset (extrowin, COLOR_PAIR(COLOR_WHITE));
  mvwprintw(extrowin, 1, 2, "You emerge, blinking, from the burrow into daylight, unaware that you");
  mvwprintw(extrowin, 2, 2, "were ever in any danger, unaware that you are currently the luckiest");
  mvwprintw(extrowin, 3, 2, "adventurer alive. You check your stats and notice that for all your");
  mvwprintw(extrowin, 4, 2, "carnage you'd only gained half a level. Disappointed you carry on your");
  mvwprintw(extrowin, 5, 2, "adventure.");
  mvwprintw(extrowin, 7, 2, "Soon enough you notice your strange luck as natural 20s become common");
  mvwprintw(extrowin, 8, 2, "place for you. You will never know that your streak of good luck began");
  mvwprintw(extrowin, 9, 2, "at such a humble adventure.");
  mvwprintw(extrowin, 12, 2, "While each encounter's probability ranged from 1/25 to 1/2, to get");
  mvwprintw(extrowin, 13, 2, "here you survived cumulative probability of %.2f out of 100 against.", (1 - odds) * 100);
  mvwprintw(extrowin, 14, 2, "(That's only %.2f in 100 in your favor.)", odds * 100);
  mvwprintw(extrowin, 16, 2, "You truly are lucky.");
  wattrset (extrowin, COLOR_PAIR(COLOR_RED)|A_BOLD);
  mvwprintw(extrowin, 19, 2, "Press any key...");
  wrefresh (extrowin); wgetch (extrowin);
  delwin (extrowin);
}
