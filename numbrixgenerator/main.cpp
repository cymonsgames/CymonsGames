// Numbrix Generator
// By Joe Larson for Cymon's Games
// Based on the Numbrix puzzle created by Maryln Vos Savant

#include <curses.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cctype>

#include "numbrixgen.h"
#include "utils.h"

#define VER "ver15Feb2009"

#ifdef PDCURSES
#define getmouse(c) nc_getmouse(c)
#endif

using namespace std;

WINDOW* puzwin;

void blankscreen () {
  wsetcolor (puzwin, BACKDROP);
  wcharfillrect (puzwin, ' ', 0, 0, 26, 50);
  wsetcolor (puzwin, MENU);
  wcharfillrect (puzwin, ' ', 0, 0, 1, 50);
  wrefresh(puzwin);
}

void init() {
  initscr();
  curs_set (0);
  cbreak ();
  noecho();
  keypad(stdscr,1);
  mousemask (ALL_MOUSE_EVENTS, NULL);
#ifdef PDCURSES
  PDC_set_title("Cymon's Games - Numbrix Generator " VER);
#endif
  start_color();
  init_pair (BACKDROP, COLOR_BLACK, COLOR_GREEN); fieldattr[BACKDROP] = 0;
  init_pair (MENU, COLOR_BLACK, COLOR_WHITE); fieldattr[MENU] = 0;
  init_pair (XBUTTON, COLOR_WHITE, COLOR_RED); fieldattr[XBUTTON] = A_BOLD;
  init_pair (SELECTION, COLOR_WHITE, COLOR_RED); fieldattr[SELECTION] = A_BOLD;
  init_pair (DESCRIPTION, COLOR_WHITE, COLOR_BLUE); fieldattr[DESCRIPTION] = A_BOLD;
  init_pair (TEXTFIELD, COLOR_WHITE, COLOR_CYAN); fieldattr[TEXTFIELD] = A_BOLD;
  init_pair (NUMFIELDS, COLOR_BLUE, COLOR_WHITE); // Special case for Numbrix
  resize_term(26,50);
  puzwin = newwin(26, 50, 0, 0);
}

void makemask(int type, int puzx, int puzy, int** mask) {
  int xx, yy;

  for (int xx = 0; xx < puzx; xx++)
    for (int yy = 0; yy < puzy; yy++)
      mask[xx][yy] = 0;
  switch (type) {
    case 2: // Donught
      for (xx = 1; xx < puzx - 1; xx++)
        for (yy = 1; yy < puzy - 1; yy++)
          if ((xx == 1) || (xx == puzx - 2) || (yy == 1) || (yy == puzy - 2))
            mask[xx][yy] = 1;
      break;
    case 3: // Strawberry
      for (xx = 0; xx < puzx; xx++)
        for (yy = 0; yy < puzy; yy++)
          if (((xx <= 1) || (xx >= puzx - 2) || (yy <= 1) || (yy >= puzy - 2))
            && ((xx + yy + 1) % 2))
            mask[xx][yy] = 1;
      break;
    case 4: // Strawberry Donught
      for (xx = 1; xx < puzx - 1; xx++)
        for (yy = 1; yy < puzy - 1; yy++)
          if (((xx <= 2) || (xx >= puzx - 3) || (yy <= 2) || (yy >= puzy - 3))
            && ((yy + xx + 1) % 2))
            mask[xx][yy] = 1;
      break;
    case 5: // Random
      for (int c = 0; c < ((puzx * puzy) / 25) + 1; c++) { // change this line if random mask generation is taking too long.
        do {
          yy = rand() % puzy; xx = rand() % puzx;
        } while (mask[xx][yy]);
        mask[xx][yy] = mask[puzx - xx - 1][puzy - yy - 1] =
        mask[xx][puzy - yy - 1] = mask[puzx - xx - 1][yy] = 1;
      }
      break;
    default: // Traditional
      for (yy = 0; yy < puzy; yy++)
        for (xx = 0; xx < puzx; xx++)
          if ((xx == 0) || (xx == puzx - 1) || (yy == 0) || (yy == puzy - 1))
            mask[xx][yy] = 1;
      break;

  }
}

void info () {
  vector<string> message;
  message.push_back ("Numbrix Solver and Generator");
  message.push_back ("----------------------------");
  message.push_back (VER); message.push_back ("");
  message.push_back ("by Joseph Larson for"); message.push_back ("Cymon's Games");
  message.push_back ("based on the Numbrix puzzle");
  message.push_back ("created by Maryln Vos Savant"); message.push_back ("");
  message.push_back ("For programming resources");
  message.push_back ("source code and a free game");
  message.push_back ("every week please visit:");
  message.push_back ("http://WWW.CYMONSGAMES.COM");
  message.push_back (""); message.push_back ("");
  wsetcolor (puzwin, MENU);
  wcentermsg (puzwin, message, 0, 0, 1);
  wsetcolor (puzwin, SELECTION);
  mvwprintw (puzwin, 19, 17, " Press any key ");
  wrefresh (puzwin);
  getch();
}

