/* Ruler by Stephen Parkes edited by Joseph Larson ver 1-May-2008
 * Ruler - a game originally for the unexpanded 1k ZX81
 * written by A. Greenhalgh of Darwen, Lancashire and
 * published as a type-in in Sinclair User March 1983 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int act_of_god(void)
{
   printf("Our crops failed Sire, perhaps the Gods are angry with us\n");
   return 0;
}

int main()
{
   srand (time(NULL));
   int people = 100;
   int food = 200;

   int sow = 0;
   int tables = 0;
   int z;

  puts ("RULER\n-----\n\n");
input:
   printf ("%d lbs of food and %d people\nInput food for sowing ", food, people);
   scanf("%d", &sow);

   printf("and the amount for our peoples table sire? ");
   scanf("%d", &tables);

   if (sow + tables > food)
   {
       printf("you can't put %d lbs on the tables and sow %d lbs this "
       "year,  sire\n", tables, sow);
       goto input;
   }

   z = food - (sow + tables);
   people = (tables / people)*(people / 2) + tables;
   if (people<10)
   {
       printf("\nThe few people that survived the hard winter have left "
         "for pastures new.\nYour empire has crumbled like many before it and"
         "many that will rise in\nits ashes");
       return 0;
   }

   if (rand()%5==0) sow = act_of_god();
   food = z + (sow * 3.5);
   if (people > 1000 && food > 1000)
   {
       printf("\nSire, your majesty is known throughout the world.\n"
       "Statues are errected in your honour today because our nation is strong "
       "and\nsafe from harm, our nation is currently %d strong and we have "
       "supplies\nof %d to feed us.  We now longer need your guidance in "
       "these matters you\ncan sit in state without vexation on our earthly "
       "matters.\n\nALL HAIL THE KING!", people, food);
       return 0;
   }

   printf("\nA New Year Dawns\n");
   goto input;
}
