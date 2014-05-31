/*
    __         __
    /  /) / / /_
   /  /\ /_/ /_  Curses v0.1 (c) copyright
   ---------------------------------------
   Created by Marcus Wiseman (madmarky122)
   allegro based roguelike libary
*/


#include "TrueCurses.h"
#include <allegro.h>
#include <iostream>
#include <math.h>
#include <ctime>
#include <sstream>
#include <string>

using namespace std;

SCREEN_SETTINGS scr;
BITMAP *buffer = NULL;
float scale;
int shiftx, shifty;
BITMAP *images[100];
SAMPLE *sound[100];

void PANEL::Set(int y0,int x0, int height, int width) {
    start_x = x0;
    start_y = y0;
    end_x = start_x+width;
    end_y = start_y+height;
}

int xCenter() {
    return (scr.width/2) / 8;
}

int yCenter() {
    return (scr.height/2) / 8;
}

void PANEL::Draw(char boarder,bool transparent,int fg_color,int bg_color) {
    if (transparent == false) {
        for (int y = start_y; y < end_y; y++) {
            for (int x = start_x; x < end_x; x++) {
                mvaddch(y,x," ",fg_color,bg_color);
            }
        }
    }
    for (int y = start_y; y < end_y+1; y++) {
        mvaddch(y,start_x,c_char(boarder),fg_color,bg_color);
        mvaddch(y,end_x,c_char(boarder),fg_color,bg_color);
    }
    for (int x = start_x; x < end_x+1; x++) {
        mvaddch(start_y,x,c_char(boarder),fg_color,bg_color);
        mvaddch(end_y,x,c_char(boarder),fg_color,bg_color);
    }
}

void PANEL::Text(const char *text, int bound_y, int bound_x,int fg_color,int bg_color) {
    int tx = start_x + bound_x;
    int ty = start_y + bound_y;
    mvprintw(ty,tx,text,fg_color,bg_color);
}

const char *add_cchar(const char *add0, const char *add1) {
    string temp;
    temp = add0;
    temp += add1;
    return temp.c_str();
}

string IntToString(int intValue) {
    char *myBuff;
    string strRetVal;
    myBuff = new char[100];
    memset(myBuff,'\0',100);
    itoa(intValue,myBuff,10);
    strRetVal = myBuff;
    delete[] myBuff;
    return(strRetVal);
}

string IntToCStr(int intValue) {
    char *myBuff;
    string strRetVal;
    myBuff = new char[100];
    memset(myBuff,'\0',100);
    itoa(intValue,myBuff,10);
    strRetVal = myBuff;
    delete[] myBuff;
    return(strRetVal.c_str());
}

const char *add_ichar(const char *add0, int add1) {
    string temp;
    temp = add0;
    temp += IntToString(add1);
    return temp.c_str();
}

int color_t(int r, int g, int b, int alpha) {
    set_alpha(alpha);
    return makeacol(r,g,b,alpha);
}

void set_alpha(int a) {
    set_trans_blender(0, 0, 0,a);
}

bool initscr(int height, int width,int color_depth,int font_scale_factor=8, int mode=GFX_AUTODETECT) {
    allegro_init();
    install_keyboard();
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0) != 0)
    {
        cout << "error loading sound" << endl;
    }
    set_color_depth(color_depth);
    scr.color_depth = color_depth;
    scr.width = width;
    scr.height = height;
    float scalex = 800.0 / (float)width;
    float scaley = 600.0 / (float)height;
    scale = (scalex < scaley) ? scalex : scaley;
    shiftx = (800 - (width * scale)) / 2;
    shifty = (600 - (height * scale)) / 2;
    scr.window = true;
    scr.font_scale_factor = font_scale_factor;
    buffer = create_bitmap_ex(color_depth,width,height);
    set_trans_blender(0, 0, 0,255);
    drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
    if (set_gfx_mode(mode, 800, 600, 0, 0) == false) {
        return false;
    } else {
        return true;
    }
}

int blend_factor(int a) {
    return a-255;
}