int settings (int* puzx, int* puzy, int* masktype, int batch) {
  wScreenMask screen (puzwin);
  int input, curloc = 0;
  string code;

  *puzy = *puzx = 9; *masktype = 1; code = "";
  while (1) {
    // Visual elements
    blankscreen ();
    wsetcolor (puzwin, MENU);
    wcharfillrect (puzwin, ' ', 6, 12, 12 + (!batch) * 2, 26);
    screen.add_element (4, 21, "#MOptions", 0);
    screen.add_element (7, 18, "#MSize: X__, Y__", 0);
    screen.add_element (7, 24, "#SX#T__", 'X'); // two underscores
    screen.add_element (7, 29, "#SY#T__", 'Y'); // two underscores
    screen.add_element (9, 15, "#MNumbrix Type:", 0);
    screen.add_element (10, 15, "#S1)#D Original         ", '1');
    screen.add_element (11, 15, "#S2)#D Donut            ", '2');
    screen.add_element (12, 15, "#S3)#D Strawberry       ", '3');
    screen.add_element (13, 15, "#S4)#D Strawberry Donut ", '4');
    screen.add_element (14, 15, "#S5)#D Random           ", '5');
    screen.add_element (16, 14, "#S Press ENTER to go on ", '\n');
    if (!batch)
      screen.add_element(18, 14, "#MPress #SC#M to use a code", 'C');
    screen.add_element (0, 46, "#M ? ", '?');
    screen.add_element (0, 49, "#XX", 'Q');
    wsetcolor (puzwin, TEXTFIELD);
    if (*masktype)  mvwprintw (puzwin, 9 + *masktype, 14, "*");
    wsetcolor (puzwin, TEXTFIELD);
    mvwprintw (puzwin, 7, 25, "%2d", *puzx);
    mvwprintw (puzwin, 7, 30, "%2d", *puzy);
    if (curloc) {
      curs_set (1);
      if (curloc < 3) wmove (puzwin, 7, 26 + (curloc - 1) * 5);
      else {
        wsetcolor (puzwin, TEXTFIELD);
        mvwprintw (puzwin, 18, 14, "%-22s", code.c_str());
      }
    } else curs_set (0);
    wrefresh(puzwin);
    input = screen.get_input ();
    if ((input >= '0') && (input <= '9')) {
      if (!curloc)
        if ((input >= '1') && (input <= '5')) *masktype = input - '0';
      if (curloc == 1) {
        *puzx = *puzx * 10 + input - '0';
        if (*puzx >= 10) {
          curloc = 0;
          if ((!batch) && (*puzx > 12)) *puzx = 12;
          else if (*puzx > 12) *puzx = 30;
        }
      }
      if (curloc == 2) {
        *puzy = *puzy * 10 + input - '0';
        if (*puzy >= 10) {
          curloc = 0;
          if ((!batch) && (*puzy > 12)) *puzy = 12;
          else if (*puzy > 12) *puzy = 30;
        }
      }
      if (curloc == 3) {
        code += input;
      }
    } else {
      if (*puzx < 5) *puzx = 5; if (*puzy < 5) *puzy = 5;
      switch (input) {
        case 'X':
        case 'x':
          curloc = 1; *puzx = 0; break;
        case 'Y':
        case 'y':
          curloc = 2; *puzy = 0; break;
        case 'C':
        case 'c':
          if (!batch) curloc = 3; break;
        case '\b':
          if ((curloc == 3) && (code.length()))
             code.erase(code.end() - 1, code.end()); break;
        case KEY_ENTER :
        case '\n':
          if ((!curloc) || (curloc == 3)) {
            int seed = 1;
            if (curloc == 3) { // parse the code for the X and Y
              if (code.length() < 8) {
                curloc = 0;
                code.clear();
                break;
              }
              *puzx = (code.at(0) - '0') * 10 + (code.at(1) - '0');
              *puzy = (code.at(2) - '0') * 10 + (code.at(3) - '0');
              if ((*puzx < 0) || (*puzx > 12) || (*puzy < 0) || (*puzy > 12)) {
                *puzy = 9; *puzx = 9; curloc = 0; break;
              }
              * masktype = (code.at(4) - '0') * 10 + (code.at(5) - '0');
              if ((*masktype < 1) || (*masktype > 5)) *masktype = 1;
              seed = 0;
              for (int c = 6; c < code.length(); c++)
                seed = seed * 10 + code.at(c) - '0';
            }
            return seed;
          } else curloc = 0;
          break;
        case '?' : info(); curloc = 0; break;
        case 'Q' :
        case 'q' :
        case KEY_EXIT :
          return 0;
        default :
          curloc = 0;
      }
    }
  }
}

