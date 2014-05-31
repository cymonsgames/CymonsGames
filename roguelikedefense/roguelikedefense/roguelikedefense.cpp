#include <iostream>
#include <curses.h>
#include <ctime>
#include <cstdlib>
#include <math.h>

using namespace std;

class Game {
public:
    int lives;
    int wave_counter;
    int gold;
    int wave;
    int wood;
    int mon_levelMax;
    int mon_onScreenMax;
    int speed;
    int basehp;
};

class Menu {
public:
    int x;
    int y;
    int h;
    int w;
    string text;
    void Draw() {
        // fill area with char
        for (int y2 = y; y2 < (y+h); y2++) {
            for (int x2 = x; x2 < (x+w); x2++) {
                mvaddch(y2,x2,'#'|COLOR_PAIR(4));
            }
        }
        // fill center with nothing
        for (int y2 = y+1; y2 < ((y+h)-1); y2++) {
            for (int x2 = x+1; x2 < ((x+w)-1); x2++) {
                mvaddch(y2,x2,' '|COLOR_PAIR(4));
            }
        }
    }
    void DrawText(int y2, int x2, string text,int num) {
        if (num != -1) {
            string tmp = text + " %i";
            mvprintw((y2+y)+1,(x2+x)+1,tmp.c_str(),num);
        } else {
            mvprintw((y2+y)+1,(x2+x)+1,text.c_str());
        }
    }
};

class MultiMenu {
public:
    Menu Menus[100];
    int Active_menus;
};

class cur {
public:
    int y;
    int x;
    char c;
};

class monster {
public:
    int x;
    float move_speed;
    float move_count;
    int lane;
    bool alive;
    bool onScreen;
    bool slow;
    void RandLane() {
        bool Done = false;
        while (Done == false) {
            lane = rand()%20;
            for (int i = 1; i < 21; i+=2) {
                if (lane == i) {
                    Done = true;
                }
            }
        }
    }
    int hp;
    int damage;
    char c;
};

class bullet {
public:
    int lane;
    int x;
    bool shoot;
    char c;
};

monster monsters[1000];
Game game;

