/*******************************************
 * File:        fireworks.c
 * Author:      el Hamil, elhamil@hotmail.com
 * additional programming by Joe Larson joealarson@gmail.com
 *      http://cymonsgames.com

 * Desc:        Cute particle effects, ikagura-ish gameplay

 * Created:     2003-07-12, started out as a non-interactive firework thing
 * Updates:
 * 2003-07-13, made an ikaruga-style game beacuse clicking is more fun than
 *      just watching
 * 2003-07-14, added game over-screen and some polish
 * 2009-12-16, changed the timing and recompiled for allegro 4.2 by Joe Larson
 * 2010-7-4,  Added 2 new firework types. Changed the data types, removed angle
 *      and velocity. Seperated "type" (INACTIVE, ROCKET, SPARK) from
 *      spark data (TYPE_1/2/etc) which will determines color and pattern.
 *      Changed the code base from C++ to C since that's pretty much what it
 *      was anyways. Added generated sound. Sounds a bit Atari, but meh.
 *******************************************/

#include <allegro.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/* different types of particles */
enum {TYPE_INACTIVE, TYPE_ROCKET, TYPE_SPARK};
enum {TYPE_1 = 1, TYPE_2, TYPE_3, TYPE_4};
const int numtype = 4;

struct CParticle {
	int type;
	int spark;
	float x, y;
	float dx, dy;
	int life;
	int color;
};

BITMAP* virscr = 0;
SAMPLE* boom;
SAMPLE* crack;

int colordepth = 16;
const int SCREENW = 640;
const int SCREENH = 480;
const int SCREENW_HALF = 320;
const int SCREENH_HALF = 240;
const float GRAVITATION = 0.01;
const int TICKS_BETWEEN_UPDATES = 2;
const int CLICK_RADIUS = 4000;

#define maxParticles 4096
int activeParticles = 0;
struct CParticle particle[maxParticles];

#define nrAngles 256
float costable[nrAngles];
float sintable[nrAngles];

void init_samples();
void destroy_samples();
int playGameOnce();

volatile int ticks;

void ticker(void)
{
	ticks++;
}
END_OF_FUNCTION(ticker)

int main(int argc, char* argv[]) {
	/* command line configuration */
	int i, fullscreen = 0;
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "full"))
			fullscreen = 1;
		if (!strcmp(argv[i], "15"))
			colordepth = 15;
		if (!strcmp(argv[i], "32"))
			colordepth = 32;
	}


	if (allegro_init() != 0) {
		printf("allegro_init() failed\n");
		exit(1);
	}

	ticks=0;
	LOCK_FUNCTION(ticker);
	LOCK_VARIABLE(ticks);
	install_int_ex(ticker, MSEC_TO_TIMER(20));

	if (install_keyboard() != 0) {
		printf("install_keyboard() failed\n");
		exit(1);
	}
    if (install_sound(DIGI_AUTODETECT,MIDI_NONE,0)) {
		printf("install_sound() failed\n");
		exit(1);
	}

	if (install_mouse() == -1) {
		printf("install_mouse() failed\n");
		exit(1);
	}

	set_color_depth(colordepth);

	int result = -1;
	if (fullscreen)
		result = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, SCREENW, SCREENH, 0,0);
	else
		result = set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREENW, SCREENH, 0, 0);
	if (result < 0) {
		printf("set_gfx_mode() failed\n");
		exit(1);
 	}

	virscr = create_bitmap(SCREENW, SCREENH);
	if (!virscr) {
		printf("create_bitmap() failed\n");
		exit(1);
 	}

	srand(time(NULL));
    init_samples();

	for (i = 0; i < nrAngles; i++) {
		float radian = (i * 2 * M_PI) / nrAngles;
		costable[i] = cos(radian);
		sintable[i] = sin(radian);
	}

	int programRunning = 1;
	while (programRunning) {
		programRunning = playGameOnce();
	}

    destroy_samples();
	destroy_bitmap(virscr);

	return 0;
}
END_OF_MAIN();

