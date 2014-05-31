#include <stdio.h>
#include <stdlib.h>

void main () {
int die1, die2, total[12], count;

  randomize();
  for (count=0;count<12;count++)
    total[count]=0;
  for (count=0;count<50;count++) {
    die1=rand()%6+1;
    die2=rand()%6+1;
    printf("Dice 1:%i Dice 2:%i Total:%i\n",die1,die2,die1+die2);
    total[die1+die2-1]++;
  }
  printf("\nTotals:\n");
  for (count=0;count<12;count++)
    printf("%i : %i\n",count+1,total[count]);
}