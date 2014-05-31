#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int chunk = RAND_MAX / 6;

int hitenter() {
  int ch = 0;
  while((ch = getc(stdin)) != EOF && ch != '\n') continue;
  return EOF == ch;
}

int passball() {
  int roll;
  roll = 1 + rand()/chunk;
  switch (roll) {
    case 1:
      printf("It's a pass to striker left, \n");
      return 2;
    case 2:
      printf( "A pass to the right striker, \n");
      return 2;
    case 3:
      printf( "A quick pass to midfield left, \n");
      return 1;
    case 4:
      printf( "Over to the right midfielder, \n");
      return 1;
    case 5:
      printf( "It's a header on goal... \n");
      return 2;
    case 6:
      printf( "Ball stolen!");
      return -1;
  }
}

int shotball() {
  int roll;
  roll = 1 + rand()/chunk;
  switch (roll) {
    case 1:
      printf("and GOOOOOOAL!!!!!\n");
      return -11;
    case 2:
      printf("The shot's caught; the goalie boots it back with a clear kick.\n");
      return -3;
    case 3:
      printf( "The shot was just wide. Play restarts with a goal kick.\n");
      return -3;
    case 4:
      printf( "It's up, on target... ooh, too high. The goalie punts it out.\n");
      return -3;
    case 5:
      printf( "Shot blocked! Play resumes with a corner kick right.\n");
      return 3;
    case 6:
      printf( "Blocked by a defender! Restarting from corner kick left.\n");
      return 3;
  }
}

int kickball() {
  int roll;
  roll = 1 + rand()/chunk;
  switch (roll) {
    case 1:
      printf("Passed to midfield right.\n");
      return 1;
    case 2:
      printf( "Narrowly passed to midfield left.\n");
      return 1;
    case 3:
      printf( "An easy pass to the fullback.\n");
      return 1;
    case 4:
      printf( "It's a long pass to the left striker!\n");
      return 2;
    case 5:
      printf( "Ball stolen!\n");
      return -1;
    case 6:
      printf( "Foul! Offense is awarded a free kick,\n");
      return 2;
  }
}

int main() {
  printf("\n\n\nFIVE POINT SOCCER\n=================\n");
  printf("First team to five points wins!\n\n\n(Press enter to begin)");
  hitenter();
  srand (time(NULL));
  int score[2] = {0,0};
  int team = 0;
  int nextmove = 1;

  printf("\n\n%s Team:\n", (team==0)?"Home":"Away" );

  do {
    if (nextmove < 0) {
      printf("\n\n%s Team:\n", (team==0)?"Home":"Away" );
    }
    switch ( abs(nextmove) ) {
      case 1:
        nextmove = passball();
        break;
      case 2:
        nextmove = shotball();
        break;
      case 3:
        nextmove = kickball();
        break;
    }
    if (nextmove < 0) {
      if (nextmove == -11) {
        score[team]++;
        nextmove = -1;
      }
      printf("\nScore - Home: %d, Away: %d\n", score[0], score[1]);
      printf("(%s player, press enter)", (team==0)?"Home":"Away" );
      hitenter();
      team = (team + 1) % 2;
    }
  } while (score[0]<5 && score[1]<5);

  printf("\n\nTHE %s TEAM ARE THE CHAMPIONS!!\n\n\n\n(Press enter to exit)"
    , (score[0] > score[1])?"HOME":"AWAY" );
  hitenter();
  return 0;
}
