/* ------------------------------------------------------------------------ *
 * Taipan version 0.9
 * A text/ncurses game for Linux.
 *
 * Created by:
 *   Art Canfil
 *
 * Programmed by:
 *   Jay Link <jlink@gmail.com>
 *
 * Apple ][ program coded by:
 *   Ronald J. Berg
 * ------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>

#define GENERIC 1
#define LI_YUEN 2

#ifdef PDCURSES
#define usleep(c) napms((c)/1000)
#endif

void splash_intro(void);
int get_one(void);
long get_num(int maxlen);
void name_firm(void);
void cash_or_guns(void);
void set_prices(void);
void port_stats(void);
int port_choices(void);
void new_ship(void);
void new_gun(void);
void li_yuen_extortion(void);
void elder_brother_wu(void);
void good_prices(void);
void buy(void);
void sell(void);
void visit_bank(void);
void transfer(void);
void quit(void);
void overload(void);
void fancy_numbers(float num, char *fancy);
int sea_battle(int id, int num_ships);
void draw_lorcha(int x, int y);
void clear_lorcha(int x, int y);
void draw_blast(int x, int y);
void sink_lorcha(int x, int y);
void fight_stats(int ships, int orders);
void mchenry(void);
void retire(void);
void final_stats(void);

char    firm[23],
        fancy_num[13];

char    *item[] = { "Opium", "Silk", "Arms", "General Cargo" };

char    *location[] = { "At sea", "Hong Kong", "Shanghai", "Nagasaki",
                        "Saigon", "Manila", "Singapore", "Batavia" };

char    *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

char    *st[] = { "Critical", "  Poor", "  Fair",
                  "  Good", " Prime", "Perfect" };

float   cash         = 0,
        bank         = 0,
        debt         = 0,
        booty        = 0,
        ec           = 20,
        ed           = .5;

long    price[4];

int     base_price[4][8] = { {1000, 11, 16, 15, 14, 12, 10, 13},
                             {100,  11, 14, 15, 16, 10, 13, 12},
                             {10,   12, 16, 10, 11, 13, 14, 15},
                             {1,    10, 11, 12, 13, 14, 15, 16} };

int     hkw_[4],
        hold_[4];

int     hold         = 0,
        capacity     = 60,
        guns         = 0,
        bp           = 0,
        damage       = 0,
        month        = 1,
        year         = 1860,
        li           = 0,
        port         = 1,
        wu_warn      = 0,
        wu_bailout   = 0;

int main(void)
{
   int choice;

   srand(getpid());

   initscr();

   cbreak();
   noecho();
#ifdef PDCURSES
   PDC_set_title("Cymon's Games - Taipan");
#endif

   splash_intro();
   name_firm();
   cash_or_guns();
   set_prices();

   for (;;)
   {
      choice = 0;

      port_stats();

      if ((port == 1) && (li == 0) && (cash > 0))
      {
         li_yuen_extortion();
      }

      if ((port == 1) && (damage > 0))
      {
         mchenry();
      }

      if ((port == 1) && (debt >= 10000) && (wu_warn == 0))
      {
         int braves = rand()%100 + 50;

         move(16, 0);
         clrtobot();
         printw("Comprador's Report\n\n");
         printw("Elder Brother Wu has sent %d braves\n", braves);
         printw("to escort you to the Wu mansion, Taipan.\n");

         refresh();
         timeout(3000);
         getch();
         timeout(-1);

         move(18, 0);
         clrtobot();
         printw("Elder Brother Wu reminds you of the\n");
         printw("Confucian ideal of personal worthiness,\n");
         printw("and how this applies to paying one's\n");
         printw("debts.\n");

         refresh();
         timeout(3000);
         getch();
         timeout(-1);

         move(18, 0);
         clrtobot();
         printw("He is reminded of a fabled barbarian\n");
         printw("who came to a bad end, after not caring\n");
         printw("for his obligations.\n\n");
         printw("He hopes no such fate awaits you, his\n");
         printw("friend, Taipan.\n");

         refresh();
         timeout(5000);
         getch();
         timeout(-1);

         wu_warn = 1;
      }

      if (port == 1)
      {
         elder_brother_wu();
      }

      if (rand()%4 == 0)
      {
         if (rand()%2 == 0)
         {
            new_ship();
         } else if (guns < 1000) {
            new_gun();
         }
      }

      if ((port != 1) && (rand()%18 == 0) && (hold_[0] > 0))
      {
         float fine = ((cash / 1.8) * ((float) rand() / RAND_MAX)) + 1;

         hold += hold_[0];
         hold_[0] = 0;
         cash -= fine;

         port_stats();

         fancy_numbers(fine, fancy_num);
         move(16, 0);
         clrtobot();
         printw("Comprador's Report\n\n");
         printw("Bad Joss!!\n");
         printw("The local authorities have seized your\n");
         printw("Opium cargo and have also fined you\n");
         printw("%s, Taipan!\n", fancy_num);

         refresh();
         timeout(5000);
         getch();
         timeout(-1);
      }

      if ((rand()%50 == 0) &&
          ((hkw_[0] + hkw_[1] + hkw_[2] + hkw_[3]) > 0))
      {
         int i;

         for (i = 0; i < 4; i++)
         {
            hkw_[i] = ((hkw_[i] / 1.8) * ((float) rand() / RAND_MAX));
         }

         port_stats();

         move(16, 0);
         clrtobot();
         printw("Comprador's Report\n\n");
         printw("Messenger reports large theft\n");
         printw("from warehouse, Taipan.\n");

         refresh();
         timeout(5000);
         getch();
         timeout(-1);
      }

      if (rand()%20 == 0)
      {
         if (li > 0) { li++; }
         if (li == 4) { li = 0; }
      }

      if ((port != 1) && (li == 0) && (rand()%4 != 0))
      {
         move(16, 0);
         clrtobot();
         printw("Comprador's Report\n\n");
         printw("Li Yuen has sent a Lieutenant,\n");
         printw("Taipan.  He says his admiral wishes\n");
         printw("to see you in Hong Kong, posthaste!\n");

         refresh();
         timeout(3000);
         getch();
         timeout(-1);
      }

      if (rand()%9 == 0)
      {
         good_prices();
      }

      if ((cash > 25000) && (rand()%20 == 0))
      {
         float robbed = ((cash / 1.4) * ((float) rand() / RAND_MAX));

         cash -= robbed;
         port_stats();

         fancy_numbers(robbed, fancy_num);
         move(16, 0);
         clrtobot();
         printw("Comprador's Report\n\n");
         printw("Bad Joss!!\n");
         printw("You've been beaten up and\n");
         printw("robbed of %s in cash, Taipan!!\n", fancy_num);

         refresh();
         timeout(5000);
         getch();
         timeout(-1);
      }

      for (;;)
      {
         while ((choice != 'Q') && (choice != 'q'))
         {
            switch (choice = port_choices())
            {
               case 'B':
               case 'b':
                  buy();
                  break;

               case 'S':
               case 's':
                  sell();
                  break;

               case 'V':
               case 'v':
                  visit_bank();
                  break;

               case 'T':
               case 't':
                  transfer();
                  break;

               case 'R':
               case 'r':
                  retire();
            }

            port_stats();
         }

         choice = 0;
         if (hold >= 0)
         {
            quit();
            break;
         } else {
            overload();
         }
      }
   }

   clear();
   refresh();
   nocbreak();
   endwin();

   return EXIT_SUCCESS;
}

void splash_intro(void)
{
   flushinp();
   clear();
   printw("\n");
   printw("         _____  _    ___ ____   _    _   _               ===============\n");
   printw("        |_   _|/ \\  |_ _|  _ \\ / \\  | \\ | |              Created by:\n");
   printw("          | | / _ \\  | || |_) / _ \\ |  \\| |                 Art Canfil\n");
   printw("          | |/ ___ \\ | ||  __/ ___ \\| |\\  |\n");
   printw("          |_/_/   \\_\\___|_| /_/   \\_\\_| \\_|              ===============\n");
   printw("                                                         Programmed by:\n");
   printw("   A game based on the China trade of the 1800's            Jay Link\n");
   printw("\n");
   printw("                      ~~|     ,                          jaylink1971\n");
   printw("                       ,|`-._/|                               @gmail.com\n");
   printw("                     .' |   /||\\                         ===============\n");
   printw("                   .'   | ./ ||`\\                         TRS80 version \n");
   printw("                  / `-. |/._ ||  \\                              by\n");
   printw("                 /     `||  `|;-._\\                         Art Canfil\n");
   printw("                 |      ||   ||   \\\n");
   printw("~^~_-~^~=~^~~^= /       ||   ||__  \\~^=~^~-~^~_~^~=      ===============\n");
   printw(" ~=~^~ _~^~ =~ `--------|`---||  `\"-`___~~^~ =_~^=        Press ");
   attrset(A_REVERSE);
   printw("ANY");
   attrset(A_NORMAL);
   printw(" key\n");
   printw("~ ~^~=~^_~^~ =~ \\~~~~~~~'~~~~'~~~~/~~`` ~=~^~ ~^=           to start.\n");
   printw(" ~^=~^~_~-=~^~ ^ `--------------'~^~=~^~_~^=~^~=~\n");
   curs_set(0);
   refresh();

   getch();
   curs_set(1);
   return;
}

int get_one(void)
{
   int input,
       choice = 0,
       character = 0;

   while ((input = getch()) != '\n')
   {
      if (((input == 8) || (input == 127)) && (character == 0))
      {
         refresh();
      } else if ((input == 8) || (input == 127)) {
         printw("%c", 8);
         printw(" ");
         printw("%c", 8);
         character--;
         refresh();
      } else if (character >= 1) {
         refresh();
      } else if (input == '\33') {
         flushinp();
         refresh();
      } else {
         printw("%c", input);
         choice = input;
         character++;
         refresh();
      }
   }

   return choice;
}

long get_num(int maxlen)
{
   char number[maxlen + 1];

   int  input,
        character = 0;

   long amount;

   while ((input = getch()) != '\n')
   {
      if (((input == 8) || (input == 127)) && (character == 0))
      {
         refresh();
      } else if ((input == 8) || (input == 127)) {
         printw("%c", 8);
         printw(" ");
         printw("%c", 8);
         number[character] = '\0';
         character--;
         refresh();
      } else if (character >= maxlen) {
         refresh();
      } else if (input == '\33') {
         flushinp();
         refresh();
      } else if (((input == 'A') || (input == 'a')) &&
                 (character == 0) && (maxlen > 1)) {
         printw("%c", input);
         number[character] = input;
         character++;
         refresh();
      } else if ((input < 48) || (input > 57)) {
         refresh();
      } else {
         printw("%c", input);
         number[character] = input;
         character++;
         refresh();
      }
   }

   number[character] = '\0';
   if ((strcmp(number, "A") == 0) || (strcmp(number, "a") == 0))
   {
      amount = -1;
   } else {
      amount = strtol(number, (char **)NULL, 10);
   }

   return amount;
}

void name_firm(void)
{
   int  input,
        character = 0;

   clear();
   move (7, 0);
   printw(" _______________________________________\n");
   printw("|     Taipan,                           |\n");
   printw("|                                       |\n");
   printw("| What will you name your               |\n");
   printw("|                                       |\n");
   printw("|     Firm:                             |\n");
   printw("|           ----------------------      |\n");
   printw("|_______________________________________|\n");

   move(12, 12);
   refresh();

   while (((input = getch()) != '\n') && (character < 22))
   {
      if (((input == 8) || (input == 127)) && (character == 0))
      {
         refresh();
      } else if ((input == 8) || (input == 127)) {
         printw("%c", 8);
         printw(" ");
         printw("%c", 8);
         firm[character] = '\0';
         character--;
         refresh();
      } else if (input == '\33') {
         flushinp();
         refresh();
      } else {
         printw("%c", input);
         firm[character] = input;
         character++;
         refresh();
      }
   }

   firm[character] = '\0';

   return;
}

void cash_or_guns(void)
{
   int choice = 0;

   clear();
   move (5, 0);
   printw("Do you want to start . . .\n\n");
   printw("  1) With cash (and a debt)\n\n");
   printw("                >> or <<\n\n");
   printw("  2) With five guns and no cash\n");
   printw("                (But no debt!)\n");

   while ((choice != '1') && (choice != '2'))
   {
      move (15, 0);
      clrtoeol();
      printw("          ?");
      refresh();
      choice = get_one();
   }

   if (choice == '1')
   {
      cash = 400;
      debt = 5000;
      hold = 60;
      guns = 0;
      li = 0;
      bp = 10;
   } else {
      cash = 0;
      debt = 0;
      hold = 10;
      guns = 5;
      li = 1;
      bp = 7;
   }

   return;
}

void set_prices(void)
{
   price[0] = base_price[0][port] / 2 * (rand()%3 + 1) * base_price[0][0];
   price[1] = base_price[1][port] / 2 * (rand()%3 + 1) * base_price[1][0];
   price[2] = base_price[2][port] / 2 * (rand()%3 + 1) * base_price[2][0];
   price[3] = base_price[3][port] / 2 * (rand()%3 + 1) * base_price[3][0];
   return;
}

void port_stats(void)
{
   int  in_use,
        status = 100 - (((float) damage / capacity) * 100),
        spacer,
        i;

   clear();
   spacer = 12 - (strlen(firm) / 2);
   for (i = 1; i <= spacer; i++)
   {
      printw(" ");
   }
   printw("Firm: %s, Hong Kong\n", firm);
   printw(" ______________________________________\n");
   printw("|Hong Kong Warehouse                   |     Date\n");
   printw("|   Opium           In Use:            |\n");
   printw("|   Silk                               |\n");
   printw("|   Arms            Vacant:            |   Location\n");
   printw("|   General                            |\n");
   printw("|______________________________________|\n");
   printw("|Hold               Guns               |     Debt\n");
   printw("|   Opium                              |\n");
   printw("|   Silk                               |\n");
   printw("|   Arms                               |  Ship Status\n");
   printw("|   General                            |\n");
   printw("|______________________________________|\n");
   printw("Cash:               Bank:\n");
   printw("________________________________________\n");

   move(3, 12);
   printw("%d", hkw_[0]);
   move(4, 12);
   printw("%d", hkw_[1]);
   move(5, 12);
   printw("%d", hkw_[2]);
   move(6, 12);
   printw("%d", hkw_[3]);
   move(8, 6);
   if (hold >= 0)
   {
      printw("%d", hold);
   } else {
      attrset(A_REVERSE);
      printw("Overload");
      attrset(A_NORMAL);
   }
   move(9, 12);
   printw("%d", hold_[0]);
   move(10, 12);
   printw("%d", hold_[1]);
   move(11, 12);
   printw("%d", hold_[2]);
   move(12, 12);
   printw("%d", hold_[3]);

   move(14, 5);
   fancy_numbers(cash, fancy_num);
   printw("%s", fancy_num);

   in_use = hkw_[0] + hkw_[1] + hkw_[2] + hkw_[3];
   move(4, 21);
   printw("%d", in_use);
   move(6, 21);
   printw("%d", (10000 - in_use));

   move(8, 25);
   printw("%d", guns);

   move(14, 25);
   fancy_numbers(bank, fancy_num);
   printw("%s", fancy_num);

   move(3, 42);
   printw("15 ");
   attrset(A_REVERSE);
   printw("%s", months[month - 1]);
   attrset(A_NORMAL);
   printw(" %d", year);

   move(6, 43);
   spacer = (9 - strlen(location[port])) / 2;
   for (i = 1; i <= spacer; i++)
   {
      printw(" ");
   }
   attrset(A_REVERSE);
   printw("%s", location[port]);
   attrset(A_NORMAL);

   move(9, 41);
   fancy_numbers(debt, fancy_num);
   spacer = (12 - strlen(fancy_num)) / 2;
   for (i = 1; i <= spacer; i++)
   {
      printw(" ");
   }
   attrset(A_REVERSE);
   printw("%s", fancy_num);
   attrset(A_NORMAL);

   i = status / 20;
   if (i < 2)
   {
      attrset(A_REVERSE);
      move(12, 51);
      printw("  ");
   }
   move(12, 42);
   printw("%s:%d", st[i], status);
   attrset(A_NORMAL);
}

int port_choices(void)
{
   int choice = 0;

   move(16, 0);
   clrtobot();
   printw("Comprador's Report\n\n");
   printw("Taipan, present prices per unit here are\n");
   printw("   Opium:          Silk:\n");
   printw("   Arms:           General:\n");
   move(19, 11);
   printw("%ld", price[0]);
   move(19, 29);
   printw("%ld", price[1]);
   move(20, 11);
   printw("%ld", price[2]);
   move(20, 29);
   printw("%ld", price[3]);

   for (;;)
   {
      move (22, 0);
      clrtobot();

      if (port == 1)
      {
         if ((cash + bank) >= 1000000)
         {
            printw("Shall I Buy, Sell, Visit bank, Transfer\n");
            printw("cargo, Quit trading, or Retire? ");
            refresh();

            choice = get_one();
            if ((choice == 'B') || (choice == 'b') ||
                (choice == 'S') || (choice == 's') ||
                (choice == 'V') || (choice == 'v') ||
                (choice == 'T') || (choice == 't') ||
                (choice == 'Q') || (choice == 'q') ||
                (choice == 'R') || (choice == 'r'))
            {
               break;
            }
         } else {
            printw("Shall I Buy, Sell, Visit bank, Transfer\n");
            printw("cargo, or Quit trading? ");
            refresh();

            choice = get_one();
            if ((choice == 'B') || (choice == 'b') ||
                (choice == 'S') || (choice == 's') ||
                (choice == 'V') || (choice == 'v') ||
                (choice == 'T') || (choice == 't') ||
                (choice == 'Q') || (choice == 'q'))
            {
               break;
            }
         }
      } else {
         printw("Shall I Buy, Sell, or Quit trading? ");
         refresh();

         choice = get_one();
         if ((choice == 'B') || (choice == 'b') ||
             (choice == 'S') || (choice == 's') ||
             (choice == 'Q') || (choice == 'q'))
         {
            break;
         }
      }
   }

   return choice;
}

void buy(void)
{
   char space[5];

   int  choice = 0;

   long afford,
        amount;

   for (;;)
   {
      move(22, 0);
      clrtobot();

      printw("What do you wish me to buy, Taipan? ");
      refresh();

      choice = get_one();
      if ((choice == 'O') || (choice == 'o'))
      {
         choice = 0;
         break;
      } else if ((choice == 'S') || (choice == 's')) {
         choice = 1;
         break;
      } else if ((choice == 'A') || (choice == 'a')) {
         choice = 2;
         break;
      } else if ((choice == 'G') || (choice == 'g')) {
         choice = 3;
         break;
      }
   }

   for (;;)
   {
      move(21, 42);
      clrtobot();

      afford = cash / price[choice];
      attrset(A_REVERSE);
      printw(" You can ");
      attrset(A_NORMAL);
      move(22, 0);
      printw("How much %s shall", item[choice]);
      move(22, 42);
      attrset(A_REVERSE);
      printw("  afford ");
      move(23, 42);
      printw("         ");
      move(23, 42);
      if (afford < 100)
      {
         strcpy(space, "    ");
      } else if (afford < 10000) {
         strcpy(space, "   ");
      } else if (afford < 1000000) {
         strcpy(space, "  ");
      } else if (afford < 100000000) {
         strcpy(space, " ");
      } else {
         strcpy(space, "");
      }
      printw("%s%ld", space, afford);
      attrset(A_NORMAL);

      move(23, 0);
      printw("I buy, Taipan: ");
      refresh();

      amount = get_num(9);
      if (amount == -1)
      {
         amount = afford;
      }
      if (amount <= afford)
      {
         break;
      }
   }

   cash -= (amount * price[choice]);
   hold_[choice] += amount;
   hold -= amount;

   return;
}

void sell(void)
{
   int  choice = 0;

   long amount;

   for (;;)
   {
      move(22, 0);
      clrtobot();

      printw("What do you wish me to sell, Taipan? ");
      refresh();

      choice = get_one();
      if ((choice == 'O') || (choice == 'o'))
      {
         choice = 0;
         break;
      } else if ((choice == 'S') || (choice == 's')) {
         choice = 1;
         break;
      } else if ((choice == 'A') || (choice == 'a')) {
         choice = 2;
         break;
      } else if ((choice == 'G') || (choice == 'g')) {
         choice = 3;
         break;
      }
   }

   for (;;)
   {
      move(22, 0);
      clrtobot();

      printw("How much %s shall\n", item[choice]);
      printw("I sell, Taipan: ");
      refresh();

      amount = get_num(9);

      if (amount == -1)
      {
         amount = hold_[choice];
      }
      if (hold_[choice] >= amount)
      {
         hold_[choice] -= amount;
         break;
      }
   }

   cash += (amount * price[choice]);
   hold += amount;

   return;
}

void visit_bank(void)
{
   long amount = 0;

   for (;;)
   {
      move(16, 0);
      clrtobot();
      printw("Comprador's Report\n\n");
      printw("How much will you deposit? ");
      refresh();

      amount = get_num(9);
      if (amount == -1)
      {
         amount = cash;
      }
      if (amount <= cash)
      {
         cash -= amount;
         bank += amount;
         break;
      } else {
         move(18, 0);
         clrtobot();
         fancy_numbers(cash, fancy_num);
         printw("Taipan, you only have %s\n", fancy_num);
         printw("in cash.\n");

         refresh();
         timeout(5000);
         getch();
         timeout(-1);
      }
   }
   port_stats();

   for (;;)
   {
      move(16, 0);
      clrtobot();
      printw("Comprador's Report\n\n");
      printw("How much will you withdraw? ");
      refresh();

      amount = get_num(9);
      if (amount == -1)
      {
         amount = bank;
      }
      if (amount <= bank)
      {
         cash += amount;
         bank -= amount;
         break;
      } else {
         fancy_numbers(cash, fancy_num);
         printw("Taipan, you only have %s\n", fancy_num);
         printw("in the bank.");

         refresh();
         timeout(5000);
         getch();
         timeout(-1);
      }
   }
   port_stats();

   return;
}

void transfer(void)
{
   int i, in_use;

   long amount = 0;

   if ((hkw_[0] == 0) && (hold_[0] == 0) &&
       (hkw_[1] == 0) && (hold_[1] == 0) &&
       (hkw_[2] == 0) && (hold_[2] == 0) &&
       (hkw_[3] == 0) && (hold_[3] == 0))
   {
      move(22, 0);
      clrtobot();
      printw("You have no cargo, Taipan.\n");

      refresh();
      timeout(5000);
      getch();
      timeout(-1);
      return;
   }

   for (i = 0; i < 4; i++)
   {
      if (hold_[i] > 0)
      {
         for (;;)
         {
            move(16, 0);
            clrtobot();
            printw("Comprador's Report\n\n");
            printw("How much %s shall I move\n", item[i]);
            printw("to the warehouse, Taipan? ");
            refresh();

            amount = get_num(9);
            if (amount == -1)
            {
               amount = hold_[i];
            }
            if (amount <= hold_[i])
            {
               in_use = hkw_[0] + hkw_[1] + hkw_[2] + hkw_[3];
               if ((in_use + amount) <= 10000)
               {
                  hold_[i] -= amount;
                  hkw_[i] += amount;
                  hold += amount;
                  break;
               } else if (in_use == 10000) {
                  move (21, 0);
                  printw("Your warehouse is full, Taipan!");
               } else {
                  move (21, 0);
                  printw("Your warehouse will only hold an\n");
                  printw("additional %d, Taipan!", (10000 - in_use));

                  refresh();
                  timeout(5000);
                  getch();
                  timeout(-1);
               }
            } else {
               move(18, 0);
               clrtobot();
               printw("You have only %d, Taipan.\n", hold_[i]);

               refresh();
               timeout(5000);
               getch();
               timeout(-1);
            }
         }
         port_stats();
      }

      if (hkw_[i] > 0)
      {
         for (;;)
         {
            move(16, 0);
            clrtobot();
            printw("Comprador's Report\n\n");
            printw("How much %s shall I move\n", item[i]);
            printw("aboard ship, Taipan? ");
            refresh();

            amount = get_num(9);
            if (amount == -1)
            {
               amount = hkw_[i];
            }
            if (amount <= hkw_[i])
            {
               hold_[i] += amount;
               hkw_[i] -= amount;
               hold -= amount;
               break;
            } else {
               move(18, 0);
               clrtobot();
               printw("You have only %d, Taipan.\n", hkw_[i]);

               refresh();
               timeout(5000);
               getch();
               timeout(-1);
            }
         }
         port_stats();
      }
   }

   return;
}

void quit(void)
{
   int choice = 0,
       result = 0;

   move(16, 0);
   clrtobot();
   printw("Comprador's Report\n\n");
   printw("Taipan, do you wish me to go to:\n");
   printw("1) Hong Kong, 2) Shanghai, 3) Nagasaki,\n");
   printw("4) Saigon, 5) Manila, 6) Singapore, or\n");
   printw("7) Batavia ? ");
   refresh();

   for (;;)
   {
      move(21, 13);
      clrtobot();

      choice = get_num(1);

      if (choice == port)
      {
         printw("\n\nYou're already here, Taipan.");
         refresh();
         timeout(5000);
         getch();
         timeout(-1);
      } else if ((choice >= 1) && (choice <= 7)) {
         port = choice;
         break;
      }
   }

   move(6, 43);
   printw(" ");
   attrset(A_REVERSE);
   printw("%s", location[0]);
   attrset(A_NORMAL);
   printw("  ");

   move(16, 0);
   clrtobot();
   printw("  Captain's Report\n\n");

   if (rand()%bp == 0)
   {
      int num_ships = rand()%((capacity / 10) + guns) + 1;

      if (num_ships > 9999)
      {
         num_ships = 9999;
      }
      printw("%d hostile ships approaching, Taipan!\n", num_ships);
      refresh();

      timeout(3000);
      getch();
      timeout(-1);

      result = sea_battle(GENERIC, num_ships);
   }

   if (result == 2)
   {
      port_stats();
      move(6, 43);
      printw(" ");
      attrset(A_REVERSE);
      printw("%s", location[0]);
      attrset(A_NORMAL);
      printw("  ");

      move(16, 0);
      clrtobot();
      printw("  Captain's Report\n\n");
      printw("Li Yuen's fleet drove them off!");
      refresh();

      timeout(3000);
      getch();
      timeout(-1);
   }

   if (((result == 0) && (rand()%(4 + (8 * li))) == 0) || (result == 2))
   {
      move(18, 0);
      clrtobot();
      printw("Li Yuen's pirates, Taipan!!\n\n");
      refresh();

      timeout(3000);
      getch();
      timeout(-1);

      if (li > 0)
      {
         printw("Good joss!! They let us be!!\n");
         refresh();

         timeout(3000);
         getch();
         timeout(-1);

         return;
      } else {
         int num_ships = rand()%((capacity / 5) + guns) + 5;

         printw("%d ships of Li Yuen's pirate\n", num_ships);
         printw("fleet, Taipan!!\n");
         refresh();

         timeout(3000);
         getch();
         timeout(-1);

         sea_battle(LI_YUEN, num_ships);
      }
   }

   if (result > 0)
   {
      port_stats();
      move(6, 43);
      printw(" ");
      attrset(A_REVERSE);
      printw("%s", location[0]);
      attrset(A_NORMAL);
      printw("  ");

      move(16, 0);
      clrtobot();
      printw("  Captain's Report\n\n");
      if (result == 1)
      {
         fancy_numbers(booty, fancy_num);
         printw("We captured some booty.\n");
         printw("It's worth %s!", fancy_num);
         cash += booty;
      } else if (result == 3) {
         printw("We made it!");
      } else {
         printw("The buggers got us, Taipan!!!\n");
         printw("It's all over, now!!!");
         refresh();

         timeout(5000);
         getch();
         timeout(-1);

         final_stats();
         return;
      }

      refresh();
      timeout(3000);
      getch();
      timeout(-1);
   }

   if (rand()%10 == 0)
   {
      move(18, 0);
      clrtobot();
      printw("Storm, Taipan!!\n\n");
      refresh();
      timeout(3000);
      getch();
      timeout(-1);

      if (rand()%30 == 0)
      {
         printw("   I think we're going down!!\n\n");
         refresh();
         timeout(3000);
         getch();
         timeout(-1);

         if (((damage / capacity * 3) * ((float) rand() / RAND_MAX)) >= 1)
         {
            printw("We're going down, Taipan!!\n");
            refresh();
            timeout(5000);
            getch();
            timeout(-1);

            final_stats();
         }
      }

      printw("    We made it!!\n\n");
      refresh();
      timeout(3000);
      getch();
      timeout(-1);

      if (rand()%3 == 0)
      {
         int orig = port;

         while (port == orig)
         {
            port = rand()%7 + 1;
         }

         move(18, 0);
         clrtobot();
         printw("We've been blown off course\n");
         printw("to %s", location[port]);
         refresh();
         timeout(3000);
         getch();
         timeout(-1);
      }
   }

   month++;
   if (month == 13)
   {
      month = 1;
      year++;
      ec += 10;
      ed += .5;
   }

   debt = debt + (debt * .1);
   bank = bank + (bank * .005);
   set_prices();

   move(18, 0);
   clrtobot();
   printw("Arriving at %s...", location[port]);
   refresh();
   timeout(3000);
   getch();
   timeout(-1);

   return;
}

void li_yuen_extortion(void)
{
   int time = ((year - 1860) * 12) + month,
       choice = 0;

   float i = 1.8,
         j = 0,
         amount = 0;

   if (time > 12)
   {
      j = rand()%(1000 * time) + (1000 * time);
      i = 1;
   }

   amount = ((cash / i) * ((float) rand() / RAND_MAX)) + j;

   fancy_numbers(amount, fancy_num);

   move(16, 0);
   clrtobot();
   printw("Comprador's Report\n\n");
   printw("Li Yuen asks %s in donation\n", fancy_num);
   printw("to the temple of Tin Hau, the Sea\n");

   while ((choice != 'Y') && (choice != 'y') &&
          (choice != 'N') && (choice != 'n'))
   {
      move (20, 0);
      clrtoeol();
      printw("Goddess.  Will you pay? ");
      refresh();
      choice = get_one();
   }

   if ((choice == 'Y') || (choice == 'y'))
   {
      if (amount <= cash)
      {
         cash -= amount;
         li = 1;
      } else {
         move (18, 0);
         clrtobot();
         printw("Taipan, you do not have enough cash!!\n\n");
         refresh();

         timeout(3000);
         getch();
         timeout(-1);

         printw("Do you want Elder Brother Wu to make up\n");
         printw("the difference for you? ");
         choice = 0;
         while ((choice != 'Y') && (choice != 'y') &&
                (choice != 'N') && (choice != 'n'))
         {
            choice = get_one();
         }

         if ((choice == 'Y') || (choice == 'y'))
         {
            amount -= cash;
            debt += amount;
            cash = 0;
            li = 1;

            move (18, 0);
            clrtobot();
            printw("Elder Brother has given Li Yuen the\n");
            printw("difference between what he wanted and\n");
            printw("your cash on hand and added the same\n");
            printw("amount to your debt.\n");

            refresh();
            timeout(5000);
            getch();
            timeout(-1);
         } else {
            cash = 0;

            printw("Very well. Elder Brother Wu will not pay\n");
            printw("Li Yuen the difference.  I would be very\n");
            printw("wary of pirates if I were you, Taipan.\n");

            refresh();
            timeout(5000);
            getch();
            timeout(-1);
         }
      }
   }

   port_stats();

   return;
}

void elder_brother_wu(void)
{
   int  choice = 0;

   long wu = 0;

   move(16, 0);
   clrtobot();
   printw("Comprador's Report\n\n");
   printw("Do you have business with Elder Brother\n");
   printw("Wu, the moneylender? ");

   for (;;)
   {
      move (19, 21);
      clrtoeol();
      refresh();

      choice = get_one();
      if ((choice == 'N') || (choice == 'n'))
      {
         break;
      } else if ((choice == 'Y') || (choice == 'y')) {
         if (((int)cash == 0) && ((int)bank == 0) && (guns == 0) &&
             (hold_[0] == 0) && (hkw_[0] == 0) &&
             (hold_[1] == 0) && (hkw_[1] == 0) &&
             (hold_[2] == 0) && (hkw_[2] == 0) &&
             (hold_[3] == 0) && (hkw_[3] == 0))
         {
            int i = rand()%1500 + 500,
                j;

            wu_bailout++;
            j = rand()%2000 * wu_bailout + 1500;

            for (;;)
            {
               move(16, 0);
               clrtobot();
               printw("Comprador's Report\n\n");
               printw("Elder Brother is aware of your plight,\n");
               printw("Taipan.  He is willing to loan you an\n");
               printw("additional %d if you will pay back\n", i);
               printw("%d. Are you willing, Taipan? ", j);
               refresh();

               choice = get_one();
               if ((choice == 'N') || (choice == 'n'))
               {
                  move(16, 0);
                  clrtobot();
                  printw("Comprador's Report\n\n");
                  printw("Very well, Taipan, the game is over!\n");

                  refresh();
                  timeout(5000);
                  getch();
                  timeout(-1);

                  final_stats();
               } else if ((choice == 'Y') || (choice == 'y')) {
                  cash += i;
                  debt += j;
                  port_stats();

                  move(16, 0);
                  clrtobot();
                  printw("Comprador's Report\n\n");
                  printw("Very well, Taipan.  Good joss!!\n");

                  refresh();
                  timeout(5000);
                  getch();
                  timeout(-1);

                  return;
               }
            }
         } else if ((cash > 0) && (debt != 0)) {
            for (;;)
            {
               move(16, 0);
               clrtobot();
               printw("Comprador's Report\n\n");
               printw("How much do you wish to repay\n");
               printw("him? ");
               refresh();

               wu = get_num(9);
               if (wu == -1)
               {
                  wu = cash;
               }
               if (wu <= cash)
               {
                  cash -= wu;
                  if ((wu > debt) && (debt > 0))
                  {
                     debt -= (wu + 1);
                  } else {
                     debt -= wu;
                  }
                  break;
               } else {
                  move(18, 0);
                  clrtobot();
                  fancy_numbers(cash, fancy_num);
                  printw("Taipan, you only have %s\n", fancy_num);
                  printw("in cash.\n");

                  refresh();
                  timeout(5000);
                  getch();
                  timeout(-1);
               }
            }
         }
         port_stats();

         for (;;)
         {
            move(16, 0);
            clrtobot();
            printw("Comprador's Report\n\n");
            printw("How much do you wish to\n");
            printw("borrow? ");
            refresh();

            wu = get_num(9);
            if (wu == -1)
            {
               wu = (cash * 2);
            }
            if (wu <= (cash * 2))
            {
               cash += wu;
               debt += wu;
               break;
            } else {
               printw("\n\nHe won't loan you so much, Taipan!");

               refresh();
               timeout(5000);
               getch();
               timeout(-1);
            }
         }
         port_stats();

         break;
      }
   }

   if ((debt > 20000) && (cash > 0) && (rand()%5 == 0))
   {
      int num = rand()%3 + 1;

      cash = 0;
      port_stats();

      move(16, 0);
      clrtobot();
      printw("Comprador's Report\n\n");
      printw("Bad joss!!\n");
      printw("%d of your bodyguards have been killed\n", num);
      printw("by cutthroats and you have been robbed\n");
      printw("of all of your cash, Taipan!!\n");

      refresh();
      timeout(5000);
      getch();
      timeout(-1);
   }

   return;
}

void good_prices(void)
{
   char item[14];

   int i = rand()%4,
       j = rand()%2;

   if (i == 0)
   {
      strcpy(item, "Opium");
   } else if (i == 1) {
      strcpy(item, "Silk");
   } else if (i == 2) {
      strcpy(item, "Arms");
   } else {
      strcpy(item, "General Cargo");
   }

   move(16, 0);
   clrtobot();
   printw("Comprador's Report\n\n");
   printw("Taipan!!  The price of %s\n", item);
   if (j == 0)
   {
      price[i] = price[i] / 5;
      printw("has dropped to %ld!!\n", price[i]);
   } else {
      price[i] = price[i] * (rand()%5 + 5);
      printw("has risen to %ld!!\n", price[i]);
   }

   refresh();
   timeout(3000);
   getch();
   timeout(-1);
}

void overload(void)
{
   move(16, 0);
   clrtobot();
   printw("Comprador's Report\n\n");
   printw("Your ship is overloaded, Taipan!!");
   refresh();
   timeout(5000);
   getch();
   timeout(-1);
   return;
}

void new_ship(void)
{
   int  choice = 0,
        time;

   float amount;

   time = ((year - 1860) * 12) + month;
   amount = rand()%(1000 * (time + 5) / 6) * (capacity / 50) + 1000;

   if (cash < amount)
   {
      return;
   }

   fancy_numbers(amount, fancy_num);

   move(16, 0);
   clrtobot();
   printw("Comprador's Report\n\n");
   printw("Do you wish to trade in your ");
   if (damage > 0)
   {
      attrset(A_REVERSE);
      printw("damaged");
      attrset(A_NORMAL);
   } else {
      printw("fine");
   }
   printw("\nship for one with 50 more capacity by\n");
   printw("paying an additional %s, Taipan? ", fancy_num);
   refresh();

   while ((choice != 'Y') && (choice != 'y') &&
          (choice != 'N') && (choice != 'n'))
   {
      choice = get_one();
   }

   if ((choice == 'Y') || (choice == 'y'))
   {
      cash -= amount;
      hold += 50;
      capacity += 50;
      damage = 0;
   }

   if ((rand()%2 == 0) && (guns < 1000))
   {
      port_stats();
      new_gun();
   }

   port_stats();

   return;
}

void new_gun(void)
{
   int choice = 0,
       time;

   float amount;

   time = ((year - 1860) * 12) + month;
   amount = rand()%(1000 * (time + 5) / 6) + 500;

   if ((cash < amount) || (hold < 10))
   {
      return;
   }

   fancy_numbers(amount, fancy_num);

   move(16, 0);
   clrtobot();
   printw("Comprador's Report\n\n");
   printw("Do you wish to buy a ship's gun\n");
   printw("for %s, Taipan? ", fancy_num);
   refresh();

   while ((choice != 'Y') && (choice != 'y') &&
          (choice != 'N') && (choice != 'n'))
   {
      choice = get_one();
   }

   if ((choice == 'Y') || (choice == 'y'))
   {
      cash -= amount;
      hold -= 10;
      guns += 1;
   }

   port_stats();

   return;
}

void fancy_numbers(float num, char *fancy)
{
   char number[18];

   if (num >= 100000000)
   {
      int num1 = (num / 1000000);
      sprintf(number, "%d", num1);
      strcpy(fancy, number);
      strcat(fancy, " Million");
   } else if (num >= 10000000) {
      int num1 = (num / 1000000);
      int num2 = (((int) num % 1000000) / 100000);
      sprintf(number, "%d", num1);
      strcpy(fancy, number);
      if (num2 > 0)
      {
         strcat(fancy, ".");
         sprintf(number, "%d", num2);
         strcat(fancy, number);
      }
      strcat(fancy, " Million");
   } else if (num >= 1000000) {
      int num1 = (num / 1000000);
      int num2 = (((int) num % 1000000) / 10000);
      sprintf(number, "%d", num1);
      strcpy(fancy, number);
      if (num2 > 0)
      {
         strcat(fancy, ".");
         sprintf(number, "%d", num2);
         strcat(fancy, number);
      }
      strcat(fancy, " Million");
   } else {
      sprintf(number, "%d", (int) num);
      strcpy(fancy, number);
   }
}

int sea_battle(int id, int num_ships)
{
   int orders = 0,
       num_on_screen = 0,
       ships_on_screen[10],
       time = ((year - 1860) * 12) + month,
       s0 = num_ships,
       ok = 0,
       ik = 1,
       x, y, i,
       input,
       status;

   booty = (time / 4 * 1000 * num_ships) + rand()%1000 + 250;

   for (i = 0; i <= 9; i++)
   {
      ships_on_screen[i] = 0;
   }

   clear();
   flushinp();
   fight_stats(num_ships, orders);

   while (num_ships > 0)
   {
      status = 100 - (((float) damage / capacity) * 100);
      if (status <= 0)
      {
         return 4;
      }
      flushinp();
      move(3, 0);
      clrtoeol();
      printw("Current seaworthiness: %s (%d%%)", st[(status / 20)], status);
      refresh();

      x = 10;
      y = 6;
      for (i = 0; i <= 9; i++)
      {
         if (i == 5)
         {
            x = 10;
            y = 12;
         }

         if (num_ships > num_on_screen)
         {
            if (ships_on_screen[i] == 0)
            {
               usleep(100000);
               ships_on_screen[i] =
                (int)((ec * ((float) rand() / RAND_MAX)) + 20);
               draw_lorcha(x, y);
               num_on_screen++;
               refresh();
            }

            x += 10;
         }
      }

      if (num_ships > num_on_screen)
      {
         move(11, 62);
         printw("+");
      } else {
         move(11, 62);
         printw(" ");
      }

      move(16, 0);
      printw("\n");
      refresh();
      timeout(3000);
      input = getch();
      timeout(-1);

      if ((input == 'F') || (input == 'f'))
      {
         orders = 1;
      } else if ((input == 'R') || (input == 'r')) {
         orders = 2;
      } else if ((input == 'T') || (input == 't')) {
         orders = 3;
      }

      if (orders == 0)
      {
         timeout(3000);
         input = getch();
         timeout(-1);

         if ((input == 'F') || (input == 'f'))
         {
            orders = 1;
         } else if ((input == 'R') || (input == 'r')) {
            orders = 2;
         } else if ((input == 'T') || (input == 't')) {
            orders = 3;
         } else {
            move(3, 0);
            clrtoeol();
            printw("Taipan, what shall we do??    (f=fight, r=run, t=throw cargo)");
            refresh();
            timeout(-1);
            while ((input != 'F') && (input != 'f') &&
                   (input != 'R') && (input != 'r') &&
                   (input != 'T') && (input != 't'))
            {
               input = getch();
            }
            if ((input == 'F') || (input == 'f'))
            {
               orders = 1;
            } else if ((input == 'R') || (input == 'r')) {
               orders = 2;
            } else {
               orders = 3;
            }
         }
      }

      fight_stats(num_ships, orders);
      if ((orders == 1) && (guns > 0))
      {
         int targeted,
             sk = 0;

         ok = 3;
         ik = 1;
         move(3, 0);
         clrtoeol();
         printw("Aye, we'll fight 'em, Taipan.");
         refresh();
         timeout(3000);
         input = getch();
         timeout(-1);

         move(3, 0);
         clrtoeol();
         printw("We're firing on 'em, Taipan!");
         timeout(1000);
         input = getch();
         timeout(-1);
         refresh();

         for (i = 1; i <= guns; i++)
         {
            if ((ships_on_screen[0] == 0) && (ships_on_screen[1] == 0) &&
                (ships_on_screen[2] == 0) && (ships_on_screen[3] == 0) &&
                (ships_on_screen[4] == 0) && (ships_on_screen[5] == 0) &&
                (ships_on_screen[6] == 0) && (ships_on_screen[7] == 0) &&
                (ships_on_screen[8] == 0) && (ships_on_screen[9] == 0))
            {
               int j;

               x = 10;
               y = 6;
               for (j = 0; j <= 9; j++)
               {
                  if (j == 5)
                  {
                     x = 10;
                     y = 12;
                  }

                  if (num_ships > num_on_screen)
                  {
                     if (ships_on_screen[j] == 0)
                     {
                        usleep(100000);
                        ships_on_screen[j] =
                         (int)((ec * ((float) rand() / RAND_MAX)) + 20);
                        draw_lorcha(x, y);
                        num_on_screen++;
                     }

                     x += 10;
                  }
               }
            }

            move(11, 62);
            if (num_ships > num_on_screen)
            {
               printw("+");
            } else {
               printw(" ");
            }

            move(16, 0);
            printw("\n");
            refresh();

            targeted = rand()%10;
            while (ships_on_screen[targeted] == 0)
            {
               targeted = rand()%10;
            }

            x = (targeted < 5) ? ((targeted + 1) * 10) : ((targeted - 4) * 10);
            y = (targeted < 5) ? 6 : 12;

            draw_blast(x, y);
            refresh();
            usleep(100000);

            draw_lorcha(x, y);
            refresh();
            usleep(100000);

            draw_blast(x, y);
            refresh();
            usleep(100000);

            draw_lorcha(x, y);
            refresh();
            usleep(100000);

            ships_on_screen[targeted] -= rand()%30 + 10;

            if (ships_on_screen[targeted] <= 0)
            {
               num_on_screen--;
               num_ships--;
               sk++;
               ships_on_screen[targeted] = 0;

               usleep(100000);

               sink_lorcha(x, y);

               if (num_ships == num_on_screen)
               {
                  move(11, 62);
                  printw(" ");
               }

               fight_stats(num_ships, orders);
               refresh();
            }

            if (num_ships == 0)
            {
               i += guns;
            } else {
               usleep(500000);
            }
         }
         move(3, 0);
         clrtoeol();
         if (sk > 0)
         {
            printw("Sunk %d of the buggers, Taipan!", sk);
         } else {
            printw("Hit 'em, but didn't sink 'em, Taipan!");
         }
         refresh();
         timeout(3000);
         input = getch();
         timeout(-1);

         if ((rand()%s0 > (num_ships * .6 / id)) && (num_ships > 2))
         {
            int ran = rand()%(num_ships / 3 / id) + 1;

            num_ships -= ran;
            fight_stats(num_ships, orders);
            move(3, 0);
            clrtoeol();
            printw("%d ran away, Taipan!", ran);

            if (num_ships <= 10)
            {
               for (i = 9; i >= 0; i--)
               {
                  if ((num_on_screen > num_ships) && (ships_on_screen[i] > 0))
                  {
                     ships_on_screen[i] = 0;
                     num_on_screen--;

                     x = (i < 5) ? ((i + 1) * 10) : ((i - 4) * 10);
                     y = (i < 5) ? 6 : 12;
                     clear_lorcha(x, y);
                     refresh();
                     usleep(100000);
                  }
               }
               if (num_ships == num_on_screen)
               {
                  move(11, 62);
                  printw(" ");
                  refresh();
               }

            }

            move(16, 0);

            refresh();
            timeout(3000);
            input = getch();
            timeout(-1);

            if ((input == 'F') || (input == 'f'))
            {
               orders = 1;
            } else if ((input == 'R') || (input == 'r')) {
               orders = 2;
            } else if ((input == 'T') || (input == 't')) {
               orders = 3;
            }
         }
      } else if ((orders == 1) && (guns == 0)) {
         move(3, 0);
         clrtoeol();
         printw("We have no guns, Taipan!!");
         refresh();
         timeout(3000);
         input = getch();
         timeout(-1);
      } else if (orders == 3) {
         int  choice = 0;

         long amount = 0,
              total = 0;

         move(18, 0);
         printw("You have the following on board, Taipan:");
         move(19, 4);
         printw("Opium: %d", hold_[0]);
         move(19, 24);
         printw("Silk: %d", hold_[1]);
         move(20, 5);
         printw("Arms: %d", hold_[2]);
         move(20, 21);
         printw("General: %d", hold_[3]);

         move(3, 0);
         clrtoeol();
         printw("What shall I throw overboard, Taipan? ");
         refresh();

         while ((choice != 'O') && (choice != 'o') &&
                (choice != 'S') && (choice != 's') &&
                (choice != 'A') && (choice != 'a') &&
                (choice != 'G') && (choice != 'g') &&
                (choice != '*'))
         {
            choice = get_one();
         }

         if ((choice == 'O') || (choice == 'o'))
         {
            choice = 0;
         } else if ((choice == 'S') || (choice == 's')) {
            choice = 1;
         } else if ((choice == 'A') || (choice == 'a')) {
            choice = 2;
         } else if ((choice == 'G') || (choice == 'g')) {
            choice = 3;
         } else {
            choice = 4;
         }

         if (choice < 4)
         {
            move(3, 0);
            clrtoeol();
            printw("How much, Taipan? ");
            refresh();

            amount = get_num(9);
            if ((hold_[choice] > 0) && ((amount == -1) || (amount > hold_[choice])))
            {
               amount = hold_[choice];
            }
            total = hold_[choice];
         } else {
            total = hold_[0] + hold_[1] + hold_[2] + hold_[3];
         }

         if (total > 0)
         {
            move(3, 0);
            clrtoeol();
            printw("Let's hope we lose 'em, Taipan!");
            if (choice < 4)
            {
               hold_[choice] -= amount;
               hold += amount;
               ok += (amount / 10);
            } else {
               hold_[0] = 0;
               hold_[1] = 0;
               hold_[2] = 0;
               hold_[3] = 0;
               hold += total;
               ok += (total / 10);
            }
            move(18, 0);
            clrtobot();
            refresh();

            timeout(3000);
            input = getch();
            timeout(-1);
         } else {
            move(3, 0);
            clrtoeol();
            printw("There's nothing there, Taipan!");
            move(18, 0);
            clrtobot();
            refresh();

            timeout(3000);
            input = getch();
            timeout(-1);
         }
      }

      if ((orders == 2) || (orders == 3))
      {
         if (orders == 2)
         {
            move(3, 0);
            clrtoeol();
            printw("Aye, we'll run, Taipan.");
            refresh();
            timeout(3000);
            input = getch();
            timeout(-1);
         }

         ok += ik++;
         if (rand()%ok > rand()%num_ships)
         {
            flushinp();
            move(3, 0);
            clrtoeol();
            printw("We got away from 'em, Taipan!");
            refresh();
            timeout(3000);
            input = getch();
            timeout(-1);
            num_ships = 0;
         } else {
            move(3, 0);
            clrtoeol();
            printw("Couldn't lose 'em.");
            refresh();
            timeout(3000);
            input = getch();
            timeout(-1);

            if ((num_ships > 2) && (rand()%5 == 0))
            {
               int lost = (rand()%num_ships / 2) + 1;

               num_ships -= lost;
               fight_stats(num_ships, orders);
               move(3, 0);
               clrtoeol();
               printw("But we escaped from %d of 'em!", lost);

               if (num_ships <= 10)
               {
                  for (i = 9; i >= 0; i--)
                  {
                     if ((num_on_screen > num_ships) && (ships_on_screen[i] > 0))
                     {
                        ships_on_screen[i] = 0;
                        num_on_screen--;

                        x = (i < 5) ? ((i + 1) * 10) : ((i - 4) * 10);
                        y = (i < 5) ? 6 : 12;
                        clear_lorcha(x, y);
                        refresh();
                        usleep(100000);
                     }
                  }
                  if (num_ships == num_on_screen)
                  {
                     move(11, 62);
                     printw(" ");
                     refresh();
                  }
               }

               move(16, 0);

               refresh();
               timeout(3000);
               input = getch();
               timeout(-1);

               if ((input == 'F') || (input == 'f'))
               {
                  orders = 1;
               } else if ((input == 'R') || (input == 'r')) {
                  orders = 2;
               } else if ((input == 'T') || (input == 't')) {
                  orders = 3;
               }
            }
         }
      }

      if (num_ships > 0)
      {
         move(3, 0);
         clrtoeol();
         printw("They're firing on us, Taipan!");

         refresh();
         timeout(3000);
         input = getch();
         timeout(-1);
         flushinp();
         for (i = 0; i < 3; i++)
         {
            for (y = 0; y < 24; y++)
            {
               for (x = 0; x < 79; x++)
               {
                  move(y, x);
                  printw("*");
               }
            }
            refresh();
            usleep(200000);
            clear();
            refresh();
            usleep(200000);
         }

         fight_stats(num_ships, orders);
         x = 10;
         y = 6;
         for (i = 0; i <= 9; i++)
         {
            if (i == 5)
            {
               x = 10;
               y = 12;
            }

            if (ships_on_screen[i] > 0)
            {
               draw_lorcha(x, y);
            }

            x += 10;
         }

         move(11, 62);
         if (num_ships > num_on_screen)
         {
            printw("+");
         } else {
            printw(" ");
         }

         move(3, 0);
         clrtoeol();
         printw("We've been hit, Taipan!!");

         refresh();
         timeout(3000);
         input = getch();
         timeout(-1);

         i = (num_ships > 15) ? 15 : num_ships;
         if ((guns > 0) && ((rand()%100 < (((float) damage / capacity) * 100)) ||
             ((((float) damage / capacity) * 100) > 80)))
         {
            i = 1;
            guns--;
            hold += 10;
            fight_stats(num_ships, orders);
            move(3, 0);
            clrtoeol();
            printw("The buggers hit a gun, Taipan!!");
            fight_stats(num_ships, orders);

            refresh();
            timeout(3000);
            input = getch();
            timeout(-1);
         }
         damage = damage + ((ed * i * id) * ((float) rand() / RAND_MAX)) + (i / 2);
         if ((id == GENERIC) && (rand()%20 == 0))
         {
            return 2;
         }
      }
   }

   if (orders == 1)
   {
      clear();
      fight_stats(num_ships, orders);
      move(3, 0);
      clrtoeol();
      printw("We got 'em all, Taipan!");
      refresh();
      timeout(3000);
      getch();
      timeout(-1);

      return 1;
   } else {
      return 3;
   }
}

void draw_lorcha(int x, int y)
{
   move (y, x);
   printw("-|-_|_  ");
   move ((y + 1), x);
   printw("-|-_|_  ");
   move ((y + 2), x);
   printw("_|__|__/");
   move ((y + 3), x);
   printw("\\_____/ ");
}

void clear_lorcha(int x, int y)
{
   move (y, x);
   printw("        ");
   move ((y + 1), x);
   printw("        ");
   move ((y + 2), x);
   printw("        ");
   move ((y + 3), x);
   printw("        ");
}

void draw_blast(int x, int y)
{
   move (y, x);
   printw("********");
   move ((y + 1), x);
   printw("********");
   move ((y + 2), x);
   printw("********");
   move ((y + 3), x);
   printw("********");
}

void sink_lorcha(int x, int y)
{
   int delay = rand()%20;

   move (y, x);
   printw("        ");
   move ((y + 1), x);
   printw("-|-_|_  ");
   move ((y + 2), x);
   printw("-|-_|_  ");
   move ((y + 3), x);
   printw("_|__|__/");
   refresh();
   usleep(500000);
   if (delay == 0)
   {
      usleep(500000);
   }

   move ((y + 1), x);
   printw("        ");
   move ((y + 2), x);
   printw("-|-_|_  ");
   move ((y + 3), x);
   printw("-|-_|_  ");
   refresh();
   usleep(500000);
   if (delay == 0)
   {
      usleep(500000);
   }

   move ((y + 2), x);
   printw("        ");
   move ((y + 3), x);
   printw("-|-_|_  ");
   refresh();
   usleep(500000);
   if (delay == 0)
   {
      usleep(500000);
   }

   move ((y + 3), x);
   printw("        ");
   refresh();
   usleep(500000);
   if (delay == 0)
   {
      usleep(500000);
   }
}

void fight_stats(int ships, int orders)
{
   char ch_orders[12];

   if (orders == 0)
   {
      strcpy(ch_orders, "\0");
   } else if (orders == 1) {
      strcpy(ch_orders, "Fight      ");
   } else if (orders == 2) {
      strcpy(ch_orders, "Run        ");
   } else {
      strcpy(ch_orders, "Throw Cargo");
   }

   move(0, 0);
   if (ships >= 1000)
   {
      printw("%d", ships);
   } else if (ships >= 100) {
      printw(" %d", ships);
   } else if (ships >= 10) {
      printw("  %d", ships);
   } else {
      printw("   %d", ships);
   }
   move(0, 5);
   if (ships == 1)
   {
      printw("ship attacking, Taipan! \n");
   } else {
      printw("ships attacking, Taipan!\n");
   }
   printw("Your orders are to: %s", ch_orders);
   move(0, 50);
   printw("|  We have");
   move(1, 50);
   printw("|  %d guns", guns);
   move(2, 50);
   printw("----------");
   move(16, 0);

   return;
}

void mchenry(void)
{
   int choice = 0;

   move(16, 0);
   clrtobot();
   printw("Comprador's Report\n\n");
   printw("Taipan, Mc Henry from the Hong Kong\n");
   printw("Shipyards has arrived!!  He says, \"I see\n");
   printw("ye've a wee bit of damage to yer ship.\n");
   printw("Will ye be wanting repairs? ");
   refresh();

   while ((choice != 'Y') && (choice != 'y') &&
          (choice != 'N') && (choice != 'n'))
   {
      choice = get_one();
   }

   if ((choice == 'Y') || (choice == 'y'))
   {
      int  percent = ((float) damage / capacity) * 100,
           time = ((year - 1860) * 12) + month;

      long br = ((((60 * (time + 3) / 4) * (float) rand() / RAND_MAX) +
                25 * (time + 3) / 4) * capacity / 50),
           repair_price = (br * damage) + 1,
           amount;

      move(18, 0);
      clrtobot();
      printw("Och, 'tis a pity to be %d%% damaged.\n", percent);
      printw("We can fix yer whole ship for %ld,\n", repair_price);
      printw("or make partial repairs if you wish.\n");
      printw("How much will ye spend? ");
      refresh();

      for (;;)
      {
         move(21, 24);
         amount = get_num(9);
         if (amount == -1)
         {
            amount = cash;
         }
         if (amount <= cash)
         {
            cash -= amount;
            damage -= (int)((amount / br) + .5);
            damage = (damage < 0) ? 0 : damage;
            port_stats();
            refresh();
            break;
         }
      }
   }

   return;
}

void retire(void)
{
   move(16, 0);
   clrtobot();
   printw("Comprador's Report\n\n");
   attrset(A_REVERSE);
   printw("                         \n");
   printw(" Y o u ' r e    a        \n");
   printw("                         \n");
   printw(" M I L L I O N A I R E ! \n");
   printw("                         \n");
   attrset(A_NORMAL);
   refresh();
   timeout(5000);
   getch();
   timeout(-1);

   final_stats();
}

void final_stats(void)
{
   int years = year - 1860,
       time = ((year - 1860) * 12) + month,
       choice = 0;

   clear();
   printw("Your final status:\n\n");
   cash = cash + bank - debt;
   fancy_numbers(cash, fancy_num);
   printw("Net cash:  %s\n\n", fancy_num);
   printw("Ship size: %d units with %d guns\n\n", capacity, guns);
   printw("You traded for %d year", years);
   if (years != 1)
   {
      printw("s");
   }
   printw(" and %d month", month);
   if (month > 1)
   {
      printw("s");
   }
   printw("\n\n");
   cash = cash / 100 / time;
   attrset(A_REVERSE);
   printw("Your score is %.0f.\n", cash);
   attrset(A_NORMAL);
   printw("\n");
   if ((cash < 100) && (cash >= 0))
   {
      printw("Have you considered a land based job?\n\n\n");
   } else if (cash < 0) {
      printw("The crew has requested that you stay on\n");
      printw("shore for their safety!!\n\n");
   } else {
      printw("\n\n\n");
   }
   printw("Your Rating:\n");
   printw(" _______________________________\n");
   printw("|");
   if (cash > 49999)
   {
      attrset(A_REVERSE);
   }
   printw("Ma Tsu");
   attrset(A_NORMAL);
   printw("         50,000 and over |\n");
   printw("|");
   if ((cash < 50000) && (cash > 7999))
   {
      attrset(A_REVERSE);
   }
   printw("Master Taipan");
   attrset(A_NORMAL);
   printw("   8,000 to 49,999|\n");
   printw("|");
   if ((cash < 8000) && (cash > 999))
   {
      attrset(A_REVERSE);
   }
   printw("Taipan");
   attrset(A_NORMAL);
   printw("          1,000 to  7,999|\n");
   printw("|");
   if ((cash < 1000) && (cash > 499))
   {
      attrset(A_REVERSE);
   }
   printw("Compradore");
   attrset(A_NORMAL);
   printw("        500 to    999|\n");
   printw("|");
   if (cash < 500)
   {
      attrset(A_REVERSE);
   }
   printw("Galley Hand");
   attrset(A_NORMAL);
   printw("       less than 500|\n");
   printw("|_______________________________|\n\n");

   while ((choice != 'Y') && (choice != 'y') &&
          (choice != 'N') && (choice != 'n'))
   {
      move (22, 0);
      clrtoeol();
      printw("Play again? ");
      refresh();
      choice = get_one();
   }

   if ((choice == 'Y') || (choice == 'y'))
   {
      bank         = 0;
      hkw_[0]      = 0;
      hkw_[1]      = 0;
      hkw_[3]      = 0;
      hkw_[4]      = 0;
      hold_[0]     = 0;
      hold_[1]     = 0;
      hold_[2]     = 0;
      hold_[3]     = 0;
      hold         = 0;
      capacity     = 60;
      damage       = 0;
      month        = 1;
      year         = 1860;
      port         = 1;

      splash_intro();
      name_firm();
      cash_or_guns();
      set_prices();

      return;
   }

   clear();
   refresh();
   nocbreak();
   endwin();

   exit(0);
}
