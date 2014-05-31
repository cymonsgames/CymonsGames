/* Reflex ver 2007-Oct-25
 * by Joseph Larson (c) 2008
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#define MAX 9
#define WIN 0.1
void playgame (void) {
  char input;
  int goal;
  clock_t start, end;
  double elapsed, range;
  goal = rand () % MAX + 1;
  printf ("\nYour goal this time is %d seconds exactly\n"
  "Press ENTER when ready to start the timer...\n", goal);
  getchar ();
  start = clock ();
  printf ("Press ENTER to stop the timer\n");
  getchar ();
  end = clock ();
  elapsed = (double)(end - start) / CLOCKS_PER_SEC;
  printf ("%.2f seconds.\n", elapsed);
  range = (elapsed > goal) ? elapsed - goal : goal - elapsed;
  if (range == 0)
    printf ("UNBELIEVABLE! Are you sure you're not a computer?\n");
  else if (range <= WIN)
    printf ("Close enough. You get a win for that one!\n");
  else printf ("Oh, %.2f away. Not close enough.\n", range);
}
int playagain (void) {
  char input;
  printf ("\nThat was fun. Would you like to play again? (y\\n) ");
  do input = getchar(); while (!isalpha(input)); getchar();
  if (input == 'y' || input == 'Y') return (1);
  else return(0);
}
int main (void) {
  puts ("Reflex\n------\n\n"
  "How's your reflexes? In this game see how accurately you can tell how\n"
  "much time has elapsed. The computer will give you a number of seconds.\n"
  "When you are ready press ENTER to start the timer, then press ENTER\n"
  "again when you think the given amount of time has elapsed.\n");
  srand (time(NULL));
  do playgame (); while (playagain ());

  puts ("\n\n\n\n\n\n\n\n\tCymon's Games\n\n\thttp://WWW.CYMONSGAMES.COM\n\n\n"
  "   This game and its code is a 2008 Cymon's Games game.\n"
  "   If you have enjoyed this game or would just like to have a new game\n"
  "   each week please visit:\n"
  "   http://WWW.CYMONSGAMES.COM for C/C++ programming resources and programs\n"
  "   for everyone, beginners and advanced users alike.\n\n");
  printf ("\n\tPress ENTER to continue...\n\n\n\n\n\n");
  getchar ();

  exit (0);
}
