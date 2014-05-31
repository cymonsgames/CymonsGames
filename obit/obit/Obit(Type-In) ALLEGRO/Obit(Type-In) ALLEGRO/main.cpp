#include <iostream>
#include <allegro.h>
#include <ctime>

using namespace std;

char Maps[20][20][41] ={
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N       E                              N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N     #                                N",
    "N  #                     #             N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N #O   #                               N",
    "N                       #              N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N                      #   E           N",
    "N   O     #                            N",
    "N                             #        N",
    "N                                      N",
    "N                                      N",
    "N                         #            N",
    "N                            #         N",
    "N                                      N",
    "N                 #                    N",
    "N   #                                  N",
    "N                                      N",
    "N                                      N",
    "N        #             #               N",
    "N                #                     N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N   ##              #             O    N",
    "N   #                                  N",
    "N                                      N",
    "N              ##     ##               N",
    "N              #       #               N",
    "N   #                        #         N",
    "N   ##                                 N",
    "N                                      N",
    "N                    #                 N",
    "N               #                 #    N",
    "N        E                             N",
    "N                                      N",
    "N                                      N",
    "N       #                              N",
    "N                           #          N",
    "N                                      N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N                                      N",
    "N    O                                 N",
    "N                  #          #        N",
    "N                              #       N",
    "N        #                             N",
    "N    #                                 N",
    "N                   #  #      #        N",
    "N                                      N",
    "N                                      N",
    "N   #    #             #               N",
    "N   ##  ##         #                   N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N     E                                N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N                                      N",
    "N          #           ########        N",
    "N                      #               N",
    "N                  #                   N",
    "N                              #       N",
    "N                                   #  N",
    "N                             #        N",
    "N                  O   ##              N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N         #                            N",
    "N                                  #   N",
    "N                     E                N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N                                      N",
    "N  #      1                    #       N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                 #O     2     #       N",
    "N   #                         ##       N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                E                     N",
    "N                                      N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N                  # #                 N",
    "N       #                          #   N",
    "N                                      N",
    "N                                      N",
    "N                  2 1                 N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                   #                  N",
    "N        #                             N",
    "N                                      N",
    "N                   #O                 N",
    "N E                                    N",
    "N                                 #    N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N #O #       #                         N",
    "N #                         #          N",
    "N   #                                  N",
    "N                                 #    N",
    "N#  #            #                     N",
    "N  ##               #                  N",
    "N           #                          N",
    "N                  #                   N",
    "N                     #            #   N",
    "N                          #           N",
    "N                 #                    N",
    "N #                                    N",
    "N                                      N",
    "N                                      N",
    "N                                 E    N",
    "N                                      N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N  O               1        4      E   N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N   #                              #   N",
    "N      3                               N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N  #               2                #  N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N                                      N",
    "N                 1                    N",
    "N                           #          N",
    "N       #                              N",
    "N                          3      #    N",
    "N        #                             N",
    "N                                      N",
    "N                          E           N",
    "N                          #O          N",
    "N                                      N",
    "N        #                             N",
    "N                 4              2     N",
    "N                                      N",
    "N       #                              N",
    "N                           #          N",
    "N                                      N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N                    #                 N",
    "N  4      #                      2  #  N",
    "N                                      N",
    "N      #                               N",
    "N                                E     N",
    "N                     #                N",
    "N  #                                   N",
    "N                                      N",
    "N                                      N",
    "N     #     #                          N",
    "N          ##                          N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N   O     1                        3   N",
    "N                                      N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                #     N",
    "N #                          #         N",
    "N               #               #   2  N",
    "N     #          #              3      N",
    "N              #         #    #        N",
    "N              E#         #            N",
    "N                                      N",
    "N#     1                               N",
    "N                #                     N",
    "N                         #         #  N",
    "N                       #              N",
    "N                                      N",
    "N                                      N",
    "N #                                    N",
    "N                                      N",
    "N 4             #                      N",
    "N                                   O  N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                 #    N",
    "N             #      #                 N",
    "N              #          #            N",
    "N              #         ##   #        N",
    "N               #                      N",
    "N                     #                N",
    "N    #         #                       N",
    "N   #                    #             N",
    "N                1                 #   N",
    "N           ##  ##                     N",
    "N           #    #                     N",
    "N          2                  #        N",
    "N    #      #    #                     N",
    "N          E##  ##      #              N",
    "N                                      N",
    "N                                      N",
    "N              O                       N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N        #                             N",
    "N  O   #          1    #E   3     #    N",
    "N                                      N",
    "N                                      N",
    "N    #    #                            N",
    "N     #  #                             N",
    "N                 #                    N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N             #                        N",
    "N                  #        4          N",
    "N    ##    #                     #     N",
    "N    #         #      4                N",
    "N                 #                    N",
    "N    2      #                          N",
    "N                                      N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    // space
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                                      N",
    "N                                      N",
    "N    O           1                     N",
    "N                2               #     N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                                      N",
    "N                           You Win    N",
    "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN",
};

