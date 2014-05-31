/* Acey Duecy */
/* by Joseph Larson 2005 */
/* Inspired by a BASIC game by Bill Palmby */
/* as found in 'BASIC Computer Games' edited by David H. Ahl (c) 1978 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define WIN 5
#define LOSE 25

int score;

void intro(void) {
  printf("\nAcey Duecy\n""----------\n"
  "Acey Ducey is played by having the computer deal out two cards. The\n"
  "player then decides on whether the next card chosen will be between the\n"
  "first two dealt, Aces high.\n\n"
  "You gain %d points every time you guess correctly and lose %d points for\n"
  "every incorrect guess.\n\n", WIN, LOSE);
  srand(time(NULL));
}

void showcard(int c) {
  if (c <= 10) printf("%d", c);
  else switch (c) {
    case 11 : printf("Jack"); break;
    case 12 : printf("Queen"); break;
    case 13 : printf("King"); break;
    case 14 : printf("Ace");
  }
}

void playhand(void) {
  int card1,card2,card3;
  char yn[50];

  printf("Here are your first two cards:\n\n\t");
  card1 = rand() % 13 + 2;
  do {
    card2 = rand() % 13 + 2;
  } while (card1 == card2);
  showcard(card1);
  printf("\t");
  showcard(card2);
  printf("\n\n");
  if (abs(card1-card2) == 1) {
    printf("Oh, tough luck. There's no card between those two.\n");
    printf("\nSorry. You lose 25 points");
    score -= LOSE;
  } else {
    printf ("Do you think the next card will be between these two? ");
    do {
      printf ("(y\\n) ");
      scanf ("%s",yn);
    } while (tolower(yn[0]) != 'y' && tolower(yn[0]) != 'n');
    if (tolower(yn[0]) == 'y') {
      printf("\nHere is the third card\n\n\t");
      do card3 = rand() % 13 + 2;
      while ((card3 == card1) || (card3 == card2));
      showcard(card3);
      if (((card1 < card3) && (card3 < card2))
      || ((card2 < card3) && (card3 < card1))) {
        printf("\n\nCongratulations. You were correct.\n");
        score += WIN;
      } else {
        printf("\n\nSorry. You lose 25 points");
        score -= LOSE;
      }
    } else {printf ("Nothing ventured, nothing gained,\n");}
  }
  printf("\nScore : %d.\n", score);
}

int playagain(void) {
  char input;

  printf("\nDo you wish to continue? (y\\n) ");
  while (!isalnum (input = getchar()));
  if (tolower (input) != 'n') return 1;
  else return 0;
}

int main(void) {
  intro();
  do playhand();
  while (playagain());
  printf ("Thanks for playing!\n");
  exit (0);
}

