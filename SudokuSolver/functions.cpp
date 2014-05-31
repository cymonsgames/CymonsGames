#include <string>
#include <curses.h>

#include "functions.h"


extern float _VERSION;
extern char _AUTHOR[];

//A structure for holding all the information used by each square.
struct square
{
    int solved; //The solved value of the square, or 0 for an unsolved square
    int possible[9];  //An array holding which values are possible in the square
};


void draw_grid(int xpos, int ypos, int grid[9][9])
{
    {//Draw the grid
        mvaddch(ypos , xpos+3, ACS_VLINE);  //The code here starts at the top of the grid
        mvaddch(ypos , xpos+7, ACS_VLINE);  //and works it's way down, drawing one row's
        mvaddch(ypos+1,xpos+3, ACS_VLINE);  //grid-lines at a time.  This doesn't draw the
        mvaddch(ypos+1,xpos+7, ACS_VLINE);  //numbers, just the grid itself.
        mvaddch(ypos+2,xpos+3, ACS_VLINE);
        mvaddch(ypos+2,xpos+7, ACS_VLINE);

        mvaddch(ypos+3,xpos+0,  ACS_HLINE);
        mvaddch(ypos+3,xpos+1,  ACS_HLINE);
        mvaddch(ypos+3,xpos+2,  ACS_HLINE);
        mvaddch(ypos+3,xpos+3,  ACS_PLUS);
        mvaddch(ypos+3,xpos+4,  ACS_HLINE);
        mvaddch(ypos+3,xpos+5,  ACS_HLINE);
        mvaddch(ypos+3,xpos+6,  ACS_HLINE);
        mvaddch(ypos+3,xpos+7,  ACS_PLUS);
        mvaddch(ypos+3,xpos+8,  ACS_HLINE);
        mvaddch(ypos+3,xpos+9,  ACS_HLINE);
        mvaddch(ypos+3,xpos+10, ACS_HLINE);

        mvaddch(ypos+4,xpos+3, ACS_VLINE);
        mvaddch(ypos+4,xpos+7, ACS_VLINE);
        mvaddch(ypos+5,xpos+3, ACS_VLINE);
        mvaddch(ypos+5,xpos+7, ACS_VLINE);
        mvaddch(ypos+6,xpos+3, ACS_VLINE);
        mvaddch(ypos+6,xpos+7, ACS_VLINE);

        mvaddch(ypos+7,xpos+0,  ACS_HLINE);
        mvaddch(ypos+7,xpos+1,  ACS_HLINE);
        mvaddch(ypos+7,xpos+2,  ACS_HLINE);
        mvaddch(ypos+7,xpos+3,  ACS_PLUS);
        mvaddch(ypos+7,xpos+4,  ACS_HLINE);
        mvaddch(ypos+7,xpos+5,  ACS_HLINE);
        mvaddch(ypos+7,xpos+6,  ACS_HLINE);
        mvaddch(ypos+7,xpos+7,  ACS_PLUS);
        mvaddch(ypos+7,xpos+8,  ACS_HLINE);
        mvaddch(ypos+7,xpos+9,  ACS_HLINE);
        mvaddch(ypos+7,xpos+10, ACS_HLINE);

        mvaddch(ypos+8, xpos+3, ACS_VLINE);
        mvaddch(ypos+8, xpos+7, ACS_VLINE);
        mvaddch(ypos+9, xpos+3, ACS_VLINE);
        mvaddch(ypos+9, xpos+7, ACS_VLINE);
        mvaddch(ypos+10,xpos+3, ACS_VLINE);
        mvaddch(ypos+10,xpos+7, ACS_VLINE);
    }

    {//draw the numbers
        int xx, yy;
        for(int x=0; x<9; x++)
        {
            if(x<3) {xx=0;}
            else if(x>2 && x<6) {xx=1;}  //This is added to the position later to make
            else if(x>5 && x<9) {xx=2;}  //up for the grid lines.

            for(int y=0; y<9; y++)
            {
                if(y<3) {yy=0;}
                else if(y>2 && y<6) {yy=1;}
                else if(y>5 && y<9) {yy=2;}

                mvaddch(y+yy+ypos, x+xx+xpos, grid[x][y]+48);  //Add the character
            }
        }
    }
}


