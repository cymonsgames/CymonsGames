// Absorber by Joseph Larson 18-Dec-2009
// Took me less than 12 hours to put this thing together to a playable state.
// Version history:
// Ver 17-Dec-2009 - First running version.
// Ver 18-Dec-2009 - minor tweaks the colors on the menu, increased the framerate.

#include <allegro.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

volatile int tick;
int fullscreen = 0;
int colordepth = 16;
BITMAP* buffer = NULL;

int SCREENW = 800;
int SCREENH = 600;
// absorber specific stuff goes here.
const int player_radius = 18;
const int max_delta = 6;

class Blob {
public:
  int x, y;
  int dx, dy;
  double radius;
  double Area;
  int playerid;
  Blob (int new_x = 0, int new_y = 0, double new_radius = 1.0) {
    this->x = new_x;
    this->y = new_y;
    this->dx = 0;
    this->dy = 0;
    this->radius = new_radius;
    this->calc_area ();
    this->playerid = 0;
  }
  int check_collide (Blob* obj);
  int collides_with (Blob* obj);
  void calc_area ();
  void calc_radius ();
};

int Blob::check_collide (Blob* obj) {
  if( (obj->x - obj->radius) > (this->x + this->radius) ) return 0;
  if( (obj->x + obj->radius) < (this->x - this->radius) ) return 0;
  if( (obj->y - obj->radius) > (this->y + this->radius) ) return 0;
  if( (obj->y + obj->radius) < (this->y - this->radius) ) return 0;
  int xx = abs(this->x - obj->x);
  int yy = abs(this->y - obj->y);
  double d = xx * xx + yy * yy; // Pythagorean theorem minus the sqrt to save processor cycles
  if (d <= ((this->radius + obj->radius) * (this->radius + obj->radius))) { // we have overlap
    return 1;
  }
  return 0;
}

int Blob::collides_with (Blob* obj) {
  if (this->check_collide (obj)) {
    int xx = abs(this->x - obj->x);
    int yy = abs(this->y - obj->y);
    double d = (sqrt ((xx * xx) + (yy * yy))); // Pythagorean theorem
    Blob *giver = this;
    Blob *reciever = obj;
    if (this->radius > obj->radius) {
      giver = obj;
      reciever = this;
    }
    double oldArea = giver->Area;
    double overlap = (giver->radius + reciever->radius) - d;
    if (overlap < 0) overlap = 0; //-overlap;
    giver->radius -= overlap;
    if (giver->radius < 0.5) {
      giver->radius = 0.0;
      giver->Area = 0;
    } else giver->calc_area();
    reciever->Area += oldArea - giver->Area;
    reciever->calc_radius();
    return 1;
  }
  return 0;
}

void Blob::calc_area () {
  this->Area = M_PI * this->radius * this->radius;
}

void Blob::calc_radius () {
  if (this->Area < 5) radius = 1;
  else this->radius = sqrt(this->Area / (double)M_PI);
}
// end Blob

const int tot_players = 3;
vector <Blob> blobs;
int player_key[tot_players][4] = {
  // up, down, left, right
  {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT},
  {KEY_W, KEY_S, KEY_A, KEY_D},
  {KEY_I, KEY_K, KEY_J, KEY_L}
};
int player_colors[tot_players][2];
int players_start[tot_players][2] = {
  {SCREENW / 4, SCREENH / 4},
  {3 * (SCREENW / 4), SCREENH / 4},
  {SCREENW / 2, 3 * (SCREENH / 4)}
};
int humanplayer [tot_players] = {1, 1, 1};
char face[6] = "";
char faces[][6] = {"", "..", "^_^","=^.^=","~_~", "\"", "O_O", "oo", "O_o", "\\/", 0};
int facenum = 0;

