#include <iostream>
#include "TrueCurses.h"

using namespace std;

class pl {
public:
    int x;
    int y;
    int aim_dir;
    int aim_x;
    int aim_y;
    bool inair;
    int level;
    bool warp;
};

pl p1;

char Map[MAPY][MAPX];

char Maps[10][16][31] = {
{
    "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv",
    ">             _   _  ___  __ <",
    "> \\    / /\\  |_) |_)  |  |_  <",
    ">  \\/\\/ /--\\ |\\, |   _|_ |__ <",
    ">                            <",
    ">    Press 'q' key to exit   <",
    ">^^^^^^^^^^^^^^^^^^^^^^^^^^^^<",
    ">        vvvvvvvvvvvv        <",
    ">        >Start Game<        <",
    ">        ^^^^^^^^^^^^        <",
    ">             O              <",
    ">                            <",
    ">                            <",
    ">                            <",
    "|              &             |",
    "------------------------------",
},{
    "------------------------------",
    "|vvvvvvvvvvv|vvvvvvvvv|vvvvvv|",
    "|>         <|>        |      |",
    "|>         <|>        |------|",
    "| &        <|>        |>     |",
    "|----------------------------|",
    "|>vvvvvvvvvvv          <|vvvv|",
    "|>                     <|   <|",
    "|>                     <|   <|",
    "|>       ^^^^^^^^       |    |",
    "|>       --------------------|",
    "|>      <|>vvvvvvvvvvvv<|vvv |",
    "|>      <|>            <|    |",
    "|>      <|>            <|    |",
    "|>^^^^^^<|>^^^^^^^^^^^^<|   O|",
    "------------------------------",
},{
    "  Warpie can warp in mid air  ",
    "------------------------------",
    "|>   |vvvvvvvvvvvvvvvvv|    <|",
    "|>   |                 |    <|",
    "|>   |                 |    <|",
    "|>   |                 |    <|",
    "|>   |                 |    <|",
    "|>   |                 |    <|",
    "|>   |                 |    <|",
    "|>   |                 |    <|",
    "|>   |                 |    <|",
    "|>   |                 |    <|",
    "|>   |                 |    <|",
    "|>   |                 |    <|",
    "|>O  |^^^^^^^^^^^^^^^^^| &  <|",
    "------------------------------",
},{
    "------------------------------",
    "|> <>vvvvvvvvv|vvvvvvvvvvvvv<|",
    "|> <>         |             <|",
    "|> <>         |             <|",
    "|> <>         |             <|",
    "|> <>---------|---          <|",
    "|> <>vvvvv vvv|vv]          <|",
    "|>         [><]  ]          <|",
    "|>         [><]  ]          <|",
    "|> <>^^^^^~^^^]  ]          <|",
    "|> <>     v  <]  ]          <|",
    "|> <> O                     <|",
    "|> <>        <]  ]          <|",
    "|> <>        <]  ]          <|",
    "|>&<>^^^^^^^^^|^^|^^^^^^^^^^^|",
    "------------------------------",
},{
    "------------------------------",
    "|vv   <|&   vvvvvvvvvvvvvvvvv|",
    "|>    <|--                  <|",
    "|>    <|vv                   |",
    "|>    <|>                   <|",
    "|>    <|>  v^^^^^^^^^^^^^^^^^|",
    "|     <|>   -----------------|",
    "|     <|>  ^vvvvvvvvvvvvvvvvv|",
    "|     <|>                   <|",
    "|     <|>                   <|",
    "|------|>     ^^^^^^^^^^    <|",
    "|vvvvvv|     [<><><><><>]    |",
    "|>    <|>     vvvvvvvvvv    <|",
    "|>    <|>                   <|",
    "|^^^O^^|^^^^^^^^^^^^^^^^^^^^^|",
    "------------------------------",
},{
    "------------------------------",
    "|       [>                  <|",
    "|       [>                  <|",
    "| &     [>  ^^^^^^^^^^^^^^^^^|",
    "|-----] [>  -----------------|",
    "|     ] [>  [O]   ]        ] |",
    "|-----] [>  ~-~-~-~-~~~~~-~~ |",
    "|     ] [>  [   ]     ]    ] |",
    "|-----] [>  ~~~~~~~~~~~~-~~~ |",
    "|     ] [>  [              | |",
    "|-----] [>  ~~~~~~~~~~~~~~~~ |",
    "|                            |",
    "|                            |",
    "|                            |",
    "|^^^^^^^^^^^^^^^^^^^^^^^^^^^^|",
    "------------------------------",
},{
    "------------------------------",
    "|&    | [v[v[v[v[v[v[v[ [   <|",
    "|------ | [ [ [ [ [ [ [ [~~ <|",
    "|vvvvvv [ | [ [ [ [ [ [ |  ]<|",
    "|>      [ [ | [ [ [ [ [ | O]<|",
    "|>      [ [ [ | [ [ [ [ [~~ <|",
    "|>      [ [ [ [ | [ [ [ [>  <|",
    "|>      [ [ [ [ [ | [ [ [>  <|",
    "|>      [ [ [ [ [ [ | [ [>  <|",
    "|>      [ [ [ [ [ [ [ | [>  <|",
    "|>      [ [ [ [ [ [ [ [ [>  <|",
    "|>      [ [ [ [ [ [ [ [ [>  <|",
    "|>      [ [ [ [ [ [ [ [ [>  <|",
    "|>      [ [ [ [ [ [ [ [ [>  <|",
    "|^^^^^^^[^[^[^[^[^[^[^[ [^^^^|",
    "------------------------------",
},{
    "------------------------------",
    "|vvvvvvvvvvvvvvvvvvvvvvvvvvvv|",
    "|>    [ | |  <              <|",
    "|> &  [-[~]  <              <|",
    "|> -  [ | [  <           [  <|",
    "|> v  [ | [  <^^^^^--|      <|",
    "|>    [ | [  <vvvvv  [      <|",
    "|>    [ | [          [      <|",
    "|>    [~[ [          [      <|",
    "|>    | [ [  <^^^^^~~]      <|",
    "|>    [ | [~~<vvvv>  ]      <|",
    "|>    [ [~[  <   <>         <|",
    "|>    [ [v[O                <|",
    "|>    [ [ [  <   <>     ~   <|",
    "|^^^^^^^^^^^^^^^^^^^^^^^^^^^^|",
    "------------------------------",
},{
    "------------------------------",
    "|vvvvvvvvvvvvvvvvvvvvvvvvvvvv|",
    "|>                          <|",
    "|>                      [   <|",
    "|> -                    [   <|",
    "|>         ~~-~~        [   <|",
    "|>         [   ]        [   <|",
    "|>         | O |        [ - <|",
    "|>         [   ]        |- [<|",
    "|>         ~~-~~        [ - <|",
    "|>                      [   <|",
    "|>]&                    [   <|",
    "|> ~                   -    <|",
    "|>                          <|",
    "|^^^^^^^^^^^^^^^^^^^^^^^^^^^^|",
    "------------------------------",
},{
    "------------------------------",
    "|vvvvv~vvvvv~vvvvvvvvvvvvvvvv|",
    "|>& [] ]]] [] ]]          []<|",
    "|>~ [][]]] []-[]          []<|",
    "|>  [][][  [] []            <|",
    "|>  [] []--] []             <|",
    "|>  [][][~ []~[]            <|",
    "|>  [][][] [] []            <|",
    "|>     []] |----]           <|",
    "|>  [] []] [                <|",
    "|>  [][]~~~[                <|",
    "|>  []     [                <|",
    "|>  []~][][[####---------O  <|",
    "|>  []                      <|",
    "|^^^^^^^^^^^^^^^^^^^^^^^^^^^^|",
    "------------------------------",
}
};

