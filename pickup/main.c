/* Pickup Pile ver 2007-Dec-31
 * by Joseph Larson (c) 2008
 * Inspired by a BASIC game Batnum by John Kemeny
 * as found in 'BASIC Computer Games' edited by David H. Ahl (c) 1978
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

/* RANDOM(x) returns a random number between 0 and x.
 * Setup this way it works slighty better than rand() % x. */
#define RANDOM(x) ((int) rand() / ((float)(RAND_MAX + 1) / x))
#define MAXW 79

int num, max, min;
int takelast, comfirst;
int skill;

void intro () {
  printf ("\nPick Up Pile\n------\n"
  "Pick Up Pile is a game with many variations on the same idea.\n"
  "You and the computer each take turns picking up items from an\n"
  "imaginary pile of objects. Depending on the type of game you choose\n"
  "to play you will have different restrictions on how much you can\n"
  "pick up at a time, and whether you want to be the one to pick up the\n"
  "last piece or not.\n"
  "The traditional setup is 23 matches, but you can play any variation.\n");
}

void random_setup () {
      num = RANDOM (25) + 10; max = RANDOM (5) + 3;
      do min = RANDOM (3) + 1; while ( max < min );
      takelast = RANDOM (2); comfirst = RANDOM (2);
}

void custom_setup() {
  char yesno;

  printf ("\nChoose the size of the pile (at least 3) ");
  do scanf("%d", &num);
  while (num < 3);
  printf ("\nWhat is the least that can be taken in a turn? ");
  do scanf("%d", &min);
  while (min <= 0);
  printf ("\nWhat is the most that can be taken in a turn (at least %d)? ", min + 1);
  do scanf("%d", &max);
  while (max < min);
  printf ("\nShould the winner take the last peace? (y/n)");
  do scanf("%c", &yesno);
  while (yesno != 'y' && yesno != 'Y' && yesno != 'n' && yesno != 'N');
  switch (yesno) {
    case 'y': case 'Y': takelast = 1; break;
    case 'n': case 'N': takelast = 0;
  }
  printf ("\nDo you want to go first? (y/n)");
  do scanf("%c", &yesno);
  while (yesno != 'y' && yesno != 'Y' && yesno != 'n' && yesno != 'N');
  switch (yesno) {
    case 'y': case'Y': comfirst = 0; break;
    case 'n': case 'N': comfirst = 1;
  }
}

int humanmove() {
  int input;

  printf ("\nHow many do you want to take (%d - %d) ",
    (min > num) ? num : min, (max > num) ? num : max);
  input = 0;
  do {
    if (input) printf ("\nInvalid move. Try again. ");
    scanf ("%d", &input);
    if (num <= min) input = (input == num) ? input : -1;
      else if (input < min || input > max || input > num) input = -1;
  }
  while (input < 0);
  num -= input;
  printf ("You leave %d", num);
  return ((num) ? 0 : (takelast)? 1 : 2);
}

int compmove () {
  int c;
  int move;

  c = min + max; move = num - !takelast;
  move = move % c;
  if (move < min || move > max || skill < RANDOM(5))
    move = RANDOM((max - min + 1)) + min;
  if (move > num) move = num;
  num -= move;
  printf ("\nComputer takes %d and leaves %d", move, num);
  return ((num) ? 0 : (takelast)? 2 : 1);
}

void showpile () {
  int n, c;

  puts ("");
  n = num;
  while (n > MAXW) {
    for (c = 0; c < MAXW; c++) putchar ('|');
    n -= MAXW;
    puts ("");
  }
  //for (c = 0; c < (MAXW - n) / 2; c++) putchar (' ');
  for (c = 0; c < n; c++) putchar ('|');
  puts ("");
}

void playgame () {
  int input;
  int winner;

  printf ("\n(1) 23 Matches (23 in pile, take at most 3, last piece looses)\n");
  printf ("(2) Random\n");
  printf ("(3) Custom\n");
  printf ("\nChoose a game type: ");
  input = 0;
  do {
    if (input) printf ("\nChoose 1, 2, or 3 please. ");
    scanf ("%d", &input);
  } while (input < 1 || input > 3);
  switch (input) {
    case 1 : num = 23; max = 3; min = 1; takelast = 0; comfirst = 0; break;
    case 2 : random_setup(); break;
    case 3 : custom_setup ();
  }
  printf ("\n%d in pile, %s first, winner %s last piece.\n", num,
    (comfirst) ? "computer goes" : "you go",
    (takelast) ? "takes" : "leaves" );
  printf ("Take at least %d and at most %d per turn.\n\n", min, max);
  printf ("On a scale of 1 to 5, 5 being best,\n");
  printf ("how well do you want the computer to play? (1-5) ");
  scanf("%d", &skill);
  winner = 0;
  if (!comfirst) {
    showpile ();
    winner = humanmove ();
  }
  while (!winner) {
    showpile ();
    winner = compmove ();
    showpile ();
    if (!winner) winner = humanmove ();
  }
  if (winner == 2)
    printf ("\nComputer wins!");
  else printf ("\nYou win!");
}

int playagain () {
  char input;

  printf ("\nThat was fun. Would you like to play again? (y\\n) ");
  do input = getchar();
  while (!isalpha(input));
  if (input == 'y' || input == 'Y') return (1);
  else return(0);
}

int main () {
  srand (time(NULL));
  intro ();
  do playgame (); while (playagain ());
  exit (0);
}
