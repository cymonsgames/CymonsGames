#include <stdio.h>
#include <ctype.h>

int main () {
  char inputbuffer[200];
  int number;
  char letter;

  do {
    printf ("\n(E)ncode or (D)ecode : ");
    scanf ("%200s", inputbuffer);

    switch (toupper(inputbuffer[0])) {
      case 'D':
        printf ("Type the Message to decode.\n");
        do {
          scanf ("%d", &number);
          scanf ("%c", &letter); // catch control characters
          printf("%c", number);
        } while (letter != '\n');
        break;
      case 'E':
        printf ("Type the Message to encode\n");
        do {
          scanf ("%c", &letter);
        } while (letter == '\n'); /* clear out the linefeeds. */
        while (letter != '\n') {
          printf("%d ", letter);
          scanf ("%c", &letter);
        }
        break;
      default:
        printf ("Invalid option\n");
    }
    printf ("\nAgain? (Y/N) : ");
    scanf("%200s", inputbuffer);
  } while ((toupper(inputbuffer[0]) != 'N') && (toupper(inputbuffer[0]) != 'Q'));

  return 0;
}