bool IsMonsterAtPoin(int y0, int x0) {
    for (int i = 0; i < 1000; i++) {
        if (monsters[i].lane == y0) {
            if (monsters[i].x == x0) {
                if (monsters[i].alive == true) {
                    if (monsters[i].onScreen == true) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool IsMonsterInLane(int lane) {
    for (int i = 0; i < 1000; i++) {
        if (monsters[i].lane == lane) {
            if (monsters[i].alive == true) {
                if (monsters[i].onScreen == true) {
                    return true;
                }
            }
        }
    }
    return false;
}

int GetMonsterAtPoin(int y0, int x0) {
    for (int i = 0; i < 1000; i++) {
        if (monsters[i].lane == y0) {
            if (monsters[i].x == x0) {
                if (monsters[i].alive == true) {
                    if (monsters[i].onScreen == true) {
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}

int dist(int y0,int x0,int y1,int x1) {
    return pow(y0-y1,2)+pow(x0-x1,2);
}
bool in_range(int y0,int x0,int y1,int x1,int r) {
    return dist(y0,x0,y1,x1)<=pow(r,2);
}

void DrawExpostion(int lane, int x0, int r,int damage) {
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 40; x++) {
            if (in_range(lane,x0,y,x,r)) {
                for (int i = 0; i < 1000; i++) {
                    if (monsters[i].alive == true) {
                        if (monsters[i].onScreen == true) {
                            if (in_range(monsters[i].lane,monsters[i].x,y,x,r)) {
                                monsters[i].hp -= damage;
                            }
                        }
                    }
                }
                int tempint = rand()%2;
                if (tempint == 0) {
                    mvaddch(y,x,'@'|COLOR_PAIR(2));
                } else if (tempint == 1) {
                    mvaddch(y,x,'#'|COLOR_PAIR(2)|A_BOLD);
                }
            }
        }
    }
}
int FireMap[21][41];


class tower {
public:
    int lane;
    int x;
    int damage;
    string name;
    bullet bullets[200];
    int shoot;
    int shoot_speed;
    int shoot_count;
    bool placed;
    int hp;
    char c;
    char bc;
    void init(int dam,int shootspeed) {
        shoot_count = 0;
        shoot_speed = shootspeed;
        damage = dam;
    }
    void reset() {
        for (int i = 0; i < 200; i++) {
            bullets[i].shoot = false;
        }
        placed = false;
    }
    void Shoot() {
        if (name != "Wood Field") {
            if (name != "Mine Deposit") {
                if (shoot_count >= shoot_speed) {
                    if (IsMonsterInLane(lane) == true) {
                        bullets[shoot].lane = lane;
                        bullets[shoot].x = x+1;
                        bullets[shoot].shoot = true;
                        shoot += 1;
                        if (shoot > 199) {
                            shoot = 0;
                        }
                        shoot_count = 0;
                    }
                } else {
                    shoot_count++;
                }
            }
        }
    }
    void GiveResource() {
        if (name == "Wood Field") {
            if (shoot_count >= shoot_speed) {
                game.wood += damage;
                shoot_count = 0;
            } else {
                shoot_count++;
            }
        } else if (name == "Mine Deposit") {
            if (shoot_count >= shoot_speed) {
                game.gold += damage;
                shoot_count = 0;
            } else {
                shoot_count++;
            }
        }
    }
    void Move_Bullets() {
        for (int i = 0; i < 200; i++) {
            if (bullets[i].shoot == true) {
                if (IsMonsterAtPoin(lane,bullets[i].x) == true) {
                    if (GetMonsterAtPoin(lane,bullets[i].x) != -1) {
                        monsters[GetMonsterAtPoin(lane,bullets[i].x)].hp -= damage;
                        if (name == "Slow Gun") {
                            monsters[GetMonsterAtPoin(lane,bullets[i].x)].slow = true;
                            FireMap[bullets[i].lane][bullets[i].x] = 8;
                        } else if (name == "Magic Wizard") {
                            FireMap[bullets[i].lane][bullets[i].x] = 5;
                        } else if (name == "Catapult") {
                            FireMap[bullets[i].lane][bullets[i].x] = 2;
                        } else if (name == "Cannon Tower") {
                            FireMap[bullets[i].lane][bullets[i].x] = 1;
                        } else if (name == "Arrow Tower") {
                            FireMap[bullets[i].lane][bullets[i].x] = 9;
                        }
                        bullets[i].shoot = false;
                    } else {
                        bullets[i].x++;
                        if (bullets[i].x > 39) {
                            bullets[i].shoot = false;
                        }
                    }
                } else {
                    bullets[i].x++;
                    if (bullets[i].x > 39) {
                        bullets[i].shoot = false;
                    }
                }
            }
        }
    }
    void PlaceTower(int y0,int x0,char c0) {
        lane = y0;
        x = x0;
        c = c0;
        placed = true;
    }
    void DrawTower() {
        mvaddch(lane,x,c);
    }
    void DrawBullets() {
        for (int i = 0; i < 200; i++) {
            if (bullets[i].shoot == true) {
                mvaddch(lane,bullets[i].x,bc);
            }
        }
    }

};

cur curs;
Menu sideMenu;
Menu errorMenu;
MultiMenu bottomMenu;
tower towers[200];
int towers_count = 0;
int unitsel = 0;
bool end = false;
bool startGame = false;

bool IsTowerAtPoin(int y0, int x0) {
    for (int i = 0; i < 200; i++) {
        if (towers[i].lane == y0) {
            if (towers[i].x == x0) {
                if (towers[i].placed == true) {
                    return true;
                }
            }
        }
    }
    return false;
}

void DoDamageAtPoin(int y0, int x0,int damage) {
    for (int i = 0; i < 200; i++) {
        if (towers[i].lane == y0) {
            if (towers[i].x == x0) {
                if (towers[i].placed == true) {
                    towers[i].hp -= damage;
                    if (towers[i].hp <= 0) {
                        towers[i].placed = false;
                    }
                }
            }
        }
    }
}

bool GetTowerAtPoin(int y0, int x0) {
    for (int i = 0; i < 200; i++) {
        if (towers[i].lane == y0) {
            if (towers[i].x == x0) {
                if (towers[i].placed == true) {
                    return i;
                }
            }
        }
    }
    return -1;
}

string SpotNames[10] = { "Wood Field", "Mine Deposit", "Arrow Tower",
                         "Cannon Tower", "Slow Gun", "Axe Throwing Troll",
                         "Elf Archer", "Catapult", "Magic Wizard"
                       };

string SpotDesc[10] =  { "Collects wood.", "Collects gold.",
                         "A wall that shoots arrows",
                         "A wall that shoots cannon balls.",
                         "Slows or freezes attacking units.",
                         "A hearty unit that dose great damage.",
                         "Does good damage. Dies easy",
                         "Fires slow. Does a lots of damage.",
                         "A frail old man with a boom stick."
                       };

string SpotChars[10] = { "W", "M", "A", "C", "S", "T", "O", "K", "P"};
char SpotChar[10] =    { 'W', 'M', 'A', 'C', 'S', 'T', 'O', 'K', 'P'};

int SpotCostG[10] = { 200,100,20,100, 80,200,100, 80, 420};
int SpotCostW[10] = { 100,200,80, 40, 40, 80,200,100, 420};

int SpotDam[10] =   {  7, 3, 1, 3, 2, 5, 4, 7,10};
int SpotSpeed[10] = { 60,20, 8,25,15,10, 7,40,100};
int SpotHp[10] =    { 6,6,20,20,10,20, 6,15, 5};
char SpotBullets[10] = {'.', '.', '-', 'o', '>', '/', '=', 'O', '~'};

char Map[22][41] = {
    "########################################",
    "* [....................................=", //1
    "*-|-------------------------------------",
    "* [....................................=", //3
    "*-|-------------------------------------",
    "* [....................................=", //5
    "*-|-------------------------------------",
    "* [....................................=", //7
    "*-|-------------------------------------",
    "* [....................................=", //9
    "*-|-------------------------------------",
    "* [....................................=", //11
    "*-|-------------------------------------",
    "* [....................................=", //13
    "*-|-------------------------------------",
    "* [....................................=", //15
    "*-|-------------------------------------",
    "* [....................................=", //17
    "*-|-------------------------------------",
    "* [....................................=", //19
    "########################################"
};

char StartMap[22][41] = {
    "########################################",
    "* [........A...........................=", //1
    "*-|-------------------------------------",
    "* [........A...........................=", //3
    "*-|-------------------------------------",
    "* [........A...........................=", //5
    "*-|-------------------------------------",
    "* [........A...........................=", //7
    "*-|-------------------------------------",
    "* [........A...........................=", //9
    "*-|-------------------------------------",
    "* [........A...........................=", //11
    "*-|-------------------------------------",
    "* [........A...........................=", //13
    "*-|-------------------------------------",
    "* [........A...........................=", //15
    "*-|-------------------------------------",
    "* [........A...........................=", //17
    "*-|-------------------------------------",
    "* [........A...........................=", //19
    "########################################"
};

void initTower(int t);

void Reset_FireMap() {
    for (int y = 0; y < 21; y++) {
        for (int x = 0; x < 41; x++) {
            FireMap[y][x] = 0;
        }
    }
}

void DrawFireMap() {
    for (int y = 0; y < 21; y++) {
        for (int x = 0; x < 41; x++) {
            if (FireMap[y][x] > 0) {
                if (FireMap[y][x] == 9) {
                    mvaddch(y,x,'*'|COLOR_PAIR(3)|A_BOLD);
                    FireMap[y][x] = 0;
                }
                else if (FireMap[y][x] == 8) {
                    mvaddch(y,x,'%'|COLOR_PAIR(3)|A_BOLD);
                    FireMap[y][x] = 0;
                } else {
                    DrawExpostion(y,x,FireMap[y][x],1);
                    FireMap[y][x] -= 1;
                }
            }
        }
    }
}

void ReplaceNewTowers() {
    for (int y = 0; y < 22; y++) {
        for (int x = 0; x < 41; x++) {
            if (StartMap[y][x] == 'A') {
                for (int i = 0; i < 200; i++) {
                    if (towers[i].placed == false) {
                        towers_count = i;
                    }
                }
                towers[towers_count].PlaceTower(y,x,SpotChar[2]);
                initTower(2);
            }
        }
    }
}

void Reset_Monsters() {
    int montype = 0;
    for (int i = 0; i < game.mon_levelMax; i++) {
        montype = rand()%8;
        if (montype == 0) {
            monsters[i].c = 'm';
            monsters[i].damage = 1+(3-(game.wave/3));
            monsters[i].hp = rand()%5+game.basehp+(3-(game.wave/3));
            monsters[i].RandLane();
            monsters[i].alive = true;
        } else if (montype == 1) {
            monsters[i].c = 'n';
            monsters[i].damage = rand()%2+1+(3-(game.wave/3));
            monsters[i].hp = rand()%7+game.basehp+(3-(game.wave/3));
            monsters[i].RandLane();
            monsters[i].alive = true;
        } else if (montype == 2) {
            monsters[i].c = 'i';
            monsters[i].damage = rand()%3+1+(3-(game.wave/3));
            monsters[i].hp = rand()%9+game.basehp+(3-(game.wave/3));
            monsters[i].RandLane();
            monsters[i].alive = true;
        } else if (montype == 3) {
            monsters[i].c = 'l';
            monsters[i].damage = rand()%4+1+(3-(game.wave/3));
            monsters[i].hp = rand()%11+game.basehp+(3-(game.wave/3));
            monsters[i].RandLane();
            monsters[i].alive = true;
            monsters[i].move_speed = 0;
        } else if (montype == 4) {
            monsters[i].c = 'd';
            monsters[i].damage = rand()%5+1+(3-(game.wave/3));
            monsters[i].hp = rand()%13+game.basehp+(3-(game.wave/3));
            monsters[i].RandLane();
            monsters[i].alive = true;
        } else if (montype == 5) {
            monsters[i].c = 's';
            monsters[i].damage = rand()%6+1+(3-(game.wave/3));
            monsters[i].hp = rand()%15+game.basehp+(3-(game.wave/3));
            monsters[i].RandLane();
            monsters[i].alive = true;
        } else if (montype == 6) {
            monsters[i].c = 'e';
            monsters[i].damage = rand()%7+1+(3-(game.wave/3));
            monsters[i].hp = rand()%17+game.basehp+(3-(game.wave/3));
            monsters[i].RandLane();
            monsters[i].alive = true;
        } else if (montype == 7) {
            monsters[i].c = 'r';
            monsters[i].damage = rand()%8+1+(3-(game.wave/3));
            monsters[i].hp = rand()%19+game.basehp+(3-(game.wave/3));
            monsters[i].RandLane();
            monsters[i].alive = true;
        } else if (montype == 8) {
            monsters[i].c = 'q';
            monsters[i].damage = rand()%9+1+(3-(game.wave/3));
            monsters[i].hp = rand()%21+game.basehp+(3-(game.wave/3));
            monsters[i].RandLane();
            monsters[i].alive = true;
        }
        monsters[i].move_speed = rand()%8+(3-(game.wave/3));
        if (monsters[i].c == 's') {
            monsters[i].move_speed = rand()%10-(game.wave/3);
            monsters[i].damage = 1;
        }
        if (monsters[i].move_speed < 1)
            monsters[i].move_speed = 1;
        monsters[i].x = 39;
        monsters[i].move_count = 0;
        monsters[i].slow = false;
        Reset_FireMap();
    }
}

void Init() {
    initscr();
    nodelay(stdscr,1);
    keypad(stdscr,1);
    noecho();
    nonl();
    curs_set(0);
    srand(time(NULL));
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_BLUE,COLOR_BLACK);
    init_pair(4,COLOR_GREEN,COLOR_BLACK);
    init_pair(5,COLOR_YELLOW,COLOR_BLACK);
    init_pair(6,COLOR_CYAN,COLOR_BLACK);
    game.gold = 700;
    game.wood = 700;
    game.lives = 30;
    game.speed = 70;
    game.mon_levelMax = 40;
    game.mon_onScreenMax = 20;
    game.wave_counter = 300;
    game.wave = 1;
    game.basehp = 0;
    Reset_Monsters();
    Reset_FireMap();
    curs.x = 2;
    curs.y = 2;
    for (int i = 0; i < 200; i++) {
        towers[i].reset();
        towers[i].init(1,3);
    }
    ReplaceNewTowers();
}

int GetMonstersOnScreen() {
    int num = 0;
    for (int i = 0; i < game.mon_levelMax; i++) {
        if (monsters[i].alive == true) {
            if (monsters[i].onScreen == true) {
                num += 1;
            }
        }
    }
    return num;
}

int GetDeadMonsters() {
    int num = 0;
    for (int i = 0; i < game.mon_levelMax; i++) {
        if (monsters[i].alive == false) {
            if (monsters[i].onScreen == false) {
                num += 1;
            }
        }
    }
    return num;
}

void NewWave() {
    if (GetDeadMonsters() == game.mon_levelMax) {
        game.wave += 1;
        game.wave_counter = 300;
        startGame = false;
        game.mon_levelMax += 20;
        game.mon_onScreenMax += 5;
        game.basehp += 2;
        game.gold += 80;
        game.wood += 80;
        Reset_Monsters();
        for (int i = 0; i < 200; i++) {
            if (towers[i].placed == true) {
                if (towers[i].c == 'W') {
                    towers[i].hp -= 1;
                } else if (towers[i].c == 'M') {
                    towers[i].hp -= 1;
                }
            }
        }
    }
}

bool TakeMoneyAndWood(int goldcost,int woodcost) {
    bool error = false;
    if (game.gold >= goldcost) {
        if (game.wood >= woodcost) {
            game.gold -= goldcost;
            game.wood -= woodcost;
            return true;
        } else {
            error = true;
        }
    } else {
        error = true;
    }
    if (error == true) {
        errorMenu.x = 15;
        errorMenu.y = 7;
        errorMenu.h = 5;
        errorMenu.w = 25;
        errorMenu.Draw();
        errorMenu.DrawText(1,1,"Not Enough Resources!",-1);
        nodelay(stdscr,0);
        getch();
        nodelay(stdscr,1);
        return false;
    }
    return error;
}

void SetDeadMonsters() {
    for (int i = 0; i < game.mon_levelMax; i++) {
        if (monsters[i].alive == true) {
            if (monsters[i].onScreen == true) {
                if (monsters[i].hp <= 0) {
                    monsters[i].alive = false;
                    monsters[i].onScreen = false;
                    game.gold += 1;
                    game.wood += 1;
                }

            }

        }
    }
}

void Add_SpawnOnMap() {
    for (int i = 0; i < game.mon_levelMax; i++) {
        if (monsters[i].alive == true) {
            if (monsters[i].onScreen == false) {
                if (GetMonstersOnScreen() < game.mon_onScreenMax) {
                    monsters[i].onScreen = true;
                }
            }
        }
    }
}

void Draw_Monster() {
    for (int i = 0; i < game.mon_levelMax; i++) {
        if (monsters[i].alive == true) {
            if (monsters[i].onScreen == true) {
                if (monsters[i].slow == true) {
                    mvaddch(monsters[i].lane,monsters[i].x,monsters[i].c|COLOR_PAIR(6)|A_BOLD);
                } else {
                    mvaddch(monsters[i].lane,monsters[i].x,monsters[i].c|COLOR_PAIR(4)|A_BOLD);
                }

            }
        }
    }
}

void GameOver() {
    if (game.lives <= 0) {
        game.gold = 700;
        game.wood = 700;
        game.lives = 30;
        game.speed = 50;
        game.mon_levelMax = 40;
        game.mon_onScreenMax = 20;
        game.wave_counter = 300;
        game.wave = 1;
        game.basehp = 0;
        Reset_Monsters();
        Reset_FireMap();
        startGame = false;
        curs.x = 2;
        curs.y = 2;
        for (int i = 0; i < 200; i++) {
            towers[i].reset();
            towers[i].init(1,3);
        }
        ReplaceNewTowers();
        errorMenu.x = 15;
        errorMenu.y = 7;
        errorMenu.h = 5;
        errorMenu.w = 33;
        errorMenu.Draw();
        errorMenu.DrawText(1,1,"You Lost! You Lasted",game.wave);
        errorMenu.DrawText(1,24,"waves.",-1);
        nodelay(stdscr,0);
        getch();
        nodelay(stdscr,1);
    }
}

void Draw_Map() {
    for (int y = 0; y < 22; y++) {
        for (int x = 0; x < 41; x++) {
            if (Map[y][x] != 0) {
                switch(Map[y][x]) {
                case '|': {
                    mvaddch(y,x,Map[y][x]|COLOR_PAIR(4));
                }
                break;
                case '.': {
                    mvaddch(y,x,' '|COLOR_PAIR(2));
                }
                break;
                case '-': {
                    mvaddch(y,x,Map[y][x]|COLOR_PAIR(4));
                }
                break;
                case '*': {
                    mvaddch(y,x,Map[y][x]|COLOR_PAIR(4));
                }
                break;
                case '#': {
                    mvaddch(y,x,Map[y][x]|COLOR_PAIR(5));
                }
                break;
                default: {
                    mvaddch(y,x,Map[y][x]|COLOR_PAIR(1));
                }
                break;
                }
            }
        }
    }
    mvprintw(24,0,"    Arrow keys to pick a unit. < > Space to build");
}

void DrawSideMenu() {
    sideMenu.x = 41;
    sideMenu.y = 1;
    sideMenu.h = 8;
    sideMenu.w = 18;
    sideMenu.Draw();
    sideMenu.DrawText(0,0,"Wave :",game.wave);
    if (startGame == true) {
        sideMenu.DrawText(1,0,"Wave Started!",-1);
    } else {
        sideMenu.DrawText(1,0,"Next Wave :",game.wave_counter);
    }
    sideMenu.DrawText(2,0,"Lives :",game.lives);
    sideMenu.DrawText(3,0,"Gold :",game.gold);
    sideMenu.DrawText(4,0,"Wood :",game.wood);
    sideMenu.DrawText(5,0,"Speed:",game.speed);
}

void GiveMoneyBack() {
    game.gold += SpotCostG[unitsel];
    game.wood += SpotCostW[unitsel];
}

void DrawDeScript(int num) {
    mvprintw(21,40,SpotNames[num].c_str());
    mvprintw(22,40,SpotDesc[num].c_str());
    mvprintw(23,40,"Costs %i Gold, %i Wood",SpotCostG[num],SpotCostW[num]);
}

void Draw_Multi_Menu() {
    bottomMenu.Active_menus = 9;
    int ystart = 21;
    int xstart = -2;
    int he = 3;
    int we = 3;
    bottomMenu.Menus[0].x = 0;
    bottomMenu.Menus[0].y = ystart;
    bottomMenu.Menus[0].h = 3;
    bottomMenu.Menus[0].w = 3;
    for (int i = 0; i < 9; i++) {
        bottomMenu.Menus[i].y = ystart;
        bottomMenu.Menus[i].x = xstart + we + 1;
        bottomMenu.Menus[i].h = he;
        bottomMenu.Menus[i].w = we;
        bottomMenu.Menus[i].Draw();
        if (unitsel == i) {
            bottomMenu.Menus[i].DrawText(0,-1,"[" + SpotChars[i] + "]",-1);
            DrawDeScript(i);
        } else {
            bottomMenu.Menus[i].DrawText(0,0,SpotChars[i],-1);
        }
        xstart += we + 1;
    }
}

float rand_FloatRange(float a, float b) {
    return ((b-a)*((float)rand()/RAND_MAX))+a;
}

void Move_Monsters() {
    for (int i = 0; i < game.mon_levelMax; i++) {
        if (monsters[i].alive == true) {
            if (monsters[i].onScreen == true) {
                if (monsters[i].move_count >= monsters[i].move_speed) {
                    if (IsTowerAtPoin(monsters[i].lane,monsters[i].x-1) == true) {
                        DoDamageAtPoin(monsters[i].lane,monsters[i].x-1,monsters[i].damage);
                        //if (towers[GetTowerAtPoin(monsters[i].lane,monsters[i].x)].hp <= 0) { towers[GetTowerAtPoin(monsters[i].lane,monsters[i].x)].placed = false; }
                        monsters[i].move_count = 0;
                    } else {
                        monsters[i].move_count = 0;

                        monsters[i].x-=1;
                    }
                } else {
                    if (monsters[i].slow == true) {
                        monsters[i].move_count += rand_FloatRange(0.1,0.1);
                    } else {
                        monsters[i].move_count += rand_FloatRange(0.1,1.0);
                    }
                }
            }
        }
    }
}

void TakeLives() {
    for (int i = 0; i < game.mon_levelMax; i++) {
        if (monsters[i].alive == true) {
            if (monsters[i].onScreen == true) {
                if(monsters[i].x < 0) {
                    monsters[i].onScreen = false;
                    monsters[i].x = 39;
                    monsters[i].alive = false;
                    game.lives -= 1;
                }
            }
        }
    }
}

void Draw_Towers() {
    for (int i = 0; i < 200; i++) {
        if (towers[i].hp <= 0) {
            towers[i].placed = false;
        }
        if (towers[i].placed == true) {
            towers[i].DrawTower();
            towers[i].DrawBullets();
        }
    }
}

void Shoot_Towers() {
    for (int i = 0; i < 200; i++) {
        if (towers[i].hp <= 0) {
            towers[i].placed = false;
        }
        if (towers[i].placed == true) {
            towers[i].Shoot();
            towers[i].GiveResource();
            towers[i].Move_Bullets();
        }
    }
}

void initTower(int t) {
    towers[towers_count].damage = SpotDam[t];
    towers[towers_count].c = SpotChar[t];
    towers[towers_count].shoot_speed = SpotSpeed[t];
    for (int i = 0; i < 200; i++) {
        towers[towers_count].bc = SpotBullets[t];
        towers[towers_count].bullets[i].lane = towers[towers_count].lane;
    }
    towers[towers_count].shoot_count = 0;
    towers[towers_count].name = SpotNames[t];
    towers[towers_count].hp = SpotHp[t];
}

void Draw_Cur() {
    bool sellmode = true;
    while (sellmode == true) {
        int key0 = getch();
        if (key0 == KEY_LEFT) {

            if (curs.x != 0) {
                curs.x -= 1;
            }
        }
        if (key0 == KEY_RIGHT) {
            if (curs.x != 39) {
                curs.x += 1;
            }
        }
        if (key0 == KEY_DOWN) {
            if (curs.y != 20) {
                curs.y += 1;
            }
        }
        if (key0 == KEY_UP) {
            if (curs.y != 0) {
                curs.y -= 1;
            }
        }
        if (key0 == 27) {
            sellmode = false;
            GiveMoneyBack();
        }
        if (key0 == 'q') {
            sellmode = false;
            GiveMoneyBack();
        }
        if (key0 == ' ') {
            if (Map[curs.y][curs.x] == '.') {
                bool can_build = true;
                for (int s = 0; s < 200; s++) {
                    if (towers[s].lane == curs.y) {
                        if (towers[s].x == curs.x) {
                            if (towers[s].placed == true) {
                                can_build = false;
                            }
                        }
                    }
                }
                if (can_build == true) {
                    for (int i = 0; i < 200; i++) {
                        if (towers[i].placed == false) {
                            towers_count = i;
                        }
                    }
                    towers[towers_count].PlaceTower(curs.y,curs.x,SpotChar[unitsel]);
                    initTower(unitsel);
                    sellmode = false;
                }
            }
        }
        flushinp();
        clear();
        Draw_Map();
        DrawSideMenu();
        Draw_Multi_Menu();
        Draw_Monster();
        Draw_Towers();
        mvaddch(curs.y,curs.x,'X'|COLOR_PAIR(3)|A_BOLD);
        refresh();
        napms(game.speed);
    }
}

int main() {
    Init();
    while (end == false) {
        int key = getch();
        if (key == 27) {
            end = true;
        } else if (key == 'q') {
            end = true;
        } else if (key == KEY_LEFT) {
            unitsel -= 1;
            if (unitsel < 0) {
                unitsel = 0;
            }
        } else if (key == KEY_RIGHT) {
            unitsel += 1;
            if (unitsel > 8) {
                unitsel = 8;
            }
        } else if (key == ' ') {
            if (TakeMoneyAndWood(SpotCostG[unitsel],SpotCostW[unitsel]) == true) {
                Draw_Cur();
            }
        } else if (key == '+')
        {
            game.speed -= 5;
            if (game.speed < 0)
            {
                game.speed = 0;
            }
        } else if (key == '-')
        {
            game.speed += 5;
            if (game.speed > 5000)
            {
                game.speed = 5000;
            }
        }
        GameOver();
        clear();
        Draw_Map();
        DrawSideMenu();
        Draw_Multi_Menu();
        Draw_Monster();
        Draw_Towers();
        DrawFireMap();
        NewWave();
        refresh();
        napms(game.speed);
        if (game.wave_counter >= 0) {
            game.wave_counter -= 1;
        } else {
            startGame = true;
            SetDeadMonsters();
            Add_SpawnOnMap();
            Move_Monsters();
            TakeLives();
        }
        Shoot_Towers();
    }
    endwin();
    return 0;
}