char mvgetch(char Map[MAPY][MAPX],int y, int x) {
    return Map[y][x];
}

int basic_movement_x(char Map[MAPY][MAPX],int y0,int x0,PASSABLE pass[]) {
    for (int i = 0; i < 100; i++) {
        if (key[KEY_RIGHT]) {
            if (mvgetch(Map,y0,x0+1) == pass[i]) {
                return x0+1;
            }
        }
        if (key[KEY_LEFT]) {
            if (mvgetch(Map,y0,x0-1) == pass[i]) {
                return x0-1;
            }
        }
    }
    return x0;
}

int basic_movement_y(char Map[MAPY][MAPX],int y0,int x0,PASSABLE pass[]) {
    for (int i = 0; i < 100; i++) {
        if (key[KEY_DOWN]) {
            if (mvgetch(Map,y0+1,x0) == pass[i]) {
                return y0+1;
            }
        }
        if (key[KEY_UP]) {
            if (mvgetch(Map,y0-1,x0) == pass[i]) {
                return y0-1;
            }
        }
    }
    return y0;
}

int getcolor(int y, int x) {
    return getpixel(buffer,x*scr.font_scale_factor,y*scr.font_scale_factor);
}

int dist(int y0,int x0,int y1,int x1) {
    return pow(y0-y1,2)+pow(x0-x1,2);
}

bool in_range(int y0,int x0,int y1,int x1,int r) {
    return dist(y0,x0,y1,x1)<=pow(r,2);
}

void DrawNoise(int y,int x,int size,int r,int g,int b) {
    for (int y0 = y; y0<y+size; y0++) {
        for (int x0 = x; x0<x+size; x0++) {
            int rd = rand()%255;
            putpixel(buffer,x0,y0,color( r+rd , g+rd , b+rd ));
        }
    }
}

void DrawPixelGrad(int y,int x,int size,int r,int g, int b,bool reverse,int blend_factor) {
    int tr = r;
    int tg = g;
    int tb = b;
    y = y*scr.font_scale_factor;
    x = x*scr.font_scale_factor;
    for (int y0 = y; y0<y+size*8; y0++) {
        for (int x0 = x; x0<x+size*8; x0++) {
            putpixel(buffer,x0,y0,color(tr,tg,tb));
        }
        if (reverse == true) {
            tr-= blend_factor;
            tg-= blend_factor;
            tb-= blend_factor;
            if (tr < 0) {
                tr = 0;
            }
            if (tg < 0) {
                tg = 0;
            }
            if (tb < 0) {
                tb = 0;
            }
        } else {
            tr+= blend_factor;
            tg+= blend_factor;
            tb+= blend_factor;
            if (tr > 255) {
                tr = 255;
            }
            if (tg > 255) {
                tg = 255;
            }
            if (tb > 255) {
                tb = 255;
            }
        }
    }
}

void DrawPixel(int y,int x,int size,int color) {
    for (int y0 = y; y0<y+size; y0++) {
        for (int x0 = x; x0<x+size; x0++) {
            putpixel(buffer,x0,y0,color);
        }
    }
}

bool los(int y0,int x0,int y1,int x1,char opaque,char Map[MAPY][MAPX]) {
    bool steep=fabs(y1-y0)>fabs(x1-x0);
    if (steep) {
        swap(x0,y0);
        swap(x1,y1);
    }
    if (x0>x1) {
        swap(x0,x1);
        swap(y0,y1);
    }
    float err_num=0.0;
    int y=y0;
    for (int x=x0; x<=x1; x++) {
        if (x>x0 && x<x1) {
            if (steep) {
                if (opaque==Map[x][y])
                    return false;
            } else {
                if (opaque==Map[y][x])
                    return false;
            }
        }

        err_num+=(float)(fabs(y1-y0))/(float)(x1-x0);
        if (0.5<fabs(err_num)) {
            y+=y1>y0?1:-1;
            err_num--;
        }
    }
    return true;
}

