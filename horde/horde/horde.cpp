#include <curses.h>
#include <string>
#include <time.h>
#define R rand()%22
#define L M[x][y]
#define H(a,b)for(a=0;a<b;++a)
#define F H(x,24)H(y,24)

int a,b,k,l,m,n,x,y,turn,gold,v,dist,M[24][24];char*t="789456123";

int main(){
  initscr();raw();srand(time(0));
#ifdef PDCURSES
  PDC_set_title("Horde by Jakub Debski");
#endif
  // Generate new level
	F L='.';
	H(m,9)M[a=8+R/4][b=8+R/4]='&';
	for (;;)
	{
		v=0;
		// Next season
		if (turn++%150==0)
		{
			// add gold for each cow
			F L^'&'?0:gold+=10;
			H(m,turn/600+1)
			{
				M[R%2*22][R]='a'+R%(turn/600+1);
				M[R][R%2*22]='a'+R%(turn/600+1);
			}
			v=1;
		}
		for(;;)
		{
			k=m=-1;
			F if(L>64&&L<91){m=x,n=y;};
			if (m<0) // no monsters to move this turn
				break;
			// find closest cow
			dist=99;
			k=a,l=b;
			F {
				if (L=='&'||L=='@')
				{
					if(abs(x-m)+abs(y-n)<dist)
					{
						dist=abs(x-m)+abs(y-n);
						k=x,l=y;
					}
				}
			};

			// closest cow is (k,l)
			// move monster into direction of closest cow
			v=x=m+(k-m?k<m?-1:1:0);
			y=n+(l-n?l<n?-1:1:0);

			// Check cell
			M[m][n]+=32;
			if (L>=176)
			{
				if (--L<176)
					L='.';
				if (L>177&&--M[m][n]==96)
				{
					M[m][n]='.';
					continue;
				}
			}
			else if (L=='@'||(x==a&&y==b))
				gold--;
			else if (L<'@')
			{
				L=M[m][n];
				M[m][n]='.';
			}
		}

		// Find moved monsters and convert to unmoved
		F if(L>96&&L<123)L-=32;

		F mvaddch(y,x,y^b|x^a?L:64);
		printf(" $%d T%d %s",gold,150-turn%150,gold<0?"DIE":gold>2000?"WIN":"");
		if (gold<0||gold>2000) {
		  getch ();
			return 0; // end game.
		}
		n=getch();
		if (n=='t'&&!v)
		{
			turn+=150-turn%150;
			continue;
		}
		k=l=1;
		H(m,9)n^t[m]||(l=m/3,k=m%3);x=a+k-1;y=b+l-1;
		if (x*y&&x^23&&y^23)
		{
			if (gold)
			{
				if (L>=176&&L<180)
					L++,gold--;
				if (gold>19&&n=='c'&&L!='&')
					L='&',gold-=20;
				else if(L=='@' && n!='c')
					L=176,gold--;

			}
			if (L>64&&L<91) // monsters
			{
				if(--L<65)
					L='.';
			}
			if (L=='.')
			{
				if (M[a][b]=='@')
					M[a][b]='.';
				a=x,b=y;
				if (L=='.')
					L='@';
			}
		}
	} // for(;;)
}
