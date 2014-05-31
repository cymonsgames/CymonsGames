#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

extern void Game_Help(void), Game_Status(void);
extern void Game_Trade(void);
extern void Game_Save(void), Game_Load(void);

extern unsigned int price;

int main (int argc, char *args[])
{
	char buf[64];
	unsigned int start = time(NULL), spent;
	srand(time(NULL));
	price=22-(rand()%15);
	printf("\n");
	printf("************************************************\n");
	printf("Welcome to TAWS (Text Adventure Without Swords)!\n");
	printf("************************************************\n");
	while (1)
	{
		printf("\nPlease enter a command: ");
		scanf("%s", buf);
		if (!strcmp(buf, "help"))
			Game_Help();
		else if (!strcmp(buf, "status"))
			Game_Status();
		else if (!strcmp(buf, "trade"))
			Game_Trade();
		else if (!strcmp(buf, "save"))
			Game_Save();
		else if (!strcmp(buf, "load"))
			Game_Load();
		else if (!strcmp(buf, "TAWS"))
		{
			printf("TAWS is most commonly known as Text Adventure Without Swords.\n");
			printf("However, it has other meanings as well, including:\n");
			printf("Text And Wealth Simulation\n");
			printf("Torrential Admission of Watery Soup\n");
			printf("Timetable Antithesis Waking Somebody\n");
			printf("... or just TAWS.\n");
		}
		else if (!strcmp(buf, "quit") || !strcmp(buf, "exit"))
		{
			printf("Would you like to save your game first? ");
			scanf("%s", buf);
			if (tolower(buf[0]) == 'y')
				Game_Save();
			break;
		}
		else
			printf("Come on, type something real (try \'help\').\n");
	}
	spent = time(NULL)-start;
	printf("You spent %i minutes and %i seconds playing this game.\n", (spent/60), (spent%60));
	printf("Well, wasn't that fun?  See you next time!\n\n");
	return 0;
}
