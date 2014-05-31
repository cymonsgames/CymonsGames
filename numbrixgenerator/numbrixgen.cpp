#include <vector>
#include <cstdlib>
#include <ctime>
#include "numbrixgen.h"

#define NUMBENDS 8

enum {NONE, UP, LEFT, DOWN, RIGHT, END};

int bends[NUMBENDS][2][2][2] = {      /* [type][before/after][y][x]*/
{{{ NONE, DOWN},{ NONE,  END}},{{ DOWN, LEFT},{RIGHT,  END}}},
{{{ NONE, NONE},{  END, LEFT}},{{ DOWN, LEFT},{  END,   UP}}},
{{{  END, NONE},{   UP, NONE}},{{  END, LEFT},{RIGHT,   UP}}},
{{{RIGHT,  END},{ NONE, NONE}},{{ DOWN,  END},{RIGHT,   UP}}},
{{{ NONE,  END},{ NONE,   UP}},{{RIGHT,  END},{   UP, LEFT}}},
{{{ NONE, NONE},{RIGHT,  END}},{{RIGHT, DOWN},{   UP,  END}}},
{{{ DOWN, NONE},{  END, NONE}},{{RIGHT, DOWN},{  END, LEFT}}},
{{{  END, LEFT},{ NONE, NONE}},{{  END, DOWN},{   UP, LEFT}}}
};

NpathGen::NpathGen (int sizex, int sizey, int randseed) {
  maxx = sizex;
  if (sizey) maxy = sizey;
  else maxy = sizex;
  if (!randseed) seed = time (NULL);
  else seed = randseed;
  board = new int[maxx * maxy];
  step[0] = 0; step[1] = -maxx; step[2] = -1; step[3] = maxx; step[4] = 1;
  generate();
}

NpathGen::~NpathGen () {
  delete[] board;
}

void NpathGen::generate () {
  do {
    srand (seed);
    seed++;
    time(&secs);
    for (int c = 0; c < maxx * maxy; c++) board[c] = NONE; // Clear the board
    start = rand() % (maxx * maxy);
    do {
      switch (rand() % 4) {
        case 0 : if (start - maxx >= 0) {
                   board[start] = UP;
                   end = start - maxx;
                } break;
        case 1 : if (start % maxx) {
                   board[start] = LEFT;
                   end = start - 1;
                 } break;
        case 2 : if ((start + maxx) < (maxx * maxx)) {
                   board[start] = DOWN;
                   end = start + maxx;
                 } break;
        case 3 : if ((start + 1) % maxx) {
                   board[start] = RIGHT;
                   end = start + 1;
                 } break;
      }
    } while (board[start] == NONE);
    board[end] = END;
  } while (change() == 0);
  seed--;        // now the puzzle is generated, but it needs to be changed from
  int c = 1;                                     // direction vectors to numbers
  int xy = start;
  int next = start + step[board[start]];
  while (xy != end) {
    board[xy] = c++;
    xy = next;
    next = xy + step[board[xy]];
  }
  board[xy] = c;
}

