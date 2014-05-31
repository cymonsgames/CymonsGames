#ifndef __CONFIG_H_
#define __CONFIG_H_

// Size of the window used by nibbles
#define		WINDOWX			80
#define		WINDOWY			25

// Number of microseconds between each tick
#define		TICKSPEED		100000

// The length of a new worm, don't set this too high!
#define		STARTLENGTH		5

// How many ticks the worm will continue to grow after start
#define		STARTELENGTH	0

// How much the worm grows when it eats
#define		WORMGROWTH		5

// Color settings - see the ncurses manpage for other colors
#define		WORMCOLOR		COLOR_YELLOW
#define		LINECOLOR		COLOR_WHITE
#define		STATUSCOLOR		COLOR_WHITE
#define		DOTCOLOR		COLOR_BLUE
#define		BACKGROUND		COLOR_BLACK

// How many moves we make room for in the history array - the higher, the
// better. When HISTORYSIZE ticks have passed, the last bytes (number equal
// to the length of the worm) are copied to the start of the array.
#define		HISTORYSIZE		8192




// Don't edit below this line!





#define		WORMCOLORPAIR	1
#define		LINECOLORPAIR	2
#define		STATUSCOLORPAIR	3
#define		DOTCOLORPAIR	4

#define		LEFT			1
#define		RIGHT			2
#define		UP				3
#define		DOWN			4

#ifndef true
#define		true			1
#endif

#ifndef false
#define		false			0
#endif

#define		GRIDX			((WINDOWX-2)>>1)
#define		GRIDY			(WINDOWY-4)

#define		Main			0
#define		Grid			1
#define		Status			2

#define		VERSION			1.1

#endif /* __CONFIG_H__ */