int editpuzzle (int puzx, int puzy, signed int** grid, int editable) {
  wScreenMask screen(puzwin);
  MEVENT mouseinput;
  int input, edit, upper, left, boxx, boxy;

  boxx = boxy = 0; edit = 1;
  upper = 13 - puzy;
  left = 25 - (puzx * 2);
  while (1) {
    // Visual Elements
    blankscreen ();
    screen.add_element (0, 1, "#SS#Mtop editing " , 'S');
    screen.add_element (0, 46, "#M ? ", '?');
    screen.add_element (0, 49, "#XX", 'Q');
    wsetcolor (puzwin, MENU);
    for (int yy = 1; yy < (2 * puzy); yy++) {
      mvwaddch (puzwin, upper + yy, left, (yy%2) ? ACS_VLINE : ACS_LTEE);
      mvwaddch (puzwin, upper + yy, left + (4 * puzx), (yy%2) ? ACS_VLINE : ACS_RTEE);
      for (int xx = 1; xx < (4 * puzx); xx++) {
        mvwaddch (puzwin, upper, left + xx, (xx%4) ? ACS_HLINE : ACS_TTEE);
        mvwaddch (puzwin, upper + 2 * puzy, left + xx, (xx%4) ? ACS_HLINE : ACS_BTEE);
        mvwaddch (puzwin, upper + yy, left + xx, (yy%2) ?
          (xx%4) ? ' ' : ACS_VLINE
          : (xx%4)? ACS_HLINE : ACS_PLUS);
      }
    }
    mvwaddch(puzwin, upper, left, ACS_ULCORNER);
    mvwaddch(puzwin, upper + 2 * puzy, left, ACS_LLCORNER);
    mvwaddch(puzwin, upper, left + 4 * puzx, ACS_URCORNER);
    mvwaddch(puzwin, upper + 2 * puzy, left + 4 * puzx, ACS_LRCORNER);

    for (int xx = 0; xx < puzx; xx++)
      for (int  yy = 0; yy < puzy; yy++) {
        screen.add_element (upper + 2 * yy + 1, left + 4 * xx + 1, "#M   ", 1); // 3 spaces
        if (grid[xx][yy] < 0) wsetcolor (puzwin, NUMFIELDS);
        else wsetcolor (puzwin, MENU);
        wmove(puzwin, upper + 2 * yy + 1, left + 4 * xx + 1);
        if (grid[xx][yy])
          wprintw (puzwin, "%3d", abs(grid[xx][yy]));
      }
    if (!editable) {curs_set (0); wrefresh (puzwin); return 1;}
    wsetcolor (puzwin, SELECTION);
    wmove (puzwin, upper + 2 * boxy, left + 4 * boxx);
    waddch (puzwin, ACS_ULCORNER); waddch (puzwin, ACS_HLINE);
    waddch (puzwin, ACS_HLINE); waddch (puzwin, ACS_HLINE);
    waddch (puzwin, ACS_URCORNER);
    wmove (puzwin, upper + 2 * boxy + 1, left + 4 * boxx);
    waddch (puzwin, ACS_VLINE);
    wmove (puzwin, upper + 2 * boxy + 1, left + 4 * boxx + 4);
    waddch (puzwin, ACS_VLINE);
    wmove (puzwin, upper + 2 * boxy + 2, left + 4 * boxx);
    waddch (puzwin, ACS_LLCORNER); waddch (puzwin, ACS_HLINE);
    waddch (puzwin, ACS_HLINE); waddch (puzwin, ACS_HLINE);
    waddch (puzwin, ACS_LRCORNER);
    if ((grid[boxx][boxy] >= 0) && (grid[boxx][boxy] < (puzx * puzy))) {
      wsetcolor (puzwin, MENU);
      wmove(puzwin, upper + 2 * boxy + 1, left + 4 * boxx + 1);
      if (grid[boxx][boxy])
        wprintw (puzwin, "%3d", abs(grid[boxx][boxy]));
      else wprintw (puzwin, "   "); // 3 spaces
      wmove(puzwin, upper + 2 * boxy + 1, left + 4 * boxx + 3);
      curs_set(1);
    } else curs_set (0);
    wrefresh (puzwin);
    input = screen.get_input ();
    if ((input >= '0') && (input <= '9')) {
      if (grid[boxx][boxy] >= 0) {
        if (edit) grid[boxx][boxy] = edit = 0;
        if (grid[boxx][boxy] * 10 + input - '0' <= (puzx * puzy))
        grid[boxx][boxy] = grid[boxx][boxy] * 10 + input - '0';
      }
    } else {
      getmouse (&mouseinput);
      switch (input) {
        case 1 :
          boxy = (mouseinput.y - upper) / 2;
          boxx = (mouseinput.x - left) / 4;
          edit = 1;
          break;
        case KEY_UP :
          if (boxy > 0) {boxy--; edit = 1;}
          break;
        case KEY_DOWN :
          if (boxy < puzy - 1) {boxy++; edit = 1;}
          break;
        case KEY_LEFT :
          if (boxx > 0) {boxx--; edit = 1;}
          break;
        case KEY_RIGHT :
          if (boxx < puzx - 1) {boxx++; edit = 1;}
          break;
        case KEY_ENTER :
        case '\n' :
          if ((grid[boxx][boxy] > 0) && edit)
            grid[boxx][boxy] = 0;
          break;
        case KEY_BACKSPACE :
        case '\b' :
          if ((grid[boxx][boxy] > 0))
            grid[boxx][boxy] = 0;
          break;
        case '?' :
          info ();
          break;
        case 's' :
        case 'S' :
          return 1;
        case 'q' :
        case 'Q' :
        case KEY_EXIT :
          return 0;
      }
    }
  }
}

