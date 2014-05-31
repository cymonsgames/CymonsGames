/////////////////////////////// GRAVWHIP.CPP ///////////////////////////////
//                                                                        //
//            Gravity Whip game by Joseph Larson, 1996                    //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// I N C L U D E S ////////////

#include "gravwhip.h"

// G L O B A L S //////////////

alph small_text;
ships ship[6];
sprite explosion;
planets planet[MAX_PLANETS];
shots shot[MAX_SHOTS];
sprite far *planet_pict;
int num_planet_pics, clocker;
sprite checks, tiks, big_icon, little_icon, slider, num_arrows, weapon,
  input_icon, logo, opening;
pcx_picture help, tally_behind;

char rot_cel_table[72]={0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,8,8,7,7,6,6,5,
                        5,4,4,3,3,2,2,1,1,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,
                        8,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0};
char rot_flip_table[4]={1,0,2,3};
char slide_cel_table[11]={0,1,2,3,4,5,4,3,2,1,0};

int available_table[6][14]={{10,10, 0,10, 0,10,10, 0, 5, 0, 0, 1},
                            {10,10, 0, 0,10, 0, 0,10, 0, 5, 5, 1},
                            {10, 0,10,10, 0,10, 0,10, 5, 0, 0, 1},
                            {10, 0, 0,10,10, 0, 0,10, 0, 5, 5, 0},
                            { 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0},
                            {10, 0,10,10,10, 0,10,10, 0, 0, 0, 1}};
char *name_table[13]={"Torpedo","Phasic","Bouncer","Fire Cracker",
                      "Fire Spray","Tripple Shot","Spawner","X Weapon",
                      "Sabotage","Planet Cracker","Super Nova",
                      "Clamp Overide"};
