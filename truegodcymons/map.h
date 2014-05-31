#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#define YSIZE 21
#define XSIZE 80
#define NPOINT 30
#define NCONNECT 2
#define TUNPOINT 5
#define Y 0
#define X 1

int dlvl;
char map[YSIZE][XSIZE];
int upy,upx,downy,downx;
void genmap(int maptype);
void drawmap();
void gendungeon();
void genbasic();
void gentunnel();
void genforest();
void genempty();
void nextlevel();
void prevlevel();
bool seen[YSIZE][XSIZE];

#endif // MAP_H_INCLUDED
