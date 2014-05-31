// Revised entry for MinorHack at October 28th, 2006 at 4:00 pm UTC
// Submitted by Kikaru
#include "allegro.h"
#include <math.h>
#include <stdio.h>

#define ALLEGRO_NO_FIX_CLASS
#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 640
#define HEIGHT 480
#define BLACK makecol(0, 0, 0)
#define BLUE makecol(0, 128, 255)
#define ORANGE makecol(255, 192, 0)
#define WHITE makecol(255, 255, 255)
#define DIST 0
#define ANGLE 1
#define BOOM 2

BITMAP *buffer;
long start_time;
int loop_num;
int block [5][3];
int p_angle = 270, score = 0;
bool gameover, active[5];

void spawn_meteor()
{
    for(int i = 0; i < 5; i++)
    {
        if (active[i] == false)
        {
                block[i][DIST] = (rand()%100)+200;
                block[i][ANGLE] = (rand()%300)+30;
                active[i] = true;
                return;
        }
    }
    return;
}

void main()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();
    set_color_depth(32);
    set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    srand(time(NULL));

    buffer = create_bitmap(WIDTH, HEIGHT);
    
    textout_centre_ex(screen, font, "Welcome to Orbit!", 320, 200, WHITE, -1);
    textout_centre_ex(screen, font, "Rotate the Moon around the earth to block the oncoming meteors", 320, 220, WHITE, -1);
    textout_centre_ex(screen, font, "Press Enter to start game", 320, 240, WHITE, -1);
    while ((!key[KEY_ENTER])&&(!key[KEY_ESC]))
    {}
    
    while (!key[KEY_ESC])
    {
        start_time = clock();
        
        /*game goes here*/
        if ((key[KEY_LEFT])||(key[KEY_DOWN]))
                p_angle -= 10;
        if ((key[KEY_RIGHT])||(key[KEY_UP]))
                p_angle += 10;
        
        p_angle += 360;
        p_angle %= 360;
      
        if (loop_num % (45-score/20) == 0)
                spawn_meteor();
        
        for(int i = 0; i < 5; i++)
        {
                if (active[i])
                                block[i][DIST] -= 2;
                if (block[i][BOOM] > 0)
                                block[i][BOOM] += 1;
                block[i][BOOM] %= 10;
                if ((block[i][DIST] < 40)&&(active[i] == true))
                                gameover = true;
                if ((block[i][DIST] < 90)&&(block[i][DIST] > 40)&&(block[i][ANGLE] > p_angle - 25)&&(block[i][ANGLE] < p_angle + 25)&&(active[i] == true))
                {
                                active[i] = false;
                                score += 1;
                                block[i][BOOM] = 1;
                }
        }
        
        /*drawing goes here*/
        circlefill(buffer, 320, 240, 40, BLUE);
        circlefill(buffer, 320+int(70*cos(p_angle/57.32)), 240+int(70*sin(p_angle/57.32)), 12, WHITE);
        for (int i = 0; i < 5; i++)
        {
                if (active[i] == true)
                circlefill(buffer, 320+int(block[i][DIST]*cos(block[i][ANGLE]/57.32)), 240+int(block[i][DIST]*sin(block[i][ANGLE]/57.32)), 7, ORANGE);
                if ((block[i][BOOM] > 1)&&(active[i] == false))
                circle(buffer, 320+int(block[i][DIST]*cos(block[i][ANGLE]/57.32)), 240+int(block[i][DIST]*sin(block[i][ANGLE]/57.32)), block[i][BOOM]*10, ORANGE);
        }
        textprintf_ex(buffer, font, 30, 20, WHITE, -1, "Score: %i", score);
        blit(buffer, screen, 0, 0, 0, 0, WIDTH, HEIGHT);
        clear_to_color(buffer, BLACK);
        
        if (gameover == true)
        {
                textout_centre_ex(screen, font, "Game Over", 320, 240, WHITE, -1);
                textout_centre_ex(screen, font, "Press esc to quit", 320, 260, WHITE, -1);
                while (!key[KEY_ESC])
                {}
                destroy_bitmap(buffer);
                return;
        }
        
        while (clock() < start_time + 20)
        {}
        loop_num += 1;
    }
    destroy_bitmap(buffer);
    return;
}
END_OF_MAIN()