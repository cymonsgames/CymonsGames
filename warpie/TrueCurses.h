/*
    __         __
    /  /) / / /_
   /  /\ /_/ /_  Curses v0.1 (c) copyright
   ---------------------------------------
   Created by Marcus Wiseman (madmarky122)
   allegro based roguelike libary
*/


#ifndef TRUECURSES_H_INCLUDED
#define TRUECURSES_H_INCLUDED

#define CHTYPE_LONG     0      /* size of chtype; long */

#include <allegro.h>
#include <winalleg.h>
#include <wchar.h>
#include <stdarg.h>
#include <stddef.h>
#include <iostream>
#include <math.h>
#include <stdio.h>             /* Required by X/Open usage below */

using namespace std;

#define true 1
#define false 0
#define bt32 32
#define bt24 24
#define bt16 16
#define bt8 8

#define MAPY 1000
#define MAPX 1000

#define COLOR_WHITE (color(255,255,255))
#define COLOR_BLACK (color(0,0,0))
#define COLOR_RED (color(255,0,0))
#define COLOR_GREEN (color(0,255,0))
#define COLOR_BLUE (color(0,0,255))
#define COLOR_CYAN (color(0,255,255))
#define COLOR_YELLOW (color(255,255,0))
#define COLOR_PURPLE (color (255,0,255))

typedef struct particle {
    double x,y,             // location of the particle on the screen
    dx,dy,             // speed and direction of movement
    rx,ry,             // the amount of randomness in the movement
    tx,ty;             // the location this particle was last at.
    int color,              // the particle's color
    type,               // the drawing type of the particle
    life;               // This is a counter!
    // When the counter hits zero we remove the particle.
    struct particle *next;  // a link to the next particle.
} particle;

#define RAIN_PARTICLE  0
#define SNOW_PARTICLE1 1
#define SNOW_PARTICLE2 2

struct SCREEN_SETTINGS {
    int height;
    int width;
    int font_scale_factor;
    bool window;
    int color_depth;
};

typedef char PASSABLE;
typedef char NONPASSABLE;

class SLOTS {
public:
    int item_id;
    int item_amount;
};

class INVENTORY {
public:
    SLOTS slots[100];
    int max_slots;
};

class PLAYER {
public:
    char *name; // player name
    char *race; // race
    int y,x;
    int hp,exp,str,def,dex // basic status
    ,staminam,will,luck,sight
    ,willpower,damage_min,damage_max;
    char c; // (@)
    int imageID;
    int gold; // currency
    INVENTORY inventory;
};

class PANEL {
public:
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    void Set(int y0,int x0, int height, int width);
    void Draw(char boarder,bool transparent,int fg_color,int bg_color);
    void Text(const char *text, int bound_y, int bound_x,int fg_color,int bg_color);
};

string IntToString(int intValue);
string IntToCStr(int intValue);
bool initscr(int height, int width,int color_depth,int font_scale_factor,int mode); // intilize screen
void clear(); // clear screen
void refresh(); // draw buffer
void freebuf(); // free buffer
int getch(); // wait for key input
void RecFill(char Map[MAPY][MAPX],int yy0, int xx0, int yy1, int xx1, char wall,char floor);
bool getkey(int input); // get key input (dont wait)
int color(int r, int g, int b); // set color with red,green,blue
int color_t(int r, int g, int b, int alpha); // set color with alpha
int dist(int y0,int x0,int y1,int x1);
bool in_range(int y0,int x0,int y1,int x1,int r); // check if in range
bool los(int y0,int x0,int y1,int x1,char opaque,char Map[MAPY][MAPX]);
void napms(int time); // sleep in mili seconds
int bound(int i,int min,int max); // check min and max or a var
void fov(char Map[MAPY][MAPX],int y0,int x0,
         int raduis,char wall,char shadow,
         int r0,int g0,int b0,
         int r1,int g1,int b1,
         int r2,int g2,int b2,
         int r3,int g3,int b3,int blend_factor); // basic fov
bool infov(char Map[MAPY][MAPX],int y0,int x0,int raduis,char wall); // check if somthing is in fov
char mvgetch(char Map[MAPY][MAPX],int y, int x); // get Map char at position
void mvaddch(int y, int x,const char *c,int foreground_color,int background_color); // add char at position , set color
void mvaddch_offset(int y, int x,int x_offset, int y_offset,const char *c,int foreground_color,int background_color); // add char at a offset
void mvprintw(int y, int x,const char *c,int foreground_color,int background_color); // add string at position , set color
int basic_movement_y(char Map[MAPY][MAPX],int y0,int x0,PASSABLE pass[100]); // basic movement on the y axis
int basic_movement_x(char Map[MAPY][MAPX],int y0,int x0,PASSABLE pass[100]); // basic movement on the x axis
const char *c_char(char c); // convert char into a const char
const char *add_cchar(const char *add0, const char *add1); // add to const char to each other
const char *add_ichar(const char *add0, int add1); // add to const char to a integer
void CaveGenMap(char Map[MAPY][MAPX], int YSIZE,int XSIZE, char fill,char sub,int steps); // genarate a map
bool IsAtPoint(int y0, int x0,int y1, int x1); // check if pos is at another pos
int xCenter();
int yCenter();
void DrawNoise(int y,int x,int size,int r,int g,int b);
void DrawPixelGrad(int y,int x,int size,int r,int g, int b,bool reverse,int blend_factor);
void DrawPixel(int y,int x,int size,int color);
int blend_factor(int a);
int getcolor(int y, int x);
void DrawBitmap(int ID, int y0, int x0, bool masked, int scalef);
void initBitmap(int ID,const char *filename);
void Solid_LightSource(char Map[MAPY][MAPX],char wall, char shadow,
                       int y0, int x0,int raduis,bool glow
                       ,int C_W_F_COL, int C_W_B_COL
                       ,int C_S_F_COL, int C_S_B_COL);
void Blend_LightSource(char Map[MAPY][MAPX],char wall, char shadow,
                       int y0, int x0,int raduis,bool glow
                       ,int COLOR,int wall_r0,int wall_g0, int wall_b0,int blend_factor);
void SolidBlend_LightSource(char Map[MAPY][MAPX],char wall, char shadow,
                            int y0, int x0,int raduis,bool glow
                            ,int COLOR,int wall_r0,int wall_g0,int wall_b0,int blend_rate);
void solid_fov(char Map[MAPY][MAPX],int y0,int x0,
               int raduis,char wall,char shadow,
               int C_W_F_COL, int C_W_B_COL,
               int C_S_F_COL, int C_S_B_COL);
void solidb_fov(char Map[MAPY][MAPX],int y0,int x0,
                int raduis,char wall,char shadow,
                int r0,int g0,int b0,
                int r1,int g1,int b1,
                int r2,int g2,int b2,
                int r3,int g3,int b3,int blend_factor);
void blend_fov(char Map[MAPY][MAPX],int y0,int x0,
               int raduis,char wall,char shadow,
               int r0,int g0,int b0,
               int r1,int g1,int b1,
               int r2,int g2,int b2,
               int r3,int g3,int b3,int blend_factor);

void set_alpha(int a);
int collide_detect(int xmin1, int ymin1, BITMAP *sprite1, int xmin2, int ymin2, BITMAP *sprite2);
void Floodfill(int y, int x, int color);
bool initSound(int id, bool play, string soundpath); // init a sound
bool PlaySound(int id); // play a sound
bool StopSound(int id); // stop a sound from player

#endif // TRUECURSES_H_INCLUDED
