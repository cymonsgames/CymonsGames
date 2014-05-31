#include "curses.h"
#include <stdlib.h>
#include <time.h>
#define S 40

int main() {
    srand(time(NULL));
    initscr();
    resize_term(S,60);
    curs_set(0);
    noecho();
    int h=100,t=0,p=2,x,y,a=20,b=20,level[S+1][S];
    char*m="lost";
    for(x=0;x<S;++x)
f:
    for(y=0;y<S;++y) level[x][y]=rand()%3?32:219;
    if (t++==0)level[19][20]=62;
    x=1;
    for(y=0;y<S;++y) level[x][y]=rand()%2?level[x][y] :79+(rand()%2)*0x20;
    x=39;
    for(y=0;y<S;++y) level[x][y]=rand()%150?level[x][y]:rand()%3?63:65+rand()%5;
    for(x=0;x<S;++x) for(y=0;y<S;++y) level[x][y]=level[x+1][y];
    x=a,y=b;
    if (!(level[x][y] &8)) {
        level[x][y]--;
        if (level[x][y]==64) {
            p++;
            level[x][y]=63;
        }
    }
    a=x,b=y;
    if (level[x][y]==63)a++,h+=20,level[x][y]=32;
    if (h>100)h=100;
    while (level[x][y]>63)a--,x=a;
    for(x=0;x<S;++x)	for(y=0;y<S;++y) mvaddch(y,x,level[x][y]);
    mvaddch(b,a,'@');
    mvprintw(20,50,"HP %d  ",h);
    mvprintw(21,50,"MP %d  ",p);
    mvprintw(22,50,"Turn %d",t);
    refresh();
    switch (getch()) {
    case'8':
        a--;
    case'9':
        b--;
        break;
    case'2':
        a--;
    case'3':
        b++;
        break;
    case'p':
        if (p>0)a+=p,p=0;
    }
    if (a<1||b<0||b>=S||h<=0)goto e;
    if (a>=S-1) {
        m="won";
        goto e;
    }
    h--,x=39;
    goto f;
e:
    mvprintw(20,22,"You %s in %d turns!",m,t);
    refresh();
    getch();
}