void solve(int grid[9][9])
{

    //An array holding information for solving
    square new_grid[9][9];

    //Copies the information from "grid" to "new_grid"
    loadGrid(grid, new_grid);


    rule_1(new_grid);  //No number can exist in two places in any one row, column or sub-grid.
    rule_2(new_grid);  //If there is only one possible value for a square, it contains that value.
    rule_3(new_grid);  //If there is only one place in a row, column or subgrid that a number fits, it goes there.


    //Output new information to original grid
    for(int row=0; row<9; row++)
    {
        for(int col=0; col<9; col++)
        {
            grid[row][col] = new_grid[row][col].solved;
        }
    }
}

void loadGrid(int grid[9][9], square new_grid[9][9])
{
    //Loop through the grid...
    for(int row=0; row<9; row++)
    {
        for(int col=0; col<9; col++)
        {
            new_grid[row][col].solved = grid[row][col];  //new_grid[][].solved is used to hold the value in the square

            if(new_grid[row][col].solved == 0)  //If the current square has not been solved...
            {
                for(int i=0; i<9; i++)  //then loop through the array holding possible numbers...
                {
                    new_grid[row][col].possible[i]=1;  //and tell it that they are all possible.
                }
            }
            else  //if the current square has been solved...
            {
                for(int i=0; i<9; i++)  //then loop through the array holding possible numbers...
                {
                    new_grid[row][col].possible[i]=0;  //and tell it that none of them are possible.
                }
            }
        }
    }
}

int editGrid(int backup[9][9], int grid[9][9])
{
    bool editing=true;
    int edit_x=0;  //The x position of the editing cursor.
    int edit_y=0;  //The y position of the editing cursor.

    int row, col;  //The number of rows/columns in the console.
    getmaxyx(stdscr,row,col);  //Gets the numbers via PDCurses.

    while(editing)
    {
        clear();  //Clear the screen.

        std::string msg[] = {"EDITING INSTRUCTIONS:",
                            "Move the cursor with the arrow keys.",  //An array of strings that holds the instructions
                            "Change the number with +, - or [1-9].", //so they can be changed easier later.
                            "Press [SPACE] to view and save changes."};

        for(int i=0; i<4; i++)  //The 4 needs to be equal to the number of lines in the instructions.
        {
            mvprintw(i+1,(col-strlen( msg[i].c_str() ))/2,"%s", msg[i].c_str() );  //Output one line of the instructions.
        }

        int gridpos_x = 34; //The x-position the grid is printed at.
        int gridpos_y = 9;  //The y position the grid is printed at.

        draw_grid(gridpos_x, gridpos_y, grid);  //Draws the grid.

        int xx, yy;
        if(edit_x<3) {xx=0;}  //A variable used to make up for the vertical lines between subgrids.
        else if(edit_x>2 && edit_x<6) {xx=1;}
        else if(edit_x>5 && edit_x<9) {xx=2;}

        if(edit_y<3) {yy=0;}  //A variable used to make up for the horizantal lines between subgrids.
        else if(edit_y>2 && edit_y<6) {yy=1;}
        else if(edit_y>5 && edit_y<9) {yy=2;}

        mvchgat(edit_y+gridpos_y+yy, edit_x+gridpos_x+xx, 1, A_NORMAL, 1, NULL);  //Change the color of the number at the cursor.

        int input = getch();  //Get user input
        if(input>=48 && input<=57)  //If the input is a number from 0-9
        {
            grid[edit_x][edit_y] = input-48;  //Set the current square to the number the user input
        }
        else  //If the input was not a number between 0-9
        {
            switch(input)
            {
                case KEY_UP:  //If the user pressed the up key
                    if(edit_y>0) {edit_y--;}  //If the cursor is not at the top of the grid then move it up
                    else {edit_y=8;}  //If it is at the top move it to the bottom
                    break;
                case KEY_DOWN:  //If the user pressed the down key
                    if(edit_y<8) {edit_y++;}  //If the cursor is not at the bottom of the grid then move it down
                    else {edit_y=0;}  //If it is at the bottom move it to the top
                    break;
                case KEY_LEFT:
                    if(edit_x>0) {edit_x--;}
                    else {edit_x=8;}
                    break;
                case KEY_RIGHT:
                    if(edit_x<8) {edit_x++;}
                    else {edit_x=0;}
                    break;
                case 45:  //If the user pressed the '-' key...
                    if(grid[edit_x][edit_y]>0) {grid[edit_x][edit_y]--;}  //If the number is not 0, subtract 1 from it
                    break;
                case 43:  //If the user pressed the '+' key...
                    if(grid[edit_x][edit_y]<9) {grid[edit_x][edit_y]++;}  //If the number is not 9, add 1 to it
                    break;
                case 32:  //If the player pressed the space-bar...
                    editing = false;  //They're done editing.
                    break;
            }
        }
    }


    while(true)  //An endless loop, correct input will break out of the loop by ending the function.
    {
        clear();  //Clear the screen.
        std::string msg = "Save changes? (y/n/c)";  //The instructions here are only one line, so there is no need for an array.
        mvprintw(0,(col-strlen( msg.c_str() ))/2, "%s", msg.c_str() );  //Output the instructions.

        draw_grid(17, 7, backup);  //Draw the old grid.

        mvhline(12, 32, ACS_HLINE, 16);
        mvaddch(12, 48, '>');    //Draw an arrow.

        draw_grid(53, 7, grid);  //Draw the new grid.

        int input = getch();  //Get user input

        switch(input)
        {
            case 'y':
            case 'Y':  //If the user pressed 'y' or 'Y'
                return 0;  //End the function by returning "true"
                break;
            case 'n':
            case 'N':  //If the user pressed 'n' or 'N'
                return 1;  //End the function by returning "false".
                break;
            case 'c':
            case 'C':  //If the user pressed 'c' or 'C'
                return 2;  //End the function by returning "true".
                break;
        }
    }
}