void movecomputer (int c) { // find the closest blob and either move towards or away from them
  int closest = c;
  double distance = SCREENW * SCREENW + SCREENH * SCREENH;

  for (int d = 0; d < blobs.size(); d++) {
    if (d == c) continue;
    int xx = abs(blobs[c].x - blobs[d].x);
    int yy = abs(blobs[c].y - blobs[d].y);
    double p = xx * xx + yy * yy;
    if (p < distance) {
      distance = p;
      closest = d;
    }
  }
  int dx = (blobs[closest].x > blobs[c].x) ? 1 : -1;
  int dy = (blobs[closest].y > blobs[c].y) ? 1 : -1;
  if (blobs[closest].radius < blobs[c].radius) {
    blobs[c].dx += dx;
    blobs[c].dy += dy;
  } else {
    blobs[c].dx -= dx;
    blobs[c].dy -= dy;
  }
  if (blobs[c].dx > max_delta / 2) blobs[c].dx = max_delta / 2;
  if (blobs[c].dy > max_delta / 2) blobs[c].dy = max_delta / 2;
  if (blobs[c].dx < -max_delta / 2) blobs[c].dx = -max_delta / 2;
  if (blobs[c].dy < -max_delta / 2) blobs[c].dy = -max_delta / 2;
}

int moveplayers () {
  int numplayers = 0;
  int players[tot_players];

  for (int c = 0; c < tot_players; c++)
    players[c] = -1;
  for (int c = 0; c < tot_players; c++) {
    if ((blobs.size() > c) && (blobs[c].playerid)) {
      players[blobs[c].playerid - 1] = c;
      numplayers++;
    }
  }

  if (key[KEY_F10]) {
    facenum ++;
    if (faces[facenum][0] == 0) facenum = 0;
    int i = 0;
    do {
      face[i] = faces[facenum][i];
    } while (faces[facenum][i++] != '\0');
  }

  for (int p = 0; p < tot_players; p++) {
    if (players[p] >= 0) {
      if (humanplayer[p]) {
        if (key[player_key[p][0]]) blobs[players[p]].dy--;
        else if (key[player_key[p][1]]) blobs[players[p]].dy++;
        else {
          if (blobs[players[p]].dy > 0) blobs[players[p]].dy--;
          if (blobs[players[p]].dy < 0) blobs[players[p]].dy++;
        }
        if (key[player_key[p][2]]) blobs[players[p]].dx--;
        else if (key[player_key[p][3]]) blobs[players[p]].dx++;
        else {
          if (blobs[players[p]].dx > 0) blobs[players[p]].dx--;
          if (blobs[players[p]].dx < 0) blobs[players[p]].dx++;
        }
      } else movecomputer (players[p]);
      if (blobs[players[p]].dx > max_delta) blobs[players[p]].dx = max_delta;
      if (blobs[players[p]].dy > max_delta) blobs[players[p]].dy = max_delta;
      if (blobs[players[p]].dx < -max_delta) blobs[players[p]].dx = -max_delta;
      if (blobs[players[p]].dy < -max_delta) blobs[players[p]].dy = -max_delta;
      blobs[players[p]].x += blobs[players[p]].dx;
      blobs[players[p]].y += blobs[players[p]].dy;
    }
  }
  return numplayers;
}

void mainmenu() {
  while (!key[KEY_SPACE] && !key[KEY_ENTER]&& !key[KEY_ESC]) {
    clear_to_color(screen, makecol(0, 0, 0));
    for (int p = 0; p < tot_players; p++)
      textprintf_centre_ex(screen, font, SCREENW / 2, SCREENH / 2 - 30 + 20 * p, player_colors[p][1], makecol(0,0,0),
        "(%d) Player %d - %s", p + 1, p + 1, humanplayer[p] ? "human   " : "computer");
    textprintf_centre_ex(screen, font, SCREENW / 2, 80, makecol(255,255,255), makecol(0,0,0), "ABSORBER");
    textprintf_centre_ex(screen, font, SCREENW / 2, 90, makecol(255,255,255), makecol(0,0,0), "There can be only one");
    textprintf_centre_ex(screen, font, SCREENW / 2, 100, makecol(255,255,255), makecol(0,0,0), "by Joe Larson");
    textprintf_centre_ex(screen, font, SCREENW / 2, SCREENH / 2 - 20 + 20 * tot_players, makecol(255,255,255), makecol(0,0,0), "Press ENTER to begin");
    textprintf_centre_ex(screen, font, SCREENW / 4, SCREENH - 130, player_colors[0][1], makecol(0,0,0), "Arrow");
    textprintf_centre_ex(screen, font, SCREENW / 4, SCREENH - 120, player_colors[0][1], makecol(0,0,0), "Keys");
    textprintf_centre_ex(screen, font, SCREENW / 4, SCREENH - 100, player_colors[0][1], makecol(0,0,0), "PLAYER 1");
    textprintf_centre_ex(screen, font, SCREENW / 2, SCREENH - 130, player_colors[1][1], makecol(0,0,0), "W");
    textprintf_centre_ex(screen, font, SCREENW / 2, SCREENH - 120, player_colors[1][1], makecol(0,0,0), "ASD");
    textprintf_centre_ex(screen, font, SCREENW / 2, SCREENH - 100, player_colors[1][1], makecol(0,0,0), "PLAYER 2");
    textprintf_centre_ex(screen, font, 3 * SCREENW / 4, SCREENH - 130, player_colors[2][1], makecol(0,0,0), "I");
    textprintf_centre_ex(screen, font, 3 * SCREENW / 4, SCREENH - 120, player_colors[2][1], makecol(0,0,0), "JKL");
    textprintf_centre_ex(screen, font, 3 * SCREENW / 4, SCREENH - 100, player_colors[2][1], makecol(0,0,0), "PLAYER 3");
    textprintf_centre_ex(screen, font, SCREENW / 2, SCREENH - 80, makecol(255,255,255), makecol(0,0,0), "F1 to return to this menu   ESC to quit");
    int in = readkey () & 0xff;
    if ((in >= '1') && (in <= '1' + tot_players)) {
      humanplayer[in - '1'] = !humanplayer[in - '1'];
    }
  }
}

