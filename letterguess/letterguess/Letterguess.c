/* Letter Guess Game ver 2005-Dec-05
 * by Joseph Larson (c) 2008
 * Inspired by the BASIC game 'Letter' written by Bob Albrecht
 * as found in 'BASIC Computer Games' edited by David H Ahl (c)1978
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int main (void) {
  char input, goal;
  int num;

  srand(time(NULL));
  printf ("Guess My Letter Game\n\n");
  do {
    goal = rand() % 26 + 'A';
    num = 0;
    printf ("I have a letter between A and Z.\n");
    do {
      num ++;
      printf ("Guess #%d (A - Z) : ", num);
      do input = getchar (); while (!isalpha (input));
      input = toupper (input);
      if (input != goal) {
        printf ("\nNo, my letter comes ");
        if (input < goal) printf ("AFTER");
          else printf ("BEFORE");
        printf (" your guess.\n");
      }
    } while (input != goal);
    printf ("\nYou got it! It took you %d tries to guess my letter.\n", num);
    if (num >= 4) printf ("I'm sure you could do better, though.\n");
    printf ("\nDo you want to play again? (y/n) ");
    do input = getchar (); while (!isalpha (input));
  } while ((input == 'y') || (input == 'Y'));
  printf ("\nGoodbye.\n");
  exit (0);
}
