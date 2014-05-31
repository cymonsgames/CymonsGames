#include <stdlib.h>
#include <time.h>
#include <curses.h>

#define BALL    'O'
#define PADDLE  ' '|A_REVERSE
#define NET     ACS_VLINE
#define B_SPEED 12 /* Ticks per ball movement at start */
#define P_SPEED 12 /* Paddle speed */
#define SPEED   2 /* Milliseconds per tick */
#define MIN     0
#define MAX     24
#define P1_Y    12
#define P1_X    1
#define P2_Y    12
#define P2_X    77
#define B_Y     12
#define B_X     38
#define B_DIRY  ((rand()%3)/4.)-.25

#ifdef NCURSES_MOUSE_VERSION
#define nc_getmouse(c) getmouse(c)
#define MOUSE_ENABLED
#endif
#ifdef PDCURSES
#define MOUSE_ENABLED
#endif

struct player {
    short y;
    short x;
    short ishuman;
    short score;
    short scorey;
    short scorex;
    short dir;
};

struct ball {
    float y;
    float x;
    short speed;
    float diry;
    float dirx;
    short spin;
};


void eraseball(struct ball b)
{
    mvaddch((int)b.y, (int)b.x, ' ');
}

void drawpaddle(struct player paddle)
{
    mvaddch(paddle.y-3, paddle.x, ' ');
    mvaddch(paddle.y-2, paddle.x, PADDLE);
    mvaddch(paddle.y-1, paddle.x, PADDLE);
    mvaddch(paddle.y, paddle.x, PADDLE);
    mvaddch(paddle.y+1, paddle.x, PADDLE);
    mvaddch(paddle.y+2, paddle.x, PADDLE);
    mvaddch(paddle.y+3, paddle.x, ' ');

    mvprintw(paddle.scorey, paddle.scorex, "%2i", paddle.score);
}

void drawnet(void)
{
    move(0, 38);
    vline(NET, 25);
}

void drawball(struct ball b)
{
    mvaddch((int)b.y, (int)b.x, BALL);
#ifdef DEBUG
    mvprintw(25, 0, "b.spin: %i", b.spin);
#endif
}

int controlpaddle(struct player *paddle, struct ball b, short control)
{
    int c;

    if (paddle->ishuman) {
        if (control == 1) {
            c = getch();
            flushinp();
            switch (c) {
                case KEY_UP:
                case 'w':
                    paddle->dir = (paddle->dir>=0)?-1:0;
                    break;
                case KEY_DOWN:
                case 's':
                    paddle->dir = (paddle->dir<=0)?1:0;
                    break;
                case 'q':
                    return 0;
                case '2':
                    return 2;
#ifdef MOUSE_ENABLED
                case '3':
                    return 3;
#endif
                default:
                    break;
            }
        } else if (control == 2) {
            c = getch();
            flushinp();
            switch (c) {
                case KEY_UP:
                case 'w':
                    paddle->dir = -1;
                    break;
                case KEY_DOWN:
                case 's':
                    paddle->dir = 1;
                    break;
                case ' ':
                    paddle->dir = 0;
                    break;
                case 'q':
                    return 0;
                case '1':
                    return 1;
#ifdef MOUSE_ENABLED
                case '3':
                    return 3;
#endif
                default:
                    break;
            }
        }
#ifdef MOUSE_ENABLED
        else if (control == 3) {
            MEVENT event;

            if (nc_getmouse(&event) == OK) {
                if (event.y > paddle->y) {
                    paddle->dir = 1;
                } else if (event.y < paddle->y) {
                    paddle->dir = -1;
                } else {
                    paddle->dir = 0;
                }
            }
            c = getch();
            flushinp();
            switch (c) {
                case 'q':
                    endwin();
                    exit(0);
                case '1':
                    return 1;
                case '2':
                    return 2;
                default:
                    break;
            }
        }
#endif
    } else { /* !paddle->ishuman */
        if ((int)b.y > paddle->y) {
            paddle->dir = 1;
        } else if ((int)b.y < paddle->y) {
            paddle->dir = -1;
        } else if ((int)b.y == paddle->y) {
            paddle->dir = 0;
            if (b.diry == 0 && b.x == paddle->x - 1) {
                paddle->dir = (rand() % 3) - 1;
            }
        }
    }
    return control;
}

void updatepaddle(struct player *paddle)
{
    paddle->y += paddle->dir;
    if (paddle->y >= MAX) {
        paddle->y = MAX;
        paddle->dir = 0;
    } else if (paddle->y <= MIN) {
        paddle->y = MIN;
        paddle->dir = 0;
    }
}