class Player {
public:
    int y;
    int x;
    char dir;
};

class Tail {
    public:
    int y;
    int x;
};

Tail tail;
Player p1;

BITMAP *buffer = NULL;
BITMAP *Player = NULL;
BITMAP *Brick = NULL;
BITMAP *Exit = NULL;
BITMAP *GreenPortal = NULL;
BITMAP *BluePortal = NULL;

int GameSpeed = 20;
int level = 0;
bool CanMove = true;
bool Stop = true;
bool EndGame = false;


void ALEG_init()
{
     allegro_init();
     install_keyboard();
     set_color_depth(16); // Set the color depth
     set_gfx_mode(GFX_AUTODETECT, 640,480,0,0);
}

void NextLevel(int amount) {
    level += amount;
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 40; x++) {
            switch (Maps[level][y][x]) {
            case 'O': {
                p1.x = x;
                p1.y = y;
            }
            break;
            }
        }
    }
    clear_keybuf();
    tail.y = p1.y;
    tail.x = p1.x;
}

void GotoPortal(int number) {
    if (number == 1) {
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                switch (Maps[level][y][x]) {
                case '1': {
                    p1.x = x;
                    p1.y = y;
                }
                break;
                }
            }
        }
    }
    if (number == 2) {
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                switch (Maps[level][y][x]) {
                case '2': {
                    p1.x = x;
                    p1.y = y;
                }
                break;
                }
            }
        }
    }
    if (number == 3) {
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                switch (Maps[level][y][x]) {
                case '3': {
                    p1.x = x;
                    p1.y = y;
                }
                break;
                }
            }
        }
    }
    if (number == 4) {
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                switch (Maps[level][y][x]) {
                case '4': {
                    p1.x = x;
                    p1.y = y;
                }
                break;
                }
            }
        }
    }
}

void Movement() {
    if (CanMove == true) {

        if (key[KEY_ESC]) {
            EndGame = true;
        } else if (key[KEY_UP])
        {
            p1.dir = 'u';
            CanMove == false;
            Stop = false;
        } else if (key[KEY_DOWN]) {
            p1.dir = 'd';
            CanMove == false;
            Stop = false;
        } else if (key[KEY_LEFT]) {
            p1.dir = 'l';
            CanMove == false;
            Stop = false;
        } else if (key[KEY_RIGHT]) {
            p1.dir = 'r';
            CanMove == false;
            Stop = false;
        }
    }

    if (Stop == false) {
        if (p1.dir == 'u') {
            int y2 = (p1.y-1);
            switch (Maps[level][y2][p1.x]) {
            case ' ': {
                tail.y = p1.y;
                tail.x = p1.x;
                p1.y--;
                CanMove = false;
            }
            break;
            case 'O': {
                p1.y--;
                CanMove = false;
            }
            break;
            case 'N': {
                NextLevel(0);
                CanMove = true;
                Stop = true;
            }
            break;
            case '1': {
                GotoPortal(2);
                CanMove = false;
            }
            break;
            case '2': {
                GotoPortal(1);
                CanMove = false;
            }
            break;
            case '3': {
                GotoPortal(4);
                CanMove = false;
            }
            break;
            case '4': {
                GotoPortal(3);
                CanMove = false;
            }
            break;
            case '#': {
                CanMove = true;
                Stop = true;
            }
            break;
            case 'E': {
                NextLevel(1);
                CanMove = true;
                Stop = true;
            }
            break;
            }
        }
        if (p1.dir == 'd') {
            int y2 = (p1.y+1);
            switch (Maps[level][y2][p1.x]) {
            case ' ': {
                tail.y = p1.y;
                tail.x = p1.x;
                p1.y++;
                CanMove = false;
            }
            break;
            case 'O': {
                p1.y++;
                CanMove = false;
            }
            break;
            case 'N': {
                NextLevel(0);
                CanMove = true;
                Stop = true;
            }
            break;
            case '1': {
                GotoPortal(2);
                CanMove = false;
            }
            break;
            case '2': {
                GotoPortal(1);
                CanMove = false;
            }
            break;
            case '3': {
                GotoPortal(4);
                CanMove = false;
            }
            break;
            case '4': {
                GotoPortal(3);
                CanMove = false;
            }
            break;
            case '#': {
                CanMove = true;
                Stop = true;
            }
            break;
            case 'E': {
                NextLevel(1);
                CanMove = true;
                Stop = true;
            }
            break;
            }
        }
        if (p1.dir == 'l') {
            int x2 = (p1.x-1);
            switch (Maps[level][p1.y][x2]) {
            case ' ': {
                tail.y = p1.y;
                tail.x = p1.x;
                p1.x--;
                CanMove = false;
            }
            break;
            case 'O': {
                p1.x--;
                CanMove = false;
            }
            break;
            case 'N': {
                NextLevel(0);
                CanMove = true;
                Stop = true;
            }
            break;
            case '1': {
                GotoPortal(2);
                CanMove = false;
            }
            break;
            case '2': {
                GotoPortal(1);
                CanMove = false;
            }
            break;
            case '3': {
                GotoPortal(4);
                CanMove = false;
            }
            break;
            case '4': {
                GotoPortal(3);
                CanMove = false;
            }
            break;
            case '#': {
                CanMove = true;
                Stop = true;
            }
            break;
            case 'E': {
                NextLevel(1);
                CanMove = true;
                Stop = true;
            }
            break;
            }
        }
        if (p1.dir == 'r') {
            int x2 = (p1.x+1);
            switch (Maps[level][p1.y][x2]) {
            case ' ': {
                tail.y = p1.y;
                tail.x = p1.x;
                p1.x++;
                CanMove = false;
            }
            break;
            case 'O': {
                p1.x++;
                CanMove = false;
            }
            break;
            case 'N': {
                NextLevel(0);
                CanMove = true;
                Stop = true;
            }
            break;
            case '1': {
                GotoPortal(2);
                CanMove = false;
            }
            break;
            case '2': {
                GotoPortal(1);
                CanMove = false;
            }
            break;
            case '3': {
                GotoPortal(4);
                CanMove = false;
            }
            break;
            case '4': {
                GotoPortal(3);
                CanMove = false;
            }
            break;
            case '#': {
                CanMove = true;
                Stop = true;
            }
            break;
            case 'E': {
                NextLevel(1);
                CanMove = true;
                Stop = true;
            }
            break;
            }
        }
    }
}

