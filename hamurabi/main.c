/* Hammurabi ver 2007-Oct-25
 * by Joseph Larson (c) 2008
 * based on a BASIC program written by David Ahl
 * as found in 'BASIC Computer Games' edited by David H Ahl (c) 1978
 * inspired by a Focal program from an unknown author
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SHORT_LAND_MSG printf ("Hammurabi, think again."\
  " You own only %d acres. Now then,\n", acres)
#define SHORT_GRAIN_MSG printf ("Hammurabi, think again."\
  " You have only %d bushels of grain. Now then,\n", grain)

int pop, acres, grain, starved, born, yield, rats, food, year;

void resign (void) {
  printf ("Hammurabi, I can not do what you wish."
    " Get yourself another steward!\n");
  exit (0);
}

void endgame (int rank) {
  switch (rank) {
    case 0:
    printf ("Due to this extreme mismanagement you have not only been"
      " impeached\nand thrown out of office but you have been also declared"
      " a national fink!\n");
    break;
    case 1:
    printf ("You heavy handed performance smacks of Nero and Ivan IV."
      " The people\n(remaining) find you an unpleasant ruler and frankly"
      " hate your guts!!\n");
    break;
    case 2:
    printf ("Your performance could have been somewhat better but really"
      " wasn't\ntoo bad at all. %d people dearly would like to see you"
      " assassinated\nbut we all have our trivial problems.\n"
      , pop * (rand () % 80) / 100);
    break;
    default:
    printf ("A fantastic performance!!! Charlemagne, Disraeli, and Jefferson\n"
      "combined could not have done better!!\n");
  }
}

void report (void) {
  printf ("\n\nHammurabi, I beg to report to you;\n\n"
    "In year %d, %d people starved, %d came to the city.\n"
    , year, starved, born);
  if (rand() < (RAND_MAX / 100 * 15) && year > 1) {
    printf ("A horrible plague struck! Half the people died.\n");
    pop /= 2;
  }
  printf ("Population is now %d.\n"
    "The city now owns %d acres.\n"
    "You harvested %d bushels per acre.\n"
    "Rats ate %d bushels.\n"
    "You now have %d bushels in store.\n\n",
    pop, acres, yield, rats, grain);
}

void buysell (void) {
  int input, price;

  price = rand() % 10 + 17;
  printf ("Land is trading at %d bushels per acre.\n", price);
  printf ("How many acres do you wish to buy? ");
  scanf ("%d", &input);
  while (price * input > grain) {
      SHORT_GRAIN_MSG;
      printf ("how many do you wish to buy? ");
      scanf ("%d", &input);
  }
  if (input < 0) resign();
  if (input) {
    acres += input;
    grain -= price * input;
  } else {
    printf ("How man acres do you wish to sell? ");
    scanf ("%d", &input);
    while (input > acres) {
      SHORT_LAND_MSG;
      printf ("how many do you wish to buy? ");
      scanf ("%d", &input);
    }
    if (input < 0) resign ();
    acres -= input;
    grain += input * price;
  }
}

void feed (void) {
  do {
    printf ("How many bushels do you wish to feed your people? ");
    scanf ("%d", &food);
    if (food < 0) resign ();
    if (food > grain) SHORT_GRAIN_MSG;
  } while (food > grain);
}

void farm (void) {
  int input, v;

  do {
    v = 1;
    printf ("How many acres do you wish to plant with seed? ");
    scanf ("%d", &input);
    if (input < 0) resign ();
    else if (input > acres) {SHORT_LAND_MSG; v = 0;}
    else if (input / 2 > grain) {SHORT_GRAIN_MSG; v = 0;}
    else if (input > 10 * pop) {
      printf ("But you only have %d people to tend the fields! Now then,\n"
      , pop);
      v = 0;
    }
  } while (!v);
  grain -= input / 2;
  yield = rand () % 5 + 1;
  rats = rand () % 5 + 1;
  rats = (rats % 2) ? grain / rats : 0;
  grain += input * yield - rats;
}

int main (void) {
  int cstarved = 0;
  int cps = 0;

  srand (time (NULL));
  printf ("Hammurabi\n\n"
  "Try your hand at governing ancient summeria for a ten-year term of office.");
  pop = 100;
  grain = 2800;
  yield = 3;
  rats = 200;
  acres = 1000;
  born = 5;
  for (year = 1; year <= 10 && starved < pop * 45 / 100; year ++){
    report ();
    buysell ();
    feed ();
    farm ();
    born = ((rand () % 5 + 1) * (20 * acres + grain) / pop / 100) + 1;
    starved = (pop < food / 20) ? 0 : pop - food / 20;
    pop += born - starved;
    cstarved += starved;
    cps += starved * 100 / pop;
  }
  if (starved >= pop * 45 / 100) {
    printf ("\nYou starved %d people in one year!!!\n", starved);
    endgame (0);
  } else {
    year--;
    report ();
    cps /= 10;
    printf ("In your 10-year term of office %d%% of the people died per year\n"
      "on the average, IE a total of %d people died!\n", cps, cstarved);
    acres /= pop;
    printf ("You started with 10 acres per person and ended with %d acres per\n"
      "person.\n\n", acres);
    endgame ((cps<34 && acres>6)+(cps<11 && acres>8)+(cps<4 && acres>9));
  }

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

