#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WAIT puts ("\nPress ENTER to continue..."); getchar(); getchar()

char *roundintro[5] = {
  "This is your first joust. You are up against the Red Knight."
  ,"This is your second joust. Your opponent is the Silver Knight."
  ,"You are doing well! Your third joust is against the Gold Knight."
  ,"This is your final test! If you win this one the princess is yours!\n"
  "This fight is against the fierce Black Knight!"
  ,"Huzzah! You are the winner!\n"
  "Your prowess on the field of tournament has proven you worthy of the hand\n"
  "of the princess. With the consent of her father you marry the fair maiden\n"
  "and live happily ever after.\n The End."},
*aimd[8] = {"1 - Helm", "2 - Shield, Upper Left", "3 - Shield, Upper Middle"
  ,"4 - Shield, Upper Right", "5 - Shield, Lower Left"
  ,"6 - Shield, Lower Middle", "7 - Shield, Lower Right", "8 - Base of Shield"},
*defd[6] = {"1 - Lower Helm", "2 - Right Lean", "3 - Left Lean"
  ,"4 - Steady Seat", "5 - Shield High", "6 - Shield Low"},
*defmsg[8] = {"He missed you.", "He glanced off your shield."
  ,"He knocked off your helm.", "He broke his lance."
  ,"He has unseated you. (Thud!)"
  ,"He has broken his lance, injured and unseated you."
  ,"He has injured and unseated you. (crash)"
  ,"He has broken his lance and unseated you. (Clang!)"},
*aimmsg[8] = {"You missed him. (Hiss!)", "You hit his shield but glanced off."
  ,"You knocked off his helm! (Cheers!)", "You broke your lance. (Crack...)"
  ,"You unseated him. (Loud cheers and huzzahs!)"
  ,"You broke your lance, unseated and injured your foe. (The crowd goes wild!)"
  ,"You injured and unseated your opponent."
  ,"You broke your lance but unseated your opponent."};
int defok[8] = {7, 15, 63, 23, 7, 63, 7, 39},
defx[6][8] = {{0, 4, 5, 1, 3, 7, 1, 3},  {0, 3, 4, 0, 6, 1, 0, 1},
  {0, 0, 1, 3, 0, 3, 7, 5}, {2, 3, 3, 1, 3, 7, 1, 3}, {4, 3, 7, 1, 0, 5, 1, 5},
  {0, 0, 6, 4, 3, 3, 1, 3}},
aimx[8][6] = {{0, 0, 0, 2, 4, 0}, {4, 3, 0, 3, 3, 0}, {5, 4, 1, 3, 7, 6},
  {1, 0, 3, 1, 1, 4}, {3, 7, 1, 3, 0, 3}, {7, 1, 3, 7, 5, 3},
  {1, 0, 7, 1, 1, 1}, {3, 1, 4, 3, 5, 3}};

void intro (void) {
  printf ("Joust\n-----\n"
  "Hear ye, hear ye. Let all the noble knights of the kingdom come to joust\n"
  "at the King's tournament for the favor of the his daughter, the beautiful\n"
  "princess.\n\n");
}

int main (void) {
  int c, aim, def, r, droll, aroll;

  intro ();
  srand (time (NULL));
  r = 0;
  puts (roundintro[r]);
  do {
    puts ("\nChoose your aiming point :");
    for (c = 0; c < 8; c++) puts (aimd[c]);
    printf ("? "); scanf ("%d",  &aim);
    aim--;
    while (aim < 0 || aim > 7) {
      printf ("Invalid. Choose a number between 1 and 8.\n? ");
      scanf ("%d", &aim); aim--;
    }
    puts ("\nYou may use one of these defenses:");
    for (c = 0; c < 6; c++) if (defok[aim] & 1 << c) puts (defd[c]);
    printf ("? "); scanf ("%d", &def); def--;
    while (!(defok[aim] & 1 << def)) {
      printf ("Invalid. Choose an option from the menu above.\n? ");
      scanf ("%d", &def); def--;
    }
    putchar('\n');
    droll = rand () % 8;
    puts (defmsg[defx[def][droll]]);
    do {
      aroll = rand () % 6;
      switch (droll) {
        case 0:
        case 4:
        case 6: if (aroll < 3) aroll = -1; break;
        case 1: if (aroll < 2) aroll = -1; break;
        case 3: if (aroll == 0 || aroll == 2) aroll = -1; break;
        case 7: if (aroll == 1 || aroll == 2) aroll = -1;
      }
    } while (aroll < 0);
    puts (aimmsg[aimx[aim][aroll]]);
    if (defx[def][droll] < 4) {
      if (aimx[aim][aroll] > 3) {
        puts ("\nYou have won this joust.");
        r++;
        WAIT;
        puts (roundintro[r]);
        if (aimx[aim][aroll] == 5 && r < 3) {
          puts ("\nDue to your mighty victory in the last round your next "
            "opponent has conceded \nto you the win in this round.");
          r++;
          WAIT;
          puts (roundintro[r]);
        }
      }
      else {
        puts ("\nYou are both ready to try again.");
        WAIT;
      }
    }
  } while (r < 4 && defx[def][droll] < 4);
  if (r < 4) {
    if (aimx[aim][aroll] < 4) {
      puts("\nToo bad, you lost. You leave the field of tournament a disgrace.");
    } else puts ("\nToo bad, you both lost. At least your honor is intact.");
  }
  puts ("\nFarewell Sir Knight.");
  WAIT;
  return 0;
}
