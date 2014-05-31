/* Risk Dice Roller ver 2005-Sept-12
 * By Joseph Larson (c) 2008
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define THRESHOLD 10
#define SORT(a,b) if (a  <b) {a ^= b; b ^= a; a ^= b;}

char *face[7] = {".---.","|   |","|o  |","| o |","|  o|","|o o|","`---'"};
char r[7][3] = {{0,0,0},{1,3,1},{2,1,4},{2,3,4},{5,1,5},{5,3,5},{5,5,5}};

void draw_dice (int dice[], int num) {
  int c, d;

  puts ("");
  for (d = 0; d < num; d++) printf ("%s  ", face[0]); puts ("");
  for (c = 0; c < 3; c++) {
    for (d = 0; d < num; d++) printf ("%s  ", face[r[dice[d]][c]]); puts ("");
  }
  for (d = 0; d < num; d++) printf ("%s  ", face[6]); puts ("");
}

int roll (void) {
  int c;

  while ((c = rand()) > RAND_MAX - (RAND_MAX % 6) - 1);
  c /= (RAND_MAX - (RAND_MAX % 6)) / 6;
  return (++c);
}

int clash (int a[], int d[]) {
  int rval;

  SORT (a[0], a[1]); SORT (a[1], a[2]); SORT (a[0], a[1]);
  SORT (d[0], d[1]);
  rval = (a[0] > d[0]);
  a[0] = d[0] = 0;
  return (rval);
}

int main (int argc, char *argv[]) {
  int ak, df, c, dloss, aloss;
  int adice[3], ddice[2];
  char yesno;

  srand (time(NULL));
  do {
    if (argc == 3) {
      ak = atoi (argv[1]); df = atoi (argv[2]); argc = 1;
    } else {
      printf ("\nHow many in the attacking army? "); scanf ("%d", &ak);
      printf ("\nHow many in the defending army? "); scanf ("%d", &df);
    }
    yesno = 'y';
    while (ak > 0 && df > 0 && tolower(yesno) != 'n') {
      aloss = dloss = 0;
      printf ("\nAttacker's Roll : ");
      for (c = 0; c < 3; c++)
        if (ak > c) adice[c] = roll(); else adice [c] = 0;
      draw_dice (adice, (ak > 3) ? 3 : ak);
      printf ("\nDefender's Roll : ");
      for (c = 0; c < 2; c++)
        if (df > c) ddice[c] = roll(); else ddice [c] = 0;
      draw_dice (ddice, (df > 2) ? 2 : df);
      (clash (adice, ddice)) ? dloss++ : aloss++;
      if (ak > 1 && df > 1) (clash (adice, ddice)) ? dloss++ : aloss++;
      printf("\nAttacker loses %d, Defender loses %d\n", aloss, dloss);
      ak -= aloss; df -= dloss;
      printf ("\n%d attacking\n%d defending\n",ak, df);
      if (ak <= THRESHOLD && df > 0 && ak > 0) {
        printf ("\nDoes the Attacker wish to continue? (y/n) ");
        do {yesno = getchar();}
        while (!isalpha(yesno));
      }
    }
    printf ("\nFinal results:\nAttacker %d\nDefender %d", ak, df);
    printf ("\n\nAgain? (y/n) ");
    do {yesno = getchar();} while (!isalpha(yesno));
  } while (tolower(yesno) == 'y');
  exit (0);
}