void Blend_LightSource(char Map[MAPY][MAPX],char wall, char shadow,
                       int y0, int x0,int raduis,bool glow
                       ,int COLOR,int wall_r0,int wall_g0, int wall_b0,int blend_factor) { // multi fov (fade out fov)
    int tmp = 0;
    if (glow == true) {
        tmp = rand()%1;
        if (tmp == 0) {
            raduis+=rand()%2;
        } else {
            raduis-=rand()%2;
        }
    }
    if (raduis < 0) {
        raduis = 0;
    }
    for (int i = 0; i < 255; i+=blend_factor) {
        if (COLOR == COLOR_YELLOW) {
            blend_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, i,i,0, 0,0,0, 50,50,50, blend_factor);
        } else if (COLOR == COLOR_RED) {
            blend_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, i,0,0, 0,0,0, 50,50,50, blend_factor);
        } else if (COLOR == COLOR_GREEN) {
            blend_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, 0,i,0, 0,0,0, 50,50,50, blend_factor);
        } else if (COLOR == COLOR_BLUE) {
            blend_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, 0,0,i, 0,0,0, 50,50,50, blend_factor);
        } else if (COLOR == COLOR_PURPLE) {
            blend_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, i,0,i, 0,0,0, 50,50,50, blend_factor);
        } else if (COLOR == COLOR_WHITE) {
            blend_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, i,i,i, 0,0,0, 50,50,50, blend_factor);
        } else if (COLOR == COLOR_BLACK) {
            blend_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, 25,25,25, 0,0,0, 50,50,50, blend_factor);
        } else if (COLOR == COLOR_CYAN) {
            blend_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, 0,i,i, 0,0,0, 50,50,50, blend_factor);
        } else {
            blend_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, i,i,i, 0,0,0, 50,50,50, blend_factor);
        }
        raduis--;
        if (raduis < 0) {
            raduis = 0;
        }
    }
}

void Solid_LightSource(char Map[MAPY][MAPX],char wall, char shadow,
                       int y0, int x0,int raduis,bool glow
                       ,int C_W_F_COL, int C_W_B_COL
                       ,int C_S_F_COL, int C_S_B_COL) {
    int tmp = 0;
    if (glow == true) {
        tmp = rand()%1;
        if (tmp == 0) {
            raduis+=rand()%4;
        } else {
            raduis-=rand()%4;
        }
    }
    if (raduis < 0) {
        raduis = 0;
    }
    solid_fov(Map,y0,x0,raduis,wall,shadow,C_W_F_COL,C_W_B_COL,C_S_F_COL,C_S_B_COL);
}

void SolidBlend_LightSource(char Map[MAPY][MAPX],char wall, char shadow,
                            int y0, int x0,int raduis,bool glow
                            ,int COLOR,int wall_r0,int wall_g0,int wall_b0,int blend_rate) {
    int tmp = 0;
    if (glow == true) {
        tmp = rand()%1;
        if (tmp == 0) {
            raduis+=rand()%2;
        } else {
            raduis-=rand()%2;
        }
    }
    if (raduis < 0) {
        raduis = 0;
    }

    for (int i = 0; i < 255; i+=blend_rate) {
        if (COLOR == COLOR_YELLOW) {
            solidb_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, i,i,0, 0,0,0, 50,50,50, blend_rate);
        } else if (COLOR == COLOR_RED) {
            solidb_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, i,0,0, 0,0,0, 50,50,50, blend_rate);
        } else if (COLOR == COLOR_GREEN) {
            solidb_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, 0,i,0, 0,0,0, 50,50,50, blend_rate);
        } else if (COLOR == COLOR_BLUE) {
            solidb_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, 0,0,i, 0,0,0, 50,50,50, blend_rate);
        } else if (COLOR == COLOR_PURPLE) {
            solidb_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, i,0,i, 0,0,0, 50,50,50, blend_rate);
        } else if (COLOR == COLOR_WHITE) {
            solidb_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, i,i,i, 0,0,0, 50,50,50, blend_rate);
        } else if (COLOR == COLOR_BLACK) {
            solidb_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, 25,25,25, 0,0,0, 50,50,50, blend_rate);
        } else if (COLOR == COLOR_CYAN) {
            solidb_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, 0,i,i, 0,0,0, 50,50,50, blend_rate);
        } else {
            solidb_fov(Map,y0,x0,raduis,wall,shadow, wall_r0,wall_g0,wall_b0, i,i,i, 0,0,0, 50,50,50, blend_rate);
        }
        raduis--;
        if (raduis < 0) {
            raduis = 0;
        }
    }
}

