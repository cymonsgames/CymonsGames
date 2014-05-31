#include <stdio.h>

main () {
  int c,d;

  for (c = 0; c < 15; c++) printf ("\n%d %d\t%c .x%d",c,c+33,(c+33),21-c);
  for (c = 0; c < 64; c++) {
    printf ("\n%d %d\t%c ",c+15,c+48,(c+48));
    for (d = 0; d < 6; d++) printf ("%c", (c & 1 << d) ? 'X' : '.');
  }
  for (c = 0; c < 15; c++) printf ("\n%d %d\t%c Xx%d",c+79,c+112,(c+112),c+7);
  getchar();
  return 0;
}
