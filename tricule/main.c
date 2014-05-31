#include <time.h>
#include "gfxwrapper.h"
#include "tricule.h"
#include "game.h"

int delta[5] = {0, 1, 0, -1, 0};

struct regions regiondata[] = {
    { 0, 0         ,99, 99, 1},
    { 1, HOR | VERT, 7, 15, 3},
    { 2, 0         ,14, 15, 3},
    { 3,       VERT, 7, 15, 5},
    { 4,       VERT,10,  5, 3},
    { 5, HOR       , 8, 20, 7},
    { 6, HOR       , 4, 10, 3},
    { 7, HOR | VERT, 5, 15, 3},
    { 8, 0         ,25,  7, 3},
}; /* if the number changes change the #define in trule.h */

struct playerstruct playerdata[] = {
    {50},
    {60},
};

void qdebug(int x, int y, int pause, char *fmt, ...) {
	char buf[4096];

	va_list ap;
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

    acquire_screen();
    textprintf_ex(screen, font, x, y, makecol(255, 100, 200), -1, "%s", buf);
    release_screen();
    if (pause)
        if (readkey()>>8 ==KEY_ESC) exit(1);
    if(key[KEY_ESC]) exit(1);
}

int main() {
    srand(time(NULL));
    initgfx(XSCREEN,YSCREEN);
    initsound();

    mainloop();

    deinitsound();
    deinitgfx();
    return 0;
}
END_OF_MAIN()
