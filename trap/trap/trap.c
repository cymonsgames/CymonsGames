/* Trap ver 2005-Jun-12
 * by Joseph Larson (c) 2008
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX 100
#define MG 8

int main (void) {
  int x, h, l, try, temp;
  char yesno;

  printf ("Trap\n----\n"
  "In this game you have to try to guess a number between 1 and %d by\n"
  "trapping it. Every guess you type a low and high number seperated by a\n"
  "comma (like \"15, 30\") and you'll be told if the number you are trying to\n"
  "find is between your number. When you think you have it type the same\n"
  "number for both the high and low guess. And remember, you only have %d\n"
  "guesses to find the number\n"
  "Good luck!\n\n", MAX, MG);
  srand (time (NULL));
  do {
    x = rand () % MAX + 1;
    printf ("I have a number. You have %d guesses.\n", MG);
    for (try = 1; try <= MG && !(l == h && h == x); try ++) {
      printf ("\nGuess %d (low , high) : ", try);
      scanf ("%d %*c %d", &l, &h);
      if (l > h) { temp = h; h = l; l = temp;}
      if (l <= x && x <= h)
        printf ("You've trapped my number.\n");
      else printf ("My number is %s than your guesses.\n",
      (l > x) ? "lower" : "higher");
    }
    if (l == h && h == x) printf ("I am undone! You caught my number.\n\n");
    else printf ("Ha ha! That's %d guesses. My number was %d.\n\n", MG, x);
    printf ("Do you want to do play again? (y/n) ");
    while (!isalpha (yesno = getchar ()));
  } while (tolower (yesno) == 'y');
  printf ("Until next time then!\n");
  exit (0);
}

