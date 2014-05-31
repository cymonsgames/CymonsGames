#ifdef NCURSES
	#include <ncurses.h>
#else
	#include <curses.h>
#endif
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
using std::random_shuffle;
#include "inout.h"

#define VER "13Mar2009"

using namespace std;

const int NumBunToNext = 6;
const int MaxInv = 4;
const int NumLevels = 4;
const int levelnumbuns[NumLevels] = {21, 15, 11, 7};
const int PlayWidth = 50;
const int invwinHeight = 5;
const int asciiwinHeight = 5; // msgwin is calculated from the remainer.


const string deadbunny[4] = {
  "(\\ /) ",
  " x-x  ",
  "() () ",
  "()-() "};

const int ESC = 27; //ASCII for escape
const int BG = COLOR_BLACK;

class bun {
  public:
  int favored;
  int x, y, dx, dy;
  int color, bold;
};

class XYc {
  public:
  int x; int y;
  int counter;
};

class pc { // player class
  public:
  int x; int y;
  vector<bun> inventory;
};

WINDOW *playwin, *msgwin, *invwin, *asciiwin;
vector<string> killmsgs;

pc player;
XYc levelexit;
vector<bun> bunnies;
int** playarea;
vector<string> gamemsgs;
int kills, turn;
double probability;

void init () {
  int c;

  initscr();
  raw (); noecho(); curs_set(0); nonl(); keypad(stdscr,1);
  clear (); refresh ();
	start_color();
	for (c = 0; c < COLORS; c++)
		init_pair(c, c, BG);
  init_pair(c++, COLOR_WHITE, COLOR_RED); // borders
  init_pair(c++, COLOR_WHITE, COLOR_GREEN); // exit

#ifdef PDCURSES
  PDC_set_title("The Favored - by Joseph Larson " VER);
#endif

  // load messages
  ifstream killmsgsfile("killmsgs.txt");
  if (killmsgsfile.is_open()) {
    while (!killmsgsfile.eof() ) {
      string line;
      getline (killmsgsfile, line);
      killmsgs.push_back(line);
    }
  }

  playarea = new int*[LINES - 2];
  for (int c = 0; c < LINES - 2; c++)
    playarea[c] = new int[PlayWidth];
  srand (time(NULL));
}

