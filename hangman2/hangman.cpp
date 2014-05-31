// HANGMAN
// Balázs Botond 2007 balazsbotond@gmail.com
// Public Domain

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <time.h>
//#include <windows.h>
using namespace std;

// FUNCTIONS ******************************************************************

// SUBSTITUTE
// Does this:
// str1: ********
// str2: asdasdfa
// ch:   a
// --------------
// str1: a**a***a
void substitute(string& str1, string str2, char ch);

// FILL
// Does this:
// howlong: asdasdfa
// ch:      *
// -----------------
// return:  ********
string fill_string (string how_long, char ch);

// COUNT_LINES
// Counts the lines in a file
int count_lines(char filename[]);

// NTH_LINE
// Returns the n-th line of a file
string nth_line(char filename[], int n);

// RANDOM_INT
// Returns a random integer between low and high, inclusive
int random_int(int low, int high);

// DRAW
// Draw the gallows
void draw (int state);

// CLRSCR
// Clear the screen
void clrscr();

// MAIN PROGRAM ***************************************************************
int main ()
{
    bool the_end = false, win = false;      // These are self-explanatory
    bool quit = false;                      // Does the user want to quit?
    bool guesses[256];                      // Characters the user guessed
    char ch;
    char quitchar;
    int err_count=0;
    string puzzle, solved;                  // The puzzle and how much has been solved from it
    string::size_type place;                // Place of guessed character in the string

    do
    {
        for(int i=0;i<=255;i++)             // No guesses made yet
            guesses[i]=false;

        puzzle = nth_line("hangman.dat",       // We choose a random line
            random_int(1,count_lines("hangman.dat")));

        solved = fill_string(puzzle, '*');  // Fill solved with '*'s

        do
        {
            clrscr();
            cout << "HANGMAN GAME\n";

            draw(err_count);                // Draw the gallows
            cout << solved << "\n";         // Print how much has been solved

            do
            {
                cout << "You have " << 10-err_count;
                cout << " tries left. Enter a letter: ";
                cin >> ch;

                // We need the extended ASCII chart to support languages other than English
                // These have a negative ASCII code - we add 128
                if (guesses[static_cast<int>(ch)+128])
                    cout << " You have already tried this!\n";
            }
            while (guesses[static_cast<int>(ch)+128]);

            guesses[static_cast<int>(ch)+128]=true;

            place=puzzle.find(ch,0);        // Search for the character guessed

            if (place!=string::npos)        // Right guess
            {
                substitute(solved, puzzle, ch);
            }
            else err_count++;               // Wrong guess

            if (err_count==10)
                the_end=true;               // Game over

            if (puzzle==solved)             // You win
            {
                the_end=true;
                win=true;
            }
        }
        while (!the_end);

        clrscr();
        cout << "HANGMAN GAME\n";
        draw(err_count);
        cout << puzzle;

        if (win)
            cout << "\nYou win.";
        else
            cout << "\nYou lose.";          // Vae victis!

        cout << "\nNew game [y,n]? ";
        cin.ignore();
        cin.get(quitchar);
        if (quitchar=='y')
        {
            the_end=false;
            win=false;
            err_count=0;
            clrscr();
        }
        else quit=true;
    }
    while(!quit);

    return 0;
}

// ****************************************************************************

void substitute(string& str1, string str2, char ch)
{
    string::size_type place;
    int i=0;

    do
    {
        place=str2.find(ch,i);
        if(place!=string::npos)              // npos: ha nem találja
        {
            str1[static_cast<int>(place)]=ch;   // castolunk; a place ugye nem
            i=(static_cast<int>(place)) + 1;     // int, hanem size_type...
        }
    } while(place!=string::npos);
}

// ****************************************************************************
string fill_string (string how_long, char ch)
{
    string buffer="";

    for (int i=0; i <= (how_long.length() - 1); i++)
    {
        buffer+=ch;
    }

    return buffer;
}

// ****************************************************************************
int random_int(int low, int high)
{
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    return rand() % (high - low + 1) + low;
}

// ****************************************************************************
int count_lines(char filename[])
{
    ifstream in_stream;
    int count=0;
    string line;

    in_stream.open(filename);

    while (!in_stream.eof())
    {
        in_stream >> line;
        count++;
    }

    in_stream.close();

    return count;
}

// ****************************************************************************
string nth_line(char filename[], int n)
{
    ifstream in_stream;
    string line="";

    in_stream.open(filename);

    if ( (!in_stream.eof()) && (!in_stream.fail()) )
        for (;n>0;n--)
            in_stream >> line;

    in_stream.close();

    return line;
}

// ****************************************************************************
void draw (int state)
{
   switch (state)
   {
      case 10:
         cout << "      ---------\n";
         cout << "      |       |\n";
         cout << "      |       |\n";
         cout << "      O       |\n";
         cout << "     \\|/      |\n";
         cout << "      |       |\n";
         cout << "     / \\      |\n";
         cout << "              |\n";
         cout << "---------------\n";
         break;

      case 9:
         cout << "      ---------\n";
         cout << "      |       |\n";
         cout << "      |       |\n";
         cout << "      O       |\n";
         cout << "     \\|/      |\n";
         cout << "      |       |\n";
         cout << "       \\      |\n";
         cout << "              |\n";
         cout << "---------------\n";
         break;

      case 8:
         cout << "      ---------\n";
         cout << "      |       |\n";
         cout << "      |       |\n";
         cout << "      O       |\n";
         cout << "     \\|/      |\n";
         cout << "      |       |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "---------------\n";
         break;

      case 7:
         cout << "      ---------\n";
         cout << "      |       |\n";
         cout << "      |       |\n";
         cout << "      O       |\n";
         cout << "      |/      |\n";
         cout << "      |       |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "---------------\n";
         break;

      case 6:
         cout << "      ---------\n";
         cout << "      |       |\n";
         cout << "      |       |\n";
         cout << "      O       |\n";
         cout << "      |       |\n";
         cout << "      |       |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "---------------\n";
         break;
      case 5:
         cout << "      ---------\n";
         cout << "      |       |\n";
         cout << "      |       |\n";
         cout << "      O       |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "---------------\n";
         break;
      case 4:
         cout << "      ---------\n";
         cout << "      |       |\n";
         cout << "      |       |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "---------------\n";
         break;
      case 3:
         cout << "      ---------\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "---------------\n";
         break;
      case 2:
         cout << "               \n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "              |\n";
         cout << "---------------\n";
         break;
      case 1:
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "---------------\n";
         break;
      case 0:
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
         cout << "               \n";
  }
}

// ****************************************************************************
void clrscr()
{
    for (int c = 0; c < 21; c++) printf("\n");
//    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
//    COORD coord = {0, 0};
//    DWORD count;
//
//    CONSOLE_SCREEN_BUFFER_INFO csbi;
//    GetConsoleScreenBufferInfo(hStdOut, &csbi);
//
//    FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y,
//      coord, &count);
//
//    SetConsoleCursorPosition(hStdOut, coord);
}
