///////////////////////////////GRAVWHIP.H///////////////////////////////////
// Header file for the GRAVWHIP.CPP.

// I N C L U D E S ////////

#include <bios.h>
#include <conio.h>
#include <io.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <memory.h>

#include "joegraph.cpp"

// D E F I N E S //////////

#define NUM_STARS      100
#define STAR_COLOR     208          // Start of star colors in game palette.
#define STAR_DEPTH     39           // Depth of star colors in game palette.
#define AIMING_SHOT    192
#define SHOT_COLOR     194          // Start of shot colors.
#define DT             .25          // Time unit constant.
#define MAX_LOSSES     3
#define MAX_PLANETS    10
#define MAX_SHIPS      6
#define MAX_LIFE       100
#define MAX_SHOTS      100
#define GAME_X         0            // (x,y) Start of Game Screen.
#define GAME_Y         0
#define GAME_X2        298          // (x,y) End of Game Screen.
#define GAME_Y2        185
#define GAME_WIDTH     299
#define GAME_HEIGHT    186
#define CRITICAL_DIST  6426
#define MAX_COLLISION  191
#define PI             M_PI
#define DEG2RAD        0.01745199   // pi/180
#define GC             0.2          // Gravitational Constant.

#define SHIFT_R             0x0001
#define SHIFT_L             0x0002
#define CTRL                0x0004
#define ALT                 0x0008
#define SCROLL_LOCK_ON      0x0010
#define NUM_LOCK_ON         0x0020
#define CAPS_LOCK_ON        0x0040
#define INSERT_MODE         0x0080
#define CTRL_L              0x0100
#define ALT_L               0x0200
#define CTRL_R              0x0400
#define ALT_R               0x0800
#define SCROLL_LOCK_DWN     0x1000
#define NUM_LOCK_DWN        0x2000
#define CAPS_LOCK_DWN       0x4000

// M A C R O S ////////////

// S T R U C T U R E S ////
typedef struct Ship_Type {
  char input;                           // 1=player, 2=computer, 3=no input.
  char losses;                 // Starts at MAX_LOSSES and decreases till 0.
  int life;                             // If greater than 0 ship is active.
  int life_level;                                      // life=life_level*10
  int energy;
  int energy_level;                             // energy=30+energy_level*10
  unsigned int cx,cy;                            // (x,y) of center of ship.
  float angle;
  float power;
  float most_power;
  char type;
// Type Key:
// 1 : Torpedo         2 : Phasic               3 : Bouncer
// 4 : FireCracker     5 : FireStorm            6 : TriShot
// 7 : Spawner         8 : Weapon X             9 : Clamp Sabotage
// 10: Planet Shatter  11: Sun Nova             12: Clamp Release
  int counter;

  int money;
  int kills;
  int total_kills;

  char inventory[15];           // The level of each of the various weapons.

  sprite image;
} ships;

typedef struct Planet_Type {

  int life;                                    // When at 0 planet blows up.
  char type;                            //0- non flipable, 1-flipble, 2-sun.
  unsigned char r;                                         // planet radius.
  unsigned int cx,cy;                          // (x,y) of center of planet.
  float d, GM;                          // Density and Gravity*Mass product.

  sprite image;
} planets;

typedef struct Shot_Type {

  char power;
  char owner;
  char type;
// Type Key:
// 1 : Torpedo         2 : Phasic               3 : Bouncer
// 4 : FireCracker     5 : FireStorm            6 : TriShot
// 7 : Spawner         8 : Weapon X             9 : Clamp Sabotage
// 10: Planet Shatter  11: Sun Nova             12: Clamp Release
  int counter;
  float x,y;               // x and y coordinates. Cast to int when ploting.
  int oldx, oldy;
  unsigned char backgrnd,                               // background color.
       color;
  float vx,vy;                           // vector x and velocity y of shot.

} shots;

// P R O T O T Y P E S ////
void BW_Fade(void);

void Init();

void Intro();

void Menu();

void Draw_Menu();

void Options();

void Slide_Right(char from, char to);

void Slide_Left(char from, char to);

void Play_Game();

void Purchase();

void Com_Buy(int player);

void Get_Shot(int player);

char Enough_Energy(char player);

void Fire_Shots();

void Draw_Shot(shots *shot);

void Update_Shot(shots *bullet);

void Grav_Pull(shots *bullet);

char Check_Collision(shots *bullet);

char Hit_Something(shots *bullet);

char Hit_Ship(shots *bullet);

char Hit_Planet(shots *bullet);

void Kill_Ship(char c);

void Kill_Planet(char c, char owner);

char Create_Shot(int x, int y, char owner, int power, int number);

void Setup_Board ();

void Assign_Planet (planets *urth);

void Draw_Board();

void Draw_Side ();

void Draw_Losses(int x, int y, char losses);

void Tally();

void Rotate_Ship(ships *enterprise);

void Draw_Border(int x, int y, int width, int height, char raised);

void Close();

void Save_Block(unsigned char far *start, int x, int y, int width,int height);

void Restore_Block(unsigned char far *start, int x, int y, int width,int height);

unsigned int Get_Control_Keys(unsigned int mask);

unsigned char Get_Scan_Code(void);
