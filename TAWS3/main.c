#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern void Game_New(void);
extern int Game_Load(void), Game_Loop(void);

void ZeroReturn (char *input) {
    char *c = strchr(input, '\n');
    if (c)
        *c = 0;
}

int main (int args, char *argc[]) {
    char buf[32];
    printf("\n");
    printf("*********************************************************\n");
    printf("Welcome to TAWS (Text Adventure Without Swords) volume 3!\n");
    printf("*********************************************************\n");
    printf("\n");

    srand(time(NULL));

    while (1) {
        while (1) {
            printf ("Commands:\n");
            printf ("new: Begin a new game.\n");
            printf ("load: Load a saved game.\n");
            printf ("exit: Exit TAWS v3, but why would you want that?\n");
            printf ("Please enter a command: ");
            fgets(buf, 31, stdin);
            ZeroReturn(buf);
            if (!strcmp(buf, "new") || !strcmp(buf, "New")) {
                Game_New();
                break;
            } else if (!strcmp(buf, "load") || !strcmp(buf, "Load")) {
                if (!Game_Load())
                    continue;
                else
                    break;
            } else if (!strcmp(buf, "exit") || !strcmp(buf, "quit")) {
                printf ("Well wasn't that fun? See you next time!\n");
                return 0;
            } else if (!strcmp(buf, "TAWS")) {
                printf("TAWS is most commonly known as Text Adventure Without Swords.\n");
                printf("However, it has other meanings as well, including:\n");
                printf("Terribly Amazing Walrus Strategy\n");
                printf("Torrential Admission of Watery Soup\n");
                printf("Timetable Antithesis Waking Somebody\n");
                printf("Top Ankle Wise Surrender\n");
                printf("... or just TAWS.\n");
            }
        }

        if (Game_Loop() == 0)
            break;
    }

    return 0;
}
