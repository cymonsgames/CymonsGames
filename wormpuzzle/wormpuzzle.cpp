// Entry for MinorHack at July 7th, 2007 at 7:00 pm UTC
// Submitted by Jonatan Hedborg
#include <allegro.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct OBSTACLE {
    int x,y,w,h;
    OBSTACLE(int  x, int  y, int  w, int  h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
};

BITMAP *buffer = NULL;
BITMAP *car = NULL;
bool running = true;

float vel_x = 0.0f, vel_y = 0.0f;
float pos_x = 400.0f, pos_y = 550.0f;
float direction = -M_PI/2;
float mass = 1000.0f;
float force = 9000.0f;
float turn_rate = 0.1f;
float damp_ratio = 0.1f;
int turns = 0;
int obstacle_count = 10;
vector<OBSTACLE> obstacle_list;

void generate_gfx() {
    car = create_bitmap(40,20);
    rectfill(car,0,0,39,19,makecol(200,50,50));
    rect(car,0,0,39,19,makecol(50,50,50));
    circlefill(car,37,3,3,makecol(240,240,240));
    circlefill(car,37,17,3,makecol(240,240,240));
}

float get_ac(float force) {
    return force/mass;
}

void generate_obstacles(int num) {
    obstacle_list.clear();
    for(int i=0; i<num; i++) {
        obstacle_list.push_back(OBSTACLE(rand()%750+25, rand()%450+25, rand()%70+10, rand()%70+10));
    }
}



int main() {
    allegro_init();
    install_keyboard();

    srand(time(0));

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800,600, 0,0);
    buffer = create_bitmap(800,600);
    generate_gfx();

    generate_obstacles(obstacle_count);

    while(running) {
        int input = readkey();
        input = input >> 8;
        switch( input ) {
            case KEY_ESC:
                running = false;
                break;

            case KEY_UP:
                vel_x += cos(direction) * get_ac(force);
                vel_y += sin(direction) * get_ac(force);
                break;

            case KEY_DOWN:
                vel_x -= cos(direction) * get_ac(force/2);
                vel_y -= sin(direction) * get_ac(force/2);
                break;

            case KEY_LEFT:
                direction -= turn_rate;
                break;

            case KEY_RIGHT:
                direction += turn_rate;
        }

        vel_x -= vel_x*damp_ratio;
        vel_y -= vel_y*damp_ratio;
        pos_x += vel_x;
        pos_y += vel_y;

        clear(buffer);

        //Bounds
        rectfill(buffer, 0,0, 15, 600, makecol(50,230,50));
        rectfill(buffer, 0,0, 800, 15, makecol(50,230,50));
        rectfill(buffer, 0,600, 800, 600-15, makecol(50,230,50));
        rectfill(buffer, 800,0, 800-15, 600-15, makecol(50,230,50));

        //Goal
        rectfill(buffer, 350,0, 450, 40, makecol(128,128,128));


        textprintf_ex(buffer,font,0,0,makecol(255,255,255),-1,"Turns taken: %i", turns);
        rotate_sprite(buffer,car, (int)round(pos_x)-20, (int)round(pos_y)-10,ftofix(direction*128/M_PI));
        for(int i=0; i<obstacle_list.size(); i++) {
            OBSTACLE ob = obstacle_list[i];
            rectfill(buffer,ob.x, ob.y, ob.x+ob.w, ob.y+ob.h,makecol(50,230,50));
        }

        blit(buffer,screen,0,0, 0,0, 800,600);

        //Hacky collision detection :D
        float tp_x = pos_x + cos(direction)*22;
        float tp_y = pos_y + sin(direction)*22;

        int col = getpixel(buffer,(int)round(tp_x),(int)round(tp_y));
        if( col == makecol(50,230,50) ) {
            textout_centre_ex(screen,font,"CRASH! GAME OVER :(",400,300,makecol(255,255,255),-1);
            readkey();
            return 0;
        } else if( col == makecol(128,128,128) ) {
            textout_centre_ex(screen,font,"GOOD JOB!! NEXT ROUND!",400,300,makecol(255,255,255),-1);
            readkey();
            vel_x = 0.0f;
            vel_y = 0.0f;
            pos_x = 400.0f;
            pos_y = 550.0f;
            direction = -M_PI/2;
            turns = 0;
            obstacle_count += 5;
            generate_obstacles(obstacle_count);
        }

        turns++;
    }

    return 0;
}END_OF_MAIN()
