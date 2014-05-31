#include<curses.h>
#include<stdlib.h>
#include<time.h>

int direct();
int power();
int roll();
void rollback();

int score=0;

char board[24][15]={
" ___________   ",
"| _________ |##",
"||         ||##",
"|| _______ ||##",
"|||  | |  |||##",
"|||__|_|__|||##",
"||         ||##",
"||_________||##",
"|===========|##",
"|^^^^^^^^^^^|##",
"|           |##",
"|           |##",
"|           |##",
"|           |o#",
"|           |o#",
"|           |o#",
"|           |o#",
"|           |o#",
"|           |o#",
"|           |o#",
"|           |o#",
"|           |o#",
"|           |##",
"|           |##"
};

int direct()
{
	nodelay(stdscr,1);
	int t=0,last=clock(),dir=0,d;
	float tcount = CLOCKS_PER_SEC /15;

	mvprintw(19,18,"-----");
	while(1)
	{
		if(getch()==' '){nodelay(stdscr,0);return dir-3;}

	    t=clock()-last;
        if(t>=tcount)
        {
			if(++dir>5)
			{dir=1;}
			for(d=1;d<=5;d++)
			{
				if(dir>=d){mvaddch(19,17+d,'*'|COLOR_PAIR(COLOR_BLUE));}
				else {mvaddch(19,17+d,'-');}
			}
		    last=clock();
		}
	}
	return dir-3;
}

int power()
{
	nodelay(stdscr,1);
	int t=0,last=clock(),pow=0,p;
	float tcount = CLOCKS_PER_SEC /20;
	for(p=1;p<24;p++)
	{mvaddch(p,20,'|');}
	while(1)
	{
		if(getch()==' '){nodelay(stdscr,0);return pow+rand()%5-2;}
		t=clock()-last;
        if(t>=tcount)
        {
        	if(++pow>24)
        	{pow=0;}
			for(p=0;p<=24;p++)
			{
				if(pow>=p){mvaddch(p,20,'*'|COLOR_PAIR(COLOR_RED));}
				else {mvaddch(p,20,'|');}
			}
		    last=clock();
        }
	}

}

int roll()
{
	int by=23,bx=6,pow,dir,t=0,last=clock(),xmod=0,dmod=0;
	float tcount = CLOCKS_PER_SEC /8;
	mvaddch(by,bx+2,'o'|COLOR_PAIR(COLOR_GREEN));
	dir=direct();
	pow=power();
	while(1)
	{
		t=clock()-last;
        if(t>=tcount)
        {
  			xmod=(dir==0)?48:(dir==-1||dir==1)?10:5;
  			dmod=(dir==0)?rand()%3-1:(dir>0)?1:-1;
  			mvaddch(by,bx+2,board[by][bx]);
  			if(rand()%xmod==0)
  			{
  				bx+=dmod;
  				if(bx==0)
  				{bx=1;dir=1;}
  				if (bx==12)
  				{bx=11;dir=-1;}
  			}

			--by;
			mvaddch(by,bx+2,'o'|COLOR_PAIR(COLOR_GREEN));
			refresh();
			last=clock();
			if(by==0||--pow==0)
			{
				if(by>=9&&by<=23)
				{
					while(by>=9&&by<=23)
					{
						mvaddch(by,bx+2,board[by][bx]);
						by++;
						mvaddch(by,bx+2,'o'|COLOR_PAIR(COLOR_GREEN));
						refresh();
						napms(200);
					}
						dir=direct();
						pow=power();
						by=23;
						mvaddch(by,bx+2,'o'|COLOR_PAIR(COLOR_GREEN));
						refresh();
						continue;
				}
				napms(1000);
				mvaddch(by,bx+2,board[by][bx]);
				if(by==0||by==8){return 0;}
				if(bx==1||bx==11||by==1||by==7){return 1;}
				if(by==2||by==3||by==6){return 2;}
				if(bx==6){return 4;}
				return 3;
			}
		}

	}
}



int main()
{
	int x,y,s,c;
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
        init_pair(c,c,COLOR_BLACK);
    }
	mvprintw(0,0,"Eric's ASCII Arcade Demo 3\nSkee-Ball\n\nUse the spacebar to stop the direction and power timers to get the ball into the middle hole.");
	getch();clear();
	for(y=0;y<24;y++)
	{
		for(x=0;x<15;x++)
		{
			mvaddch(y,x+2,board[y][x]);
		}
	}

	for(y=0;y<9;y++)
	{
		mvprintw(19,18,"-----");
		for(x=1;x<24;x++)
		{mvaddch(x,20,'|');}
		s=roll();
		mvprintw(13+y,15,"%d",s);
		score+=s;
		mvprintw(3,23,"Score:%d",score);
		getch();
	}
	clear();
	mvprintw(5,0,"Out of balls.\nYou scored:%d points",score);
	getch();
	exit(endwin());
}
