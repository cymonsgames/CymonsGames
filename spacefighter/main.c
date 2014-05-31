#include <curses.h>
#include<stdlib.h>
#include<time.h>

#define PX 5
#define YSIZE 15
#define XSIZE 80
#define TOPMOST (LINES-YSIZE)/2
#define LIVES 5
#define MIDDLE YSIZE/2
#define TURNS 200
#define BOTTOM TOPMOST+YSIZE
#define ECHANCE 100
#define SHOTS 20
#define PCHANCE 1000
#define CCHANCE 25

chtype ship='>'|COLOR_PAIR(COLOR_WHITE);
chtype flat='_'|COLOR_PAIR(COLOR_CYAN);
chtype uslope='/' |COLOR_PAIR(COLOR_CYAN);
chtype dslope='\\'|COLOR_PAIR(COLOR_CYAN);
chtype space=' '|COLOR_PAIR(COLOR_BLACK);
chtype full='.'|COLOR_PAIR(COLOR_CYAN);
chtype flame='='|COLOR_PAIR(COLOR_RED);
chtype alien='&'|COLOR_PAIR(COLOR_GREEN);
chtype explode='*'|COLOR_PAIR(COLOR_RED);
chtype shot='-'|COLOR_PAIR(COLOR_WHITE);
chtype pup='*'|COLOR_PAIR(COLOR_WHITE);
chtype civ='P'|COLOR_PAIR(COLOR_WHITE);
chtype alien2='&'|COLOR_PAIR(COLOR_BLUE);
chtype shield=')'|COLOR_PAIR(COLOR_WHITE);
chtype wall='|'|COLOR_PAIR(COLOR_CYAN);


chtype level[YSIZE][XSIZE];


int py=YSIZE/2,y=0,x=0,life=LIVES,score=0,top=0,bot=YSIZE-1,ltop=0,lbot=YSIZE-1,lv=0,echance=100,turn=TURNS,last=0,t=0,mid=MIDDLE,shots=0,sh=0;
float magicNumber= CLOCKS_PER_SEC;


void empty();
void adv();
void init();
void die();
void draw();
void newlev();
void play();
void again();
void laser();


void empty()
{
	py=YSIZE/2;
    for (y=0;y<YSIZE;y++)
    {
        for (x=0;x<XSIZE;x++)
        {
			level[y][x]=(y==0||y==YSIZE-1)?flat:space;
		}
	}
}

