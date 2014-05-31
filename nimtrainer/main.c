/* Nim Trainer
 * by Joseph Larson. ver 2008-May-01
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define MAXPILES 10
#define MAX 20

void intro () {
  puts("Nim Trainer\n----------\n"
  "Nim is a counting game where players take turns removing objects from\n"
  "piles of things. You can take as many as you want from whichever pile\n"
  "you want. The person who takes the last one loses.\n\n"
  "It is possible to play a perfect game if you:\n"
  " o Think of the number in each pile in binary.\n"
  " o Do an exclusive 'or' (xor) on the numbers.\n"
  "   or count up the number of 1s in the binary columns and if there are\n"
  "   an even number write a 1, if an odd number wirte a zero.\n"
  " o The number, in binary or integer, is called a 'nimber'.\n"
  " o If the nimber is zero, you can't win this round.\n"
  " o If the number is not zero find a piece you can take that will leave\n"
  "   your opponent with a zero nimber.\n"
  " o To win the game, when you get to the point where you'd normally leave\n"
  "   all piles of size 1, make sure there are an odd number of sized 1 piles\n"
  "   and you won't be taking the last one.\n\n"
  "This program is written so help you learn to win nim by showing you the\n"
  "piles in binary and doing the xor for you so you can seen how it works.\n");
}

void printbinary (int num) { /* Show a number in binary, 1s and 0s. */
  int c;

  for (c = 5; c >= 0; c--)
    if (num & (1 << c)) printf (" 1"); else printf (" 0");
}

void printpile (int num) { /* Show a pile, it's size, and it's binary */
  int c;

  for (c = 0; c < 20 - num; c++) putchar (' ');
  for (c = 0; c < num; c++) printf ("o ");
  for (c = 0; c < 20 - num; c++) putchar (' ');
  printf (": %d \t ", num);
  printbinary (num);
  putchar ('\n');
}

int printgame (int *p) {
  int nimber, c;

  nimber = 0;
  printf ("\n%s%40s%s\t  %s\n", "Pile", "", "Size", "Binary"); /* 2 spaces */

  for (c = 0; c < MAXPILES; c++) {
    if (!p[c]) continue;
    printf ("%4d", c + 1); printpile(p[c]);
    nimber ^= p[c];
  }

  printf ("\n%48s%d\t(","nimber = ",nimber); printbinary(nimber); printf (" )\n");
  return nimber;
}

int won (int *p) {
  int c;

  for (c = 0; c < MAXPILES; c++) if (p[c]) return 0;
  return 1;
}

int allones (int *p) {
  int c;

  for (c = 0; c < MAXPILES; c++) if (p[c] > 1) return 0;
  return 1;
}

int bestmove (int *bmoves, int *p, int nimber) {
  int c, d, nummoves, number;

  nummoves = 0;

  number = 0; /* Use number to count the number of >1 sized piles */
  for (c = 0; c < MAXPILES; c++) {
    bmoves[c] = 0; if (p[c] > 1) number++; /* clear best move array as well*/
  }
  if (number == 1) { /* We are in the end game. */

    printf ("\nThis is it, the end game. Make sure you leave an odd number of "
    "piles of size 1.\n");

    number = 0; nummoves = 1; d = 0;
    for (c = 0; c < MAXPILES; c++)
      if (p[c] > 1) d = c; /* use d to store the 1 pile >1 */
      else if (p[c] == 1) number++;

    if (number % 2) bmoves[d] = p[d];
    else bmoves[d] = p[d] - 1; /* leave an odd number of 1 piles */

  } else if (allones(p)) {
    for (c = 0; c < MAXPILES; c++) if (p[c]) {
      bmoves[c] = p[c]; nummoves++;
    }

  } else if (!nimber) {
    printf ("\nThe nimber is 0, which means there is no best move right now.");
    for (c = 0; c < MAXPILES; c++) if (p[c]) { /* random moves */
      nummoves ++; bmoves[c] = p[c] - rand() % p[c];
    }

  } else { /* the nimber has a value, so we must have a move. Easy first */
    for (c = 0; c < MAXPILES; c++)
      if ((p[c] & nimber) == nimber) {
        nummoves ++;
        bmoves[c] = nimber;
      }
    if (nummoves)
      printf ("\nThere is/are %d relatively easy good moves.\n"
      "Hint: Look for the binary nimber within a pile, and take the nimber's "
      "number\nof pieces away from that pile.\n", nummoves);

    else {
      printf ("\nGood moves are hard to find here. Look at the row(s):\n");
      for (c = 0; c < MAXPILES; c++) for (d = 0; d < p[c]; d++)
      if (!(nimber ^ p[c] ^ d)) {
        nummoves++;
        bmoves[c] = p[c] - d; printf ("Row %d, ", c + 1);
      }
      printf ("\nRemember, to leave your opponent with a nimber of zero\n"
       "Good luck.\n");
    }

  }

  return nummoves;
}

