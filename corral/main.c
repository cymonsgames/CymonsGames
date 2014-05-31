/* Corral ver 2007-Oct-24
 * by Joseph Larson (c) 2008
 * based on a BASIC game by Colin Keay
 * as found in 'More BASIC Computer Games' edited by David H. Ahl (c) 1979
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define SPIN rand() % 10

int p[10] = {0, 1, 2, 3, 3, 2, 2, 1, 0, -1};
int cpos, hpos, turn;
char corral [30], msg [10];

void draw_corral (void) {
  int c;

  for (c = 1; c < 22; c++)
    corral[c] = (c == cpos) ? 'C' : (c == hpos) ? 'H' : ' ';
  printf ("%d\t%s %s ", turn, corral, msg);
  strcpy (msg, "       "); /* 7 spaces */
}

void check_hpos (void) {if (hpos < 1) hpos = 1; if (hpos > 21) hpos = 21;}

int kick (int d) {
  hpos -= 5 * d; check_hpos ();
  strcpy (msg, "Kicked!");
  return (p[SPIN] + 2);
}

void bolt (int d) {
  hpos -= d * (9 + 2 * p[SPIN]); check_hpos ();
  if (abs(hpos - cpos) < 2) hpos += 3 * d;
  strcpy (msg, "Bolted!");
}

int setup (void) {
  int r;

  strcpy (corral, "I                     I"); /* 21 spaces between */
  cpos = 1; turn = 0; r = SPIN;
  hpos = (r < 6) ? r + 13 : r + 3;
  strcpy (msg, "       "); /* 7 spaces */
  return (2 + p[r]);
}

void catch (void) {
  int life, kicked, hits, move, dist, dir;

  life = setup ();
  hits = kicked = 0;
  do {
    turn ++;
    draw_corral();
    dist = abs (hpos - cpos);
    dir = (hpos - cpos > 0) ? 1 : -1;
    if (kicked) {
      puts ("");
      --kicked;
      hpos += dir * (p[SPIN] + 1); check_hpos ();
    } else {
      printf ("? "); scanf ("%d", &move);
      while (move < 1 || move > 5 || cpos + move * dir > 21 || cpos + move * dir < 1) {
        printf ("Illegal move. Try again ? ");
        scanf ("%d", &move);
      }
      cpos += move * dir;
      hpos += dir * p[SPIN]; check_hpos ();
      if (dist < 2 * move && move > 1) bolt (dir);
      else if (hpos < 2 || hpos > 20 && SPIN < 2 && dist < 8) bolt (dir);
      else if (abs(hpos - cpos) < 3 && SPIN < 3) {
        kicked = kick (dir); hits ++;
      }
    }
  } while (turn < 100 && hpos != cpos && hits < life);
  turn++;
  if (hpos == cpos) {
    strcpy (msg, "Caught!");
    draw_corral ();
    puts ("\nYippee! Way to go Cowboy.");
  } else if (hits >= life) {
    draw_corral ();
    puts ("\nThose kicks landed you in the hospital! Get well soon!");
  } else puts ("\nEnough! You'd do better as a camp cook!");
}

int play_more (void) {
  char yesno[50];

  printf ("\nAnother roundup ? (y/n) ");
  scanf ("%s", yesno);
  return (yesno[0] == 'y') ? 1 : 0;
}

int main (void) {
  puts ("Corral\n-----\n "
  "You are the cowboy trying to catch your horse in the corral!\n"
  "Move toward your horse from 1 to 5 steps at a time.  If you move more than\n"
  "half the distiance between you and the horse he will bolt. He may also\n"
  "bolt if he's pinned to the rail. Also, if you're close enough he may kick!"
  "\n\nAfter the '?' type a digit from 1 to 5 for the cowboy's next move.\n");
  srand (time (NULL));
  do catch (); while (play_more ());

  puts ("\n\n\n\n\n\n\n\n\tCymon's Games\n\n\thttp://WWW.CYMONSGAMES.COM\n\n\n"
  "   This game and its code is a 2008 Cymon's Games game.\n"
  "   If you have enjoyed this game or would just like to have a new game\n"
  "   each week please visit:\n"
  "   http://WWW.CYMONSGAMES.COM for C/C++ programming resources and programs\n"
  "   for everyone, beginners and advanced users alike.\n\n");
  printf ("\n\tPress ENTER to continue...\n\n\n\n\n\n");
  getchar (); getchar();

  exit (0);
}
