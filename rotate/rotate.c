/* Rotate ver 2007-Oct-25
 * by Joseph Larson (c) 2008
 * Inspired by a BASIC program of the same name by David H. Ahl
 * as found in 'More BASIC Programs' edited by David H. Ahl (c) 1978
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

#define DRAW for(c = 0; c < 16; c++) printf((c % 4) ? "%c " : "\n\n\t%c ", b[c])

int main (void) {
  int c, d, temp, turn, special, done, b[16];
  char input;
  int x[4] = {0, 4, 5, 1};
  int t[9] = {8, 9, 10, 4, 5, 6, 0, 1, 2};
  long mask;

  srand (time (NULL));
  printf ("Rotate\n-------\n"
  "Order the puzzle board from top to botom, left to right, in alphabetical\n"
  "order by rotating four blocks at a time clockwise:\n"
  "\n\tA B C D\n\t 7 8 9\n\tE F G H\n\t 4 5 6\n\tI J K L\n\t 1 2 3\n\tM N O P\n"
  "\nIn the above illustration, the numbers betweent the letters are what you\n"
  "will input. So if you want to rotate the A, D, E, and F you would input 7\n"
  "as your move.\n\n"
  "You also have one special move that you may not need where you can switch\n"
  "the location of two adjacent blocks with each other. Input 0 (zero) as\n"
  "your move if you want to do this. If you try to do a second special move\n"
  "you will be asked if you want to quit.\n"
  "\nGood luck!\n\nPress ENTER to begin...");
  getche ();
  do {
    mask = 0;
    for (c = 0; c < 16; c++) {
      while (1 << (b[c]=rand () % 16) & mask);
        mask |= 1 << b[c];
        b[c] += 'A';
    }
    done = 0;
    for (turn = special = 0; !done; turn++) {
      done = 0;
      printf ("\n\t");
      DRAW;
      printf ("\n\nWhich position to rotate (0 - 9) ? ");
      while (!isdigit(input = getche ()))
        if (isalnum(input))
          printf("\nPlease choose a number between 0 and 9 ? ");
      input -= '0';
      if (input) {
        temp = b[t[--input]];
        for (c = 0; c < 3; c++)
          b[t[input] + x[c]] = b[t[input] + x[c + 1]];
        b[t[input] + x[3]] = temp;
      } else {
        printf ("\nDo you want to quit? (y/n) ");
        while (!isalpha(input = getche ()))
          if (isalpha (input) && toupper (input) != 'Y'
          && toupper (input) != 'N') printf("\nY or N please ? ");
        if (toupper (input) == 'Y') done = 2;
        else if (!special) {
          printf ("\nWhich two letters do you want to switch? ");
          do input = getche ();
          while (toupper (input) < 'A' || toupper (input) > 'P');
          for (c = 0; b[c] != toupper(input); c++);
          do input = getche ();
          while (toupper (input) < 'A' || toupper (input) > 'P');
          for (d = 0; b[d] != toupper(input); d++);
          if (d - c != 1 && d - c != -1 && d - c != 4 && d - c != -4) {
            puts ("Those two letters are not adjacent."); turn --;
          } else {temp = b[c]; b[c] = b[d]; b[d] = temp; special = 1; turn++;}
        }
      }
      if (!done) for (c = done = 1; c < 16; c++)
        if (b[c - 1] > b[c]) done = 0;
    }
    if (!--done) {
      DRAW;
      printf ("\n\nYou ordered the board in %d moves!", turn);
      printf ("\nDo you want to play again? ");
    } else printf ("\nDo you want to try another? ");
    do input = getche ();
    while (toupper (input) != 'Y' && toupper (input) != 'N') ;
    done = (toupper (input) == 'N');
  } while (!done);
  puts ("\nGoodbye!");
  exit (0);
}

