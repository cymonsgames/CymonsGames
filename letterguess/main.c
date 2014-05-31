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

  puts ("\n\n\n\n\n\n\n\n\tCymon's Games\n\n\thttp://WWW.CYMONSGAMES.COM\n\n\n"
  "   This game and its code is a 2008 Cymon's Games game.\n"
  "   If you have enjoyed this game or would just like to have a new game\n"
  "   each week please visit:\n"
  "   http://WWW.CYMONSGAMES.COM for C/C++ programming resources and programs\n"
  "   for everyone, beginners and advanced users alike.\n\n");
  printf ("\n\tPress ENTER to continue...\n\n\n\n\n\n");
  getchar (); getchar();
  printf ("\nGoodbye.\n");
  exit (0);
}
