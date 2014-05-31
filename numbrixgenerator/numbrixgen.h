#ifndef NUMBRIXGEN_H_INCLUDED
#define NUMBRIXGEN_H_INCLUDED

class NpathGen { // Unicursal grid filling path generator for Numbrix Generation
  public:
  // constructors
  NpathGen (int sizex = 9, int sizey = 0, int randseed = 0);
  ~NpathGen ();
  // accesors
  int getXSize () {return maxx;}
  int getYSize () {return maxy;}
  int getXY(int x, int y) {return board[x + y * maxx];}
  void setSeed (int s) {seed = s;}
  int getSeed () {return seed;}
  void setMask (int m) {mask = m;}
  int getMask () {return mask;}
  // Misc. Methods

  private:
  int maxx, maxy, start, end, seed, mask;
  int step[5];
  int* board;
  clock_t timer, secs;
  void generate ();
  int change();
};

class NSolver {
  public:
  NSolver (int x, int y, int** grid);
  ~NSolver ();
  int getSolutionXY (int x, int y, unsigned int n);
  int getSolutionXY (int x, int y) {return getSolutionXY (x, y, 0);}
  int getNumSolutions () {return solutions.size();}

  private:
  std::vector<int**> solutions;
  int sizex, sizey;
  int** puzzle;
  int* puzmask;
  int findrun();
  void wiggle (int x, int y, int num, signed int dir);
  void check (int x, int y, int num, signed int dir);
};

#endif // NUMBRIXGEN_H_INCLUDED