void rule_1(square new_grid[9][9])
{
    for(int row=0; row<9; row++)
    {
        for(int col=0; col<9; col++)
        {
            int myValue = new_grid[row][col].solved; //Store the value in "solved"...

            if(myValue>0)  //If the square is solved...
            {
                for(int r=0; r<9; r++)
                {   //Loops through all the squares in the same column and tells them
                    //they can't be the same.
                    if(r!=row) {new_grid[r][col].possible[myValue-1]=0;}
                }
                for(int c=0; c<9; c++)
                {   //Loops through all the squares in the same row and tells them
                    //they can't be the same.
                    if(c!=col) {new_grid[row][c].possible[myValue-1]=0;}
                }

                int subgrid_row;  //A subgrid is a 3x3 section in a puzzle
                int subgrid_col;  //These variables track which one the current tile is in.

                if(row<=2) {subgrid_row = 0;}  //Rows 0-2 belong to the first row of subgrids
                else if(row>=3 && row<=5) {subgrid_row = 1;}  //Rows 3-5 belong to the second row of subgrids
                else if(row>=6 && row<=8) {subgrid_row = 2;}  //Rows 6-8 belong to the third row of subgrids

                if(col<=2) {subgrid_col = 0;}
                else if(col>=3 && col<=5) {subgrid_col = 1;}
                else if(col>=6 && col<=8) {subgrid_col = 2;}

                for(int r=subgrid_row*3; r<(subgrid_row*3)+3; r++)
                {
                    for(int c=subgrid_col*3; c<(subgrid_col*3)+3; c++)
                    {   //Loop through the tiles in the current subgrid
                        //and tell them they can't be the same as this one.
                        if(c!=col && r!=row)
                        {
                            new_grid[r][c].possible[myValue-1]=0;
                        }
                    }
                }

            }  //if(myValue>0)
        }
    }
}

