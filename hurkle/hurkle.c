/* Hurkle hunt in 3D ver 2005-Jun-13
 * by Joseph Larson (c) 2008
 * Inspired by a BASIC program by Bob Albrecht
 * as found in 'BASIC Computer Games' edited by David H. Ahl (c) 1978
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MG 10
#define SIZE 10

int main (void) {
  int hx, hy, hz, x, y, z, c;
  char yesno;

  printf ("Hurkle Hunt in 3D\n\n"
  "A hurkle is a cute little creature, that in this game can fly. The hurkle\n"
  "is hiding in a region %d x %d x %d. After every guess your special hurkle\n"
  "locator will tell you which way you need to go. Keep in mind that 1,1,1 is\n"
  "the lowest northwest corner.\n", SIZE, SIZE, SIZE);
  printf ("Input your move by typing the x then y then z coordinate seperated\n"
  "by commas like \"1,2,3\"\n\n");
  srand (time (NULL));
  do {
    hx = rand () % 10 +1;
    hy = rand () % 10 +1;
    hz = rand () % 10 +1;
    printf ("The hurkle is ready.");
    for (c = 1; c <= MG && !(x == hx && y == hy && z == hz); c++) {
      printf ("\nWhere do you want to look? (X,Y,Z) : ");
      scanf ("%d %*c %d %*c %d", &x, &y, &z);
      printf ("The hurkle is ");
      if (y < hy) printf ("south");
      if (y > hy) printf ("north");
      if (x < hx) printf ("east");
      if (x > hx) printf ("west");
      if ((x != hx || y != hy) && z != hz) printf (" and ");
      if (z < hz) printf ("up");
      if (z > hz) printf ("down");
    }
    if (c <= MG) printf ("HERE!\nYou found the hurkle!\n\n");
    else printf ("\nThats %d trys and you didn't find the hurkle.\n"
      "The hurkle was hiding at %d, %d, %d.\n\n", MG, hx, hy, hz);
    printf ("Would you like to play again? (y/n) ");
    while (!isalpha (yesno = getchar()));
  } while (tolower (yesno) != 'n');
  printf ("Good bye for now!");
  exit (0);
}