void init_samples() {
    int i;
    unsigned char* noise;

    boom = create_sample(8,0, 2700, 7500);
    noise = (unsigned char*)boom->data;
    for (i=0; i<750; ++i)
        noise[i] = rand()% (255 - i/3);
    crack = create_sample(8,0,16537,100);
    noise = (unsigned char*)crack->data;
    for (i=0; i<100; ++i)
        noise[i] = rand()%(255 - 2 * i);
}

void launchNewRocket(struct CParticle* par, int type) {
    float velocity = 5 + (rand() % 100) / 100.0;

	par->y = SCREENH; /* the rockets start at the bottom of the screen */
	par->x = (rand() % (SCREENW - 200)) + 100;
	par->dy = -velocity;
	par->dx = ((rand() % 100) - 50) / 200.0;
	par->type = TYPE_ROCKET;
	par->life = (rand() % 10) + 50;
	par->spark = type;
	if (type == TYPE_1)
		par->color = makecol(255,127,0);
	else if (type == TYPE_2)
		par->color = makecol(0,127,255);
	else if (type == TYPE_3)
		par->color = makecol(25,192,25);
	else
		par->color = makecol(255,255,255);
}

void destroy_samples() {
  destroy_sample(boom);
  destroy_sample(crack);
}

void explodeRocket(struct CParticle* par) {
    float sc = .7 + (float)(rand() % 100) / 100.0;
    int j, angle = 0;
    float velocity = 0.0;

    switch (par->spark) {
    case TYPE_1: { /* simple boom */
		int nrToCreate = 100;
		for (j = 0; j < maxParticles; j++) {
			if (nrToCreate && particle[j].type == TYPE_INACTIVE) {
				nrToCreate--;
				particle[j].life = rand() % 10;
				particle[j].type = TYPE_SPARK;
				particle[j].spark = par->spark;
				particle[j].x = par->x;
				particle[j].y = par->y;
				particle[j].color = par->color;

			    angle = (nrToCreate * nrAngles) / 100; angle %= nrAngles;
			    velocity = sc * (rand() % 60) / 10.0;
				particle[j].dx = costable[angle] * velocity;
				particle[j].dy = sintable[angle] * velocity;
			}
		}
		break;
    }
    case TYPE_2: { /* spiral */
		int nrToCreate = 100;
		int c = 32;
		float step = sc * 4.7 / (float)(nrToCreate - c);
		angle = rand() % nrAngles;
		for (j = 0; j < maxParticles; j++) {
			if (nrToCreate && particle[j].type == TYPE_INACTIVE) {
				nrToCreate--;
				particle[j].life = rand() % 10;
				particle[j].type = TYPE_SPARK;
				particle[j].spark = par->spark;
				particle[j].x = par->x;
				particle[j].y = par->y;
				particle[j].color = par->color;

				if (c >= 0) {
					c--;
					angle += nrAngles / 32; angle %= nrAngles;
					velocity = sc * 5.7;
				} else {
					angle += 10; angle %= nrAngles; angle %= nrAngles;
					velocity -= step;
				}
				particle[j].dx = costable[angle] * velocity;
				particle[j].dy = sintable[angle] * velocity;
			}
		}
		break;
    }
    case TYPE_3: { /* Spirograph */
		int nrToCreate = nrAngles / 3 + 1;
		int period = 3 + rand() % 10;
		float shift = (float)(rand() % 23) / 10.0;

		angle = rand() % nrAngles;
		for (j = 0; j < maxParticles; j++)
		{
			if (nrToCreate && particle[j].type == TYPE_INACTIVE) {
				nrToCreate--;
				particle[j].type = TYPE_SPARK;
				particle[j].spark = par->spark;
				particle[j].x = par->x;
				particle[j].y = par->y;
                angle += 3; angle %= nrAngles;
                velocity = sc * 3.0 *
                    (sintable[(angle * period) % nrAngles] + shift);
				particle[j].dx = costable[angle] * velocity;
				particle[j].dy = sintable[angle] * velocity;
				particle[j].color = par->color;
				particle[j].life = rand() % 10;
    		}
		}
		break;
    }
    default: { /* pretty explosion */
		int nrToCreate = 100;
		int c = 32;
		angle = rand() % nrAngles;
		for (j = 0; j < maxParticles; j++) {
			if (nrToCreate && particle[j].type == TYPE_INACTIVE) {
				nrToCreate--;
				particle[j].life = rand() % 10;
				particle[j].type = TYPE_SPARK;
				particle[j].spark = par->spark;
				particle[j].x = par->x;
				particle[j].y = par->y;
				particle[j].color = par->color;
				if (c > 0) {
				    c--;
					velocity = 6 * sc;
                    particle[j].color = par->color;
					angle += 10; angle %= nrAngles;
				} else {
					angle = rand() % nrAngles;
                    velocity = sc * (rand() % 60) / 10.0;
                    particle[j].color
                        = makecol(rand() % 255, rand() % 255, rand() % 255);
                }

				particle[j].dx = costable[angle] * velocity;
				particle[j].dy = sintable[angle] * velocity;
			}
		}
		break;
    }
    }
    stop_sample(boom);
    play_sample(boom,255,127,200 + (int)((1.0/sc) * 300.0),0);
	par->type = TYPE_INACTIVE;
}

