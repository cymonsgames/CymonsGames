/* Seeing Stars ver 2005-Oct
 * By Joseph Larson (c) 2008
 * Number guessing game inspired by the BASIC game "Stars" by Bob Albrecht
 * as found in 'BASIC Computer Games' edited by David H. Ahl (c) 1978
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#define MAX 100
#define MAX_GUESS 10

int main () {
  int goal, guess, c, wins, loses;
  char yesno;
  float d;

  srand(time(NULL));
  wins = 0;
  loses = 0;
  printf ("Seeing Stars\n------------\n"
  "This is a number guessing game with a twist. After every guess you\n"
  "will be given a number of stars to tell you how close you are to the\n"
  "number you are trying to guess. The more stars you see, the closer\n"
  "you are.\n"
  "You only have %d guesses, so think quick.\n\n", MAX_GUESS);
  do {
    goal = rand() % MAX + 1;
    guess = 0;
    printf ("\nI'm thinking of a number between 1 and %d.", MAX);
    for (c = 0; c < MAX_GUESS && guess != goal; c++) {
      printf ("\nWhat is your guess? ");
      scanf ("%d", &guess);
      for (d = MAX; (int)d > abs(guess - goal); d /= 2) printf ("**");
    }
    if (guess != goal) {
      printf ("\nSorry, thats %d guesses. My number was %d.\n", MAX_GUESS, goal);
      loses ++;
    } else {
      printf ("******\nBingo! You got my number.\n");
      wins ++;
    }
    printf ("Do you want to play again? (y/n) ");
    while (!isalpha (yesno = getchar()));
  } while (tolower (yesno) != 'n');
  printf ("\nWell then, you won %d out of %d games, or %d%%.\n",
    wins, wins + loses, wins * 100 / (wins + loses));
  exit (0);
}
