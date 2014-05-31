/* Nicomachus ver 2007-Oct-25
 * by Joseph Larson (c) 2008
 * based on a BASIC program of the same name by David Ahl
 * as found in 'BASIC Computer Games' edited by David H. Ahl (c) 1978
 */
#include <stdio.h>
#include <ctype.h>

#define ASK(x,y) printf ("Your number divided by %d has a remainder of ? ", x);\
  scanf ("%d", &y)
#define TELL(x, y) if (!(d % x == y))\
  printf ("%d divided by %d has a remainder of %d, not %d.\n", d, x, d % x, y)

int main (void) {
  int a, b, c, d, r;
  char yesno[25];

  printf ("A mathematical boomerang puzzle as invented by the ancient\n"
  "mathematician Nicomachus.\n\n");
  do {
    printf ("Think of a number between 1 and 100\n");
    ASK(3, a); ASK(5, b); ASK(7, c);
    printf ("\n\nSo your number is...\n\n");
    r = 70* a + 21 * b + 15 * c;
    while (r > 105) r -= 105;
    printf ("\t%d!\n\n", r);
    printf ("Am I correct? ");
    scanf ("%s", yesno);
    if (tolower(yesno[0]) != 'y') {
      printf ("\nWhat was your number? ");
      scanf ("%d", &d);
      if (d - r) {TELL (3, a); TELL (5, b); TELL (7, c);
      } else printf ("That's what I said, %d. You had me worried.\n", r);
    } else printf ("Yes, I knew it.\n");
    printf ("\nThank you. Would you like to play again? ");
    scanf ("%s", yesno);
  } while (tolower(yesno[0]) != 'n');
  printf ("Good Bye, then.");
  return 0;
}
