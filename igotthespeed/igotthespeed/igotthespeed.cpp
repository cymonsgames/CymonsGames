// Entry for MinorHack at September 16th, 2006 at 6:00 pm UTC
// Submitted by Kikaru
#include "allegro.h"

#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 640
#define HEIGHT 480
#define PLAYERCOL makecol (0, 255, 0)
#define WHITE makecol (255, 255, 255)
#define BLACK makecol (0, 0, 0)
#define GOLD makecol (255, 255, 0)
#define SPEED 14

BITMAP *buffer;

int player_y, obstacle_x, jumpforce, score, obstacle_t, slogan_life;
bool jump, alive, scored;

void main()
{
    allegro_init();
    install_keyboard();
    install_timer();
    set_color_depth(32);
    set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    text_mode(-1);
    
    buffer = create_bitmap(WIDTH, HEIGHT);
    jump = false;
    alive = true;
    scored = false;
    obstacle_x = 660;
    player_y = 440;
    obstacle_t = 50;
    
    //title screen
    textprintf_centre(screen, font, 320, 220, WHITE, "I Got the Speed!");
    textprintf_centre(screen, font, 320, 260, WHITE, "Press Enter to Start");
    textprintf_centre(screen, font, 320, 280, WHITE, "Press Esc to Quit");
    while ((!key[KEY_ENTER])&&(!key[KEY_ESC]))
    {
    }
    
    //main game
    while (!key[KEY_ESC])
    {

        if ((key[KEY_SPACE])&&(jump == false))
        {
                jump = true;
                jumpforce = 30;
        }
        
        if (jump == true)
        {
                player_y -= jumpforce/2;
                jumpforce -= 2;
        }
        
        if (player_y >= 435)
        {
                jump = false;
        }
        
        obstacle_t -= 1;
        obstacle_x -= SPEED + (score/3);
        
        if ((obstacle_t <= 0)&&(obstacle_x <= 0))
        {
                obstacle_x = 640;
                obstacle_t = (rand()%25) + 50 - score;
                scored = false;
        }
        
        if ((obstacle_x < 0)&&(scored == false))
        {
                score += 1;
                scored = true;
                slogan_life = 25;
        }
        
        slogan_life -= 1;

        if ((player_y > 420)&&(obstacle_x < 64)&&(obstacle_x > 0))
        {
                alive = false;
        }
        
        if (alive == false)
        {
                textprintf_centre(screen, font, 320, 240, WHITE, "Game Over");
                textprintf_centre(screen, font, 320, 255, WHITE, "Press Esc to Quit");
                while(!key[KEY_ESC])
                {
                }
        }
        
        //draw stuff
        rectfill(buffer, 0, 0, WIDTH, HEIGHT, BLACK);
        circlefill(buffer, 32, player_y, 10, PLAYERCOL);
        rectfill(buffer, obstacle_x-16, 428, obstacle_x + 16, 460, WHITE);
        textprintf(buffer, font, 10, 20, WHITE, "score: %i", score);
        textprintf(buffer, font, 300, 20, WHITE, "Press Space to Jump");
        if (slogan_life > 0)
                textprintf_centre(buffer, font, 400-slogan_life*3, 240, GOLD, "I got the speed!");
        blit(buffer, screen, 0, 0, 0, 0, WIDTH, HEIGHT);
        
        rest(20);
    }
    
    destroy_bitmap(buffer);
    return;
}
END_OF_MAIN()