/* Hello by Joseph Larson ver 2005-Nov-16 */

#include <stdio.h>

int main (void) {
  char name[50], food[50], color[50], yesno[50];

  printf ("Hello. What's your name? ");
  scanf ("%50s", name);
  printf ("Hello %s. What is your favorite food? ", name);
  scanf ("%50s", food);
  printf ("Oh, I like %s too. And what is your favorite color? ", food);
  scanf ("%50s", color);
  printf ("So, %s, would you like %s %s? ", name, color, food);
  scanf ("%50s", yesno);
  if (yesno[0] == 'y' || yesno[0] == 'Y') printf ("I knew you would.\n");
  else printf ("That's too bad.\n");
  printf ("Well %s, good bye for now.\n", name);
  return 0;
}
