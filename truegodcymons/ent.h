#ifndef ENT_H_INCLUDED
#define ENT_H_INCLUDED
#include"map.h"
#define NMON 12

bool stone=false;
int exper;

void look();
bool playerturn();
bool playerinput();
void genplayer();
void see(int y,int x);
void clearlocs();
void genmon(int i,int lv);
void monturn(int i);
void action();
void levelmonster(int i);
void demotemonster(int i);
void levelplayer();
void demoteplayer();


struct ent
{
int x,y,hp,mhp,spd,str,lvl,vis,counter;
char sym;
bool alive;
}ents[NMON];

void pattack(ent* attackee);
void monattack(ent* mon);

ent* entsl[YSIZE][XSIZE];

#endif // ENT_H_INCLUDED