void adv()
{
	turn--;
	int num=0;
    for (y=0;y<YSIZE;y++)
    {
        for (x=0;x<XSIZE-1;x++)
        {
			level[y][x]=level[y][x+1];
			if (level[y][x]==alien)
			{
				num=rand()%4;
				if (num==0&&level[y+1][x]==space)
				{
					level[y][x]=space;
					level[y+1][x]=alien;
					mvaddch(y+1+TOPMOST,x,level[y+1][x]);
				}
				if(num==1&&level[y-1][x]==space)
				{
					level[y][x]=space;
					level[y-1][x]=alien;
					mvaddch(y-1+TOPMOST,x,level[y-1][x]);
				}
				if(level[y+1][x]==civ)
				{level[y+1][x]==alien;}
			}
			if(level[y][x]==alien2)
			{
				if(py<y&&level[y-1][x]==space)
				{
					level[y][x]=space;
					level[y-1][x]=alien2;
					mvaddch(y-1+TOPMOST,x,level[y-1][x]);
				}
				if(py>y&&level[y+1][x]==space)
				{
					level[y][x]=space;
					level[y+1][x]=alien2;
					mvaddch(y+1+TOPMOST,x,level[y+1][x]);
				}
				if(py==y)
				{
					num=rand()%3;
					if (num==0&&level[y+1][x]==space)
					{
						level[y][x]=space;
						level[y+1][x]=alien2;
						mvaddch(y+1+TOPMOST,x,level[y+1][x]);
					}
					if(num==1&&level[y-1][x]==space)
					{
						level[y][x]=space;
						level[y-1][x]=alien2;
						mvaddch(y-1+TOPMOST,x,level[y-1][x]);
					}
				}
				if(level[y+1][x]==civ)
				{level[y+1][x]=alien;}
			}
			mvaddch(y+TOPMOST,x,level[y][x]);
		}
	}
	num=rand()%6;
    mid+=(num==0&&mid>YSIZE/4)?-1:(num==5&&mid<(YSIZE/4)*3)?1:0;
	num=rand()%4;
	top+=(num==0&&top>0)?-1:(num==3&&top<mid-2)?1:0;
	num=rand()%5;
	bot+=(num==0&&bot<YSIZE-1)?1:(num>2&&bot>mid+2)?-1:0;
	level[0][XSIZE-1]=flat;
	level[YSIZE-1][XSIZE-1]=flat;
	for (y=0;y<YSIZE-1;y++)
    {
		level[y][XSIZE-1]=space;
		if(y<top||y>bot){level[y][XSIZE-1]=full;}
		if(y==top&&top==ltop){level[y][XSIZE-1]=flat;}
		if(y==bot&&bot==lbot){level[y][XSIZE-1]=flat;}
		if(y==top&&top<ltop){level[y][XSIZE-1]=uslope;}
		if(y==top&&top>ltop){level[y][XSIZE-1]=dslope;}
		if(y==bot&&bot<lbot){level[y][XSIZE-1]=uslope;}
		if(y==bot&&bot>lbot){level[y][XSIZE-1]=dslope;}
		if(y==0||y==YSIZE-1){level[y][XSIZE-1]=flat;}
		if (level[y][XSIZE-1]==space&&rand()%echance==0){level[y][XSIZE-1]=(rand()%4==0)?alien2:alien;}
		if (level[y][XSIZE-1]==space&&rand()%PCHANCE==0){level[y][XSIZE-1]=pup;}
		if (level[y][XSIZE-1]==space&&rand()%PCHANCE==0){level[y][XSIZE-1]=shield;}
		if (level[y][XSIZE-1]==space&&rand()%CCHANCE==0&&level[y+1][XSIZE-1]==flat){level[y][XSIZE-1]=civ;}
		mvaddch(y+TOPMOST,XSIZE-1,level[y][XSIZE-1]);
	}
	ltop=top;
	lbot=bot;
	mvaddch(py+TOPMOST,PX,ship);
	mvaddch(py+TOPMOST,PX-1,flame);
	if(level[py][PX]==civ)
	{
		level[py][PX]=space;
		score+=lv+1;
		draw();

	}
	if(level[py][PX]==pup)
	{
		level[py][PX]=space;
		shots+=score*2+1;
		if(score>50){life++;}
		score=0;
		draw();
	}
	if(level[py][PX]==shield)
	{
		sh++;
		level[py][PX]=space;
	}
	if(sh>0)
	{
		mvaddch(py+TOPMOST,PX+1,shield);
		if(level[py][PX+1]==flat||level[py][PX+1]==uslope||level[py][PX+1]==dslope||level[py][PX+1]==alien||level[py][PX+1]==alien2)
		{
			level[py][PX+1]=space;
			sh--;
		}
	}
	if (level[py][PX]!=space)die();
	if(turn==0){newlev;}
}

void init ()
{
    int c;
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
}

void die()
{
	nodelay(stdscr,0);
	beep();
	life--;
	shots=SHOTS;
	score=0;
	sh=0;
	mvaddch(py+TOPMOST,PX,'*'|COLOR_PAIR(COLOR_RED));
	getch();
	empty();
	clear();
	lbot=YSIZE-1;
	bot=YSIZE-2;
	ltop=0;
	top=1;
	py=mid;
	if (life<1){again();}
	draw();
	nodelay(stdscr,1);
}