void solid_fov(char Map[MAPY][MAPX],int y0,int x0,
               int raduis,char wall,char shadow,
               int C_W_F_COL, int C_W_B_COL,
               int C_S_F_COL, int C_S_B_COL) {
    int raduisy = raduis*2;
    for (int yy = y0-raduisy; yy <= y0+raduisy; yy++) {
        for (int xx = x0-raduis; xx <= x0+raduis; xx++) {
            if (Map[yy][xx] != 0) {
                if  (in_range(y0,x0,yy,xx,raduis)
                        && los(y0,x0,yy,xx,wall,Map))  {
                    if (Map[yy][xx] == wall) {
                        mvaddch(yy,xx,c_char(Map[yy][xx]),C_W_F_COL,C_W_B_COL);
                    }
                } else {
                    if (in_range(y0,x0,yy,xx,raduis)) {
                        mvaddch(yy,xx,c_char(Map[yy][xx]),C_S_F_COL,C_S_B_COL);
                    }
                }
            }
        }
    }
}

void solidb_fov(char Map[MAPY][MAPX],int y0,int x0,
                int raduis,char wall,char shadow,
                int r0,int g0,int b0,
                int r1,int g1,int b1,
                int r2,int g2,int b2,
                int r3,int g3,int b3,int blend_factor) {
    int raduisy = raduis*2;
    for (int yy = y0-raduisy; yy <= y0+raduisy; yy++) {
        for (int xx = x0-raduis; xx <= x0+raduis; xx++) {
            if (Map[yy][xx] != 0) {
                if  (in_range(y0,x0,yy,xx,raduis)
                        && los(y0,x0,yy,xx,wall,Map))  {
                    if (Map[yy][xx] == wall) {
                        mvaddch(yy,xx,c_char(Map[yy][xx]),color(r0,g0,b0),color(r1,g1,b1));
                    }
                    DrawPixel(yy*8,xx*8,8,color_t(r1,g1,b1,100));
                } else {
                    if (in_range(y0,x0,yy,xx,raduis)) {
                        if (shadow == 0) {
                            DrawPixel(yy*8,xx*8,8,color_t(r3,g3,b3,10));
                        } else {
                            mvaddch(yy,xx,c_char(Map[yy][xx]),color(r2,g2,b2),color(r3,g3,b3));
                        }
                    }
                }
            }
        }
    }
}

void blend_fov(char Map[MAPY][MAPX],int y0,int x0,
               int raduis,char wall,char shadow,
               int r0,int g0,int b0,
               int r1,int g1,int b1,
               int r2,int g2,int b2,
               int r3,int g3,int b3,int blend_factor) {
    int raduisy = raduis*2;
    for (int yy = y0-raduisy; yy <= y0+raduisy; yy++) {
        for (int xx = x0-raduis; xx <= x0+raduis; xx++) {
            if (Map[yy][xx] != 0) {
                if  (in_range(y0,x0,yy,xx,raduis)
                        && los(y0,x0,yy,xx,wall,Map))  {
                    int rd = rand()%(blend_factor*2);
                    if (Map[yy][xx] == wall) {
                        mvaddch(yy,xx,c_char(Map[yy][xx]),color(r0,g0,b0),color(r1+rd,g1+rd,b1+rd));
                    }
                    DrawPixel(yy*8,xx*8,8,color_t(r1+rd,g1+rd,b1+rd,100));
                } else {
                    if (in_range(y0,x0,yy,xx,raduis)) {
                        if (shadow == 0) {
                            DrawPixel(yy*8,xx*8,8,color_t(r3,g3,b3,10));
                        } else {
                            mvaddch(yy,xx,c_char(Map[yy][xx]),color(r2,g2,b2),color(r3,g3,b3));
                        }
                    }
                }
            }
        }
    }
}