int updateball(struct ball *b, struct player *p1, struct player *p2)
{
    b->x += b->dirx;
    /* Bounce the ball off a paddle */
    if ((int)b->x == p1->x && (int)b->y <= p1->y+2 && (int)b->y >= p1->y-2) {
        b->diry += b->dirx * .25 * (float)b->spin;
        if (p1->dir == 1) {
            b->diry += .25;
            b->spin += 1;
        } else if (p1->dir == -1) {
            b->diry -= .25;
            b->spin -= 1;
        } else {
            b->spin = 0;
        }
        b->dirx = -b->dirx;
        b->x += 2;
        beep();
    } else if ((int)b->x == p2->x && (int)b->y <= p2->y+2 && \
               (int)b->y >= p2->y-2) {
        b->diry += b->dirx * .25 * (float)b->spin;
        if (p2->dir == 1) {
            b->diry += .25;
            b->spin -= 1;
        } else if (p2->dir == -1) {
            b->diry -= .25;
            b->spin += 1;
        } else {
            b->spin = 0;
        }
        b->dirx = -b->dirx;
        b->x -= 2;
        beep();
    }

    b->y += b->diry;
    /* Bounce the ball off an edge */
    if (b->y > MAX) {
        b->diry += b->dirx * .25 * (float)b->spin;
        b->spin = 0;
        b->y = MAX;
        b->diry = -b->diry;
        beep();
    } else if (b->y < MIN) {
        b->diry += b->dirx * .25 * (float)b->spin;
        b->spin = 0;
        b->y = MIN;
        b->diry = -b->diry;
        beep();
    }

    /* Ball slipped past */
    if (b->x < p1->x) {
        ++p2->score;
        b->y = B_Y;
        b->x = B_X;
        b->speed = B_SPEED;
        b->diry = B_DIRY;
        b->dirx = -b->dirx;
        b->spin = 0;
        flash();
        return 1;
    } else if (b->x > p2->x) {
        ++p1->score;
        b->y = B_Y;
        b->x = B_X;
        b->speed = B_SPEED;
        b->diry = B_DIRY;
        b->dirx = -b->dirx;
        b->spin = 0;
        flash();
        return 1;
    }
    return 0;
}

int playgame(short control)
{
    struct player p1 = {P1_Y, P1_X, 1, 0, 0, 35, 0};
    struct player p2 = {P2_Y, P2_X, 0, 0, 0, 40, 0};
    struct ball b = {B_Y, B_X, B_SPEED, B_DIRY, 1, 0};
    short gameover = 0;
    short bcounter = 0;
    short bbcounter = 0;
    short pcounter = 0;
    int c;

    while (!gameover) {
        /* Draw everything and pause for one tick. */
        drawpaddle(p1);
        drawpaddle(p2);
        drawnet();
        drawball(b);
        refresh();
        napms(SPEED);

        /* Get input. */
        control = controlpaddle(&p1, b, control);
        if (!control)
            break;
        controlpaddle(&p2, b, 0);
        if (pcounter++ == P_SPEED) {
            updatepaddle(&p1);
            updatepaddle(&p2);
            pcounter = 0;
        }
        if (bcounter++ == b.speed) {
            eraseball(b);
            if (updateball(&b, &p1, &p2) == 0) {
                bcounter = 0;
                if (bbcounter++ == 200 && b.speed > 1)
                    --b.speed;
            } else {
                bcounter = 0;
                bbcounter = 0;
                p1.y = P1_Y;
                p1.x = P1_X;
                p1.dir = 0;
                p2.y = P2_Y;
                p2.x = P2_X;
                p2.dir = 0;
                clear();
                drawpaddle(p1);
                drawpaddle(p2);
                drawnet();
                drawball(b);
                refresh();
                /* Wait for space or a mouse click */
                do {
                    c = getch();
                    napms(10);
                    if (c == 'q')
                        return 0;
                } while ((c != ' ') && (c != KEY_MOUSE));
                if (p1.score == 21) {
                    return 1;
                } else if (p2.score == 21) {
                    return 2;
                }
            }
        }
        napms(SPEED);
    }
    return 0;
}

short menu(void)
{
    struct player p = {5, 5, 1, 0, 0, -1, 0};
    short item = 1;
    int c, i, j = 0;
    char *items[4] = {
        "Play (keyboard, same to stop)",
        "Play (keyboard, space to stop)",
        "Play (mouse)",
        "Quit"
    };

    attron(A_BOLD);
    mvprintw(1, 35, "ASCIIpong");
    attroff(A_BOLD);
    for (i=0; i<4; i++)
        mvprintw(5+i*4, 10, items[i]);
    mvprintw(24, 0, "Modes may be changed during gameplay with 1, 2, and 3");
    while ((c = getch()) != '\n' && c != ' ') {
        drawpaddle(p);
        refresh();
        /* Get input */
        if ((c == KEY_UP || c == 'w') && p.y > 5 && j == 0) {
            j = -4;
            --item;
        } else if ((c == KEY_DOWN || c == 's') && p.y < 17 && j == 0) {
            j = 4;
            ++item;
        }
        if (j > 0) {
            ++p.y;
            --j;
        } else if (j < 0) {
            --p.y;
            ++j;
        }
        napms(20);
    }

    return item;
}

int main(int argc, char *argv[])
{
    short control = 1;

    /* Initialize curses. */
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
#ifdef MOUSE_ENABLED
    mousemask (ALL_MOUSE_EVENTS, NULL);
#endif
/* Let ^C work normally if it's a debug build */
#ifdef DEBUG
    cbreak();
/* Otherwise, send it to the program for it to be ignored */
#else
    raw();
#endif

    /* Set the random seed to the current time */
    srand(time(NULL));

    while ((control = menu()) != 4) {
        erase();
        playgame(control);
        erase();
    }

    /* Clean up. */
    endwin();
    return 0;
}