bool rule_2(square new_grid[9][9])
{
    // -=- SOLVE MENTHOD 1 -=-
    //If there is only one possible value for a square, it contains that value.
    for(int row=0; row<9; row++)
    {
        for(int col=0; col<9; col++)  //Loop through squares
        {
            if(new_grid[row][col].solved==0)  //If the current square is unsolved
            {
                int numberPossible = 0;  //Used to hold the number of possible values for the square

                for(int i=0; i<9; i++)  //Loop through possible values...
                {
                    numberPossible += new_grid[row][col].possible[i];  //and add 1 to numberPossible for each
                }

                if(numberPossible==1)  //if there was only one possible value
                {
                    for(int i=0; i<9; i++)  //loop through the possible values
                    {
                        if(new_grid[row][col].possible[i]==1)  //if we've found the correct one
                        {
                            new_grid[row][col].solved = i+1;  //Solve the square
                            return true;

                            for(int r=0; r<9; r++)
                            {   //Loops through all the squares in the same column and tells them
                                //they can't be the same.
                                if(r!=row) {new_grid[r][col].possible[i]=0;}
                            }
                            for(int c=0; c<9; c++)
                            {   //Loops through all the squares in the same row and tells them
                                //they can't be the same.
                                if(c!=col) {new_grid[row][c].possible[i]=0;}
                            }

                        }  //if(new_grid[row][col].possible[i]==1)

                    }  //for(int i=0; i<9; i++)

                }  //if(numberPossible==1)

            }  //if(new_grid[row][col].solved==0)
        }
    } //END OF SOLVE METHOD 1
    return false;
}

bool rule_3(square new_grid[9][9])
{
    // -=- SOLVE METHOD 2 -=-
    //If there is only one place in a row, column or subgrid that a number fits, it goes there.
    int canBe[9];

    for(int row=0; row<9; row++)
    {  //Loop through all the rows...
        for(int i=0; i<9; i++) {canBe[i]=0;}  //Reset canBe

        for(int col=0; col<9; col++) //Loop through the tiles in the row
        {
            for(int i=0; i<9; i++) //Loop though the possible values for the current tile
            {
                canBe[i] += new_grid[row][col].possible[i];  //Tell it that the number "i" is possible in this square
            }
        }

        for(int i=0; i<9; i++)  //Loop through the possible values
        {
            if(canBe[i]==1)  //If only one square in this row can hold the number...
            {
                for(int col=0; col<9; col++)  //Loop through the tiles in this row
                {
                    if(new_grid[row][col].possible[i]==1)  //And when we find the one that the number is possible in...
                    {
                        new_grid[row][col].solved = i+1;  //Solve the tile with that number.
                        return true;
                    }
                }

            }
        }
    }

    for(int col=0; col<9; col++)
    {
        for(int i=0; i<9; i++) {canBe[i]=0;}

        for(int row=0; row<9; row++)
        {
            for(int i=0; i<9; i++)
            {
                canBe[i] += new_grid[row][col].possible[i];
            }
        }

        for(int i=0; i<9; i++)
        {
            if(canBe[i]==1)
            {
                for(int row=0; row<9; row++)
                {
                    if(new_grid[row][col].possible[i]==1)
                    {
                        new_grid[row][col].solved = i+1;
                        return true;
                    }
                }

            }
        }
    }

    for(int subgrid_row=0; subgrid_row<3; subgrid_row++)  //Loop through each row of subgrids
    {
        for(int subgrid_col=0; subgrid_col<3; subgrid_col++)  //Loop through the subgrids in the current row.
        {

            for(int i=0; i<9; i++) {canBe[i]=0;}  //Clear canBe

            for(int r=subgrid_row*3; r<(subgrid_row*3)+3; r++) //Loop through the rows in this subgrid
            {
                for(int c=subgrid_col*3; c<(subgrid_col*3)+3; c++)  //Loop through the squares in this row of the subgrid...
                {
                    for(int i=0; i<9; i++)  //Loop through the possible values for the current square
                    {
                        canBe[i] += new_grid[r][c].possible[i];  //Tell it that the number "i" is possible in this square
                    }
                }
            }

            for(int i=0; i<9; i++)  //Loop through possible values
            {
                if(canBe[i]==1)  //If the current number is only possible in one square...
                {
                    for(int r=subgrid_row*3; r<(subgrid_row*3)+3; r++)  //Loop through each row of tiles in the subgrid
                    {
                        for(int c=subgrid_col*3; c<(subgrid_col*3)+3; c++)  //Loop through the squares in this row of the subgrid...
                        {
                            if(new_grid[r][c].possible[i]==1)  //If the number is possible in this square
                            {
                                new_grid[r][c].solved = i+1;  //Assign the number to this square
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }  //END OF SOLVE METHOD 2
    return false;
}
