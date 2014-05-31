#include "config.h"

#define TERRAIN_PLAINS 0
#define TERRAIN_FOREST 1
#define TERRAIN_WATER  2
#define TERRAIN_WALL   3
#define TERRAIN_MOUNTAIN 4
#define TERRAIN_MAX 5

#define UNIT_TYPE_FOOTMEN     0
#define UNIT_TYPE_SWORDSMEN   1
#define UNIT_TYPE_SHIELDMEN   2
#define UNIT_TYPE_HALBERDIER  3
#define UNIT_TYPE_PIKEMEN     4
#define UNIT_TYPE_ARCHERS     5
#define UNIT_TYPE_CROSSBOWMEN 6
#define UNIT_TYPE_MUSKETEER   7
#define UNIT_TYPE_RANGERS     8
#define UNIT_TYPE_CAVALRY     9
#define UNIT_TYPE_KNIGHTS    10
#define UNIT_TYPE_BALLISTA   11
#define UNIT_TYPE_TREBUCHET  12
#define UNIT_TYPE_CANNON     13
#define UNIT_TYPE_VULTURE    14
#define UNIT_TYPE_EAGLE      15
#define UNIT_TYPE_GRIFFON    16
#define UNIT_TYPE_OGRE       17
#define UNIT_TYPE_MAX        18

#define TERRAIN_MASK 0x1F
#define UNIT_MASK 0xE0

#define BOOL int
#define TRUE 1
#define FALSE 0

typedef unsigned char index_type;
typedef unsigned char unit_type;
typedef unsigned char position;
typedef unsigned char limited_value; // 0-255. Watch out for overflow!
typedef unsigned char special_flag;
enum {
	FLAG_NONE=0x0,
	FLAG_FIRST_STRIKE=0x1,
	FLAG_EXPLOSIVE=0x2,
	FLAG_HEAL=0x4,
	FLAG_BLESS=0x8,
	FLAG_ATTACK_AFTER_MOVE=0x10,
};

struct unit_def {
	unsigned char tile;
	char *name;
	limited_value range_min;
	limited_value range_max;
	limited_value attack;
	limited_value defense;
	limited_value move_cost[TERRAIN_MAX];
	special_flag special;
	//	limited_value price;
	//  - add resources here
	//  - add upgrades here
};


struct terrain_def {
	unsigned char tile;
	char *name;
	signed char defense_bonus;
	//  Curses only
	unsigned char color;
};
