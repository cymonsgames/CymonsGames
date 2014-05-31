#include <curses.h>

#define MAP_SIZE_X 40
#define MAP_SIZE_Y 20

#define UNITS_PER_PLAYER 32
#define MAX_UNITS (UNITS_PER_PLAYER*2)


#define CONTROL_KEYPAD 1
#if CONTROL_KEYPAD==0
#define CONTROL_VI     0
#if CONTROL_VI==0
#define CONTROL_WSAD   0
#endif
#endif

#if CONTROL_KEYPAD
#define KEY_MOVE_UP '8'
#define KEY_MOVE_DOWN '2'
#define KEY_MOVE_LEFT '4'
#define KEY_MOVE_RIGHT '6'
#define KEY_ACTION '0'
#endif
#if CONTROL_WSAD
#define KEY_MOVE_UP 'w'
#define KEY_MOVE_DOWN 's'
#define KEY_MOVE_LEFT 'a'
#define KEY_MOVE_RIGHT 'd'
#define KEY_ACTION ' '
#endif
#if CONTROL_VI
#define KEY_MOVE_UP 'k'
#define KEY_MOVE_DOWN 'j'
#define KEY_MOVE_LEFT 'h'
#define KEY_MOVE_RIGHT 'l'
#define KEY_ACTION ' '
#endif
