#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

struct square;  //Holds all the information used by a grid-square when solving.

void draw_grid(int xpos, int ypos, int grid[9][9]);  //Draws a Sudoku-grid at the given position

void solve(int grid[9][9]);  //Make one step towards solving the puzzle.

void loadGrid(int grid[9][9], square new_grid[9][9]);  //Loads the grid from a normal array into the "square" array.
int editGrid(int backup[9][9], int grid[9][9]);  //Allows editing the grid.

void rule_1(square new_grid[9][9]);  //Step one for solving.
bool rule_2(square new_grid[9][9]);  //Step two for solving.
bool rule_3(square new_grid[9][9]);  //Step three for solving.

#endif // FUNCTIONS_H_INCLUDED