void save2txt (int** grid, NpathGen* sol, string name) {
  ofstream puzfile;
  name += ".txt";
  puzfile.open (name.c_str(), ios::out | ios::trunc);
  puzfile << "Numbrix Puzzle\n"
    << "Created with the Numbrix Solver Generator by Joseph Larson\n"
    << "based on the Numbrix puzzle by Maryln Vos Savant.\n"
    << "Software version " << VER << "\n\n"
    << "Puzzle Code: " << setw(2) << setfill('0') << sol->getXSize()
    << setw(2) << setfill('0') << sol->getYSize()
    << setw(2) << setfill('0') << sol->getMask()
    << sol->getSeed() << "\n\n" << setfill (' ');
  for (int yy = 0; yy < sol->getYSize(); yy++)
    for (int line = 0; line < 4; line++) {
      for (int xx = 0; xx < sol->getXSize(); xx++) {
        switch (line) {
          case 0:
            puzfile << "+-----"; break;
          case 2:
            if (grid[xx][yy])
              puzfile << "| " << setw(3) << abs(grid[xx][yy]) << " ";
            else
          case 1:
          case 3:
            puzfile << "|     "; // 5 spaces
        }
      }
      switch (line) {
        case 0:
          puzfile << "+\n"; break;
        default :
          puzfile << "|\n"; break;
      }
    }
  for (int xx = 0; xx < sol->getXSize(); xx++)
    puzfile << "+-----";
  puzfile << "+\n\n\n";
  puzfile << "Solution\n\n";
  for (int yy = 0; yy < sol->getYSize(); yy++) {
    for (int xx = 0; xx < sol->getXSize(); xx++)
      puzfile << setw(4) << sol->getXY(xx,yy);
    puzfile << "\n\n";
  }
  puzfile.close();
}

void scrollmsg(vector<string> msg) {
  blankscreen ();
  wsetcolor (puzwin, MENU);
  mvwprintw (puzwin, 0,0, "Working...");
  int yy = 25;
  wsetcolor (puzwin, BACKDROP);
  for (vector<string>::iterator s = msg.end(); (s != msg.begin()) && (yy > 0);) {
    s--;
    mvwprintw (puzwin, yy--, 0, s->c_str());
  }
  wrefresh (puzwin);
}