void drawplay () {
  wattrset (playwin, COLOR_PAIR(COLORS));
  wborder(playwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wattrset (playwin, COLOR_PAIR(0));
  for (int yy = 0; yy < LINES - 2; yy++)
    for (int xx = 0; xx < PlayWidth; xx++)
      mvwaddch (playwin, yy + 1, xx + 1, ' ');
  wattrset (playwin, COLOR_PAIR(COLOR_GREEN));
  if (levelexit.counter > 0) {
    wattrset (playwin, COLOR_PAIR(COLORS + 1));
    mvwaddch (playwin, levelexit.y + 1, levelexit.x + 1, '0' + levelexit.counter);
  } else {
    wattrset (playwin, COLOR_PAIR(COLOR_GREEN));
    mvwaddch (playwin, levelexit.y + 1, levelexit.x + 1, '<');
  }
  wattrset (playwin, COLOR_PAIR(COLOR_WHITE));
  mvwaddch (playwin, player.y + 1, player.x + 1, '@');
  for (int c = 0; c < bunnies.size(); c++) {
    wattrset (playwin, COLOR_PAIR(bunnies[c].color) | (bunnies[c].bold) * A_BOLD);
    mvwaddch (playwin, bunnies[c].y + 1, bunnies[c].x + 1, (bunnies[c].dx > 0) ? 'd' : 'b');
  }
  wrefresh (playwin);
}

void drawinv () {
  wattrset (invwin, COLOR_PAIR (COLOR_WHITE) | A_BOLD);
  wclear (invwin);
  mvwprintw (invwin, 0, 0, "Inventory :");
  for (int yy = 0; yy < 4; yy++) {
    wmove (invwin, yy + 1, 0);
    for (int c = 0; c < player.inventory.size(); c++) {
      wattrset (invwin, COLOR_PAIR(player.inventory[c].color) |
        A_BOLD * player.inventory[c].bold);
      wprintw(invwin, "%7s", deadbunny[yy].c_str());
    }
  }
  wrefresh (invwin);
}

void drawmsg () {
  int s;
  int yy = LINES - (invwinHeight + asciiwinHeight);

  wattrset (msgwin, COLOR_PAIR (COLOR_WHITE) | A_BOLD);
  wclear (msgwin);
  for (s = gamemsgs.size(); (s > 0) && (yy > 0);) {
    s--;
    wattrset (msgwin, COLOR_PAIR (COLOR_WHITE) | (s%2) * A_BOLD);
    string line(gamemsgs[s].c_str());
    while (line.size()) {
      int c = 0;
      while ((line.size() - c) > (COLS - PlayWidth - 2)) {
        c += COLS - PlayWidth - 2;
        while (line[c - 1] != ' ') c--;
      }
      mvwprintw (msgwin, --yy, 0, "%s", line.substr(c).c_str());
      line.erase (c);
    }
  }
  gamemsgs.erase(gamemsgs.begin(), gamemsgs.begin() + s - (s%2));
  wrefresh (msgwin);
}

void drawascii () {
  wattrset (asciiwin, COLOR_PAIR(COLORS) | A_BOLD);
  wclear(asciiwin);
  wprintw(asciiwin,
 //12345678901234567890134567890
  "  \\\\  THE FAVORED           "
  "   \\\\_  by Joe Larson  //   "
  "    (\")              _//    "
  "   / )              (\")--.  "
  " o( )_\\_            _/-( )o ");
  mvwprintw (asciiwin, 4, 12, "1/%d", bunnies.size());
  wrefresh (asciiwin);
}

void setuplevel (int level) {
  for (int yy = 0; yy < LINES - 2; yy++)
    for (int xx = 0; xx < PlayWidth; xx++)
      playarea[yy][xx] = ' ';
  playarea[player.y][player.x] = '@';
  do {
    levelexit.x = rand() % PlayWidth;
    levelexit.y = rand() % (LINES - 2);
  } while (playarea[levelexit.y][levelexit.x] != ' ');
  playarea[levelexit.y][levelexit.x] = 'E';
  levelexit.counter = NumBunToNext;
  bunnies.clear();
  for (int c = 0; c < levelnumbuns[level]; c++) {
    bun newbun;
    do {
      newbun.x = rand() % PlayWidth;
      newbun.y = rand() % (LINES - 2);
    } while (playarea[newbun.y][newbun.x] != ' ');
    newbun.dx = rand() % 3 - 1; newbun.dy = rand() % 3 - 1;
    playarea[newbun.y][newbun.x] = 'b';
    newbun.color = 1 + rand()%(COLORS - 1);
#ifdef PDCURSES
    newbun.bold = 0;
#else
    newbun.bold = rand()%2;
#endif
    newbun.favored = 0;
    bunnies.push_back(newbun);
  }
  bunnies[rand()%bunnies.size()].favored = 1;
}

void movebuns () {
  for (int c = 0; c < bunnies.size(); c++) {
    playarea[bunnies[c].y][bunnies[c].x] = ' ';
    if ((bunnies[c].dx == 0) && (bunnies[c].dy == 0)) {
      bunnies[c].dx = rand() % 3 - 1; bunnies[c].dy = rand () % 3 - 1;
    }
    if (bunnies[c].y + bunnies[c].dy < 0) bunnies[c].dy = -bunnies[c].dy;
    if (bunnies[c].y + bunnies[c].dy >= LINES - 2) bunnies[c].dy = -bunnies[c].dy;
    if (bunnies[c].x + bunnies[c].dx < 0) bunnies[c].dx = -bunnies[c].dx;
    if (bunnies[c].x + bunnies[c].dx >= PlayWidth) bunnies[c].dx = -bunnies[c].dx;
    if (playarea[bunnies[c].y + bunnies[c].dy][bunnies[c].x + bunnies[c].dx] != ' ') {
      if ((playarea[bunnies[c].y][bunnies[c].x + bunnies[c].dx] == ' ') && (bunnies[c].dx != 0))
        bunnies[c].x += bunnies[c].dx;
      else if ((playarea[bunnies[c].y + bunnies[c].dy][bunnies[c].x] == ' ') && (bunnies[c].dy != 0))
        bunnies[c].y += bunnies[c].dy;
      else {
        bunnies[c].dx = 0; bunnies[c].dy = 0;
      }
    } else {
      bunnies[c].x += bunnies[c].dx; bunnies[c].y += bunnies[c].dy;
    }
    playarea[bunnies[c].y][bunnies[c].x] = 'b';
  }
}

int moveplayer () {
  int input;
  int dx, dy;

  dy = dx = 0;
  input = wgetch(playwin);
  if ((input >= 'a') && (input <= 'z')) input = input - 'a' + 'A';
  switch (input) {
    case '7':
    case 'Y':
    case 'Q':
      dy = -1; dx = -1; break;
    case KEY_UP:
    case '8':
    case 'K':
    case 'W':
      dy = -1; break;
    case '9':
    case 'U':
    case 'E':
      dy = -1; dx = 1; break;
    case KEY_LEFT:
    case '4':
    case 'H':
    case 'A':
      dx = -1; break;
    case KEY_RIGHT:
    case '6':
    case 'L':
    case 'D':
      dx = 1; break;
    case '1':
    case 'B':
    case 'Z':
      dy = 1; dx = -1; break;
    case KEY_DOWN:
    case '2':
    case 'J':
    case 'X':
      dy = 1; break;
    case '3':
    case 'N':
    case 'C':
      dy = 1; dx = 1; break;
    case ESC:
      gamemsgs.push_back("Unable to see a way out you, pull your legs to your chest and await death.");
      return 0;
  }
  if ((player.y + dy < 0) || (player.y + dy >= LINES - 2)) {
    gamemsgs.push_back("You grope at the wall but fail to gain purchase.");
    dy = 0;
  }
  if ((player.x + dx < 0) || (player.x + dx >= PlayWidth)) {
    gamemsgs.push_back("You grope at the wall but fail to gain purchase.");
    dx = 0;
  }
  playarea[player.y][player.x] = ' ';
  switch (playarea[player.y + dy][player.x + dx]) {
    case 'E' :
      if (!levelexit.counter) {
        player.y += dy; player.x += dx;
        break;
      }
      if ((player.inventory.size() < 1)) {
        if (levelexit.counter == NumBunToNext)
          gamemsgs.push_back("There's a way up here, but it's too high. Maybe if you could build a sort of rope ladder...");
        else
          gamemsgs.push_back("You see a way up to the next level here. There's a small pile of dead creatures, but not enough to get you out.");
        break;
      }
      if (levelexit.counter) {
        if (levelexit.counter == NumBunToNext) {
          gamemsgs.push_back("You start a pile with the corpses. If you had a few more you could use their bones and skin to make a ladder.");
        } else if (levelexit.counter == NumBunToNext - 1) {
          gamemsgs.push_back("You add another corpse to the pile.");
        } else if (levelexit.counter == 1){
          gamemsgs.push_back("After several minutes of dismembering bone and sinue you reconstruct a crude rope ladder from the gore.");
        }
        player.inventory.pop_back();
        levelexit.counter--;
      }
      break;
    case 'b' :
      int c;
      for (c = 0; (c < bunnies.size()) && !((bunnies[c].x == player.x + dx) && (bunnies[c].y == player.y + dy)); c++);
      if (c < bunnies.size()) {
        if (killmsgs.size() && !(kills % killmsgs.size())) {
          random_shuffle(killmsgs.begin(), killmsgs.end());
        }
        kills++;
        probability *= (double)(bunnies.size() - 1) / (double)bunnies.size();
        if (bunnies[c].favored) {
          gamemsgs.push_back ("This creature bears the mark of the favored. Suddenly the ground rumbles...");
          killedfavored(bunnies[c].color, probability);
          gamemsgs.push_back ("You are struck dead.");
          return 0;
        }
        if (killmsgs.size())
          gamemsgs.push_back(killmsgs[kills % killmsgs.size()].c_str());
        else gamemsgs.push_back("You kill the creature.");
        if (player.inventory.size() < MaxInv) {
          player.inventory.push_back(bunnies[c]);
        } else {
          gamemsgs.push_back("You don't have any room in your pack. The creature disintegrates in your grasp.");
        }
        playarea[bunnies[c].y][bunnies[c].x] = ' ';
        bunnies.erase(bunnies.begin() + c);
      }
    case ' ' :
      player.y += dy;
      player.x += dx;
      break;
  }
  playarea[player.y][player.x] = '@';
  return 1;
}

void play () {
  int level, alive = 1;
  probability = 1;
  // setup windows
  playwin = newwin (LINES, PlayWidth + 2, 0, COLS - PlayWidth - 2);
  invwin = newwin (invwinHeight , COLS - PlayWidth - 2, 0, 0);
  asciiwin = newwin (asciiwinHeight, COLS - PlayWidth - 2, invwinHeight, 0);
  msgwin = newwin (LINES - invwinHeight - asciiwinHeight, COLS - PlayWidth - 2
    , invwinHeight + asciiwinHeight, 0);
  keypad(playwin, TRUE);

  // setup player
  player.x = rand()%PlayWidth;
  player.y = rand()%(LINES - 2);
  player.inventory.clear();
  turn = kills = 0;
  gamemsgs.push_back("Welcome to the warren of the Favored.");
  for (level = 0; level < NumLevels; level++) {
    setuplevel(level);
    do {
      turn++;
      drawplay ();
      drawinv ();
      drawmsg ();
      drawascii ();
      alive = moveplayer ();
      if (alive && !(turn % 2)) movebuns ();
    } while (alive && !((player.x == levelexit.x) && (player.y == levelexit.y)));
    drawplay ();
    drawmsg ();
    drawinv ();
    drawascii ();
    if (!alive) {
      drawplay ();
      drawmsg ();
      drawinv ();
      drawascii ();
      delwin (invwin); delwin (asciiwin); delwin (msgwin); delwin (playwin);
      return;
    }
    if (level < NumLevels)
      gamemsgs.push_back("You climb the ladder to the next level.");
  } // endfor
  gamemsgs.push_back("You claw your way to the surface...");
  survival (probability);
  drawplay ();
  drawmsg ();
  drawinv ();
  drawascii ();
  delwin (invwin); delwin (asciiwin); delwin (msgwin); delwin (playwin);
}

int playagain () {
  WINDOW* againwin;
  againwin = newwin (4, 30, (LINES - 4) / 2, (COLS - 28) / 2);
  wclear(againwin);
  wattrset (againwin, COLOR_PAIR(COLOR_GREEN));
  box(againwin, 0, 0);
  wattrset (againwin, COLOR_PAIR(COLOR_WHITE));
  mvwprintw (againwin, 1, 2, "Do you want to play again?");
  mvwprintw (againwin, 2, 1, "          (Y/N)");
  wrefresh (againwin);
  while (1) {
    switch (wgetch(againwin)) {
      case 'Y':
      case 'y': delwin (againwin); return 1; break;
      case 'N':
      case 'n':
      case ESC: delwin (againwin); return 0; break;
    }
  }
}

int main() {
  init ();
  intro ();
  do
    play ();
  while (playagain());
  endwin ();
  return 0;
}