int gameloop () {
  //setup board
  player_colors[0][0] = makecol (0,128,0); player_colors[0][1] = makecol (0,156,0);
  player_colors[1][0] = makecol (0,0,128); player_colors[1][1] = makecol (0,0,156);
  player_colors[2][0] = makecol (128,128,0); player_colors[2][1] =  makecol (200,200,0);

  mainmenu();

  blobs.clear();
  for (int p = 0; p < tot_players; p++) {
    blobs.push_back(Blob(players_start[p][0], players_start[p][1], player_radius));
    blobs.back().playerid = p + 1;
  }
  for (int rad = 10; rad >= 1; rad -= 2) { // 1 blob or radius 10 to 10 blobs of radius 1.
    for (int num = 0; num < 2 * (11 - rad); num++) {
      int tries = 3;
      Blob* newblob = new Blob();
      newblob->radius = 3 * rad;
      newblob->calc_area ();
      while (tries) {
        tries--;
        newblob->x = (rand() % (SCREENW - (2 * (int)newblob->radius))) + (int)newblob->radius;
        newblob->y = (rand() % (SCREENH - (2 * (int)newblob->radius))) + (int)newblob->radius;
        newblob->dx = (rand () % 5 - 2) / 2;
        newblob->dy = (rand () % 5 - 2) / 2;
        if (blobs.size()) {
          int c;
          for (c = 0; c < blobs.size() && !newblob->check_collide (&blobs[c]); c++);
          if (c == blobs.size()) {
            blobs.push_back(*newblob);
            tries = 0;
          }
        } else blobs.push_back (*newblob);
      }
      delete newblob;
    }
  }

  int numplayers = tot_players;
  tick = 0;
  while ((!key[KEY_ESC]) && (!key[KEY_F1]) && (numplayers > 1)) {
    while (!tick) rest(1);
    // main game goes here.
    while (tick) {
      tick--;

      numplayers = moveplayers ();

      for (int c = 0; c < blobs.size(); c++) {
        blobs[c].x += blobs[c].dx;
        blobs[c].y += blobs[c].dy;
        if ((blobs[c].x - blobs[c].radius) < 0) {
          blobs[c].x = blobs[c].radius;
          blobs[c].dx = -blobs[c].dx;
        }
        if ((blobs[c].y - blobs[c].radius) < 0) {
          blobs[c].y = blobs[c].radius;
          blobs[c].dy = -blobs[c].dy;
        }
        if ((blobs[c].x + blobs[c].radius) > SCREENW) {
          blobs[c].x = SCREENW - blobs[c].radius;
          blobs[c].dx = -blobs[c].dx;
        }
        if ((blobs[c].y + blobs[c].radius) > SCREENH) {
          blobs[c].y = SCREENH - blobs[c].radius;
          blobs[c].dy = -blobs[c].dy;
        }

        for (int d = c + 1; d < blobs.size(); d++) {
          if (blobs[c].radius < 0.5) continue;
          blobs[c].collides_with(&blobs[d]);
        }
        if (blobs[c].radius < 0.5) {
          blobs.erase(blobs.begin() + c);
          c--;
        }
      }

      // draw screen
      for (int c = 0; c < blobs.size(); c++) {
        int col = makecol (156, 128 - (c * (128.0 / blobs.size())), (c * (128.0 / blobs.size())));
        for (int p = 0; p < tot_players; p++) {
          if (blobs[c].playerid == p + 1)
            col = player_colors[p][0];
        }
        circlefill (buffer, blobs[c].x, blobs[c].y, (int)blobs[c].radius, col);
        col = makecol (255, 128 - (c * (128.0 / blobs.size())), (c * (128.0 / blobs.size())));
        for (int p = 0; p < tot_players; p++) {
          if (blobs[c].playerid == p + 1)
            col = player_colors[p][1];
        }
        circlefill (buffer, blobs[c].x + (blobs[c].radius / 6), blobs[c].y - (blobs[c].radius / 6), (int)(3 * blobs[c].radius / 4), col);
        if (blobs[c].radius > 10) {
//          textprintf_centre_ex(buffer, font, blobs[c].x + blobs[c].dx, blobs[c].y - 3 + blobs[c].dy, makecol(255,255,255), -1, "oo");
//          textprintf_centre_ex(buffer, font, blobs[c].x + blobs[c].dx - 1, blobs[c].y - 5 + blobs[c].dy, makecol(255,255,255), -1, "..");
          textprintf_centre_ex(buffer, font, blobs[c].x + blobs[c].dx, blobs[c].y - 5 + blobs[c].dy, makecol(0,0,0), -1, face);
        }
      }
      blit(buffer, screen, 0, 0, 0, 0, SCREENW, SCREENH);
      clear_to_color(buffer, makecol(0, 0, 0));
    }
  }

  if (numplayers == 2) return 0;
  textout_centre_ex(screen, font, "GAME OVER", SCREENW / 2, SCREENH / 2 - 20, makecol(255, 255, 255), -1);
  if (numplayers == 0)
    textout_centre_ex(screen, font, "No winner", SCREENW / 2, SCREENH / 2 + 20, makecol(255, 255, 255), -1);
  else if (blobs[0].playerid)
    textprintf_centre_ex(screen, font, SCREENW / 2, SCREENH / 2, player_colors[blobs[0].playerid - 1][0], -1, "Playser %d wins", blobs[0].playerid);
  textout_centre_ex(screen, font, "Press ENTER", SCREENW / 2, SCREENH / 2 + 20, makecol(255, 255, 255), -1);
  while(!key[KEY_ENTER] && !key[KEY_SPACE] && !key[KEY_ESC]&& !key[KEY_F1]) { rest(1); }
  blobs.clear();
  return 1;
}