void Draw() {
    // draw current map
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 40; x++) {
            switch (Maps[level][y][x]) {
            case 'O': {
            }
            break;
            case 'N': {
            }
            break;
            case 'E': {
                //mvaddch(y,x,'E'|COLOR_PAIR(5)|A_BOLD);
                blit(Exit,buffer,0,0,x*16,y*16,16,16);
            }break;

            case '1': {
                //mvaddch(y,x,'+'|COLOR_PAIR(2)|A_BOLD);
                blit(GreenPortal,buffer,0,0,x*16,y*16,16,16);
            }
            break;

            case '2': {
                //mvaddch(y,x,'+'|COLOR_PAIR(2)|A_BOLD);
                blit(GreenPortal,buffer,0,0,x*16,y*16,16,16);
            }
            break;

            case '3': {
                //mvaddch(y,x,'+'|COLOR_PAIR(3)|A_BOLD);
                blit(BluePortal,buffer,0,0,x*16,y*16,16,16);
            }
            break;

            case '4': {
                //mvaddch(y,x,'+'|COLOR_PAIR(3)|A_BOLD);
                blit(BluePortal,buffer,0,0,x*16,y*16,16,16);
            }
            break;

            case '#': {
                //mvaddch(y,x,'+'|COLOR_PAIR(3)|A_BOLD);
                blit(Brick,buffer,0,0,x*16,y*16,16,16);
            }
            break;

            default: {
                //mvaddch(y,x,Maps[level][y][x]|COLOR_PAIR(9));
            }
            break;
            }
        }
    }
    // draw player
    //mvaddch(p1.y,p1.x,'O'|COLOR_PAIR(4)|A_BOLD);
    blit(Player,buffer,0,0,p1.x*16,p1.y*16,16,16);
    //mvprintw(21,0,"Level %i",level);
    //mvprintw(22,0,"Press Q Key To Exit");

}

int main() {
    ALEG_init();
    NextLevel(0);
    Brick = load_bitmap("Wall.bmp",NULL);
    Player = load_bitmap("Player.bmp",NULL);
    Exit = load_bitmap("Exit.bmp",NULL);
    GreenPortal = load_bitmap("GreenPortal.bmp",NULL);
    BluePortal = load_bitmap("BluePortal.bmp",NULL);
    buffer = create_bitmap(640,480);
    while ((EndGame == false) && (level != 14) ) {
        clear_bitmap(buffer);
        Movement();
        Draw();
        blit(buffer,screen,0,0,0,0,640,480);
        rest(GameSpeed);
    }
    return 0;
}
END_OF_MAIN();