int price_table[15][10]={{ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}, // Torp
                         { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, // Phas
                         { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, // Bounc
                         { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}, // FireC
                         { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}, // Spray
                         { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}, // Tri-S
                         { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40}, // Spawn
                         { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}, // X
                         { 50, 50, 50, 50, 50,  0,  0,  0,  0,  0}, // Sabot
                         { 70, 70, 70, 70, 70,  0,  0,  0,  0,  0}, // Plnet
                         { 90, 90, 90, 90, 90,  0,  0,  0,  0,  0}, // Nova
                         { 50,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // Clamp
                         { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, // Life
                         { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, // Nergy
                         {  0,  0,  0,  0,  0, 20, 20, 20, 20, 20}};// Power

float *planet_density_table;
char *planet_type_table;

RGB_tripple game_pal[256];

int last_shot, round, num_ships, next_loser, win_order[6];
char planet_type[MAX_PLANETS], next_planet;

char most_shots, most_losses, intrest, play_mode;

// F U N C T I O N S ////////////
void main() {
  Init();
  Intro();
  Menu();
  Close();
}

/////////////////////////////////
void BW_Fade(void) {
RGB_tripple BW_pal[256];
int s;

  for (s=0;s<256;s++)
    BW_pal[s].blue=(BW_pal[s].green=(BW_pal[s].red=(game_pal[s].red+game_pal[s].green+game_pal[s].blue)/3));
  Fade_To(BW_pal,1);
}
/////////////////////////////
void Init() {

FILE *datafile;
pcx_picture loading;
int s;

  Set_Mode(MCGA);
  Fill_Screen(0);

  PCX_Load("loading.pcx",&loading);
  PCX_Show(&loading);
  Free_PCX(&loading);

  datafile=fopen("planets.dat","rb");

  num_planet_pics=getc(datafile);
  planet_pict=new sprite [num_planet_pics];
  for (s=0;s<num_planet_pics;s++) {
    Load_Sprite(datafile,&planet_pict[s]);
  }
  planet_type_table=new char [num_planet_pics];
  fread(planet_type_table,sizeof(char),num_planet_pics,datafile);
  planet_density_table=new float [num_planet_pics];
  fread(planet_density_table,sizeof(float),num_planet_pics,datafile);
  for (s=0;s<144;s++)
    game_pal[s]=Load_Color(datafile);

  fclose(datafile);

  datafile=fopen("gravwhip.dat","rb");

  Load_Alph(datafile,&small_text);
  for (s=0;s<6;s++)
    Load_Sprite(datafile,&ship[s].image);
  Load_Sprite(datafile,&explosion);
  Load_Sprite(datafile,&checks);
  Load_Sprite(datafile,&tiks);
  Load_Sprite(datafile,&logo);
  Load_Sprite(datafile,&big_icon);
  Load_Sprite(datafile,&little_icon);
  Load_Sprite(datafile,&slider);
  Load_Sprite(datafile,&num_arrows);
  Load_Sprite(datafile,&weapon);
  Load_Sprite(datafile,&input_icon);
  Load_Sprite(datafile,&opening);
  for (s=144;s<256;s++)
    game_pal[s]=Load_Color(datafile);

  fclose(datafile);

  // load configuration file.
  datafile=fopen("gravwhip.cfg","rb");
  if (datafile==NULL) {
    most_losses=3;  // 1-3
    most_shots=100; // 10-25-50-100
    intrest=13; // 13-17-20-25  (money*intrest)/10
    play_mode=1;    // 1 or 2
  } else {
    most_losses=getc(datafile);
    most_shots=getc(datafile);
    intrest=getc(datafile);
    play_mode=getc(datafile);
    fclose(datafile);
  }
  PCX_Load("help.pcx",&help);
  PCX_Load("tallydat.pcx",&tally_behind);

  srand(time(NULL));                                // Set the random seed.
}

////////////////////////////////////
void Intro() {
pcx_picture screen;
RGB_tripple black_pal[256];
int c;

  for (c=0;c<256;c++) {
    black_pal[c].red=0;
    black_pal[c].blue=0;
    black_pal[c].green=0;
  }
  PCX_Load("opening.pcx",&screen);
  Wait_Retrace();
  PCX_Show(&screen);
  for (c=0;c<256;c++)
    Set_Pal_Reg(c,&black_pal[c]);
  Wait_Retrace();
  Fade_To(screen.pal,1);
  Free_PCX(&screen);
  while (Get_Scan_Code()) {};                      // Clear keyboard buffer.
  c=0;
  while (c<300) {
    Wait_Retrace();
    c++;
    if (!(c%5)) Pal_Rot(192,254);
    if (Get_Scan_Code()) return;
  }
  opening.x=132;
  opening.y=149;
  opening.cel=0;
  Draw_Sprite(&opening);
  //SOUND : Power up sort of thing.
  while (c<350) {
    Wait_Retrace();
    c++;
    if (!(c%5)) Pal_Rot(192,254);
    if (Get_Scan_Code()) return;
  }
  opening.cel=1;
  Draw_Sprite(&opening);
  //SOUND : Power up sort of thing. Maybe Higher this time
//  while (!(Get_Scan_Code())&&(c<400)) {
  while (c<400) {
    Wait_Retrace();
    c++;
    if (!(c%5)) Pal_Rot(192,254);
    if (Get_Scan_Code()) return;
  }
  opening.cel=2;
  Draw_Sprite(&opening);
  //SOUND : Power up sort of thing.
  while (c<450) {
    Wait_Retrace();
    c++;
    if (!(c%5)) Pal_Rot(192,254);
    if (Get_Scan_Code()) return;
  }
  opening.cel=3;
  Draw_Sprite(&opening);
  //SOUND : Final Powerup. (perhaps hum if you can do it.)
  while (!(Get_Scan_Code())) {
    Wait_Retrace();
    c++;
    if (!(c%5)) Pal_Rot(192,254);
  }
}

////////////////////////////////////
void Menu() {
int c, z;
unsigned char key;

  for (c=0;c<6;c++)
    ship[c].input=1;
  Draw_Menu();
  key=Get_Scan_Code();
  while ((key!=0x44)&&(key!=0x10)&&(key!=0x01)) {
    if ((key>=0x02)&&(key<=0x07)) {
      c=key-2;
      switch (ship[c].input) {
        case 1 : ship[c].input=2;
                 slider.x=151;
                 slider.y=45+c*19;
                 Slide_Right(0,6);
                 break;
        case 2 : ship[c].input=0;
                 slider.x=151;
                 slider.y=45+c*19;
                 Slide_Right(6,11);
                 break;
        case 0 : ship[c].input=1;
                 slider.x=151;
                 slider.y=45+c*19;
                 Slide_Left(11,0);
                 break;
        }
    } else
      switch (key) {
        case 0x18  : // "O"
                     Options();
                     break;
        case 0x39  : // space or
        case 0x1C  : // enter.
                     z=0;
                     for (c=0;c<MAX_SHIPS;c++)
                       if (ship[c].input!=0) {
                         z++;
                         ship[c].losses=most_losses;
                         ship[c].life_level=1;
                         ship[c].energy_level=1;
                       } else {
                         ship[c].losses=0;
                         ship[c].life_level=1;
                         ship[c].life=0;
                       }
                     if (z>1) {
                       // SOUND : Some sort of "ching" or something.
                       Play_Game();
                       while ((key=Get_Scan_Code())!=0) {};
                       Draw_Menu();
                     }
                     break;
      }
    key=Get_Scan_Code();
  }
}

void Draw_Menu() {
int c;

  for (c=248;c<256;c++)
    Set_Pal_Reg(c,&game_pal[c]);
                                                               // Draw Menu.
  // SOUND : A kind of "Ker-chunk" for the menu poping up.
  Wait_Retrace();
  Draw_Box(123,0,76,HEIGHT,253);
  Draw_Border(123,0,76,HEIGHT-1,1);
  logo.x=133;
  logo.y=3;
  logo.cel=0;
  Draw_Sprite(&logo);
  Draw_Box(136,34,49,8,255);
  Draw_Border(136,34,49,8,1);
  Alph_Write("Player Input",138,36,&small_text);
  Draw_Box(128,163,65,8,255);
  Draw_Border(128,163,65,8,1);
  Alph_Write("\"O\" for Options",130,165,&small_text);
  Draw_Box(128,178,65,8,255);
  Draw_Border(128,178,65,8,1);
  Alph_Write("SPACE to begin",132,180,&small_text);
  Draw_Box(128,188,65,8,255);
  Draw_Border(128,188,65,8,1);
  Alph_Write("F10 to quit",139,190,&small_text);
  for (c=0;c<6;c++) {
    num_arrows.cel=c+1;
    num_arrows.x=127;
    num_arrows.y=45+c*19;
    Draw_Sprite(&num_arrows);
    little_icon.cel=c;
    little_icon.x=138;
    little_icon.y=47+c*19;
    Draw_Sprite(&little_icon);
    Draw_Border(137,46+c*19,11,11,0);
    Draw_Box(150,44+c*19,43,15,255);
    Draw_Border(150,44+c*19,43,15,0);
    slider.x=151;
    slider.y=45+c*19;
    slider.cel=(ship[c].input==2)?5:0;
    slider.flip=(ship[c].input)?0:1;
    Draw_Sprite(&slider);
    input_icon.x=151;
    input_icon.y=52+c*19;
    Draw_Sprite(&input_icon);
  }
}

void Options() {
unsigned char far *background;
unsigned char key;
int c,z;
FILE *datafile;

  Save_Block(background,39,50,238,101);
  // SOUND : Menu pop up sound.
  Draw_Box(39,50,237,100,253);
  Draw_Border(39,50,237,100,1);
  logo.x=48;
  logo.y=80;
  Draw_Sprite(&logo);
  Draw_Box(63,113,31,9,255);
  Draw_Border(63,113,31,9,1);
  Alph_Write("OPTIONS",65,115,&small_text);
  Draw_Box(41,138,60,9,255);
  Draw_Border(41,138,60,8,1);
  Alph_Write("ESC to return",43,140,&small_text);
  Draw_Box(135,138,52,9,255);
  Draw_Border(135,138,52,8,1);
  Alph_Write("\"S\" to save",137,140,&small_text);
  Draw_Box(225,138,49,9,255);
  Draw_Border(225,138,49,8,1);
  Alph_Write("F10 to quit",227,140,&small_text);
  for (c=0;c<4;c++) {
    num_arrows.cel=c+1;
    num_arrows.x=126;
    num_arrows.y=60+c*19;
    Draw_Sprite(&num_arrows);
    Draw_Box(137,62+c*19,65,9,255);
    Draw_Border(137,62+c*19,65,9,0);
    Draw_Box(204,58+c*19,66,18,255);
    Draw_Border(204,58+c*19,65,17,0);
  }
  slider.x=218;
  slider.y=60;
  slider.cel=0;
  slider.flip=(play_mode==1)?0:1;
  Draw_Sprite(&slider);
  Alph_Write("Play Mode",142,64,&small_text);
  Alph_Write("Turns",206,68,&small_text);
  Alph_Write("Melee",248,68,&small_text);
  slider.y=79;
  slider.cel=((most_losses==1)||(most_losses==3))?0:5;
  slider.flip=(most_losses<=2)?0:1;
  Draw_Sprite(&slider);
  Alph_Write("Max Losses",142,83,&small_text);
  Alph_Write("1",218,87,&small_text);
  Alph_Write("2",236,87,&small_text);
  Alph_Write("3",255,87,&small_text);
  slider.y=98;
  slider.cel=((most_shots==10)||(most_shots==100))?0:4;
  slider.flip=(most_shots<50)?0:1;
  Draw_Sprite(&slider);
  Alph_Write("Shots Onscreen",139,102,&small_text);
  Alph_Write("Most",250,106,&small_text);
  Alph_Write("Fewest",206,106,&small_text);
  slider.y=117;
  slider.cel=((intrest==13)||(intrest==25))?0:4;
  slider.flip=(intrest<=17)?0:1;
  Draw_Sprite(&slider);
  Alph_Write("Intrest",139,121,&small_text);
  Alph_Write("Generous",236,125,&small_text);
  Alph_Write("Weak",206,125,&small_text);

  while ((key!=0x01)&&(key!=0x1C)) {
      switch (key) {
        case 0x02  : // 1
                     slider.x=218;
                     slider.y=60;
                     if (play_mode==1) {
                       play_mode=2;
                       Slide_Right(0,11);
                     } else {
                       play_mode=1;
                       Slide_Left(11,0);
                     }
                     break;
        case 0x03  : // 2
                     slider.x=218;
                     slider.y=79;
                     switch (most_losses) {
                       case 1 : most_losses=2;
                                Slide_Right(0,6);
                                break;
                       case 2 : most_losses=3;
                                Slide_Right(6,11);
                                break;
                       case 3 : most_losses=1;
                                Slide_Left(11,0);
                                break;
                       }
                     break;
        case 0x04  : // 3
                     slider.x=218;
                     slider.y=98;
                     switch (most_shots) {
                       case 10: most_shots=25;
                                Slide_Right(0,4);
                                break;
                       case 25: most_shots=50;
                                Slide_Right(4,8);
                                break;
                       case 50: most_shots=100;
                                Slide_Right(8,11);
                                break;
                       case 100:most_shots=10;
                                Slide_Left(11,0);
                                break;
                       }
                     break;
        case 0x05  : // intrest
                     slider.x=218;
                     slider.y=117;
                     switch (intrest) {
                       case 13 : intrest=17;
                                Slide_Right(0,4);
                                break;
                       case 17 : intrest=20;
                                Slide_Right(4,8);
                                break;
                       case 20 : intrest=25;
                                Slide_Right(8,11);
                                break;
                       case 25 : intrest=13;
                                Slide_Left(11,0);
                                break;
                       }
                     break;
        case 0x1F  : // "S"
                     datafile=fopen("gravwhip.cfg","wb");
                     putc(most_losses,datafile);
                     putc(most_shots,datafile);
                     putc(intrest,datafile);
                     putc(play_mode,datafile);
                     fclose(datafile);
                     // SOUND : A positive bleep.
                     break;
        case 0x44  : // F10
                     Close();
                     exit(0);
                     break;
        }
      key=Get_Scan_Code();
      }
  Restore_Block(background,39,50,238,101);
}
////////////////////////////////////
void Slide_Right(char from, char to) {
int z;

  while (from<to) {
    slider.cel=slide_cel_table[from];
    slider.flip=(from<6)?0:1;
    // SOUND : the sound of a slider moving. Very Short
    for (z=0;z<3;z++) Wait_Retrace();
    Draw_Box(slider.x,slider.y,slider.xsize,slider.ysize,255);
    Draw_Sprite(&slider);
    from++;
  }
}
////////////////////////////////////
void Slide_Left(char from, char to) {
int z;

  while (from>to) {
    from--;
    slider.cel=slide_cel_table[from];
    slider.flip=(from<6)?0:1;
    // SOUND : the sound of a slider moving. Very short
    for (z=0;z<3;z++) Wait_Retrace();
    Draw_Box(slider.x,slider.y,slider.xsize,slider.ysize,255);
    Draw_Sprite(&slider);
  }
}

////////////////////////////////////
void Play_Game() {
int c, player;

  next_loser=0;
  round=0;
  for (player=0;player<MAX_SHIPS;player++)
    if (ship[player].losses>0) {
      ship[player].inventory[1]=1;
      for (c=2;c<15;c++)
        ship[player].inventory[c]=0;
      ship[player].most_power=5;
      ship[player].money=200;
      ship[player].kills=0;
      ship[player].total_kills=0;
      win_order[next_loser++]=player;
    }
  num_ships=next_loser;
  do {
    Purchase();
    Setup_Board();
    Draw_Board();
    round++;
    player=0;
    if (play_mode==1) player=rand()%6;
    last_shot=0;
    do {
      if (ship[player].life>0) {
        Get_Shot(player);
        if (play_mode==1)
          Fire_Shots();
        else {
          // SOUND : Set shot sound. Kind of a reverbed tang.
        }
      }
      player++;
      if (player>=MAX_SHIPS) {
        player=0;
        if (play_mode==2)
          Fire_Shots();
      }
    } while (next_loser>1);
    for (c=0;c<MAX_SHIPS;c++)
      if (ship[c].life>0)
        win_order[0]=c;
    Tally();
    num_ships=0;
    for (c=0;c<MAX_SHIPS;c++)
      if (ship[c].losses>0)
        num_ships++;
    next_loser=num_ships;
  } while (num_ships>1);
//  BW_Fade();
}
////////////////////////////////////
void Purchase() {
int c, s, num_weapons, player, select, kills, available[8];
char key;
float temp;
pcx_picture background;
char name[15]="buypict .pcx";

  for (c=0;c<num_ships;c++) {
    player=win_order[c];
    if (ship[player].input==2)
      Com_Buy(player);
    else {
      num_weapons=0;
      for (s=0;s<13;s++)
        if (available_table[player][s])
          available[num_weapons++]=s;
      name[7]='1'+player;
      PCX_Load(name,&background);
      PCX_Show(&background);
      Free_PCX(&background);
      for (s=248;s<256;s++)
        Set_Pal_Reg(s,&game_pal[s]);
      // Weapons //
      num_arrows.cel=10;
      num_arrows.x=1;
      weapon.x=11;
      Draw_Box(23,3,18,9,255);
      Draw_Border(23,3,18,9,1);
      Alph_Write("cost",25,5,&small_text);
      Draw_Box(57,3,29,9,255);
      Draw_Border(57,3,29,9,1);
      Alph_Write("weapon",59,5,&small_text);
      Draw_Box(115,3,18,9,255);
      Draw_Border(115,3,18,9,1);
      Alph_Write("level",117,5,&small_text);
      for (s=0;s<num_weapons;s++) {
        num_arrows.y=(s*17)+15;
        Draw_Sprite(&num_arrows);
        Draw_Border(10,(s*17)+15,13,13,1);
        weapon.cel=available[s];
        weapon.y=(s*17)+16;
        Draw_Sprite(&weapon);
        Draw_Box(25,(s*17)+17,15,9,255);
        Draw_Border(25,(s*17)+17,15,9,0);
        if (ship[player].inventory[available[s]+1]<available_table[player][available[s]])
          Alph_Write(price_table[available[s]][ship[player].inventory[available[s]+1]],27,(s*17)+19,&small_text);
        else
          Alph_Write("X",27,(s*17)+19,&small_text);
        Draw_Box(42,(s*17)+17,60,9,255);
        Draw_Border(42,(s*17)+17,60,9,0);
        Alph_Write(name_table[available[s]],44,(s*17)+19,&small_text);
        temp=(float)ship[player].inventory[available[s]+1]/(float)available_table[player][available[s]]*30.0;
        Draw_Box(108+(int)temp,(s*17)+17,30-(int)temp,9,253);
        Draw_Border(108+(int)temp,(s*17)+17,30-(int)temp,9,0);
        Draw_Box(108,(s*17)+17,(int)temp,9,255);
        Draw_Border(108,(s*17)+17,(int)temp,9,1);
      }
      // Armor (life) //
      num_arrows.y=134;
      Draw_Sprite(&num_arrows);
      Draw_Box(25,136,15,9,255);
      Draw_Border(25,136,15,9,0);
      if (ship[player].life_level<10)
        Alph_Write(price_table[12][ship[player].life_level],27,138,&small_text);
      else
        Alph_Write("X",27,138,&small_text);
      Draw_Box(42,136,60,9,255);
      Draw_Border(42,136,60,9,0);
      Alph_Write("Armor",44,138,&small_text);
      temp=(float)ship[player].life_level/10.0*30.0;
      Draw_Box(108+(int)temp,136,30-(int)temp,9,253);
      Draw_Border(108+(int)temp,136,30-(int)temp,9,0);
      Draw_Box(108,136,(int)temp,9,255);
      Draw_Border(108,136,(int)temp,9,1);
      // Energy Level //
      num_arrows.y=151;
      Draw_Sprite(&num_arrows);
      Draw_Box(25,153,15,9,255);
      Draw_Border(25,153,15,9,0);
      if (ship[player].energy_level<10)
        Alph_Write(price_table[13][ship[player].energy_level],27,155,&small_text);
      else
        Alph_Write("X",27,155,&small_text);
      Draw_Box(42,153,60,9,255);
      Draw_Border(42,153,60,9,0);
      Alph_Write("Energy",44,155,&small_text);
      temp=(float)ship[player].energy_level/10.0*30.0;
      Draw_Box(108+(int)temp,153,30-(int)temp,9,253);
      Draw_Border(108+(int)temp,153,30-(int)temp,9,0);
      Draw_Box(108,153,(int)temp,9,255);
      Draw_Border(108,153,(int)temp,9,1);
      // Power //
      num_arrows.y=168;
      Draw_Sprite(&num_arrows);
      Draw_Box(25,171,15,9,255);
      Draw_Border(25,171,15,9,0);
      if (ship[player].most_power<10)
        Alph_Write(price_table[14][ship[player].most_power],27,173,&small_text);
      else
        Alph_Write("X",27,173,&small_text);
      Draw_Box(42,171,60,9,255);
      Draw_Border(42,171,60,9,0);
      Alph_Write("Power",44,173,&small_text);
      temp=(float)ship[player].most_power/10.0*30.0;
      Draw_Box(108+(int)temp,171,30-(int)temp,9,253);
      Draw_Border(108+(int)temp,171,30-(int)temp,9,0);
      Draw_Box(108,171,(int)temp,9,255);
      Draw_Border(108,171,(int)temp,9,1);

      Draw_Box(185,HEIGHT-33,85,9,255);
      Draw_Border(185,HEIGHT-33,85,9,1);
      Alph_Write("Press ENTER to buy",188,HEIGHT-31,&small_text);
      Draw_Box(170,HEIGHT-22,115,9,255);
      Draw_Border(170,HEIGHT-22,115,9,1);
      Alph_Write("Press SPACE when finished",175,HEIGHT-20,&small_text);

      Draw_Box(WIDTH-24,HEIGHT-12,22,9,255);
      Draw_Border(WIDTH-24,HEIGHT-12,22,9,1);
      Alph_Write("Kills",WIDTH-21,HEIGHT-10,&small_text);
      kills=ship[player].total_kills;
      tiks.cel=1;
      tiks.y=HEIGHT-10;
      s=33;
      while (kills>=10) {
        tiks.x=WIDTH-s;
        s+=8;
        Draw_Sprite(&tiks);
        kills-=10;
      }
      tiks.cel=0;
      while (kills>0) {
        tiks.x=WIDTH-s;
        s+=3;
        Draw_Sprite(&tiks);
        kills--;
      }

      Draw_Box(40,HEIGHT-15,25,9,255);
      Draw_Border(40,HEIGHT-15,25,9,1);
      Alph_Write("Money",42,HEIGHT-13,&small_text);
      Draw_Box(67,HEIGHT-15,40,9,255);
      Draw_Border(67,HEIGHT-15,40,9,0);
      Alph_Write(ship[player].money,69,HEIGHT-13,&small_text);


      key=Get_Scan_Code();
      select=0;
      num_arrows.cel=11;
      num_arrows.y=15;
      num_arrows.oldx=1;
      num_arrows.oldy=15;
      Save_Behind(&num_arrows);
      Draw_Sprite(&num_arrows);
      while (key!=0x39) {
        switch (key) {
          case 0x48 : // up key.
                      select--;
                      if (select<0) {
                        select=9;
                        for (num_arrows.y=15;num_arrows.y<=168;num_arrows.y+=3) {
                          Wait_Retrace();
                          Erase_Sprite(&num_arrows);
                          Save_Behind(&num_arrows);
                          Draw_Sprite(&num_arrows);
                        }
                        num_arrows.y=168;
                        Erase_Sprite(&num_arrows);
                        Save_Behind(&num_arrows);
                        Draw_Sprite(&num_arrows);
                      } else {
                        if ((select<7)&&(select>=num_weapons))
                          select=num_weapons-1;
                        for (;num_arrows.y>=(select*17)+15;num_arrows.y--) {
                          Wait_Retrace();
                          Erase_Sprite(&num_arrows);
                          Save_Behind(&num_arrows);
                          Draw_Sprite(&num_arrows);
                        }
                      }
                      break;
          case 0x50 : // down key.
                      select++;
                      if (select>9) {
                        select=0;
                        for (num_arrows.y=168;num_arrows.y>=15;num_arrows.y-=3) {
                          Wait_Retrace();
                          Erase_Sprite(&num_arrows);
                          Save_Behind(&num_arrows);
                          Draw_Sprite(&num_arrows);
                        }
                        num_arrows.y=15;
                        Erase_Sprite(&num_arrows);
                        Save_Behind(&num_arrows);
                        Draw_Sprite(&num_arrows);
                      } else {
                        if ((select>=num_weapons)&&(select<=7))
                          select=7;
                        for (;num_arrows.y<=(select*17)+15;num_arrows.y++) {
                          Wait_Retrace();
                          Erase_Sprite(&num_arrows);
                          Save_Behind(&num_arrows);
                          Draw_Sprite(&num_arrows);
                        }
                      }
                      break;
          case 0x4D : // Right arrow
          case 0x1C : // Enter
                      if (select<7)
                        if ((ship[player].money>=price_table[available[select]][ship[player].inventory[available[select]+1]])&&
                         (ship[player].inventory[available[select]+1]<available_table[player][available[select]])) {
                          // SOUND : A cash register Ka-Ching.
                          ship[player].money-=price_table[available[select]][ship[player].inventory[available[select]+1]];
                          ship[player].inventory[available[select]+1]++;
                          Draw_Box(25,(select*17)+17,15,9,255);
                          Draw_Border(25,(select*17)+17,15,9,0);
                          if (ship[player].inventory[available[select]+1]<available_table[player][available[select]])
                            Alph_Write(price_table[available[select]][ship[player].inventory[available[select]+1]],27,(select*17)+19,&small_text);
                          else
                            Alph_Write("X",27,(select*17)+19,&small_text);
                          temp=(float)ship[player].inventory[available[select]+1]/(float)available_table[player][available[select]]*30.0;
                          Draw_Box(108+(int)temp,(select*17)+17,30-(int)temp,9,253);
                          Draw_Border(108+(int)temp,(select*17)+17,30-(int)temp,9,0);
                          Draw_Box(108,(select*17)+17,(int)temp,9,255);
                          Draw_Border(108,(select*17)+17,(int)temp,9,1);
                          Draw_Box(40,HEIGHT-15,25,9,255);
                          Draw_Border(40,HEIGHT-15,25,9,1);
                          Alph_Write("Money",42,HEIGHT-13,&small_text);
                          Draw_Box(67,HEIGHT-15,40,9,255);
                          Draw_Border(67,HEIGHT-15,40,9,0);
                          Alph_Write(ship[player].money,69,HEIGHT-13,&small_text);
                        } else {
                          // Sound : A negitive kind of Honk.
                        }
                      else switch (select){
                        case 7 : // Life
                                 if ((ship[player].money>=price_table[12][ship[player].life_level])&&
                                  (ship[player].life_level<10)) {
                                   // SOUND : Ka-ching.
                                   ship[player].money-=price_table[12][ship[player].life_level];
                                   ship[player].life_level++;
                                   Draw_Box(25,136,15,9,255);
                                   Draw_Border(25,136,15,9,0);
                                   if (ship[player].life_level<10)
                                     Alph_Write(price_table[12][ship[player].life_level],27,138,&small_text);
                                   else
                                     Alph_Write("X",27,138,&small_text);
                                   temp=(float)ship[player].life_level/10.0*30.0;
                                   Draw_Box(108+(int)temp,136,30-(int)temp,9,253);
                                   Draw_Border(108+(int)temp,136,30-(int)temp,9,0);
                                   Draw_Box(108,136,(int)temp,9,255);
                                   Draw_Border(108,136,(int)temp,9,1);
                                   Draw_Box(40,HEIGHT-15,25,9,255);
                                   Draw_Border(40,HEIGHT-15,25,9,1);
                                   Alph_Write("Money",42,HEIGHT-13,&small_text);
                                   Draw_Box(67,HEIGHT-15,40,9,255);
                                   Draw_Border(67,HEIGHT-15,40,9,0);
                                   Alph_Write(ship[player].money,69,HEIGHT-13,&small_text);
                                 } else {
                                   // Sound : A negitive kind of Honk.
                                 }
                                 break;
                        case 8 : // Energy
                                 if ((ship[player].money>=price_table[13][ship[player].energy_level])&&
                                  (ship[player].energy_level<10)) {
                                   // SOUND : Ka-ching
                                   ship[player].money-=price_table[13][ship[player].energy_level];
                                   ship[player].energy_level++;
                                   Draw_Box(25,153,15,9,255);
                                   Draw_Border(25,153,15,9,0);
                                   if (ship[player].energy_level<10)
                                     Alph_Write(price_table[13][ship[player].energy_level],27,155,&small_text);
                                   else
                                     Alph_Write("X",27,155,&small_text);
                                   temp=(float)ship[player].energy_level/10.0*30.0;
                                   Draw_Box(108+(int)temp,153,30-(int)temp,9,253);
                                   Draw_Border(108+(int)temp,153,30-(int)temp,9,0);
                                   Draw_Box(108,153,(int)temp,9,255);
                                   Draw_Border(108,153,(int)temp,9,1);
                                   Draw_Box(40,HEIGHT-15,25,9,255);
                                   Draw_Border(40,HEIGHT-15,25,9,1);
                                   Alph_Write("Money",42,HEIGHT-13,&small_text);
                                   Draw_Box(67,HEIGHT-15,40,9,255);
                                   Draw_Border(67,HEIGHT-15,40,9,0);
                                   Alph_Write(ship[player].money,69,HEIGHT-13,&small_text);
                                 } else {
                                   // Sound : A negitive kind of Honk.
                                 }
                                 break;
                        case 9 : // Power
                                 if ((ship[player].money>=price_table[14][ship[player].most_power])&&
                                  (ship[player].most_power<10)) {
                                   // SOUND : Ka-ching.
                                   ship[player].money-=price_table[14][ship[player].most_power];
                                   ship[player].most_power++;
                                   Draw_Box(25,171,15,9,255);
                                   Draw_Border(25,171,15,9,0);
                                   if (ship[player].most_power<10)
                                     Alph_Write(price_table[14][ship[player].most_power],27,173,&small_text);
                                   else
                                     Alph_Write("X",27,173,&small_text);
                                   temp=(float)ship[player].most_power/10.0*30.0;
                                   Draw_Box(108+(int)temp,171,30-(int)temp,9,253);
                                   Draw_Border(108+(int)temp,171,30-(int)temp,9,0);
                                   Draw_Box(108,171,(int)temp,9,255);
                                   Draw_Border(108,171,(int)temp,9,1);
                                   Draw_Box(40,HEIGHT-15,25,9,255);
                                   Draw_Border(40,HEIGHT-15,25,9,1);
                                   Alph_Write("Money",42,HEIGHT-13,&small_text);
                                   Draw_Box(67,HEIGHT-15,40,9,255);
                                   Draw_Border(67,HEIGHT-15,40,9,0);
                                   Alph_Write(ship[player].money,69,HEIGHT-13,&small_text);
                                 } else {
                                   // Sound : A negitive kind of Honk.
                                 }
                      }
                      break;
          case 0x44  : // F10
                       Close();
                       exit(0);
                       break;
        }
        key=Get_Scan_Code();
      }
    }
  }
}

void Com_Buy(int player) {
int s, num_weapons, select, available[8];

  num_weapons=0;
  for (s=0;s<13;s++)
    if (available_table[player][s])
      available[num_weapons++]=s;
  while (ship[player].money>price_table[0][ship[player].inventory[1]]) {
    select=rand()%8;
    if (select<7)
      if ((ship[player].money>=price_table[available[select]][ship[player].inventory[available[select]+1]])&&
       (ship[player].inventory[available[select]+1]<available_table[player][available[select]])) {
        ship[player].money-=price_table[available[select]][ship[player].inventory[available[select]+1]];
        ship[player].inventory[available[select]+1]++;
      } else {}
    else switch (select) {
      case 7 : // Life
               if ((ship[player].money>=price_table[12][ship[player].life_level])&&
                (ship[player].life_level<10)) {
                 ship[player].money-=price_table[12][ship[player].life_level];
                 ship[player].life_level++;
               }
               break;
      case 8 : // Energy
               if ((ship[player].money>=price_table[13][ship[player].energy_level])&&
                (ship[player].energy_level<10)) {
                 ship[player].money-=price_table[13][ship[player].energy_level];
                 ship[player].energy_level++;
               }
               break;
    }
  }
}
////////////////////////////////////
void Setup_Board () {
int register s;
int c;
double temp;
double step,alpha;

  for (s=0;s<MAX_PLANETS;s++)
    planet[c].life=0;

  next_planet=0;
  step=2*PI/num_ships;
  alpha=0;

  for (s=0;s<MAX_SHIPS;s++)                                  // place ships.
    if (ship[s].losses>0) {
      ship[s].life=ship[s].life_level*10;
      ship[s].energy=30+ship[s].energy_level*10;
      ship[s].angle=(PI+alpha)*180.0/PI;
      if (ship[s].angle>=360.0) ship[s].angle-=360.0;
      ship[s].power=0;
      ship[s].type=1;
      ship[s].counter=0;
      Rotate_Ship(&ship[s]);
      ship[s].cx=(rand()%50-25)+(110*cos(alpha))+150;
      ship[s].cy=(rand()%20-10)-(70*sin(alpha))+90;
      ship[s].image.x=ship[s].cx-10;
      ship[s].image.y=ship[s].cy-10;
      ship[s].image.oldx=ship[s].image.x;
      ship[s].image.oldy=ship[s].image.y;
      alpha+=step;
    }

  for (s=0;s<MAX_PLANETS;s++) {
    Assign_Planet(&planet[s]);

    for (c=0;c<MAX_SHIPS;c++)                // Check planet-ship collision.
      if (ship[c].losses>0) {
        temp=(planet[s].cx-ship[c].cx)*(planet[s].cx-ship[c].cx)
          +(planet[s].cy-ship[c].cy)*(planet[s].cy-ship[c].cy);
        if (sqrt(temp) < (double)(planet[s].r+14)) {
          planet[s].life=0;
          next_planet--;
          c=MAX_SHIPS;
        }
      }
    if (planet[s].life>0)
      for (c=0;c<s;c++)                    // Check planet-planet collision.
        if (planet[c].life>0) {
          temp=(planet[s].cx-planet[c].cx)*(planet[s].cx-planet[c].cx)
            +(planet[s].cy-planet[c].cy)*(planet[s].cy-planet[c].cy);
          if (sqrt(temp)<(double)(planet[s].r+planet[c].r+10)) {
            planet[s].life=0;
            next_planet--;
            c=s;
          }
        }
  }

  while (next_planet<3) {
    s=0;
    Assign_Planet(&planet[s]);

    for (c=0;c<MAX_SHIPS;c++)                // Check planet-ship collision.
      if (ship[c].losses>0) {
        temp=(planet[s].cx-ship[c].cx)*(planet[s].cx-ship[c].cx)
          +(planet[s].cy-ship[c].cy)*(planet[s].cy-ship[c].cy);
        if (sqrt(temp) < (double)(planet[s].r+14)) {
          planet[s].life=0;
          next_planet--;
          c=MAX_SHIPS;
        }
      }
    if (planet[s].life>0)
      for (c=0;c<s;c++)                    // Check planet-planet collision.
        if (planet[c].life>0) {
          temp=(planet[s].cx-planet[c].cx)*(planet[s].cx-planet[c].cx)
            +(planet[s].cy-planet[c].cy)*(planet[s].cy-planet[c].cy);
          if (sqrt(temp)<(double)(planet[s].r+planet[c].r+10)) {
            planet[s].life=0;
            next_planet--;
            c=s;
          }
        }
  }
}

void Assign_Planet (planets *urth) {
char unique, type, c;

  do {
    unique=1;
    type=rand()%num_planet_pics;
    for (c=0;c<next_planet;c++)
      if (planet_type[c]==type)
        unique=0;
  } while (unique==0);
  planet_type[next_planet++]=type;
                                          // Set up image info for planet.
  urth->image.xsize=planet_pict[type].xsize;
  urth->image.ysize=planet_pict[type].ysize;
  urth->image.num_cels=planet_pict[type].num_cels;
  urth->image.size=planet_pict[type].size;
  urth->image.image=planet_pict[type].image;
  urth->image.cel=0;
  urth->image.x=urth->image.oldx=
    GAME_X+rand()%(GAME_WIDTH-urth->image.xsize-1)+1;
  urth->image.y=urth->image.oldy=
    GAME_Y+rand()%(GAME_HEIGHT-urth->image.ysize-1)+1;
  urth->life=100;
  urth->type=planet_type_table[type];
  if (urth->type==1) urth->image.flip=rand()%4;
    else urth->image.flip=0;
  urth->d=planet_density_table[type];
  urth->r=urth->image.xsize/2+1;
  urth->cx=urth->image.x+urth->r;
  urth->cy=urth->image.y+urth->r;
  urth->GM=2*PI*urth->r*urth->r*urth->d*GC;
}

void Draw_Board() {
int s;

  Fill_Screen(0);
  for (s=0;s<256;s++)
    Set_Pal_Reg(s,&game_pal[s]);

  for (s=0;s<NUM_STARS;s++)                                    // Starfield.
    PLOT(rand()%WIDTH,rand()%HEIGHT,(rand()%STAR_DEPTH)+STAR_COLOR);

  Draw_Side();

  setmem(&screen[GAME_X+(GAME_Y<<6)+(GAME_Y<<8)],GAME_WIDTH,252);
  setmem(&screen[GAME_X+(GAME_Y2<<6)+(GAME_Y2<<8)],GAME_WIDTH,252);
  for (s=GAME_Y;s<GAME_Y2;s++)
    PLOT(GAME_X,s,252);
  for (s=GAME_Y;s<GAME_Y2;s++)
    PLOT(GAME_X2,s,252);

  for (s=0;s<MAX_PLANETS;s++)
    if (planet[s].life>0) {
      delete planet[s].image.behind;
      planet[s].image.behind=new unsigned char [planet[s].image.xsize*planet[s].image.ysize];
      Save_Behind(&planet[s].image);
      Draw_Sprite(&planet[s].image);
    }

  for (s=0;s<MAX_SHIPS;s++)
    if (ship[s].life>0) {
      Save_Behind(&ship[s].image);
      Draw_Sprite(&ship[s].image);
    }
}

void Draw_Side () {
float temp;

  Draw_Box(WIDTH-21,0,21,HEIGHT-13,253);
  big_icon.x=WIDTH-20;
  big_icon.y=5;
  big_icon.cel=0;
  Draw_Sprite(&big_icon);
  temp=(float)ship[0].life/(float)ship[0].life_level*1.7;
  Draw_Box(WIDTH-19,6,17-(int)temp,17,SHOT_COLOR);
  Draw_Losses(WIDTH-21,28,ship[0].losses);
  big_icon.x=WIDTH-20;
  big_icon.y=35;
  big_icon.cel=1;
  Draw_Sprite(&big_icon);
  temp=(float)ship[1].life/(float)ship[1].life_level*1.7;
  Draw_Box(WIDTH-19,36,17-(int)temp,17,SHOT_COLOR+1);
  Draw_Losses(WIDTH-21,58,ship[1].losses);
  big_icon.x=WIDTH-20;
  big_icon.y=65;
  big_icon.cel=2;
  Draw_Sprite(&big_icon);
  temp=(float)ship[2].life/(float)ship[2].life_level*1.7;
  Draw_Box(WIDTH-19,66,17-(int)temp,17,SHOT_COLOR+2);
  Draw_Losses(WIDTH-21,88,ship[2].losses);
  big_icon.x=WIDTH-20;
  big_icon.y=95;
  big_icon.cel=3;
  Draw_Sprite(&big_icon);
  temp=(float)ship[3].life/(float)ship[3].life_level*1.7;
  Draw_Box(WIDTH-19,96,17-(int)temp,17,SHOT_COLOR+3);
  Draw_Losses(WIDTH-21,117,ship[3].losses);
  big_icon.x=WIDTH-20;
  big_icon.y=125;
  big_icon.cel=4;
  Draw_Sprite(&big_icon);
  temp=(float)ship[4].life/(float)ship[4].life_level*1.7;
  Draw_Box(WIDTH-19,126,17-(int)temp,17,SHOT_COLOR+4);
  Draw_Losses(WIDTH-21,148,ship[4].losses);
  big_icon.x=WIDTH-20;
  big_icon.y=155;
  big_icon.cel=5;
  Draw_Sprite(&big_icon);
  temp=(float)ship[5].life/(float)ship[5].life_level*1.7;
  Draw_Box(WIDTH-19,156,17-(int)temp,17,SHOT_COLOR+5);
  Draw_Losses(WIDTH-21,178,ship[5].losses);
}

void Draw_Losses(int x, int y, char losses) {

  checks.x=x+14-3*(3-most_losses);
  checks.y=y;
  if (losses<1)
    checks.cel=2;
  else
    checks.cel=0;
  Draw_Sprite(&checks);
  if (most_losses>1) {
    checks.x-=7;
    if (losses<2)
      checks.cel=1;
    else
      checks.cel=0;
    Draw_Sprite(&checks);
  }
  if (most_losses>2) {
    checks.x-=7;
    if (losses<3)
      checks.cel=1;
    else
      checks.cel=0;
    Draw_Sprite(&checks);
  }
}
///////////////////////////////////////////

void Get_Shot(int player) {
unsigned char key;
float k, temp;
int c;

  Draw_Box(0,HEIGHT-14, WIDTH, 14, 253);              // Clear Dialogue Bar.
  Draw_Box(2,HEIGHT-13, 23, 12, 255);
  Draw_Border(1,HEIGHT-14, 24, 13, 1);
  Alph_Write("ROUND",3,HEIGHT-13,&small_text);
  Alph_Write(round,10,HEIGHT-7,&small_text);

  Draw_Box(28,HEIGHT-13, 24, 12, 255);
  Draw_Border(27,HEIGHT-14, 25, 13, 1);
  Alph_Write("Player",29,HEIGHT-13,&small_text);
  Alph_Write(player+1,37,HEIGHT-7,&small_text);

  Draw_Box(55,188,26,9,255);
  Draw_Border(55,188,26,9,0);
  Alph_Write("Energy",57,190,&small_text);
  if (ship[player].energy<1+ship[player].inventory[1]/2)
    ship[player].energy=1+ship[player].inventory[1]/2;
  temp=(float)ship[player].energy/(float)(30+ship[player].energy_level*10)*94.0;
  Draw_Box(83+(int)temp,188,(int)(94-temp),9,255);
  Draw_Border(83+(int)temp,188,(int)(94-temp),9,0);
  Draw_Box(83,188,temp,9,SHOT_COLOR+player);
  Draw_Border(83,188,temp,9,1);

  Draw_Border(179,HEIGHT-14,13,13,0);
  weapon.x=180;
  weapon.y=HEIGHT-13;
  if (!Enough_Energy(player))
    ship[player].type=1;
  weapon.cel=ship[player].type-1;
  Draw_Sprite(&weapon);

  Draw_Box(194,HEIGHT-13, 24, 12, 255);
  Draw_Border(194,HEIGHT-14, 23, 13, 0);
  Alph_Write("Angle:",197,HEIGHT-13,&small_text);
  Alph_Write((int)ship[player].angle,196,HEIGHT-7,&small_text);
  Alph_Write(".",211,HEIGHT-7,&small_text);
  Alph_Write((int)(ship[player].angle*10)-((int)ship[player].angle)*10,213,HEIGHT-7,&small_text);

  Draw_Box(220,HEIGHT-13, 27, 12, 255);
  Draw_Border(219,HEIGHT-14, 28, 13, 0);
  Alph_Write("Power:",222,HEIGHT-13,&small_text);
  Alph_Write((int)ship[player].power,227,HEIGHT-7,&small_text);
  Alph_Write(".",235,HEIGHT-7,&small_text);
  Alph_Write((int)(ship[player].power*10)-((int)ship[player].power)*10,237,HEIGHT-7,&small_text);

  Draw_Box(250,HEIGHT-13, 34, 12, 255);
  Draw_Border(249,HEIGHT-14, 35, 13, 0);
  Alph_Write("Counter:",252,HEIGHT-13,&small_text);
  Alph_Write(ship[player].counter,265,HEIGHT-7,&small_text);

  Draw_Box(288,188,29,9,255);
  Draw_Border(288,188,29,9,1);
  Alph_Write("F1 Help",290,190,&small_text);

  int clocker=0;
  k=ship[player].angle*DEG2RAD;
  shot[last_shot].backgrnd=0;
  shot[last_shot].color=AIMING_SHOT;
  shot[last_shot].oldx=shot[last_shot].x=ship[player].cx+12*cos(k);
  shot[last_shot].oldy=shot[last_shot].y=ship[player].cy-12*sin(k);
  PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);

  if (ship[player].input==1) {
    key=Get_Scan_Code();                                     // Human input.
    while (key!=0) key=Get_Scan_Code();
    while ((key!=0x1C)&&(key!=0x39)) {
      key=Get_Scan_Code();
         // Forgive the following use of Goto statements, but they reduced the
                            // amount of code generated with no loss in speed.
      switch (key) {
        case 0x48 : // up arrow
                    if (Get_Control_Keys(SHIFT_R))
                      ship[player].power+=0.1;
                    else if (Get_Control_Keys(SHIFT_L))
                      ship[player].power+=2.0;
                    else
                      ship[player].power+=1.0;
                    if (ship[player].power>ship[player].most_power) ship[player].power=ship[player].most_power;
                    clocker++;
                    goto Draw_Power;
        case 0x50 : // down arrow
                    if (Get_Control_Keys(SHIFT_R))
                      ship[player].power-=0.1;
                    else if (Get_Control_Keys(SHIFT_L))
                      ship[player].power-=2.0;
                    else
                      ship[player].power-=1.0;
                    if (ship[player].power<0.0) ship[player].power=0.0;
        Draw_Power:
                    clocker++;
                    Wait_Retrace();
                    Draw_Box(220,HEIGHT-13, 27, 12, 255);
                    Draw_Border(219,HEIGHT-14, 28, 13, 0);
                    Alph_Write("Power:",222,HEIGHT-13,&small_text);
                    Alph_Write((int)ship[player].power,227,HEIGHT-7,&small_text);
                    Alph_Write(".",235,HEIGHT-7,&small_text);
                    Alph_Write((int)(ship[player].power*10)-((int)ship[player].power)*10,237,HEIGHT-7,&small_text);
                    goto Blink;
        case 0x4D : // left arrow
                    if (Get_Control_Keys(SHIFT_R))
                      ship[player].angle-=0.1;
                    else if (Get_Control_Keys(SHIFT_L))
                      ship[player].angle-=10.0;
                    else
                      ship[player].angle-=1.0;
                    if (ship[player].angle<0.0) ship[player].angle+=360.0;
                    goto Draw_Angle;
        case 0x4B : // right arrow
                    if (Get_Control_Keys(SHIFT_R))
                      ship[player].angle+=0.1;
                    else if (Get_Control_Keys(SHIFT_L))
                      ship[player].angle+=10.0;
                    else
                      ship[player].angle+=1.0;
                    if (ship[player].angle>=360.0) ship[player].angle-=360.0;
        Draw_Angle:
                    // SOUND : Perhaps a hum for the ships turning.
                    Rotate_Ship(&ship[player]);
                    k=ship[player].angle*DEG2RAD;
                    shot[last_shot].x=ship[player].cx+12*cos(k);
                    shot[last_shot].y=ship[player].cy-12*sin(k);
                    clocker++;
                    Wait_Retrace();
                    Erase_Sprite(&ship[player].image);
                    Draw_Sprite(&ship[player].image);
                    Draw_Box(194,HEIGHT-13, 24, 12, 255);
                    Draw_Border(194,HEIGHT-14, 23, 13, 0);
                    Alph_Write("Angle:",197,HEIGHT-13,&small_text);
                    Alph_Write((int)ship[player].angle,196,HEIGHT-7,&small_text);
                    Alph_Write(".",211,HEIGHT-7,&small_text);
                    Alph_Write((int)(ship[player].angle*10)-((int)ship[player].angle)*10,213,HEIGHT-7,&small_text);
                    PLOT(shot[last_shot].oldx,shot[last_shot].oldy,shot[last_shot].backgrnd);
                    shot[last_shot].backgrnd=screen[(int)shot[last_shot].x+((int)shot[last_shot].y<<6)+((int)shot[last_shot].y<<8)];
                    PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
                    shot[last_shot].oldx=(int)shot[last_shot].x;
                    shot[last_shot].oldy=(int)shot[last_shot].y;
                    goto Blink;
        case 0x33 : // '<'
                    if (Get_Control_Keys(SHIFT_L))
                      ship[player].counter-=10;
                    else
                      ship[player].counter--;
                    if (ship[player].counter<0) ship[player].counter=0;
                    goto Draw_Counter;
        case 0x34 : // '>'
                    if (Get_Control_Keys(SHIFT_L))
                      ship[player].counter+=10;
                    else
                      ship[player].counter++;
                    if (ship[player].counter>99) ship[player].counter=99;
        Draw_Counter:
                    clocker++;
                    Wait_Retrace();
                    Draw_Box(250,HEIGHT-13, 34, 12, 255);
                    Draw_Border(249,HEIGHT-14, 35, 13, 0);
                    Alph_Write("Counter:",252,HEIGHT-13,&small_text);
                    Alph_Write(ship[player].counter,265,HEIGHT-7,&small_text);
                    goto Blink;
        case 0x11 : // 'W' :change weapon
                    do {
                      ship[player].type++;
                      if (ship[player].type==13)
                        ship[player].type=1;
                    } while ((!ship[player].inventory[ship[player].type])||(!Enough_Energy(player)));
                    // SOUND : A weapon select sound. S-s-sh-Ching.
                    Draw_Border(179,HEIGHT-14,13,13,0);
                    weapon.x=180;
                    weapon.y=HEIGHT-13;
                    weapon.cel=ship[player].type-1;
                    Draw_Sprite(&weapon);
                    break;
        case 0x3B : // f1- Help
                    PCX_Show(&help);
                    // SOUND : kind of a spacey whaaa sound. Or something.
                    key=Get_Scan_Code();
                    while (key==0) key=Get_Scan_Code();
                    while (key!=0) key=Get_Scan_Code();
        case 0x42 : // f8- Redraw
                    Draw_Board();
                    Draw_Box(0,HEIGHT-14, WIDTH, 14, 253);
                    Draw_Box(2,HEIGHT-13, 23, 12, 255);
                    Draw_Border(1,HEIGHT-14, 24, 13, 1);
                    Alph_Write("ROUND",3,HEIGHT-13,&small_text);
                    Alph_Write(round,10,HEIGHT-7,&small_text);

                    Draw_Box(28,HEIGHT-13, 24, 12, 255);
                    Draw_Border(27,HEIGHT-14, 25, 13, 1);
                    Alph_Write("Player",29,HEIGHT-13,&small_text);
                    Alph_Write(player+1,37,HEIGHT-7,&small_text);

                    Draw_Box(55,188,26,9,255);
                    Draw_Border(55,188,26,9,0);
                    Alph_Write("Energy",57,190,&small_text);
                    temp=(float)ship[player].energy/(float)(30+ship[player].energy_level*10)*93.0;
                    if ((int)(93-temp)) {
                      Draw_Box(84+(int)temp,188,(int)(93-temp),9,255);
                      Draw_Border(84+(int)temp,188,(int)(93-temp),9,0);
                    }
                    if ((int)(temp)) {
                      Draw_Box(83,188,temp,9,SHOT_COLOR+player);
                      Draw_Border(83,188,temp,9,1);
                    }

                    Draw_Border(179,HEIGHT-14,13,13,0);
                    weapon.x=180;
                    weapon.y=HEIGHT-13;
                    weapon.cel=ship[player].type-1;
                    Draw_Sprite(&weapon);

                    Draw_Box(194,HEIGHT-13, 24, 12, 255);
                    Draw_Border(194,HEIGHT-14, 23, 13, 0);
                    Alph_Write("Angle:",197,HEIGHT-13,&small_text);
                    Alph_Write((int)ship[player].angle,196,HEIGHT-7,&small_text);
                    Alph_Write(".",211,HEIGHT-7,&small_text);
                    Alph_Write((int)(ship[player].angle*10)-((int)ship[player].angle)*10,213,HEIGHT-7,&small_text);

                    Draw_Box(220,HEIGHT-13, 27, 12, 255);
                    Draw_Border(219,HEIGHT-14, 28, 13, 0);
                    Alph_Write("Power:",222,HEIGHT-13,&small_text);
                    Alph_Write((int)ship[player].power,227,HEIGHT-7,&small_text);
                    Alph_Write(".",235,HEIGHT-7,&small_text);
                    Alph_Write((int)(ship[player].power*10)-((int)ship[player].power)*10,237,HEIGHT-7,&small_text);

                    Draw_Box(250,HEIGHT-13, 34, 12, 255);
                    Draw_Border(249,HEIGHT-14, 35, 13, 0);
                    Alph_Write("Counter:",252,HEIGHT-13,&small_text);
                    Alph_Write(ship[player].counter,265,HEIGHT-7,&small_text);

                    Draw_Box(288,188,29,9,255);
                    Draw_Border(288,188,29,9,1);
                    Alph_Write("F1 Help",290,190,&small_text);
                    break;
        case 0x44 : // f10-quit
                    Close();
                    exit(1);
                    break;
        default   : clocker++;
                    Wait_Retrace();
             Blink:                                   // make the stars blink.
                    if (clocker%10==0) {
                      for (c=0;c<MAX_PLANETS;c++)
                        if ((planet[c].life>0)&&(planet[c].type==2)) {
                          planet[c].image.flip+=1+rand()%3;
                          if (planet[c].image.flip>4) planet[c].image.flip-=4;
                          Erase_Sprite(&planet[c].image);
                          Draw_Sprite(&planet[c].image);
                        }
                      Pal_Rot(STAR_COLOR,STAR_COLOR+STAR_DEPTH);
                    }
                    break;
      }
    }
  } else {                                                // Computer input.
    ship[player].power=rand()%50/10;
    ship[player].counter=rand()%50;
    ship[player].type=rand()%12+1;
    while ((!ship[player].inventory[ship[player].type])||(!Enough_Energy(player)))
      ship[player].type=rand()%12+1;
    Draw_Border(179,HEIGHT-14,13,13,0);
    weapon.x=180;
    weapon.y=HEIGHT-13;
    weapon.cel=ship[player].type-1;
    Draw_Sprite(&weapon);

    Draw_Box(194,HEIGHT-13, 24, 12, 255);
    Draw_Border(194,HEIGHT-14, 23, 13, 0);
    Alph_Write("Angle:",197,HEIGHT-13,&small_text);
    Alph_Write((int)ship[player].angle,196,HEIGHT-7,&small_text);
    Alph_Write(".",211,HEIGHT-7,&small_text);
    Alph_Write((int)(ship[player].angle*10)-((int)ship[player].angle)*10,213,HEIGHT-7,&small_text);

    Draw_Box(220,HEIGHT-13, 27, 12, 255);
    Draw_Border(219,HEIGHT-14, 28, 13, 0);
    Alph_Write("Power:",222,HEIGHT-13,&small_text);
    Alph_Write((int)ship[player].power,227,HEIGHT-7,&small_text);
    Alph_Write(".",235,HEIGHT-7,&small_text);
    Alph_Write((int)(ship[player].power*10)-((int)ship[player].power)*10,237,HEIGHT-7,&small_text);

    Draw_Box(250,HEIGHT-13, 34, 12, 255);
    Draw_Border(249,HEIGHT-14, 35, 13, 0);
    Alph_Write("Counter:",252,HEIGHT-13,&small_text);
    Alph_Write(ship[player].counter,265,HEIGHT-7,&small_text);
    while (rand()%100) {
      // SOUND : Turning Hum.
      ship[player].angle++;
      if (ship[player].angle>=360.0) ship[player].angle-=360.0;
      k=ship[player].angle*DEG2RAD;
      shot[last_shot].x=ship[player].cx+12*cos(k);
      shot[last_shot].y=ship[player].cy-12*sin(k);
      Rotate_Ship(&ship[player]);
      clocker++;
      Wait_Retrace();
      Erase_Sprite(&ship[player].image);
      Draw_Sprite(&ship[player].image);
      PLOT(shot[last_shot].oldx,shot[last_shot].oldy,shot[last_shot].backgrnd);
      shot[last_shot].backgrnd=screen[(int)shot[last_shot].x+((int)shot[last_shot].y<<6)+((int)shot[last_shot].y<<8)];
      PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
      shot[last_shot].oldx=(int)shot[last_shot].x;
      shot[last_shot].oldy=(int)shot[last_shot].y;
      Draw_Box(194,HEIGHT-13, 24, 12, 255);
      Draw_Border(194,HEIGHT-14, 23, 13, 0);
      Alph_Write("Angle:",197,HEIGHT-13,&small_text);
      Alph_Write((int)ship[player].angle,196,HEIGHT-7,&small_text);
      Alph_Write(".",211,HEIGHT-7,&small_text);
      Alph_Write((int)(ship[player].angle*10)-((int)ship[player].angle)*10,213,HEIGHT-7,&small_text);
      if (clocker%10==0) {
        for (c=0;c<MAX_PLANETS;c++)
          if ((planet[c].life>0)&&(planet[c].type==2)) {
            planet[c].image.flip+=1+rand()%3;
            if (planet[c].image.flip>4) planet[c].image.flip-=4;
            Erase_Sprite(&planet[c].image);
            Draw_Sprite(&planet[c].image);
          }
        Pal_Rot(STAR_COLOR,STAR_COLOR+STAR_DEPTH);
      }
    }
  }
  switch (ship[player].type) {
    case 1 : // Standard Torpedo
             ship[player].energy-=1+ship[player].inventory[1]/2;
             shot[last_shot].type=1;
             shot[last_shot].power=ship[player].inventory[1]*10;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 2 : // Phasic
             ship[player].energy-=ship[player].inventory[2];
             shot[last_shot].type=2;
             shot[last_shot].power=ship[player].inventory[2]*10;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 3 : // Bouncer
             ship[player].energy-=ship[player].inventory[3];
             shot[last_shot].type=3;
             shot[last_shot].power=ship[player].inventory[3]*10;
             shot[last_shot].counter=ship[player].inventory[3]*3;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 4 : // Firecracker
             ship[player].energy-=2*ship[player].inventory[4];
             shot[last_shot].type=4;
             shot[last_shot].power=ship[player].inventory[4]*7;
             shot[last_shot].counter=ship[player].counter;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 5 : // FireSpray
             ship[player].energy-=2*ship[player].inventory[5]+ship[player].inventory[5]/2;
             shot[last_shot].type=5;
             shot[last_shot].counter=3+2*ship[player].inventory[5];
             shot[last_shot].power=3;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 6 : // Tri-Shot
             ship[player].energy-=2*ship[player].inventory[6];
             shot[last_shot].type=1;
             shot[last_shot].power=ship[player].inventory[6]*10;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             shot[last_shot].type=1;
             shot[last_shot].power=ship[player].inventory[6]*5;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             k=(ship[player].angle+ship[player].counter)*DEG2RAD;
             shot[last_shot].x=shot[last_shot-1].x;
             shot[last_shot].y=shot[last_shot-1].y;
             shot[last_shot].backgrnd=shot[last_shot-1].backgrnd;
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             shot[last_shot].type=1;
             shot[last_shot].power=ship[player].inventory[6]*5;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             k=(ship[player].angle-ship[player].counter)*DEG2RAD;
             shot[last_shot].x=shot[last_shot-2].x;
             shot[last_shot].y=shot[last_shot-2].y;
             shot[last_shot].backgrnd=shot[last_shot-2].backgrnd;
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 7 : // Spawner
             ship[player].energy-=3*ship[player].inventory[7];
             shot[last_shot].type=7;
             shot[last_shot].power=ship[player].inventory[7]*20;
             shot[last_shot].counter=ship[player].counter;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 8 : // WeaponX
             ship[player].energy-=5*ship[player].inventory[8];
             shot[last_shot].type=8;
             shot[last_shot].power=10*ship[player].inventory[8];
             shot[last_shot].counter=ship[player].counter;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 9 : // Clamp Sabotage
             ship[player].energy-=4*ship[player].inventory[9];
             shot[last_shot].type=9;
             shot[last_shot].power=1;
             shot[last_shot].counter=ship[player].inventory[9]*10;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 10: // Planet Shatterer
             ship[player].energy-=7*ship[player].inventory[10];
             shot[last_shot].type=10;
             shot[last_shot].power=50+ship[player].inventory[10]*10;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 11: // Sun Nova
             ship[player].energy-=7*ship[player].inventory[11];
             shot[last_shot].type=11;
             shot[last_shot].power=50+ship[player].inventory[11]*10;
             shot[last_shot].owner=player;
             shot[last_shot].color=SHOT_COLOR+player;
             PLOT((int)shot[last_shot].x,(int)shot[last_shot].y,shot[last_shot].color);
             shot[last_shot].vx=ship[player].power*cos(k);
             shot[last_shot].vy=ship[player].power*(-sin(k));
             last_shot++;
             break;
    case 12: // GravClamp Release
             shot[last_shot].type=12;
             PLOT(shot[last_shot].oldx,shot[last_shot].oldy,shot[last_shot].backgrnd);
             shot[last_shot].oldx=shot[last_shot].x=ship[player].cx;
             shot[last_shot].oldy=shot[last_shot].y=ship[player].cy;
             shot[last_shot].backgrnd=screen[(int)shot[last_shot].x+((int)shot[last_shot].y<<6)+((int)shot[last_shot].y<<8)];
             shot[last_shot].color=shot[last_shot].backgrnd;
             shot[last_shot].vx=0;
             shot[last_shot].vy=0;
             shot[last_shot].counter=ship[player].counter;
             shot[last_shot].power=1;
             shot[last_shot].owner=player;
             last_shot++;
             break;
  }
}

char Enough_Energy(char player) {
char toreturn;

  switch(ship[player].type) {
    case 1 : // Torpedo
        toreturn=(ship[player].energy>=(1+ship[player].inventory[1]/2));
        break;
    case 2 : // Phasic
        toreturn=(ship[player].energy>=(ship[player].inventory[2]));
        break;
    case 3 : // Bouncer
        toreturn=(ship[player].energy>=(ship[player].inventory[3]));
        break;
    case 4 : // Firecracker
        toreturn=(ship[player].energy>=(2*ship[player].inventory[4]));
        break;
    case 5 : // FireStorm
        toreturn=(ship[player].energy>=(2*ship[player].inventory[5]+2*ship[player].inventory[5]/2));
        break;
    case 6 : // TriShot
        toreturn=(ship[player].energy>=(2*ship[player].inventory[6]));
        break;
    case 7 : // Spawner
        toreturn=(ship[player].energy>=(3*ship[player].inventory[7]));
        break;
    case 8 : // WeaponX
        toreturn=(ship[player].energy>=(5*ship[player].inventory[8]));
        break;
    case 9 : // Clamp Sabotage
        toreturn=(ship[player].energy>=(4*ship[player].inventory[9]));
        break;
    case 10: // Planet Shatterer
        toreturn=(ship[player].energy>=(7*ship[player].inventory[10]));
        break;
    case 11: // Nova Weapon
        toreturn=(ship[player].energy>=(7*ship[player].inventory[11]));
        break;
    case 12: // Clamp Release
        toreturn=1;
  }
  return toreturn;
}

void Fire_Shots() {
int c, num_shots, x, y;
unsigned char key;

  char speed=1;
  clocker=0;
  Draw_Box(0,HEIGHT-14, WIDTH, 14, 253);
  Draw_Box(64,HEIGHT-10,244,7,255);
  Draw_Border(63,HEIGHT-11,244,8,0);
  Alph_Write("Press \"Esc\" to kill all shots.  Press \"S\" to Speed up.",65,HEIGHT-9,&small_text);
  // SOUND : >BANG<
  do {
    clocker++;
    if (clocker>30000) clocker=3000;
    if (speed) {
      Wait_Retrace();
      if (clocker%10==0)                                       // Burn Suns.
        for (c=0;c<MAX_PLANETS;c++)
          if ((planet[c].life>0)&&(planet[c].type==2)) {
            planet[c].image.flip+=1+rand()%3;
            if (planet[c].image.flip>4) planet[c].image.flip-=4;
            Erase_Sprite(&planet[c].image);
            Draw_Sprite(&planet[c].image);
          }
      if (clocker<1000) {
        Draw_Box(5,HEIGHT-10,50,7,255);
        Draw_Border(4,HEIGHT-11,50,8,0);
        Alph_Write("TIME:",6,HEIGHT-9,&small_text);
        Alph_Write(clocker,27,HEIGHT-9,&small_text);
      } else
        Draw_Box(4,HEIGHT-11,51,9,253);
    }
    for (c=0;c<most_shots;c++)                                // Clear shot.
      if (shot[c].power)
        PLOT(shot[c].oldx,shot[c].oldy,shot[c].backgrnd);
    for (c=0;c<most_shots;c++)                           // Save background.
      if (shot[c].power) {
        x=(int)shot[c].x;
        y=(int)shot[c].y;
        if (x<GAME_X) x=GAME_X;
        if (x>GAME_X2) x=GAME_X2;
        if (y<GAME_Y) y=GAME_Y;
        if (y>GAME_Y2) y=GAME_Y2;
        shot[c].oldx=x;
        shot[c].oldy=y;
        shot[c].backgrnd=screen[x+(y<<6)+(y<<8)];
      }
    for (c=0;c<most_shots;c++)                                 // Draw shot.
      if (shot[c].power) {
        x=(int)shot[c].x;
        y=(int)shot[c].y;
        if (x<GAME_X) x=GAME_X;
        if (x>GAME_X2) x=GAME_X2;
        if (y<GAME_Y) y=GAME_Y;
        if (y>GAME_Y2) y=GAME_Y2;
        PLOT(x,y,shot[c].color);
      }
    if (speed) {
      if (clocker%10==0)                               // Rotate star color.
        Pal_Rot(STAR_COLOR,STAR_COLOR+STAR_DEPTH);
    }
    num_shots=0;
    for (c=0;c<most_shots;c++)                              // Update Shots.
      if (shot[c].power) {
        Update_Shot(&shot[c]);
        num_shots+=Check_Collision(&shot[c]);
      }
    key=Get_Scan_Code();
    switch (key) {
      case 0x1F : // S - toggle speed.
                  speed=!speed;
                  break;
      case 0x01 : // esc-kill shots.
                  num_shots=0;
                  break;
      case 0x42 : // f8- Redraw
                  Draw_Board();
                  Draw_Box(0,HEIGHT-14, WIDTH, 14, 253);
                  Draw_Box(64,HEIGHT-10,244,7,255);
                  Draw_Border(63,HEIGHT-11,244,8,0);
                  Alph_Write("Press \"Esc\" to kill all shots.  Press \"S\" to Speed up.",65,HEIGHT-9,&small_text);
                  break;
      case 0x44 : // f10-quit
                  Close();
                  exit(1);
    }

  } while ((num_shots>0)&&(next_loser>0));
  for (c=0;c<most_shots;c++)
    if (shot[c].power) {
      shot[c].power=0;
      PLOT(shot[c].oldx,shot[c].oldy,shot[c].backgrnd);
      if (shot[c].type==12) {
        ship[shot[c].owner].cx=shot[c].x;
        ship[shot[c].owner].cy=shot[c].y;
      }
    }
  last_shot=0;
}

void Update_Shot(shots *bullet) {
int c;
float k;

  switch (bullet->type) {
    case 1 : // Torpedo
    case 2 : // Phasic
    case 3 : // Multi-Staged
             Grav_Pull(bullet);
             break;
    case 4 : // FireCracker
             if ((--bullet->counter)<1) {
               // SOUND : Kind of a firecracker crackle. Very short.
               Create_Shot(bullet->x,bullet->y,bullet->owner,5*ship[bullet->owner].inventory[4],3+2*ship[bullet->owner].inventory[4]);
               bullet->power=0;
             } else Grav_Pull(bullet);
             break;
    case 5 : // FireSpray
             bullet->power--;
             if (bullet->power>0) {
               c=0;
               while (shot[c].power>0) c++;
               shot[c].type=1;
               shot[c].power=5*ship[bullet->owner].inventory[5];
               shot[c].owner=bullet->owner;
               shot[c].color=SHOT_COLOR+bullet->owner;
               shot[c].backgrnd=0;
               shot[c].oldx=shot[c].x=bullet->x;
               shot[c].oldy=shot[c].y=bullet->y;
               k=(ship[bullet->owner].angle+(rand()%210/10)-11)*DEG2RAD;
                                  // +/- a 5.0 degree difference.
               shot[c].vx=ship[bullet->owner].power*cos(k);
               shot[c].vy=ship[bullet->owner].power*(-sin(k));
               // SOUND : A quiet schwump or something.
               bullet->counter--;
               if (bullet->counter>0)
                 bullet->power=5;
               else {
                 PLOT(bullet->oldx,bullet->oldy,0);
                 bullet->power=0;
               }
             }
             break;
    case 7 : // Spawner
             Grav_Pull(bullet);
             if ((--bullet->counter)<1) {
               c=0;
               while (shot[c].power>0) c++;
               shot[c].type=1;
               shot[c].power=5*ship[bullet->owner].inventory[7];
               shot[c].owner=bullet->owner;
               shot[c].color=SHOT_COLOR+bullet->owner;
               shot[c].backgrnd=bullet->backgrnd;
               shot[c].oldx=bullet->oldx;
               shot[c].x=bullet->x;
               shot[c].oldy=bullet->oldy;
               shot[c].y=bullet->y;
               shot[c].vx=0;
               shot[c].vy=0;
               // Sound : a quiet soft pop.
               bullet->power-=5;
               if (bullet->power>0)
                 bullet->counter=5;
               else
                 PLOT(bullet->oldx,bullet->oldy,bullet->backgrnd);
             }
             break;
    case 8 : // WeaponX
             Grav_Pull(bullet);
             if ((--bullet->counter)<0) {
               bullet->type=1;
               bullet->power=7*ship[bullet->owner].inventory[8];
               int number=1+ship[bullet->owner].inventory[8];
               for (c=0;((c<most_shots)&&(number>0));c++)
                 if (!shot[c].power) {
                   // SOUND : >Bang<
                   shot[c].type=1;
                   shot[c].power=7*ship[bullet->owner].inventory[8];
                   shot[c].owner=bullet->owner;
                   shot[c].counter=0;
                   shot[c].x=bullet->x;
                   shot[c].oldx=bullet->oldy;
                   shot[c].y=bullet->y;
                   shot[c].oldy=bullet->oldy;
                   shot[c].color=SHOT_COLOR+bullet->owner;
                   shot[c].backgrnd=bullet->backgrnd;
                   shot[c].vx=bullet->vx+number;
                   shot[c].vy=bullet->vy+number;
                   c++;
                   while ((c<most_shots)&&(shot[c].power)) c++;
                   shot[c].type=1;
                   shot[c].power=7*ship[bullet->owner].inventory[8];
                   shot[c].owner=bullet->owner;
                   shot[c].counter=0;
                   shot[c].x=bullet->x;
                   shot[c].oldx=bullet->oldy;
                   shot[c].y=bullet->y;
                   shot[c].oldy=bullet->oldy;
                   shot[c].color=SHOT_COLOR+bullet->owner;
                   shot[c].backgrnd=bullet->backgrnd;
                   shot[c].vx=bullet->vx+number;
                   shot[c].vy=bullet->vy-number;
                   while ((c<most_shots)&&(shot[c].power)) c++;
                   shot[c].type=1;
                   shot[c].power=7*ship[bullet->owner].inventory[8];
                   shot[c].owner=bullet->owner;
                   shot[c].counter=0;
                   shot[c].x=bullet->x;
                   shot[c].oldx=bullet->oldy;
                   shot[c].y=bullet->y;
                   shot[c].oldy=bullet->oldy;
                   shot[c].color=SHOT_COLOR+bullet->owner;
                   shot[c].backgrnd=bullet->backgrnd;
                   shot[c].vx=bullet->vx-number;
                   shot[c].vy=bullet->vy+number;
                   while ((c<most_shots)&&(shot[c].power)) c++;
                   shot[c].type=1;
                   shot[c].power=7*ship[bullet->owner].inventory[8];
                   shot[c].owner=bullet->owner;
                   shot[c].counter=0;
                   shot[c].x=bullet->x;
                   shot[c].oldx=bullet->oldy;
                   shot[c].y=bullet->y;
                   shot[c].oldy=bullet->oldy;
                   shot[c].color=SHOT_COLOR+bullet->owner;
                   shot[c].backgrnd=bullet->backgrnd;
                   shot[c].vx=bullet->vx-number;
                   shot[c].vy=bullet->vy-number;
                   number--;
                 }
             }
             break;
    case 9 : // Clamp Sabotage
    case 10: // Planet Shatterer
    case 11: // Sun Nova
             Grav_Pull(bullet);
             break;
    case 12: // GravClamp Release
             if (ship[bullet->owner].life>0) {
               int oldcx=ship[bullet->owner].cx;
               int oldcy=ship[bullet->owner].cy;
               ship[bullet->owner].cx=bullet->x;
               ship[bullet->owner].cy=bullet->y;
               Grav_Pull(bullet);
               ship[bullet->owner].image.x=(int)bullet->x-10;
               ship[bullet->owner].image.y=(int)bullet->y-10;
               ship[bullet->owner].angle+=10;
               if(ship[bullet->owner].angle>=360.0) ship[bullet->owner].angle-=360.0;
               Rotate_Ship(&ship[bullet->owner]);
               Erase_Sprite(&ship[bullet->owner].image);
               Save_Behind(&ship[bullet->owner].image);
               Draw_Sprite(&ship[bullet->owner].image);
               if ((int)sqrt((bullet->x-oldcx)*(bullet->x-oldcx)+(bullet->y-oldcy)*(bullet->y-oldcy))
                 >bullet->counter) bullet->power=0;
               else {
                 ship[bullet->owner].cx=oldcx;
                 ship[bullet->owner].cy=oldcy;
               }
             }
  }
}

void Grav_Pull(shots *bullet) {
////////////////////////////////////////////////////////////////////////////
//        Gravity Equation from  Sohrab Ismail-Beigi's Gravity Wars.      //
////////////////////////////////////////////////////////////////////////////
float ax, ay,                                // Stores acceleration x and y.
      dx, dy, dr, k, vc;
char count;

  ax=ay=0;
  for (count=0;count<MAX_PLANETS;count++) {
    if (planet[count].life>0) {
      dx=bullet->x-planet[count].cx;
      dy=bullet->y-planet[count].cy;
      dr=sqrt(dx*dx+dy*dy);     // how far away from the shot the planet is.
      k=1/(dr*dr*dr);
      ax=ax-planet[count].GM*dx*k;
      ay=ay-planet[count].GM*dy*k;
    }
  }
  bullet->vx+=(ax*DT);
  bullet->vy+=(ay*DT);
  bullet->x+=bullet->vx*DT;
  bullet->y+=bullet->vy*DT;
}

char Check_Collision(shots *bullet) {    // 1-still active. 0-Hit something.
int c, s;
char toreturn;

  toreturn=1;
  switch (bullet->type) {
    case 1 : // Torpedo
             if (Hit_Something(bullet)) {
               PLOT((int)bullet->oldx,(int)bullet->oldy,bullet->backgrnd);
               toreturn=0;
               c=Hit_Ship(bullet);
               if (c>0) {
                 ship[--c].life-=bullet->power;
                 if (ship[c].life<=0) {
                   ship[bullet->owner].kills++;
                   Kill_Ship(c);
                 }
                 Draw_Side();
               } else {
                 c=Hit_Planet(bullet);
                 if (c>0){
                   planet[--c].life-=bullet->power/10;
                   if (!(planet[c].life>0))
                     Kill_Planet(c,bullet->owner);
                 }
               }
             bullet->power=0;
             }
             break;
    case 2 : // Phasic
             if (Hit_Something(bullet)) {
               c=Hit_Ship(bullet);
               if (c>0) {
                 ship[--c].life-=bullet->power;
                 bullet->power=-ship[c].life;
                 if (bullet->power<=0) {
                   bullet->power=0;
                   PLOT((int)bullet->oldx,(int)bullet->oldy,bullet->backgrnd);
                   toreturn=0;
                 }
                 if (!(ship[c].life>0)) {
                   ship[bullet->owner].kills++;
                   Kill_Ship(c);
                 }
                 Draw_Side();
               }
             }
             break;
    case 3 : // Bouncer
             if (Hit_Something(bullet)) {
               PLOT((int)bullet->oldx,(int)bullet->oldy,bullet->backgrnd);
               c=Hit_Ship(bullet);
               if (c>0) {
                 ship[--c].life-=bullet->power;
                 if (!(ship[c].life>0)) {
                   ship[bullet->owner].kills++;
                   Kill_Ship(c);
                 }
                 Draw_Side();
                 toreturn=0;
                 bullet->power=0;
               } else {
                 c=Hit_Planet(bullet);
                 if (c>0)
                   if (planet[--c].type<2) {                   // Not a sun.
                     bullet->counter--;
                     if (bullet->counter>0) {
                       // SOUND : -boing-
                       bullet->x-=bullet->vx*DT;
                       bullet->y-=bullet->vy*DT;
                       float oldvx=bullet->vx;
                       if (oldvx<0) oldvx=-oldvx;
                       float oldvy=bullet->vy;
                       if (oldvy<0) oldvy=-oldvy;
                       if (bullet->x>planet[c].cx) bullet->vx=1;
                         else bullet->vx=-1;
                       if (bullet->y>planet[c].cy) bullet->vy=1;
                         else bullet->vy=-1;
                       bullet->vx*=oldvy;
                       bullet->vy*=oldvx;
                       bullet->x+=bullet->vx*DT;
                       bullet->y+=bullet->vy*DT;
                     } else toreturn=0;
                   } else {                                     // is a sun.
                     toreturn=0;
                     planet[c].life-=bullet->power/10;
                     if (planet[c].life<=0)
                       Kill_Planet(c,bullet->owner);
                     bullet->power=0;
                   }
               }
             }
             break;
    case 4 : // FireCracker
             if (Hit_Something(bullet)) {
               PLOT((int)bullet->oldx,(int)bullet->oldy,bullet->backgrnd);
               toreturn=0;
               c=Hit_Ship(bullet);
               if (c>0) {
                 ship[--c].life-=bullet->power;
                 if (!(ship[c].life>0)) {
                   ship[bullet->owner].kills++;
                   Kill_Ship(c);
                 }
                 Draw_Side();
               } else {
                 c=Hit_Planet(bullet);
                 if (c>0) {
                   planet[--c].life-=bullet->power/10;
                   if (!(planet[c].life>0))
                     Kill_Planet(c,bullet->owner);
                 }
               }
             bullet->power=0;
             }
             break;
    case 7 : // Spawner
             if (Hit_Something(bullet)) {
               PLOT((int)bullet->oldx,(int)bullet->oldy,bullet->backgrnd);
               toreturn=0;
               c=Hit_Ship(bullet);
               if (c>0) {
                 ship[--c].life-=bullet->power;
                 if (!(ship[c].life>0)) {
                   ship[bullet->owner].kills++;
                   Kill_Ship(c);
                 }
                 Draw_Side();
               } else {
                 c=Hit_Planet(bullet);
                 if (c>0) {
                   planet[--c].life-=bullet->power/10;
                   if (!(planet[c].life>0))
                     Kill_Planet(c,bullet->owner);
                 }
               }
             bullet->power=0;
             }
             break;
    case 8 : // WeaponX
             if (Hit_Something(bullet)) {
               PLOT((int)bullet->oldx,(int)bullet->oldy,bullet->backgrnd);
               toreturn=0;
               c=Hit_Ship(bullet);
               if (c>0){
                 ship[--c].life-=bullet->power;
                 if (!(ship[c].life>0)) {
                   ship[bullet->owner].kills++;
                   Kill_Ship(c);
                 }
                 Draw_Side();
               } else {
                 c=Hit_Planet(bullet);
                 if (c>0){
                   planet[--c].life-=bullet->power/10;
                   if (!(planet[c].life>0))
                     Kill_Planet(c,bullet->owner);
                 }
               }
               bullet->power=0;
             }
             break;
    case 9 : // Clamp Sabotage
             if (Hit_Something(bullet)) {
               PLOT((int)bullet->oldx,(int)bullet->oldy,bullet->backgrnd);
               c=Hit_Ship(bullet);
               if (c>0){
                 c--;
                 // Release Clamp
                 bullet->type=12;
                 PLOT(bullet->oldx,bullet->oldy,bullet->backgrnd);
                 bullet->oldx=bullet->x=ship[c].cx;
                 bullet->oldy=bullet->y=ship[c].cy;
                 bullet->backgrnd=screen[(int)bullet->x+((int)bullet->y<<6)+((int)bullet->y<<8)];
                 bullet->color=bullet->backgrnd;
                 bullet->vx=0;
                 bullet->vy=0;
                 bullet->counter=ship[bullet->owner].inventory[9]*10;
                 bullet->power=1;
                 bullet->owner=c;
               } else {
                 toreturn=0;
                 bullet->power=0;
               }
             }
             break;
    case 10: // Planet Shatterer
             if (Hit_Something(bullet)) {
               PLOT((int)bullet->oldx,(int)bullet->oldy,bullet->backgrnd);
               toreturn=0;
               c=Hit_Ship(bullet);
               if (c>0){
                 ship[--c].life-=bullet->power/100;
                 if (!(ship[c].life>0)) {
                   ship[bullet->owner].kills++;
                   Kill_Ship(c);
                 }
                 Draw_Side();
               } else {
                 c=Hit_Planet(bullet);
                 if (c>0)
                   if (planet[--c].type!=2) {                  // Not a sun.
                     planet[c].life-=bullet->power;
                     if (!(planet[c].life>0))
                       Kill_Planet(c,bullet->owner);
                   } else {                                     // is a sun.
                     planet[c].life-=bullet->power/100;
                     if (!(planet[c].life>0))
                       Kill_Planet(c,bullet->owner);
                   }
               }
               bullet->power=0;
             }
             break;
    case 11: // Sun Nova
             if (Hit_Something(bullet)) {
               PLOT((int)bullet->oldx,(int)bullet->oldy,bullet->backgrnd);
               toreturn=0;
               c=Hit_Ship(bullet);
               if (c>0) {
                 ship[--c].life-=bullet->power/100;
                 if (!(ship[c].life>0)) {
                   ship[bullet->owner].kills++;
                   Kill_Ship(c);
                 }
                 Draw_Side();
               } else {
                 c=Hit_Planet(bullet);
                 if (c>0)
                   if (planet[--c].type!=2) {                  // Not a sun.
                     planet[c].life-=bullet->power/100;
                     if (!(planet[c].life>0))
                       Kill_Planet(c,bullet->owner);
                   } else {                                     // is a sun.
                     planet[c].life-=bullet->power;
                     if (!(planet[c].life>0))
                       Kill_Planet(c,bullet->owner);
                   }
               }
               bullet->power=0;
             }
             break;
    case 12: // Clamp_Release
             float temp;
             for (c=0;c<MAX_PLANETS;c++)
               if (planet[c].life>0) {
                 temp=(bullet->x-planet[c].cx)*(bullet->x-planet[c].cx)+(bullet->y-planet[c].cy)*(bullet->y-planet[c].cy);
                 if (sqrt(temp)<(planet[c].r+12)) {
                   Kill_Ship(bullet->owner);
                   Draw_Side();
                   bullet->backgrnd=0;
                   bullet->power=0;
                   c=MAX_PLANETS;
                   toreturn=0;
                 }
               }
             if (bullet->power>0)
               for (c=0;c<MAX_SHIPS;c++)
                 if ((ship[c].life>0)&&(c!=bullet->owner)) {
                   temp=(bullet->x-ship[c].cx)*(bullet->x-ship[c].cx)+(bullet->y-ship[c].cy)*(bullet->y-ship[c].cy);
                   if (sqrt(temp)<22.0) {
                     Kill_Ship(bullet->owner);
                     ship[bullet->owner].kills++;
                     Kill_Ship(c);
                     Draw_Side();
                     bullet->backgrnd=0;
                     bullet->power=0;
                     c=MAX_PLANETS;
                     toreturn=0;
                   }
                 }
  }
  return toreturn;
}

char Hit_Something(shots *bullet) {
int x,y,count;
float dx,dy,dr,vc,k;

  x=(int)bullet->x;
  y=(int)bullet->y;
  if ((x>GAME_X)&&(y>GAME_Y)&&(x<GAME_X2)&&(y<GAME_Y2))        // on screen.
    if ((screen[x+(y<<6)+(y<<8)]<=MAX_COLLISION)
      &&(screen[x+(y<<6)+(y<<8)]!=0)) {
      return 1;
  }
  dx=x-139; dy=y-62; dr=sqrt((dx*dx)+(dy*dy));
  if (dr>=CRITICAL_DIST) {
    vc=(dx*bullet->vx+dy*bullet->vy)/dr;
    k=0;
    if (clocker<1000) {
      for (count=1;count<MAX_PLANETS;count++) k=k+planet[count].GM;
      if ((0.5*vc*vc)>=k/dr)                       // Have we escape velocity.
        return 1;
    } else {
      count=10^(clocker/1000);
      if ((x<(GAME_X+count))||(y<(GAME_Y+count))||(x>(GAME_X2+count))||(y>(GAME_Y2+count)))
        return 1;
    }
  }
  return 0;
}

char Hit_Ship(shots *bullet) {
int x, y, c;
float temp;

  x=(int)bullet->x;
  y=(int)bullet->y;
  for (c=0;c<MAX_SHIPS;c++)               // check to see if it hit someone.
    if (ship[c].life>0) {
      temp=(x-ship[c].cx)*(x-ship[c].cx)+(y-ship[c].cy)*(y-ship[c].cy);
      if (sqrt(temp)<12) {
        // SOUND : *ping*
        return ++c;
      }
    }
  return 0;
}

char Hit_Planet(shots *bullet) {
int x, y, c;
float temp;

  x=(int)bullet->x;
  y=(int)bullet->y;
  for (c=0;c<MAX_PLANETS;c++)
    if (planet[c].life>0) {
      temp=(x-planet[c].cx)*(x-planet[c].cx)+(y-planet[c].cy)*(y-planet[c].cy);
      if (sqrt(temp)<planet[c].r+2) {
        // SOUND : thump
        return ++c;
      }
    }
  return 0;
}

void Kill_Ship(char c) {
int z,s;

  ship[c].life=0;
  if (next_loser==num_ships)
    ship[c].losses--;
  win_order[--next_loser]=c;
  explosion.x=explosion.oldx=ship[c].image.x;
  explosion.y=explosion.oldy=ship[c].image.y;
  explosion.flip=rand()%4;
  Save_Behind(&explosion);
  // SOUND : Burning explosion. About 1 second.
  for (s=0;s<5;s++) {
    explosion.cel=s;
    for (z=0;z<8;z++)
      Wait_Retrace();
    Erase_Sprite(&explosion);
    Draw_Sprite(&explosion);
  }
  explosion.flip=rand()%4;
  for (s=5;s<10;s++) {
    explosion.cel=s;
    for (z=0;z<8;z++)
      Wait_Retrace();
    Erase_Sprite(&ship[c].image);
    Draw_Sprite(&explosion);
  }
  Erase_Sprite(&ship[c].image);
}

void Kill_Planet(char c, char owner) {
int z;
RGB_tripple white_out[256];

  planet[c].life=0;
  for (z=0;z<256;z++) {
    white_out[z].blue=63;
    white_out[z].green=63;
    white_out[z].red=63;
  }
  // SOUND : A kind of sh-sh-Sh-Sh-SH-SH-SHA.
  Fade_To(white_out,1);
  for (z=0;z<25;z++) Wait_Retrace();
  Erase_Sprite(&planet[c].image);
  // SOUND : BANG!
  Create_Shot(planet[c].cx,planet[c].cy,owner,30,planet[c].r/2*planet[c].d);
  Fade_To(game_pal,1);

}

char Create_Shot(int x, int y, char owner, int power, int number) {
int c;

  for (c=0;((c<most_shots)&&(number));c++)
    if (!shot[c].power) {
      shot[c].type=1;
      shot[c].power=power;
      shot[c].owner=owner;
      shot[c].counter=0;
      shot[c].x=shot[c].oldx=x;
      shot[c].y=shot[c].oldy=y;
      shot[c].color=SHOT_COLOR+owner;
      shot[c].backgrnd=0;
      shot[c].vx=rand()%90/10-5;
      shot[c].vy=rand()%90/10-5;
      number--;
    }
  if (!number)
    return 1;
  else
    return 0;
}
////////////////////////////////////
void Tally() {
int c, player, score;
long total;
char *s;
int x[6]={1,108,215,1,108,215};
int y[6]={1,1,1,96,96,96};
char *names[6]={"Ouhiayo","Jdomklna","Mfnmrh","Hkmmnana","Dork","Human"};
char *places[6]={"first","second","third","fourth","fifth","last"};

  player=0;
  for(c=0;c<num_ships;c++)
    if (ship[win_order[c]].losses>0)
      player++;
  if (player==1) {
    BW_Fade();
    // SOUND : winner tada or something.
    for (c=248;c<256;c++)
      Set_Pal_Reg(c,&game_pal[c]);
    Draw_Box(133,90,30,9,255);
    Draw_Border(133,90,30,9,1);
    Alph_Write("WINNER",135,92,&small_text);
    Draw_Box(125,102,40,9,255);
    Draw_Border(125,102,40,9,1);
    Alph_Write(names[win_order[0]],127,104,&small_text);

    Draw_Box(140,120,22,9,255);
    Draw_Border(140,120,22,9,1);
    Alph_Write("Kills",142,122,&small_text);
    score=ship[win_order[0]].total_kills+ship[win_order[0]].kills;
    tiks.cel=1;
    tiks.y=122;
    tiks.x=130;
    while (score>=10) {
      Draw_Sprite(&tiks);
      tiks.x-=8;
      score-=10;
    }
    tiks.cel=0;
    while (score>0) {
      Draw_Sprite(&tiks);
      tiks.x-=3;
      score--;
    }

    Draw_Box(103,HEIGHT-30,114,9,255);
    Draw_Border(103,HEIGHT-30,114,9,1);
    Alph_Write("PRESS ANY KEY TO CONTINUE",105,HEIGHT-28,&small_text);
  } else {
    PCX_Show(&tally_behind);
    Draw_Box(0,HEIGHT-10,WIDTH,10,253);
    Draw_Box(103,HEIGHT-10,114,9,255);
    Draw_Border(103,HEIGHT-10,114,9,1);
    Alph_Write("PRESS ANY KEY TO CONTINUE",105,HEIGHT-8,&small_text);
    for (s=screen+106;s<screen+(HEIGHT-10)*320;s+=320) *s=253;
    for (s=screen+213;s<screen+(HEIGHT-10)*320;s+=320) *s=253;
    Draw_Border(0,0,105,94,0);
    Draw_Border(107,0,105,94,0);
    Draw_Border(214,0,105,94,0);
    Draw_Border(0,95,105,94,0);
    Draw_Border(107,95,105,94,0);
    Draw_Border(214,95,105,94,0);
    for (c=0;c<num_ships;c++) {
      player=win_order[c];
      little_icon.x=x[player]+13;
      little_icon.y=y[player]+2;
      little_icon.cel=player;
      Draw_Border(x[player]+12,y[player]+1,11,11,1);
      Draw_Sprite(&little_icon);
      Draw_Box(x[player]+25,y[player]+2,55,9,255);
      Draw_Border(x[player]+25,y[player]+2,55,9,1);
      Alph_Write(names[player],x[player]+27,y[player]+4,&small_text);
      Draw_Box(x[player]+35,y[player]+14,26,9,255);
      Draw_Border(x[player]+35,y[player]+14,26,9,1);
      Alph_Write(places[c],x[player]+37,y[player]+16,&small_text);
      if (ship[player].losses>0) {
        Draw_Box(x[player]+5,y[player]+25,57,40,255);
        Draw_Border(x[player]+5,y[player]+25,57,40,1);
        Draw_Box(x[player]+65,y[player]+25,15,40,255);
        Draw_Border(x[player]+65,y[player]+25,15,40,0);
        Draw_Box(x[player]+85,y[player]+25,15,40,255);
        Draw_Border(x[player]+85,y[player]+25,15,40,0);
        score=0;
        Alph_Write("Kills",x[player]+8,y[player]+27,&small_text);
        Alph_Write(ship[player].kills,x[player]+67,y[player]+27,&small_text);
        Alph_Write(ship[player].kills*(30/num_ships),x[player]+87,y[player]+27,&small_text);
        score=ship[player].kills*(30/num_ships);
        ship[player].total_kills+=ship[player].kills;

        Alph_Write("Extra Energy",x[player]+8,y[player]+33,&small_text);
        Alph_Write(ship[player].energy,x[player]+67,y[player]+33,&small_text);
        Alph_Write(ship[player].energy,x[player]+87,y[player]+33,&small_text);
        score+=ship[player].energy;

        Alph_Write("Place Bonus",x[player]+8,y[player]+39,&small_text);
        Alph_Write((6-c)*10,x[player]+87,y[player]+39,&small_text);
        score+=(6-c)*10;

        if (ship[player].life>0) {
          Alph_Write("Survival",x[player]+8,y[player]+45,&small_text);
          Alph_Write(10,x[player]+87,y[player]+45,&small_text);
          score+=10;
          if (ship[player].kills==num_ships-1) {
            Alph_Write("Total Kill",x[player]+8,y[player]+51,&small_text);
            Alph_Write(10,x[player]+87,y[player]+51,&small_text);
            score+=10;
          }
        } else if (ship[player].kills==num_ships) {
          Alph_Write("Total Kill",x[player]+8,y[player]+51,&small_text);
          Alph_Write(10,x[player]+87,y[player]+51,&small_text);
          score+=10;
        }
        ship[player].kills=0;
        Draw_Box(x[player]+30,y[player]+67,25,9,255);
        Draw_Border(x[player]+30,y[player]+67,25,9,1);
        Alph_Write("Total",x[player]+32,y[player]+69,&small_text);
        Draw_Box(x[player]+85,y[player]+67,15,9,255);
        Draw_Border(x[player]+85,y[player]+67,15,9,0);
        Alph_Write(score,x[player]+87,y[player]+69,&small_text);

        total=(ship[player].money*intrest)/10+score;
        ship[player].money=total;
      } else {
        Draw_Box(x[player]+10,y[player]+25,80,9,255);
        Draw_Border(x[player]+10,y[player]+25,80,9,1);
        // SOUND : Kind of a short honk or something.
        switch (rand()%6) {
          case 0 : Alph_Write("Kiss 'em Good-bye",x[player]+12,y[player]+27,&small_text);
                   break;
          case 1 : Alph_Write("  I let you Win",x[player]+12,y[player]+27,&small_text);
                   break;
          case 2 : Alph_Write("  T-T-That's all!",x[player]+12,y[player]+27,&small_text);
                   break;
          case 3 : Alph_Write("That's gotta hurt",x[player]+12,y[player]+27,&small_text);
                   break;
          case 4 : Alph_Write("Not close enough",x[player]+12,y[player]+27,&small_text);
                   break;
          case 5 : Alph_Write("Everyone wave bye!",x[player]+12,y[player]+27,&small_text);
                   break;
        }
      }
    }
  }
  while (Get_Scan_Code()) {};
  while (!Get_Scan_Code()) {};
}
////////////////////////////////////
void Rotate_Ship(ships *enterprise) {

  enterprise->image.flip=rot_flip_table[enterprise->angle/90];
  enterprise->image.cel=rot_cel_table[enterprise->angle/5];
}

////////////////////////////////////
void Draw_Border(int x, int y, int width, int height, char raised) {
unsigned char *s, *stop;

  char color1=raised ? 254 : 252;
  char color2=raised ? 252 : 254;
  setmem(&screen[x+(y<<6)+(y<<8)],width,color1);
  setmem(&screen[x+((y+height)<<6)+((y+height)<<8)],width,color2);
  stop=&screen[x+((y+height)<<6)+((y+height)<<8)];
  for (s=&screen[x+(y<<6)+(y<<8)];s<=stop;s+=WIDTH)
    *s=color2;
  stop=&screen[x+width+((y+height)<<6)+((y+height)<<8)];
  for (s=&screen[x+width+(y<<6)+(y<<8)];s<=stop;s+=WIDTH)
    *s=color1;
}
////////////////////////////////////
void Close() {

  Set_Mode(TEXT);
}

////////////////////////////////////
void Save_Block(unsigned char far *start, int x, int y, int width,int height) {
unsigned char *line_start;
unsigned char far *background, far *offset, far *endline, far *workback;

  workback=start;
  endline=screen+((y+height)<<8)+((y+height)<<6)+x;

  for (offset=screen+(y<<8)+(y<<6)+x;offset<endline;offset+=320) {
    _fmemcpy(workback, offset, width);
    workback+=width;
  }
}
////////////////////////////////////

void Restore_Block(unsigned char far *start, int x, int y, int width,int height) {
unsigned char far *background, far *offset, far *endline, far *workback;

  workback=start;
  endline=screen+((y+height)<<8)+((y+height)<<6)+x;

  for (offset=screen+(y<<8)+(y<<6)+x;offset<endline;offset+=320) {
    _fmemcpy(offset, workback, width);
    workback+=width;
  }
}
////////////////////////////////////
unsigned int Get_Control_Keys(unsigned int mask) {
// returns the status of requested control keys on enhansed keyboard.

  return(mask&_bios_keybrd(_NKEYBRD_SHIFTSTATUS));
}

unsigned char Get_Scan_Code(void) {
// get the scan code of a key press, since we have to look at status bits
// let's use the inline assembler


// is a key ready?

  asm {
    mov ah,01h         // function 1: is a key ready?
    int 16h            // call the interrupt
    jz empty           // there was no key so exit
    mov ah,00h         // function 0: get the scan code please
    int 16h            // call the interrupt
    mov al,ah          // result was in ah so put into al
    xor ah,ah          // zero out ah
    jmp done           // data's in ax...let's blaze!
  }
empty:
  asm xor ax,ax        // clear out ax i.e. 0 means no key
done:

// since data is in ax it will be returned properly. (skip warning)

} // end Get_Scan_Code