PASSABLE pass[] = { ' ' , '&'  ,  'v'  ,  '^' , '<' , '>' , 'O','#' };

void DrawMap() {
    int back = color(255,255,255);
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 31; x++) {
            if (Map[y][x] == '<') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%55;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(200+color_shake,10+color_shake,10+color_shake),back);
                }
            } else if (Map[y][x] == '>') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%55;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(200+color_shake,10+color_shake,10+color_shake),back);
                }
            } else if (Map[y][x] == '^') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%55;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(200+color_shake,10+color_shake,10+color_shake),back);
                }
            } else if (Map[y][x] == 'v') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%55;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(200+color_shake,10+color_shake,10+color_shake),back);
                }
            } else if (Map[y][x] == 'O') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%55;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(10+color_shake,200+color_shake,10+color_shake),back);
                }
            } else if (Map[y][x] == '|') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%10;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(30+color_shake,30+color_shake,30+color_shake),back);
                }
            }
            else if (Map[y][x] == ']') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%10;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(30+color_shake,30+color_shake,30+color_shake),back);
                }
            }
            else if (Map[y][x] == '[') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%10;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(30+color_shake,30+color_shake,30+color_shake),back);
                }
            }
            else if (Map[y][x] == '~') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%10;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(30+color_shake,30+color_shake,30+color_shake),back);
                }
            }
            else if (Map[y][x] == '/') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%10;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(30+color_shake,30+color_shake,30+color_shake),back);
                }
            }
            else if (Map[y][x] == '#') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%10;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(30+color_shake,30+color_shake,30+color_shake),back);
                }
            }
            else if (Map[y][x] == '\\') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%10;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(30+color_shake,30+color_shake,30+color_shake),back);
                }
            }
            else if (Map[y][x] == '-') {
                for (int i = 0; i < 3; i++) {
                    int xoffset = (rand()%2);
                    int yoffset = (rand()%2);
                    int color_shake = rand()%10;
                    mvaddch_offset(y,x,yoffset,xoffset,c_char(Map[y][x]),color(30+color_shake,30+color_shake,30+color_shake),back);
                }
            } else {
                mvaddch(y,x,c_char(Map[y][x]),COLOR_BLACK,back);
            }
        }
    }
}

