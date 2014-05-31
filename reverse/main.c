/* Reverse 2005-Nov-21
 * by Joseph Larson (c) 2008
 * Inspired by a BASIC game of the same name by Peter Sessions
 * as found in 'BASIC Computer Games' edited by David H. Ahl (c) 1978
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define X 16

int main (void) {
  int list[X], c, d, turns;
  long mask;
  char yn;

  srand (time(NULL));
  printf ("Reverse\n-------\n\n"
    "This is the game of Reverse. To win all you have to do is arrange a list\n"
    "of numbers (1 through %d) in numerical order left to right.\n"
    "To move, you input how many numbers from the right you want to reverse.\n"
    "You will no doubt like this game but, if you want to quit just input '0'\n"
    "as your move.\n\n", X);
  do {
    mask = 0;
    for (c = 0; c < X; c++) {
      while (1 << (list[c] = rand () % X + 1) & mask);
      mask |= 1 << list[c];
    }
    turns = 0;
    puts ("Here we go...\n");
    do {
      putchar ('\t');
      for (c = X; c > 0;) printf ("%2d ",list[--c]);
      printf ("\t? ");
      scanf ("%d", &d);
      if (d > 1 && d <= X) {
        for (c = 0; c < (d / 2); c++) {
          list[c] ^= list[d - c - 1];
          list[d - c - 1] ^= list[c];
          list[c] ^= list[d - c - 1];
        }
        turns++;
      } else printf ("You can not reverse %d. Try again.\n", d);
      for (c = 0; c < X - 1 && list[c] > list [c + 1]; c++);
    } while (d && c < X - 1);
    if (d) {
      putchar ('\t');
      for (c = X; c > 0;) printf ("%2d ",list[--c]);
      printf ("\n\nYou ordered the list in %d moves.\n", turns);
      if (turns > X * 2 - 3)
        printf ("But it shouldn't have taken more than %d moves.\n" , X * 2 - 3);
      if (turns <= X)
        puts ("Wow, you are either extremely good, or extremely lucky.");
    }
    printf ("\nWould you like to try another? (y/n) : ");
    while (!isalpha (yn = getchar ()));
  } while (yn == 'y');

  puts ("\n\n\n\n\n\n\n\n\tCymon's Games\n\n\thttp://WWW.CYMONSGAMES.COM\n\n\n"
  "   This game and its code is a 2008 Cymon's Games game.\n"
  "   If you have enjoyed this game or would just like to have a new game\n"
  "   each week please visit:\n"
  "   http://WWW.CYMONSGAMES.COM for C/C++ programming resources and programs\n"
  "   for everyone, beginners and advanced users alike.\n\n");
  printf ("\n\tPress ENTER to continue...\n\n\n\n\n\n");
  getchar (); getchar();

    puts ("Good bye for now then!");
  exit (0);
}
