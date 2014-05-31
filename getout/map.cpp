/*
map.cpp
Part of Get Out
Copyright 2008 James E. Ward
Licensed under GNU 3 see included copying.txt
*/
#include <iostream>
#include <stdlib.h>
#include "class.h"
#include <time.h>

using namespace std;


map::map()
{
  int z;

  exity=rand()%18+1;
  exitx=rand()%38+1;
  cardy=rand()%18+1;
  cardx=rand()%38+1;

  for (int yy=0;yy<20;yy++)
    {
      for (int xx=0;xx<40;xx++)
        {
          show[yy][xx] = (xx==0 || xx==39 || yy==0 ||yy==19) ? '*' : '.';
          z=rand() % 6;
          if (z==4 && (xx!=1 || yy!=1))
            show[yy][xx] = '*';
        }
    }
  if (show[exity][exitx] == '*')
    show[exity][exitx]=' ';
  if (show[cardy][cardx] == '*')
    show[cardy][cardx]=' ';
  card='!';
}


