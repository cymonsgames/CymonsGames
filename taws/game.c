#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RESET_AT 10000000
#define RESET_TO 10000

unsigned int credits=100;
unsigned int merchandise=10;
unsigned int price=15;
unsigned int stars=0;
char saveName[64]="\0";

void Game_Help (void)
{
	printf("The object of TAWS (Text Adventure Without Swords) is to accumulate\n");
	printf("wealth by selling merchandise when the price is high, and buying it\n");
	printf("when the price is low.  To get a new quote on the price, type status.\n");
	printf("-- Commands --\n");
	printf("status: Show current credits, merchandise and prices.\n");
	printf("trade: Trade merchandise for credits, or vice versa.\n");
	printf("load: Load a previously saved game.\n");
	printf("save: Save your game.\n");
	printf("help: Show this helpful information.\n");
	printf("exit: Exit the game, but why would you want that?\n");
	printf("--   Tips   --\n");
	printf("Careful with those status updates, they don't come cheap!\n");
}

void Game_CheckCredits(void)
{
	if (credits < RESET_AT)
		return;
	printf("\n");
	printf("Congratulations!\n");
	printf("You have accumulated over ten million credits!\n");
	printf("You will receive a star in exchange for each ten million!\n");
	printf("\n");
	while (credits >= RESET_AT)
	{
		credits = credits - RESET_AT;
		stars++;
	}
	credits = RESET_TO;
}

void Game_Status(void)
{
	price=22-(rand()%15);

	if (credits >= 2)
	{
		printf("Charged 2 credits for status update.\n");
		credits-=2;
	}

	Game_CheckCredits();
	printf("You have %u credits.\n", credits);
	printf("You have %u units of merchandise.\n", merchandise);
	printf("The current PPU (price per unit) of merchandise is $%u.\n", price);
	printf("The average PPU is $15.\n");
	if (!stars)
		printf("You have no stars.\n");
	else
		printf("You have %u stars.\n", stars);
}

void Game_Trade (void)
{
	int num;
	if (!credits && !merchandise)
	{
		printf("You have no credits and no merchandise. You have failed.\n");
		return;
	}
	printf("You have %u credits.\n", credits);
	printf("You have %u units of merchandise.\n", merchandise);
	printf("The current PPU (price per unit) of merchandise is $%u.\n", price);
	printf("Enter a negative number to sell, or a positive number to buy: ");
	scanf("%i", &num);
	if (num == 0)
		return;
	else if (num < 0 && -num > merchandise)
	{
		printf("You can't sell that many units of merchandise!\n");
		return;
	}
	else if (num > 0 && num*price > credits)
	{
		printf("You can't buy that many units of merchandise!\n");
		return;
	}
	credits -= (unsigned int)(price*num);
	merchandise += num;
	if (num < 0)
		printf("Sold %u units of merchandise for $%u.\n", -num, -num*price);
	else if (num > 0)
		printf("Bought %u units of merchandise for $%u.\n", num, num*price);
	Game_CheckCredits();
}

void Game_Load (void)
{
	char buf[64];
	char filename[80];
	FILE *f;
	time_t timeData;
	struct tm * dateInfo;
	// ask the player for a filename
	printf("Enter your save name please: ");
	scanf("%s", buf);
	sprintf(filename, "GameData/%s.sav", buf);
	f = fopen(filename, "rb");
	if (f)
	{
		fread(&credits, sizeof(unsigned int), 1, f);
		fread(&merchandise, sizeof(unsigned int), 1, f);
		fread(&stars, sizeof(unsigned int), 1, f);
		fread(&timeData, sizeof(time_t), 1, f);
		fclose(f);
		printf("Game loaded successfully.\n");
		dateInfo = localtime(&timeData);
		printf("This game was last saved at %s", asctime(dateInfo));
		sprintf(saveName, "%s", buf);
	}
	else
		printf("Save file %s not found.\n", buf);
}

void Game_Save (void)
{
	char buf[64];
	char filename[80];
	time_t dateInfo;
	FILE *f;
	// ask the player for a filename
	if (saveName[0] != '\0')
	{
		printf("Do you want to save your game as \"%s\"? ", saveName);
		scanf("%s", buf);
		if (tolower(buf[0]) == 'y')
			sprintf(filename, "GameData/%s.sav", saveName);
		else
		{
			printf("Enter a save name please: ");
			scanf("%s", buf);
			sprintf(filename, "GameData/%s.sav", buf);
		}
	}
	else
	{
		printf("Enter a save name please: ");
		scanf("%s", buf);
		sprintf(filename, "GameData/%s.sav", buf);
	}
	f = fopen(filename, "wb");
	if (!f)
	{
#ifdef WIN32
		if (mkdir("GameData") == 0)
#else
		if (mkdir("GameData", 0777) == 0)
#endif
		{
			f = fopen(filename, "wb");
			if (!f)
			{
				printf("Save failed.\n");
				return;
			}
		}
		else
		{
			printf("Save failed.\n");
			return;
		}
	}
	if (f)
	{
		fwrite(&credits, sizeof(unsigned int), 1, f);
		fwrite(&merchandise, sizeof(unsigned int), 1, f);
		fwrite(&stars, sizeof(unsigned int), 1, f);
		dateInfo = time(NULL);
		fwrite(&dateInfo, sizeof(time_t), 1, f);
		fclose(f);
		printf("Game saved successfully.\n");
	}
}