void RecFill(char Map[MAPY][MAPX],int yy0, int xx0, int yy1, int xx1, char wall,char floor) {
    // fill with walls to do the outside first
    for (int y = yy0; y < yy1; y++) {
        for (int x = xx0; x < xx1; x++) {
            Map[y][x] = wall;
        }
    }
    yy0++;
    yy1--;
    xx0++;
    xx1--;
    for (int y = yy0; y < yy1; y++) {
        for (int x = xx0; x < xx1; x++) {
            Map[y][x] = floor;
        }
    }
}

void CaveGenMap(char Map[MAPY][MAPX], int YSIZE,int XSIZE, char fill,char sub,int steps) {
    for (int y = 0; y < YSIZE; y++) {
        for (int x = 0; x < XSIZE; x++) {
            Map[y][x] = fill; // fill map with fill char
        }
    }
    int randx = rand()%XSIZE;
    int randy = rand()%YSIZE;
    int dir = 0;
    for (int movecount = 0; movecount < steps; movecount++) {
        dir = rand()%4+1;
        if (dir == 1) { // go up
            if (randy > 1) {
                randy--;
            }
        }
        if (dir == 2) { // go down
            if (randy < YSIZE-2) {
                randy++;
            }
        }
        if (dir == 3) { // go left
            if (randx > 1) {
                randx--;
            }
        }
        if (dir == 4) { // go right
            if (randx < XSIZE-2) {
                randx++;
            }
        }
        Map[randy][randx] = sub; // subtract map with sub char
    }

    int s = 0;
    while (s == 0) {
        int tempx = rand()%XSIZE;
        int tempy = rand()%YSIZE;

        if ( (Map[tempy-1][tempx] == sub)
                && (Map[tempy+1][tempx] == sub)
                && (Map[tempy][tempx+1] == sub)
                && (Map[tempy][tempx-1] == sub)
                && (Map[tempy-1][tempx+1] == sub)
                && (Map[tempy+1][tempx-1] == sub)
                && (Map[tempy+1][tempx-1] == sub)
                && (Map[tempy-1][tempx+1] == sub)) {
            Map[tempy][tempx] = '>';
            s = 1;
        }
    }
    s = 0;
    while (s == 0) {
        int tempx = rand()%XSIZE;
        int tempy = rand()%YSIZE;

        if (Map[tempy][tempx] == sub) {
            if ( (Map[tempy-1][tempx] == sub)
                    && (Map[tempy+1][tempx] == sub)
                    && (Map[tempy][tempx+1] == sub)
                    && (Map[tempy][tempx-1] == sub)
                    && (Map[tempy-1][tempx+1] == sub)
                    && (Map[tempy+1][tempx-1] == sub)
                    && (Map[tempy+1][tempx-1] == sub)
                    && (Map[tempy-1][tempx+1] == sub)) {
                Map[tempy][tempx] = '<';
                s = 1;
            }
        }
    }
}


int collide_detect(int xmin1, int ymin1, BITMAP *sprite1, int xmin2, int ymin2, BITMAP *sprite2) {
    int xmax1 = xmin1 + sprite1->w, ymax1 = ymin1 + sprite1->h;
    int xmax2 = xmin2 + sprite2->w, ymax2 = ymin2 + sprite2->h;
    int xmin = max(xmin1, xmin2);
    int ymin = max(ymin1, ymin2);
    int xmax = min(xmax1, xmax2);
    int ymax = min(ymax1, ymax2);
    if (xmax <= xmin || ymax <= ymin) {
        return 0;
    }
    int mask1 = bitmap_mask_color(sprite1);
    int mask2 = bitmap_mask_color(sprite2);
    for (int y = ymin; y < ymax; y++) {
        for (int x = xmin; x < xmax; x++) {
            int x1 = x - xmin1, y1 = y - ymin1;
            int x2 = x - xmin2, y2 = y - ymin2;
            int color1 = getpixel(sprite1, x1, y1);
            int color2 = getpixel(sprite2, x2, y2);
            if (color1 != mask1 && color2 != mask2) {
                return 1;
            }
        }
    }
    return 0;
}