void compymove (int *p, int nimber) {
  int num, c, bests[MAXPILES];

  num = bestmove (bests, p, nimber);
  num -= rand () % num; /* randomly select a move */
  for (c = 0; c < MAXPILES && num; c++)
    if (bests[c]) num--;
  c--;
  printf ("\nComputer takes %d from pile %d leaving...\n", bests[c], c + 1);
  p[c] -= bests[c];
}

void humanmove (int *p, int nimber) {
  int c, nummoves, pile, number;
  int bests[10];

  nummoves = bestmove (bests, p, nimber);
  do {
    printf ("\nTake from which pile? ");
    scanf ("%d", &pile);
    if (!p[--pile]) printf ("\nThere is nothing to take from that pile.");
  } while (!p[pile]);
  do {
    printf ("\nHow many? ");
    scanf ("%d", &number);
    if (!number || number > p[pile])
      printf ("Choose a number between 1 and %d.", p[pile]);
  } while (!number || number > p[pile]);
  p[pile] -= number; /* do the move */
  if (nimber) {
    nimber = 0; /* Calculate the new nimber */
    for (c = 0; c < MAXPILES; c++) nimber ^= p[c];
    if ((nimber) && (!allones(p))) {
      printf ("\nThat wasn't the best move. Move(s) that would have left a "
      "nimber of zero would\nhave been:\n");
      for (c = 0; c < MAXPILES; c++) if (bests[c])
        printf ("%d from pile %d\n", bests[c], c + 1);
      printf ("\nPress ENTER...\n"); getchar (); getchar ();
    }
  }
  printf ("\nYou take %d from pile %d leaving...\n", number, pile + 1);
}

void play () {
  int piles[MAXPILES];
  int c, nimber;
  char ch;

  do {
    puts ("\nDo you want a (S)tandard or (R)andom setup? (S/R) ");
    while (!isalnum(ch = getchar ()));
  } while ((tolower(ch) != 'r') && (tolower(ch) != 's'));
  if (tolower(ch) == 'r')
    for (c = 0; c < MAXPILES; c++) piles[c] = rand () % MAX;
  else
    for (c = 0; c < MAXPILES; c++) piles[c] = (c < 3) ? 3 + c : 0; /* "Standard" setup */

  nimber = printgame (piles);

  while (!won (piles)) {

    if ((!nimber) && (!allones (piles))) {
      printf ("\nComputer is going to win. Do you want to continue? (Y/N) ");
      while (!isalnum(ch = getchar ()));
      if ((tolower(ch) == 'n') || (tolower(ch) == 'q')) return;
    }

    humanmove (piles, nimber);
    nimber = printgame (piles);
    if (won (piles))
      printf ("\nComputer won that round. Try harder next time.");
    else {

      compymove (piles, nimber);
      nimber = printgame (piles);
      if (won (piles)) printf ("\nYou won! You're getting pretty good.");

    }

  }
}

int playmore (void) {
  char yesno;

  printf ("\nWould you like to play again? ");
  while (!isalpha (yesno = getchar ()));
  if (tolower(yesno) != 'n') return 1;
  return 0;
}

int main () {

  intro ();
  srand (time (NULL));
  do play (); while (playmore ());
  return 0;
}
