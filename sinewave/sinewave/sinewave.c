/* Sine Wave by R. Alan Monroe
 * It's "Sine Wave" from _BASIC Computer Games_:
 * http://www.atariarchives.org/basicgames/showpage.php?page=146
 * by David H. Ahl
*/

#include <stdio.h>
#include <math.h>

int main()
{
   float t, a;
   int s, b;

   b=0;
   for (t=0; t<40; t+=.25) {
       a = 26 + 25 * sin(t);
       for (s=0; s < a; s++) printf(" ");
       if (b==0) {
           printf( "Hello\n" );
           b=1;
       } else {
           printf( "World\n" );
           b=0;
       }
   }

   return 0;
}
