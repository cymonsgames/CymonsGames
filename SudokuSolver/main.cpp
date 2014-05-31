#include <curses.h>

#include "functions.h"

float _VERSION = 1.0;
char _AUTHOR[] = "Timothy Sassone";

int defboard[9][9] = {  {0,0,0, 0,0,0, 0,0,0,},
                        {0,0,0, 0,0,0, 0,0,0,},
                        {0,0,0, 0,0,0, 0,0,0,},

                        {0,0,0, 0,0,0, 0,0,0,},
                        {0,0,0, 0,0,0, 0,0,0,},  //The default puzzle board.
                        {0,0,0, 0,0,0, 0,0,0,},

                        {0,0,0, 0,0,0, 0,0,0,},
                        {0,0,0, 0,0,0, 0,0,0,},
                        {0,0,0, 0,0,0, 0,0,0,}};

int main()
{
        initscr();  //Initialize PDCurses
        curs_set(0);  //Disable the normal cursor
        noecho();  //Don't echo input
        keypad(stdscr, true);  //Enable the keypad
        start_color();  //Enable color

        init_pair(1, COLOR_BLUE, COLOR_WHITE);  //Define color pair 1 as blue on white.

        int grid[9][9];  //Starting grid
        int solvedgrid[9][9];  //The grid that gets solved

        for(int x=0; x<9; x++)
        {
            for(int y=0; y<9; y++)
            {
                grid[x][y]=defboard[x][y];  //Set the information in the grid to the default board.
            }
        }

        for(int x=0; x<9; x++)
        {
            for(int y=0; y<9; y++)
            {
                solvedgrid[x][y]=grid[x][y];  //Copy the information in the grid to the "solvedgrid".
            }
        }

        bool quit = false;

        while(quit==false)
        {
            clear();  //Clear the screen.

            mvprintw(2,4, "Sudoku Solver v%2.3f", _VERSION);
            mvprintw(3,5, "by %s", _AUTHOR);

            mvprintw(2,34, "Press a number [1-9] to make that");
            mvprintw(3,36, "many solve-steps.");                 //Draw the menu.
            mvprintw(4,34, "Press [SPACE] to input new puzzle data.");

            draw_grid(17, 10, grid);  //Draw the first grid

            mvhline(15, 32, ACS_HLINE, 16);
            mvaddch(15, 48, '>');      //Draw an arrow

            draw_grid(53, 10, solvedgrid);  //Draw the second grid

            int input = getch();  //Get the user's input

            if(input>=49 && input<=57)  //If the input was a number from 1-9
            {
                int times = input-48;  //Set times to the number that was input (converting from a key to a number)
                for(int i=0; i<times; i++)  //Repeat "times" time.
                {
                    solve(solvedgrid);  //Solve using the "solvedgrid", this is repeated x number of times, see above.
                }
            }
            else  //if(input>=49 && input<=57)
            {
                switch(input)
                {
                    case 'q':  //If the user pressed q
                    case 'Q':  //or Q, in case shift is being held down
                        quit=true; //Set quit to true to exit the program
                        break;
                    case 32: //If the user pressed the space-bar

                        int backup[9][9];  //A backup grid to hold information incase the user wants to revert after editing.
                        for(int x=0; x<9; x++)
                        {
                            for(int y=0; y<9; y++)
                            {
                                backup[x][y] = grid[x][y];  //Fill in the backup grid with current information before editing begins.
                            }
                        }

                        bool editing=true;
                        int edit;

                        while( editing )
                        {
                            edit = editGrid(backup, grid);  //Let the user edit the grid and store their last choice in "edit"

                            if(edit==0)  //If the user picked yes when finished editing...
                            {
                                for(int x=0; x<9; x++)
                                {
                                    for(int y=0; y<9; y++)
                                    {
                                        solvedgrid[x][y]=grid[x][y];  //Copy the new information from grid to solvedgrid...
                                    }
                                }
                                editing=false;  //The user is done editing, so we can break out of this loop
                            }
                            else if(edit==2)  //If the user picked cancel when finished editing
                            {
                                for(int x=0; x<9; x++)
                                {
                                    for(int y=0; y<9; y++)
                                    {
                                        grid[x][y] = backup[x][y];  //Return the old information
                                    }
                                }
                                editing=false;  //The user is done editing, so we can break out of this loop
                            }
                        }

                        break;
                }
            }
        }

        endwin();  //Uninitialize PDCurses

	return 0;
}