// Generic stuff I always need down here.

void ticker(void)
{
  tick++;
}
END_OF_FUNCTION(ticker)

void init () {
  if (allegro_init() != 0)
  {
    cout << "allegro_init() failed" << endl;
    exit(1);
  }

  tick=0;
  LOCK_FUNCTION(ticker);
  LOCK_VARIABLE(tick);
  install_int_ex(ticker, MSEC_TO_TIMER(66)); // change this value to change the speed of the game.

  if (install_keyboard() != 0)
  {
    cout << "install_keyboard() failed" << endl;
    exit(1);
  }

//  if (install_mouse() == -1)
//  {
//    cout << "install_mouse() failed" << endl;
//    exit(1);
//  }

  set_color_depth(colordepth);

  int result = -1;
  if (fullscreen)
    result = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, SCREENW, SCREENH, 0, 0);
  else
    result = set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREENW, SCREENH, 0, 0);
  if (result < 0)
  {
    cout << "set_gfx_mode() failed" << endl;
    exit(1);
   }

  buffer = create_bitmap(SCREENW, SCREENH);
  if (!buffer)
  {
    cout << "create_bitmap() failed" << endl;
    exit(1);
   }

  srand(time(NULL));
}

int main(int argc, char* argv[])
{
  init ();

  do gameloop(); while (!key[KEY_ESC]);

  destroy_bitmap(buffer);

  return 0;
}
END_OF_MAIN();