void newlev()
{
  lv++;
  turn=TURNS*lv;
	shots+=SHOTS;
  magicNumber = CLOCKS_PER_SEC /((float)lv*2+5);
	if(ECHANCE-(lv*2)>5);
	echance=ECHANCE-(lv*2);
}

void draw()
{
    for (y=0;y<YSIZE;y++)
    {
        for (x=0;x<XSIZE-1;x++)
        {
			mvaddch(y+TOPMOST,x,level[y][x]);
		}
	}
	mvaddch(py+TOPMOST,PX,ship);
	mvaddch(py+TOPMOST,PX-1,flame);
	for (x=0;x<life;x++){mvaddch(BOTTOM,x,'>');}
	mvprintw(BOTTOM+1,0,"Ammo:%d   Score:%d   ",shots,score);
}

void play()
{
	newlev();
	clear();
	empty();
	nodelay(stdscr,1);
	draw();
	last=clock();
	while(1)
	{
		switch(getch())
		{
		case KEY_UP:
		case'8':
		case'k':
			--py;adv();last=clock();break;
		case KEY_DOWN:
		case '2':
		case'j':
			++py;adv();last=clock();break;
		case KEY_RIGHT:
		case'6':
		case'l':
			adv();last=clock();break;
		case' ':
			if(shots>0){
			laser();}break;
		case'p':nodelay(stdscr,0);getch();nodelay(stdscr,1);break;
		case 'q':
			again();
		default: break;

		}
        t=clock()-last;
        if (t>=magicNumber)
        {
            adv();
			last=clock();
        }
	}
}

void laser()
{
	shots--;
	int l=0;
	for(l=PX+1;l<XSIZE;l++)
	{

		mvaddch(py+TOPMOST,l,shot);
		if(level[py][l]==alien||level[py][l]==alien2)
		{
			mvaddch(py+TOPMOST,l,explode);
			score++;
			level[py][l]=space;
			break;
		}
		if(level[py][l]!=space)
		{
			if(level[py][l]==pup)
			{
				level[py][l]=space;
				shots+=2;
				mvaddch(py+TOPMOST,l,explode);
				laser();
				laser();
			}
			if(level[py][l]==civ)
			{
				mvaddch(py+TOPMOST,l,explode);
				score--;
				level[py][l]=space;
			}
			if(level[py][l]==shield)
			{
				level[py][l]=wall;
			}
			break;
		}
	}

	refresh();
	napms(100);
	draw();
}

void again()
{
	nodelay(stdscr,0);
	clear();
	py=YSIZE/2;
	y=0;x=0;
	life=LIVES;
	score=0;top=0;
	bot=YSIZE-1;
	ltop=0;
	lbot=YSIZE-1;
	lv=0;
	echance=100;
	turn=TURNS;
	last=0;t=0;
	mid=MIDDLE;
	shots=0;sh=0;
	mvprintw(0,0,"Play again??");
	switch(getch())
	{
		case'y':
		case'Y':
		play();break;
		case'n':
		case'N':
		exit(endwin());
		default:again();
	}

}

int main(int argc,char* argv[])
{
    init();
	mvprintw(0,0,"Welcome to genEric ASCII Space Shooter\nBy J.E. Ward (Idontexist)\n\nWarning\tThis game is still buggy.");
	mvaddch(6,0,flame);mvaddch(6,1,ship);mvprintw(6,3,"- Move your ship and use the space bar to fire");

	mvaddch(8,0,pup);mvaddch(8,1,shield);mvprintw(8,3,"- Powerups- Ammo and shields");
	mvaddch(10,0,alien);mvaddch(10,1,alien2);mvprintw(10,3,"- Bad guys. Dodge or kill 'em");
	mvaddch(12,0,civ);mvprintw(12,2,"- Innocent people. Save them before the aliens get them");

	getch();
	play();
	exit(endwin());
}
