#include "gfxwrapper.h"

BITMAP *scrbuf = 0;
BITMAP *console = 0;
volatile int ticks;
volatile int closebuttonpressed = 0;
float scale;
int scrx, scry, shiftx, shifty;
#define consolelines 60
#define consolewidth 80
char consolemessages[consolelines][consolewidth];
int showcon = 0;

struct Samples_t {
  SAMPLE* square1;
  SAMPLE* square2;
  SAMPLE* saw;
  SAMPLE* noise;
};

struct Samples_t Samples;

void ticker(void)
{
	ticks++;
}
END_OF_FUNCTION(ticker)

void X_button_handler(void) {
    closebuttonpressed = 1;
}
END_OF_FUNCTION(close_button_handler)

int initgfx(int width, int height) {
    if (allegro_init() != 0)
        return -1;
    install_keyboard();
    set_color_depth(32);
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, XSCREEN, YSCREEN, 0, 0) != 0) {
        if (set_gfx_mode(GFX_SAFE, XSCREEN, YSCREEN, 0, 0) != 0) {
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
            allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
            return -1;
        }
    }
	if (install_mouse() == -1) {
	    allegro_message("Unable to initiate mouse.\n%s\n", allegro_error);
	}
 	LOCK_FUNCTION(X_button_handler);
 	set_close_button_callback(X_button_handler);

    clear_to_color(screen, makecol(0, 0, 0));
 	scrx = width;
    scry = height;
    float scalex = (float)XSCREEN / (float)width;
    float scaley = (float)YSCREEN / (float)height;
    scale = (scalex < scaley) ? scalex : scaley;
    shiftx = (XSCREEN - (width * scale)) / 2;
    shifty = (YSCREEN - (height * scale)) / 2;

 	scrbuf = create_bitmap(scrx, scry);
    if (!scrbuf) {
        allegro_message("create_bitmap() failed.\n%s\n", allegro_error);
		exit(-1);
 	}

 	console = create_bitmap(scrx, scry);
    memset(consolemessages, 0, sizeof *consolemessages);
 	return 0;
}

void deinitgfx () {
    destroy_bitmap(scrbuf);
    allegro_exit();
}

int refresh(BITMAP *scr) {

    acquire_screen();
    stretch_sprite(screen, scr, shiftx, shifty, scrx * scale, scry * scale);
    if (showcon)
        draw_sprite(screen, console, 0, 0);
    release_screen();
    return 0;
}

void showconsole() {
    showcon = 1;
}

void hideconsole() {
    showcon = 0;
}

void consolemsg(char *msg, ...) {
	char buf[4096];
	int yy;

	va_list ap;
	va_start(ap, msg);
	vsprintf(buf, msg, ap);
	va_end(ap);

    for (yy = 0; yy < consolelines - 1; yy++) {
        sprintf (consolemessages[yy], consolemessages[yy + 1]);
    }
//    memcpy(consolemessages[0], consolemessages[1], consolelines - 1 * consolewidth);
    snprintf(consolemessages[consolelines - 1], consolewidth, buf);
    clear_to_color(console, bitmap_mask_color(console));
    for (yy = 0; yy < consolelines; yy++) {
        textout_ex(console, font, consolemessages[yy], 0, yy * 10,
                   makecol(yy * 3, yy * 3, yy * 3), -1);
    }
    if (showcon)
        refresh(scrbuf);
}

void begintimer() {
    ticks=0;
	LOCK_FUNCTION(ticker);
	LOCK_VARIABLE(ticks);
	install_int_ex(ticker,BPS_TO_TIMER(60));
}

int wait() {
    int running = 1;

    if (ticks > 1) {
        /* Log that we've overrun the timer */
qdebug(0,0,0,"Lag");
    }
    while (!ticks && running) {
        rest(1);
        if (closebuttonpressed || key[KEY_ESC])
            running = 0;
    }
    if (closebuttonpressed || key[KEY_ESC])
            running = 0;
    ticks--;
    return running;
}

void initsound () {
    int sq1_len = 100;
    int sq2_len = 200;
    int saw_len = 400;
    int noise_len = 5000;
    int i;

    if (install_sound(DIGI_AUTODETECT,MIDI_NONE,0)) {
        allegro_message("install_sound() failed.\n%s\n", allegro_error);
		exit(1);
    }

    Samples.square1 = create_sample(8,0,22050,sq1_len);
    unsigned char* square1 = (unsigned char*)Samples.square1->data;
    Samples.square2 = create_sample(8,0,22050,sq2_len);
    unsigned char* square2 = (unsigned char*)Samples.square2->data;
    Samples.saw = create_sample(8,0,22050,saw_len);
    unsigned char* saw = (unsigned char*)Samples.saw->data;

    for (i=0; i<sq1_len; ++i) {
        if (i<sq1_len/3)
            square1[i] = 200;
        else
            square1[i] = 0;
    }
    for (i=0; i<sq2_len; ++i) {
        if (i<sq2_len/2)
            square2[i] = 200;
        else
            square2[i] = 0;
    }
    for (i=0; i<saw_len; ++i) {
        saw[i] = 255 - (int)((double)i/(double)saw_len*(double)255);
    }

    Samples.noise = create_sample(8,0,22050,noise_len);
    unsigned char* noise = (unsigned char*)Samples.noise->data;
    for (i=0; i<noise_len; ++i) {
        noise[i] = rand()%255;
    }
}

void deinitsound() {
    destroy_sample(Samples.square1);
    destroy_sample(Samples.square2);
    destroy_sample(Samples.saw);
    destroy_sample(Samples.noise);
}

int randint (int low, int high) {
	int x, range = high - low - 1;
	if (range < 0) range = -range;
	range++;
	do {
        x = rand();
	} while (x > RAND_MAX - (RAND_MAX % range));
	return low + x % range;
}