int NpathGen::change() {
  int c, d;

  using namespace std;

  time(&timer);
  if (difftime(timer, secs) > (maxx * maxy) / 25) return 0; // bandaid
          // First perform a check for completeness and for isolated squares.
  d = 0;
  for (int xy = 0; xy < (maxx * maxy); xy++) {
    if (board[xy] == NONE) {
      c = 0; d++;
      for (int s = 1; s < 5; s++) {                // check for going off edges
        if (((xy + step[s]) >= 0) && (xy + step[s] < (maxx * maxy))// up & down
          && ((xy % maxx) || ((xy + step[s] + 1) % maxx))           // left and
          && (((xy + step[s]) % maxx) || ((xy + 1) % maxx)))           // right
            if ((board[xy + step[s]] == NONE) // see if empty sqaure is alone
              || (xy + step[s] == start) || (xy + step[s] == end))
              c++;
      }
      if (c == 0) return 0;
    }
  }
  if (d == 0) return 1;

  vector<int> modsloc;
  vector<int> modstype;
  modsloc.push_back(999);
  modstype.push_back(999); // Need a value so insert() works. Will be ignored.
         // Now we make a list of all possible modifications. First the bends...
  for (int xy = 0; xy < (maxx * maxy) - maxx; xy++) {
    if ((xy % maxx) == (maxx - 1)) continue;
    for (c = 0; c < NUMBENDS; c++) {
      if ((bends[c][0][0][0] != END) && (bends[c][0][0][0] != board[xy])) continue;
      if ((bends[c][0][0][1] != END) && (bends[c][0][0][1] != board[xy + 1])) continue;
      if ((bends[c][0][1][0] != END) && (bends[c][0][1][0] != board[xy + maxx])) continue;
      if ((bends[c][0][1][1] != END) && (bends[c][0][1][1] != board[xy + maxx + 1])) continue;
      d = rand() % modsloc.size();
      modsloc.insert(modsloc.begin() + d,xy);
      modstype.insert(modstype.begin() + d,c);
    }
  }
  for (int s = 1; s < 5; s++) {            // ...then extending the beginning...
    if (((start + step[s]) >= 0) && (start + step[s] < (maxx * maxy))// up & down
      && ((start % maxx) || ((start + step[s] + 1) % maxx))          // left and
      && (((start + step[s]) % maxx) || ((start + 1) % maxx)))          // right
      if (board[start + step[s]] == NONE) {
        d = rand() % modsloc.size();
        modsloc.insert(modsloc.begin() + d, start + step[s]);
        modstype.insert(modstype.begin() + d, 100);
      }
  }
  for (int s = 1; s < 5; s++) {                       // ...then extending the end.
    if (((end + step[s]) >= 0) && (end + step[s] < (maxx * maxy))  // up & down
      && ((end % maxx) || ((end + step[s] + 1) % maxx))             // left and
      && (((end + step[s]) % maxx) || ((end + 1) % maxx)))             // right
        if (board[end + step[s]] == NONE) {
          d = rand() % modsloc.size();
          modsloc.insert(modsloc.begin() + d, end + step[s]);
          modstype.insert(modstype.begin() + d, 101);
        }
  }
  if (modsloc.size() == 0) return 0; // no possbile modifications
  while (modsloc.size()) {                  // iterate through the list.
    int kind = modstype.back(); modstype.pop_back();
    int xy = modsloc.back(); modsloc.pop_back();
    if (kind == 999) continue;
    if(kind < 100) {                          // apply the modification
      if (bends[kind][0][0][0] != END) board[xy]           = bends[kind][1][0][0];
      if (bends[kind][0][0][1] != END) board[xy + 1]       = bends[kind][1][0][1];
      if (bends[kind][0][1][0] != END) board[xy + maxx]     = bends[kind][1][1][0];
      if (bends[kind][0][1][1] != END) board[xy + maxx + 1] = bends[kind][1][1][1];
    } else if (kind == 100) {
      if ((xy - start) == -1) board[xy] = RIGHT;
      if ((xy - start) == 1) board[xy] = LEFT;
      if ((xy - start) == -maxx) board[xy] = DOWN;
      if ((xy - start) == maxx) board[xy] = UP;
      d = start; // Save the old starting point in case of undo.
      start = xy;
    } else {
      if ((xy - end) == -1) board[end] = LEFT;
      if ((xy - end) == 1) board[end] = RIGHT;
      if ((xy - end) == -maxx) board[end] = UP;
      if ((xy - end) == maxx) board[end] = DOWN;
      d = end; // save the old ending point in case of undo
      end = xy;
      board[end] = END;
    }
    if (change()) return 1;                         // recursive call to self

              // If that didn't work, undo the modification and try the next one
    if(kind < 100) {
      if (bends[kind][1][0][0] != END) board[xy]            = bends[kind][0][0][0];
      if (bends[kind][1][0][1] != END) board[xy + 1]        = bends[kind][0][0][1];
      if (bends[kind][1][1][0] != END) board[xy + maxx]     = bends[kind][0][1][0];
      if (bends[kind][1][1][1] != END) board[xy + maxx + 1] = bends[kind][0][1][1];
    } else if (kind == 100) {
      board[start] = NONE;
      start = d;
    } else {
      board[end] = NONE;
      end = d;
      board[end] = END;
    }
  } // end of loop through list
  return 0;       // None of the modifications worked, so back up and try again.
}
// End NpathGen class

