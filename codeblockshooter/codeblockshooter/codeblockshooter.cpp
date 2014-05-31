/* unknown by Terry Cavanagh */
#include <allegro.h>
#include <math.h>
BITMAP *b;
class m {
public:
    double x,y,d;
    int t;
};
class pc {
public:
    void c(int i, int j, double d, int z) {
        p[n].x=i;
        p[n].y=j;
        p[n].t=z;
        p[n].d=d;
        n++;
    }
    void cp(int i, int j) {
        p[i].x=p[j].x;
        p[i].y=p[j].y;
        p[i].t=p[j].t;
        p[i].d=p[j].d;
    }
    void d(int t) {
        if (t==n-1)n--;
        else {
            for (int i=t;i<n;i++)cp(i,i+1);
            n--;
        }
    }
    m p[999];
    int n;
};

void r(int x, int y, int s, int c) {
    rectfill(b, x, y, x+s, y+s, c);
}

int main() {
    allegro_init();
    srand(time(0));
    install_keyboard();
    double x=315, y=235, d=0, t, s, f, g=0, k=0, u=0, o=0;
    int a,w=640, h=480;
    b=create_bitmap(w, h);
    pc p;
    p.n=0;
    set_gfx_mode(2, w, h, 0, 0);

    while (!key[KEY_ESC]&&u==0) {
        acquire_bitmap(b);
        clear_bitmap(b);

        for (int i=0;i<p.n;i++) {
            t=p.p[i].x;
            s=p.p[i].y;
            o=p.p[i].t;
            if (o==0) r(t, s, 2, makecol(255, 255, 255));
            if (o==1) r(t, s, 15, makecol(255, 0, 0));
        }

        circlefill(b, x, y, 5, makecol(0, 128, 255));
        t=x+(sin(d)*40);
        s=y+(cos(d)*40);
        hline(b, t-3, s, t+3, makecol(196, 196, 196));
        vline(b, t, s-3, s+3, makecol(196, 196, 196));

        blit(b, screen, 0, 0, 0, 0, w, h);
        release_bitmap(b);
        vsync();

        //input
        if (key[84]) {
            x+=sin(d)*4;
            y+=cos(d)*4;
        }
        if (key[82]) d+=0.125;
        if (key[85]) {
            x-=sin(d)*4;
            y-=cos(d)*4;
        }
        if (key[83]) d-=0.125;
        if (key[26]) f=1;
        else f=0;

        //logic
        if (d<0) d+=6.24;
        if (d>7)d-=6.24;
        if (x<0)x=0;
        if (y<0)y=0;
        if (x>w)x=w;
        if (y>h)y=h;
        if (g<0) {
            if (f==1) {
                g=10;
                p.c(x,y,d-0.1,0);
                p.c(x,y,d+0.1,0);
                p.c(x,y,d,0);
            }
        } else g--;

        //update pars
        for (int i=0;i<p.n;i++) {
            t=p.p[i].x;
            s=p.p[i].y;
            o=p.p[i].t;
            if (o==0) {
                for (int j=0;j<p.n;j++) {
                    if (p.p[j].t==1 && abs(p.p[j].x-t)<12 && abs(p.p[j].y-s)<12) p.d(j);
                }
                p.p[i].x+=sin(p.p[i].d)*10;
                p.p[i].y+=cos(p.p[i].d)*10;
                if (t<-10||t>w||s<-10||s>h) p.d(i);
            }
            if (o==1) {
                if (t>x)t--;
                if (t<x)t++;
                if (s>y)s--;
                if (s<y)s++;
                p.p[i].x=t;
                p.p[i].y=s;
                if (abs(t-x)<10 && abs(s-y)<10) u=1;
            }
        }

        if (k<0) {
            k=10;
            t=rand()%4;
            a=rand();
            if (t==0)p.c(a%w,0,0,1);
            if (t==1)p.c(a%w,w,0,1);
            if (t==2)p.c(0,a%h,0,1);
            if (t==3)p.c(w,a%h,0,1);
        } else k--;
    }
}
END_OF_MAIN()
