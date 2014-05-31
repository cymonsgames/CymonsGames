#include "curses.h"
#include <string>
#include <time.h>
#define B rand()%24
#define L M[x][y]
#define O N[x][y]
#define H(a,b)for(a=0;a<b;++a)
#define F H(x,24)H(y,24)
#define T H(k,3)H(l,3)
#define Q if(L>96&&M[a][b]<60)if(abs(x-f)<2&&abs(y-i)<2)h--,L-=32;else M[a]\
  [b]=L-32,L=46;O=L;if(L<36)break;

int c,e,x,y,k,l,f,i,h=20,g,d=7,M[24][24],N[24][24],v,w,a,b,r,s;
char*t="789456123";
float m,n,o,p;

int main() {
    initscr();
    raw();
    srand(time(0));
j:
    d--;
    F L=35;
    M[9][9]=46;
    H(r,9999) {
        x=B,y=B,a=2;
        T M[x+k-1][y+l-1]^35||a--;
        a*a>4&&x*y&&x^23&&y^23?L=B?f=x,i=y,46:B/9?97+B/(d+2):43:0;
    }
    L=62;
    for (;;) {
        F O=L^35?32:37;
        F {v=f,w=i,c=x,e=y;
        if (abs(o=x-v)>abs(p=y-w)) {
        m=p/o,n=w-m*v;
        while (v^c) {
                a=v,b=s,x=v+=o<0?-1:1,y=s=m*v+n;
                Q
            }
        } else {
            m=o/p,n=v-m*w;
            while (w^e) {
                a=r,b=w,y=w+=p<0?-1:1,x=r=m*w+n;
                Q
            }
        }
        x=c,y=e;
          }
        F L>64&L<91&&(O=L+=32);
        F mvaddch(y,x,y^i|x^f?O:64);
        printf(" HP%d $%d L%d ",h,g,9-d);
        x=getch();
        if (h*d<0)break;
        H(a,9)x^t[a]||(l=a/3,k=a%3);
        x=f+k-1;
        y=i+l-1;
        if (L>96) {
            if (--L==96)g++,L=59;
        } else if (L!=35)f=x,i=y;
        if (L==62)goto j;
        L^43||(h+=5,L=46);
    }
}
