#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <curses.h>

int wide, high;
//const char outchars[] = "#I:  :I#";
//const char outchars[] = "²±°  °±²";
const int outchars[] =
  {ACS_BLOCK,ACS_CKBOARD,ACS_BOARD,' ', ' ',ACS_BOARD,ACS_CKBOARD,ACS_BLOCK, 0};

int sgn(float n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    return 1;
}

void printintro() {
    mvprintw( (high * 0.2)  , (wide/2) -  8, "--== MANDALA ==--");

    mvprintw( (high * 0.8)  , (wide/2) - 24,
      "Original BASIC version: Creative Computing, 1979");
    mvprintw( (high * 0.8)+1, (wide/2) - 26,
      "C conversion for Cymon's Games: R. Alan Monroe, 2010");

    refresh();
    getch();
    clear();
}


void printlegend(dipfreq, dipimp, ripfreq, ripimp, combo) {
    mvprintw( 0,  (wide-14), "M A N D A L A");
    mvprintw( 1,  (wide-14), "=============");

    mvprintw( 4,  (wide-14), "        <d D>");
    mvprintw( 5,  (wide-14), "dipfreq: %2d", dipfreq);

    mvprintw( 8,  (wide-14), "        <e E>");
    mvprintw( 9,  (wide-14), "dipimp:  %2d", dipimp);

    mvprintw( 12,  (wide-14), "        <r R>");
    mvprintw( 13,  (wide-14), "ripfreq: %2d", ripfreq);

    mvprintw( 16, (wide-14), "        <f F>");
    mvprintw( 17, (wide-14), "ripimp:  %2d", ripimp);

    mvprintw( 20, (wide-14), "        <c C>");
    mvprintw( 21, (wide-14), "combo:   %2d", combo);

    mvprintw( 24, (wide-14), "ESC to quit", combo);
}

int drawmandala(s, dipfreq, dipimp, ripfreq, ripimp, combo) {
    if (dipfreq==0 && dipimp==0 && ripfreq==0 && ripimp==0 && combo==0)
      return 0;

    int f1=(dipfreq + 0.5);
    int f2=(ripfreq + 0.5);

    float c1 = abs(dipimp);
    float c2 = abs(ripimp);
    float c3 = abs(combo);
    float c = c1 + c2 + c3;
    c1 = 4 * c1 / c;
    c2 = 4 * c2 / c;
    c3 = 4 * c3 / c;

    float x, x1, x2, x3, y, r, a, g1, g2;
    int q, y2;
    char cp[] = ".";

    move(0, 0);
    for (y = s; y >= -s; y--) {
        y2 = y * y;
        x1 = s;
        x2 = -s;
        x3 = -0.5;

        for (x = x1; x > x2; x += x3) {
            r = sqrt(x * x + y2) / s;
            if (x != 0) {
                a = atan2(y,x);
            } else {
                a = M_PI * sgn(y) / 2.0;
            }

            g1 = sin(f1 * a);
            g2 = sin(f2 * r * M_PI);
            q = (c1 * g1 + c2 * g2 + c3 * g1 * g2 + 4) + 1;
            if (q>8) q=8; if (q<1) q=1;

            cp[0] = outchars[q-1];
            attron(A_ALTCHARSET);
            if (q>4) {
                attron(COLOR_PAIR(1));
                printw("%s", cp);
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                printw("%s", cp);
                attroff(COLOR_PAIR(2));
            }
            attroff(A_ALTCHARSET);

         }
         printw("\n");

    }
    printw("\n");
    return 0;
}



int main() {

    //setup --------------------------------------------------
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    getmaxyx(stdscr, high, wide);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);


    //main ====================================================
    int s=high/2 - 1;
    int dipfreq=7;
    int dipimp=0;
    int ripfreq=2;
    int ripimp=4;
    int combo=5;
    bool playing = TRUE;
    int k;

    printintro();

    drawmandala(s, dipfreq, dipimp, ripfreq, ripimp, combo);
    printlegend(dipfreq, dipimp, ripfreq, ripimp, combo);
    while (playing) {
        k = getch();
        switch (k) {
        //case 'q':
        //case 'Q':
        case 27:
            playing = FALSE;
            break;

        case 'd':
            if (dipfreq > 0) dipfreq--;
            break;
        case 'D':
            dipfreq++;
            break;

        case 'e':
            if (dipimp > 0) dipimp--;
            break;
        case 'E':
            dipimp++;
            break;

        case 'r':
            if (ripfreq > 0) ripfreq--;
            break;
        case 'R':
            ripfreq++;
            break;

        case 'f':
            if (ripimp > 0) ripimp--;
            break;
        case 'F':
            ripimp++;
            break;

        case 'c':
            if (combo > 0) combo--;
            break;
        case 'C':
            combo++;
            break;
        }
        drawmandala(s, dipfreq, dipimp, ripfreq, ripimp, combo);
        printlegend(dipfreq, dipimp, ripfreq, ripimp, combo);
    }

    //teardown -------------------------------------------------
    curs_set(1);
    endwin();
    return 0;
}
