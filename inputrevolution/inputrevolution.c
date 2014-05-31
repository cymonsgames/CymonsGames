#include<time.h>
#include<stdlib.h>
#include<curses.h>
#define YSIZE 24
#define XSIZE 21
#define UCOL 4
#define LCOL 8
#define RCOL 12
#define DCOL 16
#define LEFTMOST (COLS - XSIZE) / 2

void init();
void check(int d);
void newlev();
void adv();

int lev=1,chance=15,score=0,lives=10,dir=0,error=0,turn=0;
float tcount = CLOCKS_PER_SEC;
chtype board[YSIZE][XSIZE];
chtype blank=' '|COLOR_PAIR(COLOR_WHITE),uarrow='^'|COLOR_PAIR(COLOR_BLUE),darrow='v'|COLOR_PAIR(COLOR_CYAN),top='-'|COLOR_PAIR(COLOR_WHITE),side='|'|COLOR_PAIR(COLOR_WHITE),larrow='<'|COLOR_PAIR(COLOR_RED),rarrow='>'|COLOR_PAIR(COLOR_MAGENTA);

void init ()
{
    int c,x,y;
    srand (time(NULL));
    keypad(initscr(),1);
    raw ();
    nodelay(stdscr,0);
    noecho();
    curs_set(0);
    nonl();
    start_color();
    for (c=0; c<=8; c++)
    {
        init_pair(c,c,COLOR_WHITE);
    }
	mvprintw(0,0,"Welcome to genEric Input Revolution \nWhen an arrow on the top of the screen brightens press the corresponding \narrow key.");
	getch();
	clear();
    for(y=0;y<YSIZE;y++)
    {
    	for(x=0;x<XSIZE;x++)
		{
			board[y][x]=blank;
			mvaddch(y,x+LEFTMOST,board[y][x]);
		}
	}
	mvaddch(1,UCOL+LEFTMOST,uarrow);mvaddch(1,DCOL+LEFTMOST,darrow);mvaddch(1,LCOL+LEFTMOST,larrow);mvaddch(1,RCOL+LEFTMOST,rarrow);
}

void check(int d)
{
	if(board[1][d]!=blank)
	{
		score++;
		board[1][d]=blank;
		beep();
	}
	else error++;
}

void newlev()
{
	int i,y,x;
	clear();
	nodelay(stdscr,0);
	clear();
	mvprintw(0,0,"Level over");
	refresh();
	napms(1000);
	for (i=0;i<=score;i++)
	{
		mvprintw(2,10,"Score: %d",i);
		napms(50);
		refresh();
	}
	napms(500);
	for (i=0;i<=error;i++)
	{
		mvprintw(4,10,"Errors: %d",i);
		napms(50);
		refresh();
	}
	napms(500);
	mvprintw(6,10,"%d% (%d% to pass)",(100*score)/(score + error), 60);
	refresh();
	napms(2000);
	if((100*score)/(score + error) < 60)
	{
		mvprintw(8,0,"FAIL!!");
		refresh();
		napms(1000);
		getch();
		exit(endwin());
	}
	else
	{
		mvprintw(8,0,"PASS!!");
		getch();
		score=0;
		error=0;
		lev++;
		turn=0;
		tcount = CLOCKS_PER_SEC/(2+lev);
		if(chance>4){chance--;}
		getch();
		clear();
	}
	nodelay(stdscr,1);
	    for(y=0;y<YSIZE;y++)
    {
    	for(x=0;x<XSIZE;x++)
		{
			board[y][x]=blank;
			mvaddch(y,x+LEFTMOST,board[y][x]);
		}
	}

}

void adv()
{
	int y,x;
	turn++;
    for(y=0;y<YSIZE-1;y++)
    {
    	for(x=0;x<XSIZE;x++)
		{
			board[y][x]=board[y+1][x];
			mvaddch(y,x+LEFTMOST,board[y][x]);
		}
	}

	x=rand()%chance;
	switch(x)
	{
		case 0:
			board[YSIZE-1][UCOL]=uarrow;
			board[YSIZE-1][LCOL]=blank;
			board[YSIZE-1][RCOL]=blank;
			board[YSIZE-1][DCOL]=blank;
			break;
		case 1:
			board[YSIZE-1][UCOL]=blank;
			board[YSIZE-1][LCOL]=larrow;
			board[YSIZE-1][RCOL]=blank;
			board[YSIZE-1][DCOL]=blank;
			break;
		case 2:
			board[YSIZE-1][UCOL]=blank;
			board[YSIZE-1][LCOL]=blank;
			board[YSIZE-1][RCOL]=rarrow;
			board[YSIZE-1][DCOL]=blank;
			break;
		case 3:
			board[YSIZE-1][UCOL]=blank;
			board[YSIZE-1][LCOL]=blank;
			board[YSIZE-1][RCOL]=blank;
			board[YSIZE-1][DCOL]=darrow;
			break;
		default:
			for(x=1;x<XSIZE-1;x++)
			{
				board[YSIZE-1][x]=blank;
			}
			break;
	}
	for(x=2;x<XSIZE-2;x++)
	{
		if(board[0][x]!=blank)
		{
			error++;
		}
	}
	dir=0;
		mvaddch(1,UCOL+LEFTMOST,uarrow);mvaddch(1,DCOL+LEFTMOST,darrow);mvaddch(1,LCOL+LEFTMOST,larrow);mvaddch(1,RCOL+LEFTMOST,rarrow);
		if(board[1][UCOL]==uarrow){mvaddch(1,UCOL+LEFTMOST,uarrow|A_BOLD);}
		if(board[1][DCOL]==darrow){mvaddch(1,DCOL+LEFTMOST,darrow|A_BOLD);}
		if(board[1][LCOL]==larrow){mvaddch(1,LCOL+LEFTMOST,larrow|A_BOLD);}
		if(board[1][RCOL]==rarrow){mvaddch(1,RCOL+LEFTMOST,rarrow|A_BOLD);}
}

int main()
{
	int t=0,last=clock();
	tcount = CLOCKS_PER_SEC/3;
	init();

	nodelay(stdscr,1);
	while(1)
	{

		switch(getch())
		{
			case KEY_UP:
			case '8':
			case 'k':
			case 'w':
				check(UCOL);
				break;
			case KEY_DOWN:
			case '2':
			case 'j':
			case 's':
				check(DCOL);
				break;
			case KEY_LEFT:
			case '4':
			case 'h':
			case 'a':
				check(LCOL);
				break;
			case KEY_RIGHT:
			case '6':
			case 'l':
			case 'd':
				check(RCOL);
				break;
			case 'p':	nodelay(stdscr,0);getch();nodelay(stdscr,1);break;
			case 'q':
			exit(endwin());
			default:break;
		}

	    t=clock()-last;
        if(t>=tcount)
        {
        	adv();
        	last=clock();
        }
        if (turn>=(150+(lev*50))){newlev();turn=0;}
	}
}


