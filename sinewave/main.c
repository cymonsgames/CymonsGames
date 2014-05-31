/* Sine Wave by R. Alan Monroe
 * It's "Sine Wave" from _BASIC Computer Games_:
 * http://www.atariarchives.org/basicgames/showpage.php?page=146
 * by David H. Ahl
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

void sleep (int msec) {
// 1000 ticks per second.
  clock_t start, stop;

	start = clock();
	do {
    stop = clock();
  } while ((stop - start) < msec);
}
int main()
{
   float t, a;
   int s, b;
   char c;

   b=0;
   for (t=0; t<40; t+=.25) {
       a = 26 + 25 * sin(t);
       for (s=0; s < a; s++) printf(" ");
       if (b==0) {
           printf( "Cymon's\n" );
           b=1;
       } else {
           printf( " Games\n" );
           b=0;
       }
       sleep(50);
   }
   printf("Press any key...");
   getchar();
   return 0;
}
