/*
===========================
Snake Clone
2008-12-17
by Andreas Jörgensen
Uses the Allegro multimedia library
Install it and link with -lalleg
===========================
*/

#include <allegro.h> //Graphics!
#include <stdio.h> //Sprintf! And rand()!
#include <ctime> //Random seed!
#include "snake.h" //Snake and SnakeTail classes!

#define SCR_H 20 //Resolutions!
#define SCR_W 25 //Resolutions again!
#define STR 20

void init();
void deinit();

SnakeTail TailArray[256];
Snake Player;

BITMAP* BOARD = NULL; //Double BOARDing, ofc.
BITMAP* BUFFER = NULL;

int AppleX, AppleY;

int main() {
	init();
    std::string gamestate = "menu"; //State used for displaying different parts of game
    std::string oldstate = "none";
    srand(time(0));

    AppleX = rand()%(SCR_W);
    AppleY = rand()%(SCR_H);

    char textbuf[50] = "HELLOOOOO"; //Temporary text FTW
    BOARD = create_bitmap(SCR_W, SCR_H); //Ahh, double BOARDing <3
    BUFFER = create_bitmap(SCR_W, SCR_H);
    std::string selectedItem = "S"; //For the menu!

    int selectCounter = 0;  //Counter for the menu options
	while (true) {

        if (gamestate == "game")
        {

            if (oldstate == "menu") //Check if we just started the game, and if such, reset everything
            {
                Player.reset();
                AppleX = rand()%(SCR_W);
                AppleY = rand()%(SCR_H);
                oldstate = "game"; //So we don't reset again and again...
                for (int j = 0; j < 256; ++j)
                {
                    if (j > 2)
                    {
                    TailArray[j].setActive(false); //Deactivate all but the first 3 tail-pieces
                    }
                    else
                    {
                        TailArray[j].setActive(true);
                        TailArray[j].setXY(Player.getX(), Player.getY());
                    }
                }
            }

		Player.move(); //Move the player, includes key handling
    TailArray[0].setXY(Player.getOldX(), Player.getOldY()); //Set the first tail piece to follow the player

    if (Player.getX() > SCR_W - 1) Player.setX(0); //Wrap player around screen
    if (Player.getX() < 0 ) Player.setX(SCR_W - 1);
    if (Player.getY() > SCR_H - 1) Player.setY(0);
    if (Player.getY() < 0) Player.setY(SCR_H - 1);

    sprintf(textbuf, "%d", Player.score);  //Print score! A bit hackish, mixing game logic and drawing like this.
    textout_ex(BUFFER, font, textbuf, 2, 2, makecol(50, 50, 50), -1);

		if (key[KEY_ESC]) {gamestate = "menu";} //Go back to the menu, durr

		for (int i = 0; i < 256; ++i)
		{
		    if (TailArray[i].getActive())
		    {

		        if (i > 0)
		        {
		            TailArray[i].setXY(TailArray[i-1].getOldX(), TailArray[i-1].getOldY()); //Make tail pieces follow each other!
		        }
		        TailArray[i].draw(BOARD); //Now DRAW THEM.
		    }
		}

        for (int i = 0; i < 256; ++i)
        {
            if ((Player.getX() == TailArray[i].getX()) && (Player.getY() == TailArray[i].getY()) && TailArray[i].getActive())
            {
                gamestate = "lose"; //OH NOES, I HIT MYSELF
                break;
            }
        }

		if ((Player.getX() == AppleX) && (Player.getY() == AppleY))
		{
		    AppleX = rand()%(SCR_W);
        AppleY = rand()%(SCR_H);
        Player.score++;
        for (int i = 0; i < 256; ++i)
        {
            if (!TailArray[i].getActive())
            {
                TailArray[i].setActive(true); //Grow by one tile-thing
                break;
            }
        }
		}

        putpixel(BOARD, AppleX, AppleY, makecol(0, 255, 0)); //Draw apple!
        Player.draw(BOARD);

        int sx = (SCR_W + Player.getX() - SCR_W/2) % SCR_W;
        int sy = (SCR_H + Player.getY() - SCR_H/2) % SCR_H;
        //putpixel(BUFFER, SCR_W - sx, SCR_H - sy, makecol(0, 0, 128));
        for (int xx = 0; xx < SCR_W; xx++)
          for (int yy = 0; yy < SCR_H; yy++) {
            int col = getpixel(BOARD, (xx + sx) % SCR_W, (yy + sy) % SCR_H);
            if (col) putpixel (BUFFER, xx, yy, col);
          }
        }

        else if (gamestate == "menu")
        {
            textout_ex(BUFFER, font, "S", 2, 2, makecol(255, 255, 255), -1); //Draw menu options
            textout_ex(BUFFER, font, "Q", 2, 12, makecol(255, 255, 255), -1);

            if (selectedItem == "S") textout_ex(BUFFER, font, "<", 10, 2, makecol((rand()%50)+180, 0, 0), -1); //Draw selection arrow
            if (selectedItem == "Q") textout_ex(BUFFER, font, "<", 10, 12, makecol((rand()%50)+180, 0, 0), -1);

            selectCounter++; //Just so the item selection won't flail around wildly
            if ((key[KEY_UP] || key[KEY_DOWN]) && selectedItem == "S" && selectCounter > 2) { selectedItem = "Q"; selectCounter = 0; }
            else if ((key[KEY_UP] || key[KEY_DOWN]) && selectedItem == "Q" && selectCounter > 2) { selectedItem = "S"; selectCounter = 0; }

            if (selectedItem == "S" && key[KEY_ENTER]) { gamestate = "game"; oldstate = "menu"; } //S as in START
            else if (selectedItem == "Q" && key[KEY_ENTER]) gamestate = "quit"; //Q as in QUIT
            //(only three characters could fit horizontally)
        }

        if (gamestate == "lose")
        {
            textout_ex(BUFFER, font, "scr", 1, 2, makecol(255, 255, 255), -1); //Haha, you lost! Printing score here!
            textout_ex(BUFFER, font, textbuf, 1, 12, makecol(80, 80, 80), -1);

            if (key[KEY_ESC]) gamestate = "menu";
        }

        if (gamestate == "quit") { deinit(); return 0; } //EXIT THE ALMIGHTY MAIN FUNCTION
		stretch_sprite(screen, BUFFER, 0, 0, SCR_W*STR, SCR_H*STR); //Draw BOARD stretched
		clear_bitmap(BOARD);
		clear_bitmap(BUFFER);
		vsync(); //Vsync, no tearing!
		rest(50); //Quick hack to make the game not go too fast. Change value to increase/decrease speed and responsiveness,
                //(responsiveness is inversely proportionate to speed)
	}

	deinit();
	return 0;
}
END_OF_MAIN()

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCR_W*STR, SCR_H*STR, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();
	set_window_title ("SNAAAKE");
	/* add other initializations here */
}

void deinit() {
	clear_keybuf();
	destroy_bitmap(BOARD);
	/* add other deinitializations here */
}