bool infov(char Map[MAPY][MAPX],int y0,int x0,int raduis,char wall) {
    int raduisy = raduis*2;
    for (int yy = y0-raduisy; yy <= y0+raduisy; yy++) {
        for (int xx = x0-raduis; xx <= x0+raduis; xx++) {
            if (Map[yy][xx] != 0) {
                if  (in_range(y0,x0,yy,xx,raduis)
                        && los(y0,x0,yy,xx,wall,Map))  {
                    return true;
                }
            }
        }
    }
    return false;
}

void clear() {
    clear_bitmap(buffer);

}

void Floodfill(int y, int x, int color)
{
    floodfill(buffer,x,y,color);
}

int color(int r, int g, int b) {
    return makecol(r,g,b);
}

int getch() {
    return readkey();
}

bool getkey(int input) {
    if (key[input]) {
        return true;
    } else {
        return false;
    }
}

void refresh() {
    // blit(buffer,screen,0,0,0,0,scr.width,scr.height);
    stretch_sprite(screen, buffer, shiftx, shifty, scr.width * scale, scr.height * scale);
}

void freebuf() {
    destroy_bitmap(buffer);
    for (int i = 0; i < 100; i++) {
        destroy_bitmap(images[i]);
        destroy_sample(sound[i]);
    }
    remove_sound();
}

bool initSound(int id, bool play, string soundpath)
{
    sound[id] = load_sample(soundpath.c_str());
    if (!sound[id])
    {
        //allegro_message("Error Loading Sound File!");
        return false;
    } else {
        if (play)
        {
            play_sample(sound[id], 255, 128, 1000, 0);
        }
    }
    return true;
}

bool PlaySound(int id)
{
    if (!sound[id])
    {
        //allegro_message("Error Loading Sound File!");
        return false;
    } else {
        play_sample(sound[id], 255, 128, 1000, 0);
    }
    return true;
}

bool StopSound(int id)
{
    if (!sound[id])
    {
        //allegro_message("Error Loading Sound File!");
        return false;
    } else {
        stop_sample(sound[id]);
    }
    return true;
}

int bound(int i,int min,int max) {
    if (i>max) {
        i=max;
    }
    if (i<min) {
        i=min;
    }
    return i;
}

void mvaddch(int y, int x,const char *c,int foreground_color,int background_color) {

    textout_ex(buffer, font, c, x*scr.font_scale_factor, y*scr.font_scale_factor, foreground_color, background_color);
}

void mvaddch_offset(int y, int x,int x_offset, int y_offset,const char *c,int foreground_color,int background_color) {

    textout_ex(buffer, font, c, (x*scr.font_scale_factor)+x_offset, (y*scr.font_scale_factor)+y_offset, foreground_color, background_color);
}

const char *c_char(char c) {
    string temp;
    temp = c;
    return temp.c_str();
}

void napms(int time) {
    rest(time);
}

bool IsAtPoint(int y0, int x0,int y1, int x1) {
    if (y0 == y1) {
        if (x0 == x1) {
            return true;
        }
    }
    return false;
}

void mvprintw(int y, int x,const char *c,int foreground_color,int background_color) { // add string at position , set color
    textout_ex(buffer, font, c, x*scr.font_scale_factor, y*scr.font_scale_factor, foreground_color, background_color);
}

void DrawBitmap(int ID, int y0, int x0, bool masked, int scalef) {
    if (images[ID] != 0) {
        if (masked == true ) {
            masked_blit(images[ID],buffer,0,0,x0*scalef,y0*scalef,images[ID]->w,images[ID]->h);
        } else {
            blit(images[ID],buffer,0,0,x0*scalef,y0*scalef,images[ID]->w,images[ID]->h);
        }
    }

}

void initBitmap(int ID,const char *filename) {
    images[ID] = load_bitmap(filename,NULL);
}