void batch () {
  int input;
  wScreenMask screen (puzwin);
  signed int** grid;
  NSolver* check;
  string name = "numbrix";
  int puzx, puzy, masktype, num, curloc, seed;
  time_t start, timer;
  double secs, totsecs;

  num = 1; curloc = 0; seed = time(NULL);
  if (!settings (&puzx, &puzy, &masktype, 1)) return;
  do {
    blankscreen();
    wsetcolor (puzwin, MENU);
    wcharfillrect (puzwin, ' ', 6, 5, 7, 39);
    screen.add_element (4, 17, "#M Batch Options ", 0);
    screen.add_element (7, 6, "#SN#Mumber to generate : #T   ", 'N');
    screen.add_element (9, 6, "#SF#Mile base name : #4T                    ", 'F'); // 20 spaces
    screen.add_element (11, 14, "#S Press ENTER to start ", '\n');
    wsetcolor (puzwin, TEXTFIELD);
    mvwprintw (puzwin, 7, 27, "%3d", num);
    mvwprintw (puzwin, 9, 23, "%-20s", name.c_str());
    vector <string> message;
    message.push_back ("Time to generate varies greatly.");
    message.push_back ("If you are generating many puzzles");
    message.push_back ("or very large puzzles");
    message.push_back ("please be patient."); message.push_back ("");
    message.push_back ("Generated puzzle files will appear");
    message.push_back ("in the directory this program is");
    message.push_back ("being run from. Any files with");
    message.push_back ("the same name will be overwritten.");
    wsetcolor (puzwin, TEXTFIELD);
    wcentermsg (puzwin, message, 15, 0, 1);
    if (curloc) {
      curs_set (1);
      if (curloc == 1) wmove (puzwin, 7, 29);
      else wmove (puzwin, 9, 23 + name.length ());
    } else curs_set (0);
    wrefresh (puzwin);
    input = screen.get_input ();
     if (curloc == 2) {
      if (isalnum(input)) name += input;
      if (input == '\b') if (name.length()) name.erase(name.end() - 1);
      if (input == '\n') curloc = 0;
      input = 0;
    } else {
      if (curloc == 1) {
        if ((input >= '0') && (input <= '9'))
          num = num * 10 + input - '0';
        if (input == '\b') num /= 10;
        if ((input == '\n') || (num >= 100)) curloc = 0;
      }
      if (num < 0) num = 1;
      switch (input) {
      case 'N' :
      case 'n' :
        if (curloc != 2) {num = 0; curloc = 1;} break;
      case 'F' :
      case 'f' :
        if (curloc != 2) {curloc = 2;} break;
      case '?' :
        info (); break;
      case 'Q' :
      case 'q' :
        return;
      }
    }
  } while (input != '\n');
  curs_set (1);
  grid = new int*[puzx];
    for (int xx = 0; xx < puzx; xx++)
      grid[xx] = new int[puzy];
  vector <string> messages;

  messages.push_back ("Beginning Generation");
  totsecs = 0;
  for (int c = 0; c < num; c ++) {
    time (&start);
    ostringstream digits;
    digits  << setw(3) << setfill ('0') << c + 1;
    string nametemp = name + digits.str();
    messages.push_back ("Generating " + nametemp + "...");
    scrollmsg(messages);
    seed += rand ();
    NpathGen puzzle(puzx, puzy, seed);
    puzzle.setMask (masktype);
    makemask (masktype, puzx, puzy, grid);
    for (int xx = 0; xx < puzx; xx++)
      for (int  yy = 0; yy < puzy; yy++)
        grid[xx][yy] = (grid[xx][yy]) ? puzzle.getXY(xx, yy) : 0;
    messages.push_back ("Checking uniqueness...");
    scrollmsg(messages);
    check = new NSolver(puzx, puzy, grid);
    while (check->getNumSolutions () > 1) {
      scrollmsg(messages);
      int xx; int yy;
      do {
        xx = rand() % puzx; yy = rand () % puzy;
      } while (grid[xx][yy] == 0);
      if (masktype != 5) {
        grid[xx][yy] = 0; // take one away.
        grid[xx][puzy - yy - 1] = 0;
        grid[puzx - xx - 1][yy] = 0;
        grid[puzx - xx - 1][puzy - yy - 1] = 0;
      }
      do {
        xx = rand() % puzx; yy = rand () % puzy;
      } while (check->getSolutionXY(xx, yy, 0) == check->getSolutionXY(xx, yy, 1));
      grid[xx][yy] = puzzle.getXY(xx, yy); // add a different one in.
      grid[xx][puzy - yy - 1] = puzzle.getXY(xx, puzy - yy - 1);
      grid[puzx - xx - 1][yy] = puzzle.getXY(puzx - xx - 1, yy);
      grid[puzx - xx - 1][puzy - yy - 1] = puzzle.getXY(puzx - xx - 1, puzy - yy - 1);
      delete check;
      check = new NSolver(puzx, puzy, grid);
      messages.push_back ("Multiple solutions found. Fixed. Checking again.");
      scrollmsg(messages);
    }
    ostringstream tictoc;
    tictoc.flush();
    time (&timer);
    secs = difftime (timer, start);
    totsecs += secs;
    tictoc << (int)secs;
    messages.push_back (tictoc.str() + " seconds to generate.");
    messages.push_back ("Saving " + nametemp + "...");
    scrollmsg(messages);
    save2txt (grid, &puzzle, nametemp);
  }
  ostringstream minutes, seconds, average;
  minutes << (int)totsecs / 60;
  seconds << (int)totsecs % 60;
  messages.push_back (minutes.str() + ":" + seconds.str() + " spent generating.");
  totsecs /= num;
  average << totsecs;
  messages.push_back (seconds.str() + " seconds per on average.");
  scrollmsg(messages);
  curs_set (0);
  wsetcolor (puzwin, SELECTION);
  vector <string> message;
  message.push_back ("Batch Complete."); message.push_back ("");
  message.push_back ("Files have been created in the");
  message.push_back ("directory this program was");
  message.push_back ("run from."); message.push_back ("");
  message.push_back ("Press any key...");
  wcentermsg (puzwin, message, 0, 0, 1);
  wrefresh (puzwin);
  getch ();
}

