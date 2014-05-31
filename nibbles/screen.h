#ifndef __SCREEN_H_
#define __SCREEN_H_

void endgraphics (void);
void startgraphics (WINDOW **w);
void drawblock (WINDOW **w, int x, int y);
void clearblock (WINDOW **w, int x, int y);
void newdot (WINDOW **w, bool grid[GRIDX][GRIDY], int *x, int *y);

#endif /* __SCREEN_H_ */
