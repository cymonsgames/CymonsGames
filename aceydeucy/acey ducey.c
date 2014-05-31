/* Acey Duecy ver 2007-Dec-29
 * by Joseph Larson (c) 2008
 * Inspired by a BASIC game by Bill Palmby
 * as found in 'BASIC Computer Games' edited by David H. Ahl (c) 1978
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define WIN 10
#define LOSE 25

int score;
char suit[4] = {'H', 'D', 'S', 'C'};
char face[16] = "0123456789XJQKA";
char body[4][6] = {" JACK", "QUEEN", " KING", " ACE "};
int odd[4][3] = {{7, 11, 7}, {3, 11, 5}, {9, 1, 9}, {5, 11, 3}};
int even[4][3] = {{2, 8, 4}, {10, 6, 11}, {11, 6, 10}, {4, 8, 2}};

void intro() {
  printf("\nAcey Duecy\n""----------\n"
  "Acey Ducey is played by having the computer deal out two cards. The\n"
  "player then decides on whether the next card chosen will be between the\n"
  "first two dealt, Aces high.\n\n"
  "You gain %d points every time you guess correctly and lose %d points for\n"
  "every incorrect guess.\n\n"
  "The deck is shuffeled after every hand, so no card counting.\n", WIN, LOSE);
  srand(time(NULL));
}

void showcard(int c1, int c2) {
  int a, n, x, y, s[2], c[2];

  c[0] = c1; c[1] = c2; if (c[1]) n = 2; else n = 1;
  s[0] = rand () %4;
  do {s[1] = rand () %4;} while (s[0] == s[1]);
  putchar ('\n'); for (a = 0; a < n; a++) printf ("\t,-------.");
  putchar ('\n'); for (a = 0; a < n; a++) printf ("\t|      %c|", face[c[a]]);
  putchar ('\n'); for (a = 0; a < n; a++) printf ("\t|      %c|", suit[s[a]]);
  for (y = 0; y < 4; y++) {
    putchar ('\n');
    for (a = 0; a < n; a++) {
      printf ("\t| ");
      if (c[a] < 11) {
        putchar (' ');
        for (x = 0; x < 3; x++)
          if (c[a] % 2)
            if (c[a] >= odd[y][x]) putchar (suit[s[a]]); else putchar (' ');
          else if (c[a] >= even[y][x]) putchar (suit[s[a]]); else putchar (' ');
        putchar (' ');
      } else printf ("%s", body[c[a] - 11]);
      printf (" |");
    }
  }
  putchar ('\n'); for (a = 0; a < n; a++) printf ("\t|%c      |", suit[s[a]]);
  putchar ('\n'); for (a = 0; a < n; a++) printf ("\t|%c      |", face[c[a]]);
  putchar ('\n'); for (a = 0; a < n; a++) printf ("\t`-------'");
  putchar ('\n');
}

void playhand() {
  int card1,card2,card3;
  char yn[50];

  printf("\nHere are your first two cards:");
  card1 = rand() % 13 + 2;
  do {card2 = rand() % 13 + 2;} while (card1 == card2);
  showcard(card1, card2);
  if (abs(card1-card2) == 1) {
    printf("\nOh, tough luck. There's no card between those two."
      "\nSorry. You lose %d points", LOSE);
    score -= LOSE;
  } else {
    printf ("\nDo you think the next card will be between these two? ");
    do {
      printf ("(y\\n) ");
      scanf ("%s",yn);
    } while (tolower(yn[0]) != 'y' && tolower(yn[0]) != 'n');
    if (tolower(yn[0]) == 'y') {
      printf("\nHere is the third card");
      do card3 = rand() % 13 + 2;
      while ((card3 == card1) || (card3 == card2));
      showcard(card3, 0);
      if (((card1 < card3) && (card3 < card2))
      || ((card2 < card3) && (card3 < card1))) {
        printf("\nCongratulations. You were correct.");
        score += WIN;
      } else {
        printf("\nSorry. You lose 25 points");
        score -= LOSE;
      }
    } else {printf ("\nNothing ventured, nothing gained,");}
  }
  printf("\nScore : %d.", score);
}

int playagain() {
  char input;

  printf("\nDo you wish to continue? (y\\n) ");
  while (!isalnum (input = getchar()));
  if (tolower (input) != 'n') return 1;
  else return 0;
}

int main() {
  intro();
  do playhand(); while (playagain());
  printf ("\nThanks for playing!\n");
  exit (0);
}