void solve () {
  wScreenMask screen(puzwin);
  int input;
  int curloc;
  signed int** grid;
  NSolver* check;
  int puzx, puzy;
  time_t start, timer;
  double secs;

  puzy = puzx = 9; curloc = 0;
  while (1) {
    blankscreen ();
    wsetcolor (puzwin, MENU);
    wcharfillrect (puzwin, ' ', 6, 13, 5, 24);
    screen.add_element(4, 21, "#MOptions", 0);
    screen.add_element(7, 18, "#MSize: X__, Y__", 0);
    screen.add_element(9, 14, "#S Press ENTER to go on ", '\n');
    screen.add_element(7, 24, "#SX#T__", 'X');
    screen.add_element(7, 29, "#SY#T__", 'Y');
    wsetcolor (puzwin, TEXTFIELD);
    mvwprintw (puzwin, 7, 25, "%2d", puzx);
    mvwprintw (puzwin, 7, 30, "%2d", puzy);
    screen.add_element (0, 46, "#M ? ", '?');
    screen.add_element (0, 49, "#XX", 'Q');
    vector<string> message;
    wsetcolor (puzwin, TEXTFIELD);
    message.push_back ("Editor Commands:          "); message.push_back ("");
    message.push_back ("Use Mouse or Keyboard"); message.push_back ("");
    message.push_back ("Arrow Keys - Navigate     ");
    message.push_back ("Numbers Keys - Enter guess");
    message.push_back ("BackSpace - Clear Square  ");
    message.push_back ("'S' - Solve               ");
    wcentermsg (puzwin, message, 13, 0, 1);
    if (curloc) {
      curs_set (1);
      wmove (puzwin, 7, 26 + (curloc - 1) * 5);
    } else curs_set (0);
    wrefresh(puzwin);
    // Process Input
    input = screen.get_input ();
    if ((input >= '0') && (input <= '9')) {
      if (curloc == 1) {
        puzx = puzx * 10 + input - '0';
        if (puzx >= 10) {
          curloc = 0;
          if (puzx > 12) puzx = 12;
        }
      }
      if (curloc == 2) {
        puzy = puzy * 10 + input - '0';
        if (puzy >= 10) {
          curloc = 0;
          if (puzy > 12) puzy = 12;
        }
      }
    } else {
      if (puzx < 5) puzx = 5; if (puzy < 5) puzy = 5;
      switch (input) {
        case 'X':
        case 'x':
          curloc = 1; puzx = 0; break;
        case 'Y':
        case 'y':
          curloc = 2; puzy = 0; break;
        case KEY_ENTER :
        case '\n':
          if (!curloc) {
            grid = new int*[puzx];
            for (int xx = 0; xx < puzx; xx++) {
              grid[xx] = new int[puzy];
              for (int yy = 0; yy < puzy; yy++)
                grid[xx][yy] = 0;
            }
            do {
              if (!editpuzzle(puzx, puzy, grid, 1)) return;
              wsetcolor (puzwin, SELECTION);
              mvwprintw (puzwin, 13, 19, " Solving... "); wrefresh (puzwin);
              check = new NSolver(puzx, puzy, grid);
              if (check->getNumSolutions() > 1) {
                vector<string> message;
                wsetcolor (puzwin, SELECTION);
                message.push_back("Multiple solutions found.");
                message.push_back("Press 'C' to change between");
                message.push_back("them.");
                message.push_back("Press 'Q' to Quit.");
                message.push_back("Press Any Key");
                wcentermsg (puzwin, message, 0, 0, 1);
                wrefresh(puzwin);
                getch ();
              }
              if (check->getNumSolutions() < 1) {
                vector<string> message;
                wsetcolor (puzwin, SELECTION);
                message.push_back("No solutions found."); message.push_back("");
                message.push_back("You may have accidently entered");
                message.push_back("a number wrong."); message.push_back("");
                message.push_back("Press any key to edit the puzzle.");
                wcentermsg (puzwin, message, 0, 0, 1);
                wrefresh(puzwin);
                getch ();
              } else {
                do {
                  wsetcolor (puzwin, TEXTFIELD);
                  wcharfillrect (puzwin, ' ', 10, 5, 6, 40);
                  screen.add_element (11, 6, "#TDo you want to try solving this puzzle", 0);
                  screen.add_element (12, 8, "#TBefore you are shown the solution?", 0);
                  screen.add_element (14, 19, "#S Yes ", 'Y');
                  screen.add_element (14, 27, "#S No ", 'N');
                  screen.add_element (0, 46, "#M ? ", '?');
                  screen.add_element (0, 49, "#XX", 'Q');
                  wrefresh (puzwin);
                  input = screen.get_input();
                  switch (input) {
                    case 'y' : input = 'Y'; break;
                    case 'n' : input = 'N'; break;
                    case 'Q' :
                    case 'q' : return;
                    case '?' : info();
                  }
                } while ((input != 'N') && (input != 'Y'));
                if (input == 'Y') {
                  for (int xx = 0; xx < puzx; xx++)
                    for (int  yy = 0; yy < puzy; yy++)
                      grid[xx][yy] = -grid[xx][yy];
                  int won = 1;
                  time (&start);
                  do {
                    editpuzzle(puzx, puzy, grid, 1);
                    won = 1;
                    for (int xx = 0; xx < puzx; xx++)
                      for (int  yy = 0; yy < puzy; yy++)
                        if (abs(grid[xx][yy]) != check->getSolutionXY(xx, yy, 0))
                          won = 0;
                    if (!won) {
                      wsetcolor (puzwin, TEXTFIELD);
                      wcharfillrect (puzwin, ' ', 11, 8, 5, 34);
                      screen.add_element (12, 10, "#TAre you sure you want to stop?", 0);
                      screen.add_element (14, 19, "#S Yes ", 'Y');
                      screen.add_element (14, 27, "#S No ", 'N');
                      screen.add_element (0, 46, "#M ? ", '?');
                      screen.add_element (0, 49, "#XX", 'Q');
                      wrefresh (puzwin);
                      do {
                        input = screen.get_input ();
                        switch (input) {
                          case 'y' :
                          case 'Y' :
                          case 'Q' :
                          case 'q' :
                            input = 1; break;
                          case 'n' :
                          case 'N' :
                            input = 2; break;
                          case '?' :
                            info ();
                          default :
                            input = 0;
                        }
                      } while (!input);
                    }
                  } while ((input != 1) && !won);
                  time (&timer);
                  secs = difftime (timer, start);
                  if (won) {
                    ostringstream minutes, seconds;
                    minutes << (int)secs / 60;
                    seconds << setw(2) << setfill ('0') << (int)secs % 60;
                    wsetcolor (puzwin, TEXTFIELD);
                    vector <string> message;
                    message.push_back ("Congratulations");
                    message.push_back ("");
                    message.push_back ("You solved that puzzle in.");
                    message.push_back (minutes.str() + ":" + seconds.str());
                    message.push_back ("");
                    message.push_back ("Press Any key...");
                    wcentermsg (puzwin, message, 0, 0, 1);
                    wrefresh (puzwin);
                    getch ();
                    return;
                  }
                  for (int xx = 0; xx < puzx; xx++)
                    for (int  yy = 0; yy < puzy; yy++)
                      grid[xx][yy] = abs(grid[xx][yy]);
                }
                int sol = 0;
                do {
                  for (int yy = 0; yy < puzy; yy++)
                    for (int xx = 0; xx < puzx; xx++)
                      if (grid[xx][yy] == check->getSolutionXY (xx, yy, sol))
                        grid[xx][yy] = -check->getSolutionXY (xx, yy, sol);
                      else grid[xx][yy] = check->getSolutionXY (xx, yy, sol);
                  blankscreen ();
                  if (!editpuzzle (puzx, puzy, grid, 0)) return;
                  wsetcolor (puzwin, MENU);
                  screen.add_element (0, 0, "#SE#Mdit ", 'E');
                  screen.add_element (0, 5, "#SQ#Muit     ", 'Q');
                  if (check->getNumSolutions() > 1 )
                    screen.add_element (0, 10, "#SC#Mhange Solution", 'C');
                  wrefresh(puzwin);
                  input = screen.get_input();
                  switch (input) {
                    case 'e' :
                      input = 'E'; break;
                    case 'C' :
                    case 'c' :
                      sol = (++sol)%check->getNumSolutions(); break;
                    case '?' :
                      info (); break;
                    case 'Q' :
                    case 'q' :
                      return;
                  }
                  for (int yy = 0; yy < puzy; yy++)
                    for (int xx = 0; xx < puzx; xx++)
                      if (grid[xx][yy] < 0) grid[xx][yy] = -grid[xx][yy];
                      else grid[xx][yy] = 0;
                } while (input != 'E');
              }
              delete check;
            } while (1);
          } else curloc = 0;
          break;
        case '?' :
          info (); break;
        case 'Q' :
        case 'q' :
          return;
        default :
          curloc = 0;
      }
    }
  }
}