void SetMap(int id) {
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 31; x++) {
            Map[y][x] = Maps[id][y][x];
            if (Map[y][x] == '&') {
                p1.y = y;
                p1.x = x;
                Map[y][x] = ' ';
            }
        }
    }
}

int GetYSetUP(int y0, int x0) {
    for (int y = y0; y > 0; y--) {
        if (Map[y][x0] == ' ') {
            return y;
        } else if (Map[y][x0] == '>') {
            SetMap(p1.level);
            return y;
        } else if (Map[y][x0] == '<') {
            SetMap(p1.level);
            return y;
        } else if (Map[y][x0] == '^') {
            SetMap(p1.level);
            return y;
        } else if (Map[y][x0] == 'v') {
            SetMap(p1.level);
            return y;
        } else if (Map[y][x0] == 'O') {
            p1.level++;
            SetMap(p1.level);
            return y;
        }
    }
    return y0;
}

int GetYSetDOWN(int y0, int x0) {
    for (int y = y0; y < 16; y++) {
        if (Map[y][x0] == ' ') {
            return y;
        } else if (Map[y][x0] == '>') {
            SetMap(p1.level);
            return y;
        } else if (Map[y][x0] == '<') {
            SetMap(p1.level);
            return y;
        } else if (Map[y][x0] == '^') {
            SetMap(p1.level);
            return y;
        } else if (Map[y][x0] == 'v') {
            SetMap(p1.level);
            return y;
        } else if (Map[y][x0] == 'O') {
            p1.level++;
            SetMap(p1.level);
            return y;
        }
    }
    return y0;
}

