#include <curses.h>
#include <vector>
#include <string>
#include "utils.h"

int fieldattr[NUMFIELDS];

wScreenMask::wScreenMask (WINDOW* w) {
  win = w;
  getmaxyx (win, wy, wx);
  keymask = new int*[wy];
  for (int yy = 0; yy < 26; yy++) {
    keymask[yy] = new int[wx];
    for (int xx = 0; xx < 50; xx++)
      keymask[yy][xx] = 0;
  }
}

wScreenMask::~wScreenMask () {
  for (int yy = 0; yy < 26; yy++)
    delete keymask[yy];
  delete keymask;
}

void wScreenMask::add_element (int y, int x, std::string prompt, char key) {

  wmove (win, y, x);
  for (unsigned int c = 0; c < prompt.size(); c++) {
    if ('#' == prompt[c]) {
      switch (prompt[++c]) {
        case 'B': wsetcolor(win, BACKDROP); break;
        case 'M': wsetcolor(win, MENU); break;
        case 'X': wsetcolor(win, XBUTTON); break;
        case 'S': wsetcolor(win, SELECTION); break;
        case 'D': wsetcolor(win, DESCRIPTION); break;
        case 'T': wsetcolor(win, TEXTFIELD); break;
      }
      c++;
    }
    waddch (win, (char)prompt[c]);
    keymask[y][x++] = key;
  }
}

int wScreenMask::get_input () {
  MEVENT mouseinput;
  int input = getch ();

  if (input == KEY_MOUSE) {
    getmouse (&mouseinput);
    input = keymask[mouseinput.y][mouseinput.x];
  }
  return input;
}
//end class wScreenMask

void wcharfillrect(WINDOW *w, char c, int sy, int sx, int height, int width) {
	int x, y;

  for (y = sy; y < (sy+height); y++) {
    wmove (w, y, sx);
    for (x = 0; x < width; x++) waddch (w, c);
  }
}

void wcentermsg (WINDOW *w, std::vector<std::string> msg, int y, int x, int box) {
  if (!y) y = (LINES - msg.size()) / 2;
  unsigned int maxx = 0;
  for (unsigned int c = 0; c < msg.size(); c++)
    if (msg[c].size() > maxx) maxx = msg[c].size();
  if (!x) x = (COLS - maxx) / 2;
  if (box) wcharfillrect (w, ' ', y - 1, x - 1, msg.size() + 2, maxx + 2);
  for (unsigned int yy = 0; yy < msg.size(); yy++)
    mvwprintw (w, y + yy, x + (maxx - msg[yy].size()) / 2, msg[yy].c_str());
}

int wsetcolor(WINDOW* win, int color) {
  return wattrset (win, COLOR_PAIR (color) | fieldattr[color]);
}