void play () {
  int input = 0;
  wScreenMask screen (puzwin);
  signed int** grid;
  NSolver* check;
  int puzx, puzy, masktype, won, seed;
  time_t start, timer;
  double secs;

  seed = settings (&puzx, &puzy, &masktype, 0);
  if (!seed) return;
  if (seed == 1) seed = time(NULL);
  blankscreen();
  wsetcolor (puzwin, SELECTION);
  curs_set (1);
  mvwprintw (puzwin, 10, 11, "This may take a few minutes");
  mvwprintw (puzwin, 12, 14, " GENERATING PUZZLE... "); wrefresh(puzwin);
  NpathGen puzzle(puzx, puzy, seed);
  puzzle.setMask (masktype);
  grid = new int*[puzx];
  for (int xx = 0; xx < puzx; xx++)
    grid[xx] = new int[puzy];
  makemask (masktype, puzx, puzy, grid);
  for (int xx = 0; xx < puzx; xx++)
    for (int  yy = 0; yy < puzy; yy++)
      grid[xx][yy] = (grid[xx][yy]) ? puzzle.getXY(xx, yy) : 0;
  mvwprintw (puzwin, 14, 13, " CHECKING UNIQUENESS... "); wrefresh(puzwin);
  check = new NSolver(puzx, puzy, grid);
  while (check->getNumSolutions () > 1) {
    mvwprintw (puzwin, 16, 19, " FIXING... "); wrefresh(puzwin);
    int xx; int yy;
    do {
      xx = rand() % puzx; yy = rand () % puzy;
    } while (grid[xx][yy] == 0);
    if (masktype != 5) {
      grid[xx][yy] = 0; // take one away.
      grid[xx][puzy - yy - 1] = 0;
      grid[puzx - xx - 1][yy] = 0;
      grid[puzx - xx - 1][puzy - yy - 1] = 0;
    }
    do {
      xx = rand() % puzx; yy = rand () % puzy;
    } while (check->getSolutionXY(xx, yy, 0) == check->getSolutionXY(xx, yy, 1));
    grid[xx][yy] = puzzle.getXY(xx, yy); // add a different one in.
    grid[xx][puzy - yy - 1] = puzzle.getXY(xx, puzy - yy - 1);
    grid[puzx - xx - 1][yy] = puzzle.getXY(puzx - xx - 1, yy);
    grid[puzx - xx - 1][puzy - yy - 1] = puzzle.getXY(puzx - xx - 1, puzy - yy - 1);
    delete check;
    check = new NSolver(puzx, puzy, grid);
  }
  for (int  xx = 0; xx < puzx; xx++)
    for (int yy = 0; yy < puzy; yy++)
      grid[xx][yy] = -grid[xx][yy]; // "lock" masked squares
  save2txt (grid, &puzzle, "last");
  curs_set (0);
  wsetcolor(puzwin, TEXTFIELD);
  vector<string> message;
  message.push_back ("Ready!"); message.push_back ("");
  message.push_back ("Use Mouse or Keyboard"); message.push_back ("");
  message.push_back ("Editor Keys:              "); message.push_back ("");
  message.push_back ("Arrow Keys - Navigate     ");
  message.push_back ("Numbers Keys - Enter guess");
  message.push_back ("BackSpace - Clear Square  ");
  message.push_back ("'S' - Stop                "); message.push_back ("");
  message.push_back ("If you have the correct solution");
  message.push_back ("when you hit stop your time will");
  message.push_back ("be counted and you'll win."); message.push_back ("");
  message.push_back ("Press Any Key to Start");
  wcentermsg (puzwin, message, 0, 0, 1);
  wsetcolor (puzwin, SELECTION);
  mvwprintw (puzwin, 25, 0, "Code: %02d%02d%02d%d ",
    puzzle.getXSize(), puzzle.getYSize(), puzzle.getMask(), puzzle.getSeed());
  wrefresh (puzwin); getch ();
  time (&start);
  do {
    editpuzzle(puzx, puzy, grid, 1);
    won = 1;
    for (int xx = 0; xx < puzx; xx++)
      for (int  yy = 0; yy < puzy; yy++)
        if (abs(grid[xx][yy]) != puzzle.getXY(xx, yy)) won = 0;
    if (!won) {
      wsetcolor (puzwin, TEXTFIELD);
      wcharfillrect (puzwin, ' ', 11, 8, 5, 34);
      screen.add_element (12, 10, "#TAre you sure you want to stop?", 0);
      screen.add_element (14, 19, "#S Yes ", 'Y');
      screen.add_element (14, 27, "#S No ", 'N');
      screen.add_element (0, 46, "#M ? ", '?');
      screen.add_element (0, 49, "#XX", 'Q');
      wsetcolor (puzwin, SELECTION);
      mvwprintw (puzwin, 25, 0, "Code: %02d%02d%02d%d ",
      puzzle.getXSize(), puzzle.getYSize(), puzzle.getMask(), puzzle.getSeed());
      wrefresh (puzwin);
      do {
        input = screen.get_input ();
        switch (input) {
          case 'y' :
          case 'Y' :
          case 'Q' :
          case 'q' :
            input = 1; break;
          case 'n' :
          case 'N' :
            input = 2; break;
          case '?' :
            info ();
          default :
            input = 0;
        }
      } while (!input);
    }
  } while ((input != 1) && !won);
  for (int yy = 0; yy < puzy; yy++)
    for (int xx = 0; xx < puzx; xx++)
      grid[xx][yy] = (abs(grid[xx][yy]) == puzzle.getXY(xx, yy))
        ? -puzzle.getXY(xx, yy) : puzzle.getXY(xx, yy);
  editpuzzle (puzx, puzy, grid, 0);
  wsetcolor (puzwin, MENU);
  time (&timer);
  secs = difftime (timer, start);
  mvwprintw (puzwin, 0, 1, "Time %d:%02d  Press Any Key", (int) secs / 60, (int) secs % 60);
  wsetcolor (puzwin, SELECTION);
  mvwprintw (puzwin, 25, 0, "Code: %02d%02d%02d%d ",
  puzzle.getXSize(), puzzle.getYSize(), puzzle.getMask(), puzzle.getSeed());
  if (won) {
    ostringstream minutes, seconds;
    minutes << (int)secs / 60;
    seconds << setw(2) << setfill ('0') << (int)secs % 60;
    wsetcolor (puzwin, SELECTION);
    vector <string> message;
    message.push_back ("Congratulations");
    message.push_back ("");
    message.push_back ("You solved that puzzle in.");
    message.push_back (minutes.str() + ":" + seconds.str());
    message.push_back ("");
    message.push_back ("Press Any key...");
    wcentermsg (puzwin, message, 0, 0, 1);
  }
  wrefresh (puzwin);
  getch ();
  return;
}