NSolver::NSolver (int x, int y, int** grid) {
  int go;
  signed int dir = -1;

  puzzle = grid;
  sizex = x; sizey = y;
  puzmask = new int[sizex * sizey + 2]; // Adding space for 0.
  for (int c = 0; c < sizex * sizey + 1; c++) puzmask[c] = 0;
  for (int xx = 0; xx < sizex; xx++)
    for (int yy = 0; yy < sizey; yy++)
      if (puzzle[xx][yy])
        puzmask[puzzle[xx][yy]] = 1;
  puzmask[0] = 0; puzmask[sizex * sizey + 1] = 1;
  go = findrun (); // find the shortest run and start there.
  if (go > sizex * sizey) { // go is at the end. Back up and turn around.
    dir = 1;
    while (!puzmask[--go]);
  }
  for (int xx = 0; xx < sizex; xx++) // find the go number on the board.
    for (int yy = 0; yy < sizey; yy++)
      if (go == puzzle[xx][yy]) wiggle (xx, yy, go, dir);
}

NSolver::~NSolver () {
  delete [] puzmask;
  for (unsigned int c = 0; c < solutions.size(); c++) {
    int** temp = solutions.at(c);
    for (int x = 0; x < sizex; x++)
      delete [] temp[x];
    delete [] temp;
  }
}

int NSolver::getSolutionXY (int x, int y, unsigned int n) {
  if (solutions.size() < n + 1) return 0;
  int** temp = solutions.at(n);
  return temp[x][y];
}

int NSolver::findrun() { // find the shortest run
  int minrun = sizex * sizey;
  int loc = 0;
  int run = 0;

  for (int c = 1; c <= sizex * sizey + 1; c++) {
    if (puzmask[c]) {
      if ((run) && (run < minrun)) {
        minrun = run;
        loc = c;
      }
      run = 0;
    } else run++;
  }
  return loc;
}

void NSolver::wiggle(int x, int y, int num, int dir) {
  if (x - 1 >= 0)    check (x - 1, y, num + dir, dir);
  if (y - 1 >= 0)    check (x, y - 1, num + dir, dir);
  if (x + 1 < sizex) check (x + 1, y, num + dir, dir);
  if (y + 1 < sizey) check (x, y + 1, num + dir, dir);
}

void NSolver::check(int x, int y, int num, int dir) {

  //if (solutions.size() > 2) return; // bandaid solution if generating batches
  if (!puzzle[x][y]) {
    if (puzmask[num]) return;
    puzzle[x][y] = num;
    puzmask[num] = 2;
  } else if (puzzle[x][y] != num) return;
  if ((puzmask[num] == 1) || (num == 1) || (num == sizex * sizey)) {
    int dirtemp = -1;
    int go = findrun (); // find the shortest run and start there.
    if (!go)  { // We've found a solution. Store it in solutions.
      int** temp = new int*[sizex];
      for (int xx = 0; xx < sizex; xx++) {
        temp[xx] = new int[sizey];
        for (int yy = 0; yy < sizey; yy++)
          temp[xx][yy] = puzzle[xx][yy];
      }
      solutions.push_back (temp);
    } else if (go > sizex * sizey) { // go is at the end. Back up and turn around.
      dirtemp = 1;
      while (!puzmask[--go]);
    }
    for (int xx = 0; xx < sizex; xx++) // find the go number on the board.
      for (int yy = 0; yy < sizey; yy++)
        if (go == puzzle[xx][yy]) wiggle (xx, yy, go, dirtemp);
  } else wiggle (x, y, num, dir);
  if (2 == puzmask[num]) {
    puzzle[x][y] = 0;
    puzmask[num] = 0;
  }
}
// End NSolver class
