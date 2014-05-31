#include <curses.h>
#include <stdlib.h>
#include <time.h>

/* 'while' short form */
#define W(c)while(c)
/* 'for' short form */
#define F(v,h)for(v=0;v<h;v++)
/* launch instruction 'i' for every tile in the dungeon */
#define D(i)F(y,S)F(x,T){i;}
/* short form for dungeon tile at position x,y */
#define G d[x][y]
/* short form for dungeon tile at position x+f,y+g - useful for movement/generation */
#define M d[x+f][y+g]
/* sign of number 'n' */
#define Z(n)n<0?-1:(n>0?1:0)
/* random number */
#define r rand()

/*
	Variables:
	x,y - always indicate a position in the dungeon, used mostly through G
	c   - helper variable, stores pressed character, used for color pairs initialization
	d[][] - dungeon map
	L   - current dungeon level
	l,p - HP current/maximum
	v   - gold collected
	f,g - secondary dungeon position, used in generation/movement
	h,i - room dimensions, used in generation
	b,z - temporary variables

	Constants:
	S,T - size of dungeon, SxT
	e   - a constant meaning '.'
	u   - a constant used for monster handling, 'a'-1
	k[] - a table used for coloring output, putting stuff in the dungeon, and doing chests
*/
int x,y,S=24,T=60,c,d[90][90],L=1,b,
	f,g,z,h,i,l=9,p=20,e=46,v,u=96;
wchar_t k[]=L"$`*$.@?$?>";

/* find a random empty position and insert t in it */
int E(int t){W(d[x=r%T][y=r%S]-e);G=t;}

int N(){
	/* fill with walls */
	D(G=35)
	/* add rooms */
	x=y=9;
	F(b,S)
	{
		f=r%58+1;g=r%22+1;
		W(f^x||g^y){
			G=e,
			r&1?(x+=Z(f-x)):(y+=Z(g-y));
		}
	}
	/* put stairs, PC, blood gem */
	F(b,10)E(k[b]);
	/* put monsters */
	F(b,L*2)E(98+r%L);
}

/* move the actor at (x,y) by [f,g] */
int m(int f, int g){
	/* is player? */
	G-64?
		/* not a player, so a monster */
		/* monster moved into player? */
		M-64||(l-=G-u)>0||(M=e)
	:
	(
		/* player code */
		/* stairs */
		M^62||L++&&(L^7?N():(G=e)),
		/* talismans */
		M^u||(p+=L,M=e),
		/* blood gems */
		M^42||(l=p,M=e),
		/* gold */
		M^36||(v+=x%9*L+L,M=e),
		/* chests */
		M^63||(M=r%6?k[r&1]:97+L),
		/* attacking monsters */
		M>u&&--M
	)
	;

	/* actual movement */
	M-e?M-35||(g&&m(f,0),f&&m(0,g)):(M=G|256,G=e);
}

/* main */
int main()
{
	/* curses/srand initialization */
	PDC_set_title("Cymon's Games - A Journey to Hell");
	srand(time(NULL));
	initscr();raw();start_color();
	/* set up colors */
	F(c,8)
#ifdef PDCURSES
  init_pair(c,(c&2)|((c&4) / 4)|((c&1) * 4),0);
#else
	init_pair(c,c,0);
#endif

	/* generate a level */
	N();

	/* play until Q is pressed */
	W(c-81)
	{
		/* find player, store position in f,g */
		D(G-64||(f=x,g=y))

		/* draw screen */
		clear();
		D(
			/* determine tile colour */
			b=1;W(G^k[b]&&b<7)b++;
			/* check for visibility */
			h=x;i=y;c=0;W(c++<u)(d[f+(z=(h-f)*c*.01)][g+(z=(i-g)*c*.01)]-35)||(b=0);
			/* draw, if visible at all */
			b&&mvaddch(y,x,G|COLOR_PAIR(b))
		)

		/* status bar */
		mvprintw(S,0,"HP:%d/%d L:%d $%d %s",l,p,L,v,L^7?"":"WIN");

		/* get input */
		c=getch();

		/* move monsters */
		D(G>u&&G<123&&m(Z(f-x),Z(g-y)))

		/* move player, if needed */
		D(c<49||c>57||G-64||m((c-1)%3-1,1-(c-49)/3))

		/* clear move flags for player and monsters */
		D(G&=255)
	}

	/* clean up */
	endwin();
	return 0;
}