void mainmenu () {
  int input;
  wScreenMask screen(puzwin);

  while (1) {
    // Visual elements.
    blankscreen ();
    wsetcolor (puzwin, MENU);
    wcharfillrect (puzwin, ' ', 8, 9, 11, 32);
    screen.add_element (6, 10, "#M Numbrix Solver and Generator ", 0);
    screen.add_element (9, 17, "#MDo you want to:",0);
    screen.add_element (11, 13, "#S1)#D Play a random puzzle? ", '1');
    screen.add_element (13, 15, "#S2)#D Solve a Numbrix? ", '2');
    screen.add_element (15, 11, "#S3)#D Make a batch of puzzles? ", '3');
    screen.add_element (17, 17, "#SPress Q to quit", 'Q');
    screen.add_element (0, 46, "#M ? ", '?');
    screen.add_element (0, 49, "#XX", 'Q');
    wrefresh (puzwin);
    // get and process input.
    input = screen.get_input ();
    switch (input) {
      case '1' : play (); break;
      case '2' : solve (); break;
      case '3' : batch (); break;
      case 'Q' :
      case 'q' :
      case KEY_EXIT :
        exit (0);
      case '?' : info();
    }
  }
}

int main() {
  init();
  mainmenu();
  endwin();
  return 0;
}