int GetXSetLEFT(int y0, int x0) {
    for (int x = x0; x > 0; x--) {
        if (Map[y0][x] == ' ') {
            return x;
        } else if (Map[y0][x] == '>') {
            SetMap(p1.level);
            return x;
        } else if (Map[y0][x] == '<') {
            SetMap(p1.level);
            return x;
        } else if (Map[y0][x] == '^') {
            SetMap(p1.level);
            return x;
        } else if (Map[y0][x] == 'v') {
            SetMap(p1.level);
            return x;
        } else if (Map[y0][x] == 'O') {
            p1.level++;
            SetMap(p1.level);
            return x;
        }
    }
    return x0;
}

int GetXSetRIGHT(int y0, int x0) {
    for (int x = x0; x < 31; x++) {
        if (Map[y0][x] == ' ') {
            return x;
        } else if (Map[y0][x] == '>') {
            SetMap(p1.level);
            return x;
        } else if (Map[y0][x] == '<') {
            SetMap(p1.level);
            return x;
        } else if (Map[y0][x] == '^') {
            SetMap(p1.level);
            return x;
        } else if (Map[y0][x] == 'v') {
            SetMap(p1.level);
            return x;
        } else if (Map[y0][x] == 'O') {
            p1.level++;
            SetMap(p1.level);
            return x;
        }
    }
    return x0;
}

void Draw_Player() {
    for (int i = 0; i < 3; i++) {
        int xoffset = (rand()%2);
        int yoffset = (rand()%2);
        int color_shake = rand()%55;
        mvaddch_offset(p1.y,p1.x,yoffset,xoffset,c_char('A'),color(10+color_shake,200+color_shake,10+color_shake),color(255,255,255));
    }
}

void Draw_Aim() {
    for (int i = 0; i < 3; i++) {
        int xoffset = (rand()%2);
        int yoffset = (rand()%2);
        int color_shake = rand()%55;
        mvaddch_offset(p1.aim_y,p1.aim_x,yoffset,xoffset,c_char('.'),color(10+color_shake,10+color_shake,200+color_shake),color(255,255,255));
    }
}

