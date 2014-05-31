#include <iostream>
#include <conio.h>
#include <time.h>

using namespace std;

int main(int argc, char *argv[])
{
    int length, letter=0, score=0, last;
    char word[16];
    char vowels[8] = "aeiouy";
    char input[128];
    int total=0;
    bool paused = false;
    unsigned int startPause = 0;
    unsigned int totalPaused = 0;
    unsigned int game = time(NULL);
    unsigned int start = time(NULL);
   	printf("\n");
	printf("************************************************\n");
	printf("Welcome to TAWS (Text Adventure Without Swords)!\n");
	printf("************************************************\n"); 
    printf("Just type the words on screen as fast as possible.\n");
    printf("Score is based on how fast you type the word,\n");
    printf("compared to how long the word is.  Beware, you can\n");
    printf("lose points for going to slowly!\n");
    printf("To pause the game, hit the space bar.\n");
    printf("To quit, type 0.\n");
    printf("\n");
    srand(time(NULL));
    last = 0;
    length = 2+(rand()%13);
    for (int i=0; i < length; i++)
    {
        if (last > 1)
        {
            word[i] = vowels[rand()%6];
            last = 0;
        }
        else
        {
            word[i] = 'a'+rand()%('z'-'a');
            last++;
        }
        word[i+1] = '\0';
    }
    printf("Word: %s\n", word);
    while (input[letter-1] != '0')
    {
        if (_kbhit())
        {
           input[letter] = _getch();
           if (input[letter] == '\b')
           {
              input[letter] = '\0';
              if (letter > 0)
              {
                 letter--;
                 input[letter] = ' ';
                 printf("\r");
                 printf(input);
                 input[letter] = '\0';
                 printf("\r");
                 printf(input);
              }
           }
           else if (input[letter] == ' ') // space to pause
           {
                if (paused)
                {
                   paused = false;
                   printf("Game unpaused.\n");
                   totalPaused += time(NULL) - startPause;
                   
                   last = 0;
                   length = 2+(rand()%13);
                   for (int i=0; i < length; i++)
                   {
                       if (last > 1)
                       {
                           word[i] = vowels[rand()%6];
                           last = 0;
                       }
                       else
                       {
                           word[i] = 'a'+rand()%('z'-'a');
                           last++;
                       }
                       word[i+1] = '\0';
                   }
                   printf("Word: %s\n", word);
                   start = time(NULL);
                   
                   // reset the input
                   letter = 0;
                   for (int i=0; i < 128; i++)
                       input[i] = '\0';                   
                }
                else
                {
                    paused = true;
                    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nGame paused.\n");
                    startPause = time(NULL);
                }
           }
           else if (input[letter] >= 'a' && input[letter] <= 'z' || input[letter] == '0')
           {
               if (letter < 127)
               {
                  printf("%c", input[letter]);
                  letter++;
                  input[letter] = '\0';
               }
           }
        }
        
        if (!paused && !strcmp(input, word))
        {
           if ((time(NULL)-start) > length + 2)
               score -= 2;
           else
               score += length-(time(NULL)-start);
           printf("\n");
           printf("Score: %i\n", score);
           
           last = 0;
           length = 2+(rand()%13);
           for (int i=0; i < length; i++)
           {
               if (last > 1)
               {
                   word[i] = vowels[rand()%6];
                   last = 0;
               }
               else
               {
                   word[i] = 'a'+rand()%('z'-'a');
                   last++;
               }
               word[i+1] = '\0';
           }
           printf("Word: %s\n", word);
           start = time(NULL);
           
           total += length;
           
           // reset the input
           letter = 0;
           for (int i=0; i < 128; i++)
               input[i] = '\0';
        }
    }
    printf("\n\n");
    game += totalPaused; // fix total gametime calculations for being paused
    printf("Final score: %i points in %i seconds\n", score, time(NULL)-game);
    printf("%.2f letters per second\n", (float)total/(float)(time(NULL)-game));
    system("pause");
    return 0;
}
