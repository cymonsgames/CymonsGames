/* Hangman
 * by Joseph Larson 2005
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <ctype.h>

char missed[11] = "==========";
char dict[255][25] = {"artfull", "baton", "brazier", "create", "disgard",
  "flight", "great", "hydrant", "justify", "lacerate", "master", "niagra" ,
  "oxygen", "pieces", "question", "rough", "sachet", "trumpet", "vestibule"};
int dictnum = 19;

void drawgallows (int d) {
  printf ("\n                   %c ||\n", d < 9 ? ' ' : 'O');
  printf ("||    %c  %s%s%s%s ||\n", d < 1 ? ' ' : '@', d < 3 ? "   " : "/==", 
    d < 7 ? "  " : "O ", d < 5 ? "    " : "/===", d < 9 ? "  " : "//");
  printf ("||%s%s      ||\n", d < 1 ? "--ss(O)" : "-O):o{)", 
    d < 2 ? "      " : "X###]<");
  printf ("||\\   %c  %s%s%s%s ||\n", d < 1 ? ' ' : '@', d < 4 ? "   " : "\\==", 
    d < 8 ? "  " : "O ", d < 6 ? "    " : "\\===", d < 10 ? "  " : "\\\\");
  printf ("||\\\\               %c ||\n", d < 10 ? ' ' : 'O');
  printf ("|==========%s||\n\n", missed);
}

void play (void) {
unsigned long guessed = 0;
char *word, input[25];
int c, found, nummissed = 0;

  for (c = 0; c < 10; c++) missed[c] = '=';
  c = rand () % dictnum;
  word = dict[c];
  guessed = ULONG_MAX << strlen (word);
  for (c = 0; c < strlen (word); c++) 
    if (toupper(word[c]) < 'A' || toupper(word[c]) > 'Z')
      guessed |= (1 << c);
  do {
    drawgallows (nummissed);
    for (c = 0; c < strlen (word); c++) 
      putchar (guessed & (1 << c) ? word[c] : '-');
    printf ("\nWhat is your guess? "); scanf ("%s", input);
    if (strlen (input) > 1)
      puts ("Only guess oneletter at a time please");
    else {
      found = 0;
      for (c = 0; c < strlen (word); c++) 
        if (toupper(word[c]) == toupper(input[0])) {
          found = 1;
          guessed |= (1 << c);
        }
      if (toupper(input[0]) < 'A' || toupper(input[0]) > 'Z') {
        found = 1;
        puts ("\nPlease guess a letter.");
      }
      if (!found) {
        printf ("\nNope, no %c.\n", input[0]); 
        missed[nummissed++] = toupper(input[0]);
      }
    }
  } while (nummissed < 10 && guessed < ULONG_MAX);
  drawgallows (nummissed);
  if (nummissed == 10) printf ("\nYour man is hanged. My word was '%s'.\n", word);
  else printf ("%s\n\nGood work! You guessed my word!", word);
}

int play_again (void) {
  char yesno;

  printf ("\nWould you like to try another one? ");
  while (!isalpha (yesno = getchar ()));
  if (tolower(yesno) != 'n') return 1;
  return 0;
}

int main (int argc, char *arg[]) {
FILE *fp;
char buffer[25];

  srand (time (NULL));
  if (--argc > 0) {
    dictnum = 0;
    fp = fopen (arg[1], "r");
    while (fgets (buffer, 25, fp)) {
      buffer[strlen(buffer) - 1] = 0;
      strcpy (dict[dictnum++], buffer);
    }
  }
  puts ("Hangman\n-------\n"
  "Try to guess the word and save yourself from the gallows.\n"
  "(To see the graphic tilt your head to the left.)\n");
  do {play ();} while (play_again ());
  puts ("Good-bye.\n");
  exit (0);
}
