
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

int tries = 0;
int clue = 0;

void PrintScreen (int leading)
{
     if (leading > 0)
     {
        for (int i=0; i < leading; ++i)
            printf ("\n");
     }
     printf (" %i\n", tries);
     printf ("\n");
     printf ("  __________________\n"); 
     if (clue)
     {
        printf (" |  !     |         |\n");
        printf (" |_    ___|__ ! ____|\n");              
     }
     else
     {
        printf (" |  ?     |         |\n");
        printf (" |__    __|__ ? ____|\n");
     }
     printf ("    |  |     |  |\n");
     printf ("    |  | +   |  |\n");
     printf ("    |  |     |  |\n");
     printf ("  __|  |__   |  |\n");
     if (clue)
     {
         printf (" |        | !|  |\n");
     }
     else
     {
         printf (" |        | ?|  |\n");
     }
         printf (" |________|  |__|\n");
}

void PrintGibberish (void)
{
     for (int i=0; i < 450; ++i)
     {
         for (int x=0; x < 60; ++x)
             printf("%c", 32+rand()%224);
         
         printf("\n");
     }
}

int ProcessCommand (char *command)
{
    clue = 0;
    if (!strcmp(command, "help"))
    {
        ++tries;
        PrintScreen(20);
        printf("Haha, good one, this is a Towlr game.\n\n");
        return 1;
    }
    else if (!strncmp(command, "figure it", 9) || !strcmp(command, "solve it") ||
      !strncmp(command, "i figured it", 12) || !strcmp(command, "i know it") ||
      !strcmp(command, "find it out") || !strcmp(command, "decipher it") ||
      !strncmp(command, "unravel it", 10))
    {
        ++tries;
        return 2;
    }
    else if (!strncmp(command, "figure", 6) || !strncmp(command, "solve", 5) ||
         !strncmp(command, "i know", 6) || !strcmp(command, "it"))
    {
         ++tries;
         clue = 1;
         PrintScreen(20);
         return 1;
    }
    else if (!strcmp(command, "look"))
    {
        PrintScreen(20);
        printf("Behind you is a long and winding road, with a tower\n");
        printf("at the end, and a blocky, square mountain behind that.\n");
        printf("Ahead is the mystery of IT, whatever IT is.\n");
        return 1;
    }
    else if (!strncmp(command, "arg", 3))
    {
        ++tries;
        PrintScreen(20);
        printf("Arg? Don't give up!\n");
        return 1;
    }
    else if (!strcmp(command, "quit") || !strcmp(command, "exit"))
    {
        printf("Victory is not far!\n");
        return -1;
    }
    
    ++tries;
    
    PrintGibberish();
    PrintScreen(20);
    
    return 0;
}

int main(int argc, char *argv[])
{
    char input[128];
    int letter = 0, result;
    
    srand(time(NULL));
    
    printf("Welcome to TAWS Volume 6\n");
    printf("by Entar\n");
    printf("TAWS v6 is also a Towlr game. Good luck.\n");
    printf("\n");
    printf("IT is waiting for you. A mystery, waiting for\n");
    printf("you to unravel its... mysteries. It continually\n");
    printf("awaits a brilliant person to understand it.\n");
    printf("\n");
    printf("Press a key to begin.");
    while (1)
    {
          if (_kbhit())
          {
             _getch();
             break;
          }
    }

    PrintScreen(20);
    
    while (1)
    {
          gets(input);
          result = ProcessCommand(input);
          if (result == 2)
          {
             printf("Congratulations, you figured IT out!\n");
             printf("Cake in %i tries!\n", tries);
             break;
          }
          else if (result == -1)
             break;          
    }
    printf("Press a button to leave...\n");
    while (1)
    {
          if (_kbhit())
             break;
    }
}
