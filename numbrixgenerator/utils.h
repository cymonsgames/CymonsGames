#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#ifdef PDCURSES
#define getmouse(c) nc_getmouse(c)
#endif

enum ColorFields
  {BASE, BACKDROP, MENU, XBUTTON, SELECTION, DESCRIPTION, TEXTFIELD, NUMFIELDS};
extern int fieldattr[NUMFIELDS];

class wScreenMask {
  public:
  wScreenMask (WINDOW* w);
  ~wScreenMask ();
  void add_element (int y, int x, std::string prompt, char key);
  int get_input ();

  private:
  WINDOW* win;
  int wx, wy;
  int** keymask;
};

void wcharfillrect(WINDOW *w, char c, int sy, int sx, int height, int width);

void wcentermsg (WINDOW *w, std::vector<std::string> msg, int y, int x, int box);

int wsetcolor(WINDOW* win, int color);

#endif // UTILS_H_INCLUDED
