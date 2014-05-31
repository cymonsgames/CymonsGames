/*
class.h
Part of Get Out
Copyright 2008 James E. Ward
Licensed under GNU 3 see included copying.txt
*/

class map
{
public:
  //20 changed to 25 for test
  char show[25][40], card;
  map();
  int exity,exitx,cardx,cardy;
};

class people
{
public:
  char sym;
  int x,y;
};