int numtypes() {
    int i, retval = 0;
    int types[numtype];

    for (i = 0; i < numtype; i++) {
        types[i] = 0;
    }
	for (i = 0; (i < maxParticles) && (retval < numtype); i++) 	{
		if ((particle[i].type != TYPE_INACTIVE) &&
            !types[particle[i].spark - TYPE_1]) {
		    types[particle[i].spark - TYPE_1] = 1;
		    retval++;
		}
	}

	return retval;
}

int isintypes (int check) {
    int i;
    for (i = 0; i < maxParticles; i++) {
	    if (particle[i].spark == check)
		    return 1;
    }

	return 0;
}

int isinrockets (int check) {
    int i;
    for (i = 0; i < maxParticles; i++) {
	    int t = -1;
	    if (particle[i].type == TYPE_ROCKET)
            t = particle[i].spark;
	    if (t == check)
		    return 1;
    }

	return 0;
}

int playGameOnce() { /* returns 1 to play again, or 0 to quit */
	int timeToLaunch = 0;
	int timeBetweenLaunches = 30;
	int mouseButtonState = 0;

	int maxChains = 0;
	int nrChains = 0;
	int chainColor = 0;
	int chainType = 0;
	int chain123 = 0;
	int nrLives = 4;

	int gameRunning = 1;

	int lastTick = ticks;

    int i, j;
	clear_to_color(virscr, makecol(0, 0, 0));

	for (i = 0; i < maxParticles; i++) {
		particle[i].type = TYPE_INACTIVE;
	}

	while (gameRunning) {
	  while (lastTick >= ticks) rest(1);
		lastTick = ticks;

		/* input */
		if (key[KEY_ESC] || key[KEY_Q] || key[KEY_X])
			gameRunning = 0;

		if (mouse_b) {
			if (mouseButtonState < 2)
				mouseButtonState++;
			/* state 1 means you just clicked
			 * state 2 means you are holding the button down
			 * this is very useful */
		}
		else
  			mouseButtonState = 0;


		if (nrLives <= 0) {
			/* draw game over-screen */
			textprintf_centre_ex(virscr, font, SCREENW_HALF,SCREENH_HALF-20,
                makecol(255,255,255), makecol(0,0,0), "K.O.!!");
			textprintf_centre_ex(virscr, font, SCREENW_HALF,SCREENH_HALF-10,
                makecol(255,255,255), makecol(0,0,0),
                "The big chain is at %d", maxChains);
			textprintf_centre_ex(virscr, font, SCREENW_HALF,SCREENH_HALF,
                makecol(255,255,255), makecol(0,0,0), "See you next mission");

			/* frenetic button masher protection */
			if (nrLives > -30) /* thirty frames that you cannot click */
				nrLives--;
			else
				if (mouseButtonState == 1)
					gameRunning = 0;
		} else {
			/* do game stuff */

			int launchNewType = 0; /* launch a rocket this frame? */

			if (timeToLaunch <= 0) {
			    int numallowed = (int)(60.0 / timeBetweenLaunches);
			    if (numallowed > numtype)
                    numallowed = numtype;
                if (numallowed > numtypes()) {
                    launchNewType = TYPE_1 + (rand() % 4);
                } else if ((chain123) && !isinrockets(chainType)) {
                    launchNewType = chainType; /* don't leave the player stuck*/
                }
                if (!launchNewType)
                    do {
                        launchNewType = TYPE_1 + (rand() % 4);
                    } while (!isintypes(launchNewType));
                timeToLaunch = (rand() % timeBetweenLaunches)
                    + timeBetweenLaunches;

			}
			timeToLaunch--;


			/* big particle update loop */
			activeParticles = 0;
			int closestIndex = -1;
			int closestDistance = SCREENW * SCREENH;
			for (i = 0; i < maxParticles; i++) {
				struct CParticle* par = particle + i;

				if (par->type == TYPE_INACTIVE) {
					if (launchNewType) {
						launchNewRocket(particle + i, launchNewType);
						launchNewType = TYPE_INACTIVE;
					}

					continue;
				}

				activeParticles++;


				/* the only physics update common to all particle types: */
				par->x += par->dx;
				par->y += par->dy;

				/* now do different things for all particle types: */

				if (par->type == TYPE_ROCKET) {
					par->dy += 0.04; /* rockets have a different gravitation */

					if (par->y > SCREENH + 1) {
						if (nrLives)
							nrLives--;
						textout_centre_ex(virscr, font, "MISS!!",
                            (int)par->x, SCREENH-9,
                            makecol(0,0,0), makecol(255,0,0));
						par->type = TYPE_INACTIVE;
                        for (j = 0; j < maxParticles; j++) {
                            struct CParticle* buster = particle + j;

                            if (buster->type == TYPE_ROCKET)
                                explodeRocket(buster);
                        }
                        stop_sample(boom);
                        play_sample(boom,255,127,350,0);
                        timeBetweenLaunches = 30;
					}

					circle(virscr, (int)par->x, (int)par->y, 2, par->color);

					/* find the rocket closest to the mouse pointer
                        using pythagorathing */
					if (mouseButtonState == 1 && par->type == TYPE_ROCKET) {
						int dx = (int)(mouse_x - par->x);
						int dy = (int)(mouse_y - par->y);
						if (dx < 0) dx = -dx;
						if (dy < 0) dy = -dy;
						int distance = dx * dx + dy * dy;
						if (distance < closestDistance) {
							closestDistance = distance;
							closestIndex = i;
						}
					}
				} else { /* all other particles */
					par->dx *= 0.95;
					par->dy *= 0.95;
					par->dy += par->life * GRAVITATION;

					putpixel(virscr, (int)par->x, (int)par->y, par->color);
					if (par->life < 20)
                        putpixel(virscr, (int)par->x + 1, (int)par->y,
                                 par->color);
                    if (par->life < 15)
                        putpixel(virscr, (int)par->x, (int)par->y + 1,
                                 par->color);
                    if (par->life < 10)
                        putpixel(virscr, (int)par->x + 1, (int)par->y + 1,
                                 par->color);

					par->life++;
					if (par->life > 34) {
					    stop_sample(crack);
                        play_sample(crack,10,127,1000,0);
						par->type = TYPE_INACTIVE;
					}
				}

			}

			/* textprintf_ex(virscr, font, 0,0, makecol(255,255,255), makecol(0,0,0), "timebetween %d ", timeBetweenLaunches); */

			/* shoot rockets */
			if (closestIndex != -1 && closestDistance <= CLICK_RADIUS) {
				if (chain123 != 0
                    && (chainColor != particle[closestIndex].color)) {
					chain123 = 0;
					chainColor = 0;
					chainType = 0;
					nrChains = 0;
					timeBetweenLaunches = 30;
				}
				chainColor = particle[closestIndex].color;
				chainType = particle[closestIndex].spark;
				chain123++;
				explodeRocket(particle + closestIndex);
			}

		}

		/* draw chain-counter */
		int color = chainColor;
		if (chain123 > 0)
			circlefill(virscr, 13, SCREENH - 21, 10, color);
		if (chain123 > 1)
			circlefill(virscr, 36, SCREENH - 21, 10, color);
		if (chain123 > 2)
			circlefill(virscr, 59, SCREENH - 21, 10, color);
		textprintf_ex(virscr, font, 0,SCREENH-9,
            makecol(255,255,255), makecol(0,0,0),
            "chains %d / %d", nrChains, maxChains);


		if (chain123 == 3) {
			chain123 = 0;
			nrChains++;
            if (timeBetweenLaunches > 8)
                timeBetweenLaunches -= 2;

			if (nrChains > maxChains)
				maxChains = nrChains;
		}

		/* draw life energy */
		if (nrLives > 0) {
			for (i = 0; i < nrLives; i++) {
				int x = SCREENW - 17 - i * 28;
   				circlefill(virscr, x - 6, 11, 6, makecol(255,0,0));
   				circlefill(virscr, x + 6, 11, 6, makecol(255,0,0));
				triangle(virscr, x - 12, 13, x + 12, 13, x, 25,
                    makecol(255,0,0));
			}

			textprintf_right_ex(virscr, font, SCREENW, SCREENH-9,
                makecol(255,255,255), makecol(0,0,0), "freeware");
		}
		else
			textprintf_right_ex(virscr, font, SCREENW, SCREENH-9,
                makecol(255,255,255), makecol(0,0,0),
                "el Hamil (2003) and Joe Larson (2010)");


		/* draw mouse pointer */
		hline(virscr, mouse_x - 5, mouse_y, mouse_x + 5, makecol(255,255,255));
		vline(virscr, mouse_x, mouse_y - 5, mouse_y + 5, makecol(255,255,255));


		blit(virscr, screen, 0,0, 0,0, SCREENW, SCREENH);


		/* blur the screen and fade to black */
		if (colordepth == 32) {
		    int x, y;
			for (y = 0; y < SCREENH; y++) {
				int r2 = 0;
				int g2 = 0;
				int b2 = 0;
				long* p = (long*)(virscr->line[y]);
				for (x = 1; x < SCREENW; x++) {
					if (*p || *(p-1)) {
						int r = getr32(*p) * 3;
						int g = getg32(*p) * 3;
						int b = getb32(*p) * 3;

						/* weighted average:
						 * (this pixel * 0.75) + (the other pixel * 0.25) */
						r2 = (r + r2) >> 2;
						g2 = (g + g2) >> 2;
						b2 = (b + b2) >> 2;

						*p = makecol32(r2, g2, b2);

						/* actually, multiply the other pixel with 0.75
                            to make things darker */
						r2 = r >> 2;
						g2 = g >> 2;
						b2 = b >> 2;
					}
					p++;
				}
			}
		} else {
		    int x, y;
			for (y = 0; y < SCREENH; y++) {
				int r2 = 0;
				int g2 = 0;
				int b2 = 0;
				short* p = (short*)(virscr->line[y]);
				for (x = 1; x < SCREENW; x++) {
					if (*p || *(p-1)) {
						int r = getr(*p) * 3;
						int g = getg(*p) * 3;
						int b = getb(*p) * 3;

						/* weighted average:
						 * (this pixel * 0.75) + (the other pixel * 0.25) */
						r2 = (r + r2) >> 2;
						g2 = (g + g2) >> 2;
						b2 = (b + b2) >> 2;

						*p = makecol(r2, g2, b2);

						/* actually, multiply the other pixel
						 * with 0.75 to make things darker */
						r2 = r >> 2;
						g2 = g >> 2;
						b2 = b >> 2;
					}
					p++;
				}
			}
		}
	} /* end while (gameRunning) */

	if (nrLives <= 0)
		return 1; /* this is very silly...
                   * it shouldn't return 1 if you clicked ESC */

	return 0;
}