void Aim_and_Movement() {
    // movement
    if (getkey(KEY_UP)) {
        if (p1.aim_dir == 0) {
            p1.aim_dir = 0;
            p1.aim_x = p1.x;
            p1.aim_y = p1.y-1;
        } else {
            p1.aim_dir = 0;
            p1.aim_x = p1.x;
            p1.aim_y = p1.y-1;
        }
    } else if (getkey(KEY_RIGHT)) {
        if (p1.aim_dir == 1) {
            p1.x = basic_movement_x(Map,p1.y,p1.x,pass);
            if (Map[p1.y][p1.x] == '>') {
                SetMap(p1.level);
            } else if (Map[p1.y][p1.x] == '<') {
                SetMap(p1.level);
            } else if (Map[p1.y][p1.x] == 'v') {
                SetMap(p1.level);
            } else if (Map[p1.y][p1.x] == '^') {
                SetMap(p1.level);
            } else if (Map[p1.y][p1.x] == 'O') {
                p1.level++;
                SetMap(p1.level);
            }
            p1.aim_dir = 1;
            p1.aim_x = p1.x+1;
            p1.aim_y = p1.y;
        } else {
            p1.aim_dir = 1;
            p1.aim_x = p1.x+1;
            p1.aim_y = p1.y;
        }
    } else if (getkey(KEY_DOWN)) {
        if (p1.aim_dir == 2) {
            p1.aim_dir = 2;
            p1.aim_x = p1.x;
            p1.aim_y = p1.y+1;
        } else {
            p1.aim_dir = 2;
            p1.aim_x = p1.x;
            p1.aim_y = p1.y+1;
        }
    } else if (getkey(KEY_LEFT)) {
        if (p1.aim_dir == 3) {
            p1.x = basic_movement_x(Map,p1.y,p1.x,pass);
            if (Map[p1.y][p1.x] == '>') {
                SetMap(p1.level);
            } else if (Map[p1.y][p1.x] == '<') {
                SetMap(p1.level);
            } else if (Map[p1.y][p1.x] == 'v') {
                SetMap(p1.level);
            } else if (Map[p1.y][p1.x] == '^') {
                SetMap(p1.level);
            } else if (Map[p1.y][p1.x] == 'O') {
                p1.level++;
                SetMap(p1.level);
            }
            p1.aim_dir = 3;
            p1.aim_x = p1.x-1;
            p1.aim_y = p1.y;
        } else {
            p1.aim_dir = 3;
            p1.aim_x = p1.x-1;
            p1.aim_y = p1.y;
        }
    } else if (getkey(KEY_SPACE)) {
        PlaySound(1);
        if (p1.aim_dir == 0) {
            p1.warp = true;
            while(p1.warp == true) {
                if (p1.y < 0) {
                    p1.y = 16;
                    p1.y = GetYSetUP(p1.y,p1.x);
                    p1.warp = false;
                } else {
                    if (Map[p1.y-1][p1.x] == '-') {
                        if (Map[p1.y-2][p1.x] == ' ') {
                            p1.y -= 2;
                            p1.warp = false;
                        } else {
                            p1.y--;
                        }
                    } else if (Map[p1.y-1][p1.x] == '~') { p1.warp = false; }
                    else {
                        p1.y--;
                    }
                }
                // clear
                clear();
                if (Map[p1.y][p1.x] == '^') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == 'v') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == '>') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == '<') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == 'O') {
                    p1.level++;
                    SetMap(p1.level);
                    p1.warp = false;
                }

                // Draw
                //if (getkey(KEY_SPACE) == true) {
                //    p1.warp = false;
                //}
                Floodfill(1,1,COLOR_WHITE);
                DrawMap();
                //Draw_Aim();
                Draw_Player();
                // refresh
                refresh();
                napms(5);
            }
        } else if (p1.aim_dir == 1) {
            p1.warp = true;
            while(p1.warp == true) {
                if (p1.x > 31) {
                    p1.x = 0;
                    p1.x = GetXSetRIGHT(p1.y,p1.x);
                    p1.warp = false;
                } else {
                    if (Map[p1.y][p1.x+1] == '|') {
                        if (Map[p1.y][p1.x+2] == ' ') {
                            p1.x += 2;
                            p1.warp = false;
                        } else {
                            p1.x++;
                        }
                    } else if (Map[p1.y][p1.x+1] == '[') { p1.warp = false; }
                    else {
                        p1.x++;
                    }
                }
                // clear
                clear();
                if (Map[p1.y][p1.x] == '^') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == 'v') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == '>') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == '<') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == 'O') {
                    p1.level++;
                    SetMap(p1.level);
                    p1.warp = false;
                }

                // Draw
                //if (getkey(KEY_SPACE) == true) {
                //    p1.warp = false;
                //}
                Floodfill(1,1,COLOR_WHITE);
                DrawMap();
                //Draw_Aim();
                Draw_Player();
                // refresh
                refresh();
                napms(5);
            }
        } else if (p1.aim_dir == 2) {
            p1.warp = true;
            while(p1.warp == true) {
                if (p1.y > 16) {
                    p1.y = 0;
                    p1.y = GetYSetDOWN(p1.y,p1.x);
                    p1.warp = false;
                } else {
                    if (Map[p1.y+1][p1.x] == '-') {
                        if (Map[p1.y+2][p1.x] == ' ') {
                            p1.y += 2;
                            p1.warp = false;
                        } else {
                            p1.y++;
                        }
                    } else if (Map[p1.y+1][p1.x] == '~') { p1.warp = false; }
                    else {
                        p1.y++;
                    }
                }
                // clear
                clear();
                if (Map[p1.y][p1.x] == '^') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == 'v') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == '>') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == '<') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == 'O') {
                    p1.level++;
                    SetMap(p1.level);
                    p1.warp = false;
                }

                // Draw
                //if (getkey(KEY_SPACE) == true) {
                //    p1.warp = false;
                //}
                Floodfill(1,1,COLOR_WHITE);
                DrawMap();
                //Draw_Aim();
                Draw_Player();
                // refresh
                refresh();
                napms(5);
            }
        } else if (p1.aim_dir == 3) {
            p1.warp = true;
            while(p1.warp == true) {
                if (p1.x < 0) {
                    p1.x = 31;
                    p1.x = GetXSetLEFT(p1.y,p1.x);
                    p1.warp = false;
                } else {
                    if (Map[p1.y][p1.x-1] == '|') {
                        if (Map[p1.y][p1.x-2] == ' ') {
                            p1.x -= 2;
                            p1.warp = false;
                        } else {
                            p1.x--;
                        }
                    } else if (Map[p1.y][p1.x-1] == ']') { p1.warp = false; }
                    else {
                        p1.x--;
                    }
                }
                // clear
                clear();
                if (Map[p1.y][p1.x] == '^') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == 'v') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == '>') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == '<') {
                    SetMap(p1.level);
                    p1.warp = false;
                } else if (Map[p1.y][p1.x] == 'O') {
                    p1.level++;
                    SetMap(p1.level);
                    p1.warp = false;
                }

                // Draw
                //if (getkey(KEY_SPACE) == true) {
                //    p1.warp = false;
                //}
                Floodfill(1,1,COLOR_WHITE);
                DrawMap();
                //Draw_Aim();
                Draw_Player();
                // refresh
                refresh();
                napms(5);
            }
        }
    }
    // gravity
    int g = p1.y+1;
    switch(Map[g][p1.x]) {
    case '^': {
        SetMap(p1.level);
    }
    break;
    case 'v': {
        SetMap(p1.level);
    }
    break;
    case '>': {
        SetMap(p1.level);
    }
    break;
    case '<': {
        SetMap(p1.level);
    }
    break;
    case 'O': {
        p1.level++;
        SetMap(p1.level);
    }
    break;
    case ' ': {
        p1.inair = true;
        p1.y++;
    }
    break;
    case '#': {
        p1.inair = true;
        p1.y++;
    }
    break;
    default: {
        p1.inair = false;
    }
    break;
    }

    // warp
    if (p1.y < 0) {
        p1.y = 16;
    }
    if (p1.x < 0) {
        p1.x = 31;
    }

    if (p1.x > 31) {
        p1.x = 0;
    }
    if (p1.y > 16) {
        p1.y = 0;
    }

    if (p1.aim_dir == 0) {
        p1.aim_x = p1.x;
        p1.aim_y = p1.y-1;
    } else if (p1.aim_dir == 1) {
        p1.aim_x = p1.x+1;
        p1.aim_y = p1.y;
    } else if (p1.aim_dir == 2) {
        p1.aim_x = p1.x;
        p1.aim_y = p1.y+1;
    } else if (p1.aim_dir == 3) {
        p1.aim_x = p1.x-1;
        p1.aim_y = p1.y;
    }
}

int main() {
    initscr(128,240,bt16,8,GFX_AUTODETECT_WINDOWED);
    initSound(1,false,"Teleport.wav");
    bool running = true;
    p1.level = 0;
    SetMap(p1.level);
    while (running == true) {
        // movement
        Aim_and_Movement();

        // system keys
        if ((getkey(KEY_Q) == true) || (getkey(KEY_ESC) == true)) {
            running = false;
        }
        if (getkey(KEY_R) == true) {
            SetMap(p1.level);
        }
        if (p1.level > 9)
        {
            p1.level = 0;
            SetMap(p1.level);
        }

        // clear
        clear();

        // Draw
        Floodfill(1,1,COLOR_WHITE);
        DrawMap();
        Draw_Aim();
        Draw_Player();
        // refresh
        refresh();
        napms(80);
    }
    freebuf();
    return 0;
}
END_OF_MAIN()
