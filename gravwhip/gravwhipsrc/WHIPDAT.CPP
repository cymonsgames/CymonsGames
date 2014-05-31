////////////////////////////////////////////////////////////////////////////
//             WhipDat : Gravity Whip data file maker.                    //
////////////////////////////////////////////////////////////////////////////

// I N C L U D E S ////////

#include <bios.h>
#include <dos.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <malloc.h>

#include "joegraph.cpp"

// M A I N ////////////////////

void main() {
pcx_picture letters, ships;
sprite checks, tiks, ship[6], explosion, boxed_icons, little_icons,
       slider_bar, weapons, ship_input, number_pointers, logo, opening;
FILE *datafile;
int c,s;
alph alphabet;

  cout << "LOADING:\n";
  cout << " ALPHABET : ";
  PCX_Load("alphab.pcx",(pcx_picture *)&letters);
  cout << "check.\n SHIPS : ";
  PCX_Load("shipdata.pcx",(pcx_picture *)&ships);
  cout << "check.\nGRABING:\n";
  cout << " ALPHABET : ";
  Init_Alph(&alphabet,6);

  for (c=0;c<26;c++)                                   // grab alphabet.
    Grab_Alph(0,&letters,&alphabet,c,1+c*6,1,5);
  cout << ".";

  for (c=0;c<26;c++)                                   // grab alphabet.
    Grab_Alph(1,&letters,&alphabet,c,1+c*6,8,5);
  cout << ".";

  for (c=0;c<10;c++)                                    // grab numbers.
    Grab_Alph(2,&letters,&alphabet,c,1+c*6,15,5);
  cout << ".";

  for (c=0;c<10;c++)                                      // grab splat.
    Grab_Alph(3,&letters,&alphabet,c,1+c*6,22,5);

  cout << "check.\n SHIPS : ";
  for (s=0;s<6;s++) {                        // Grab and save all ships.
    Init_Sprite(&ship[s],19,19,10);
    for (c=0;c<10;c++)
      Grab_Sprite(&ships,c*19,s*19,c,&ship[s]);
    cout << ".";
  }

  cout << "check.\n LOSS COUNTER : ";
  Init_Sprite(&checks,7,5,3);
  for (c=0;c<3;c++) {
    Grab_Sprite(&ships,c*7,195,c,&checks);
    cout << ".";
  }

  cout << "check.\n KILL COUNTERS : ";
  Init_Sprite(&tiks,8,8,2);
  for (c=0;c<2;c++) {
    Grab_Sprite(&ships,c*8,187,c,&tiks);
    cout << ".";
  }

  cout << "check.\n EXPLOSION : ";
  Init_Sprite(&explosion,19,19,10);
  for (c=0;c<10;c++) {
    Grab_Sprite((pcx_picture *)&ships,c*19,114,c,(sprite *)&explosion);
    cout << ".";
  }

  cout << "check.\n LOGO : ";
  Init_Sprite(&logo,58,29,1);
  Grab_Sprite(&ships,210,90,0,&logo);

  cout << "check.\n BOXED ICONS : ";
  Init_Sprite(&boxed_icons,19,19,6);
  for (c=0;c<6;c++) {
    Grab_Sprite(&ships,190,c*19,c,&boxed_icons);
    cout << ".";
  }

  cout << "check.\n LITTLE ICONS : ";
  Init_Sprite(&little_icons,10,10,6);
  for (c=0;c<6;c++) {
    Grab_Sprite(&ships,210,c*10,c,&little_icons);
    cout << ".";
  }

  cout << "check.\n SLIDER BAR : ";
  Init_Sprite(&slider_bar,41,7,6);
  for (c=0;c<6;c++) {
    Grab_Sprite(&ships,240,c*7,c,&slider_bar);
    cout << ".";
  }

  cout << "check.\n NUMBER POINTERS : ";
  Init_Sprite(&number_pointers,8,13,12);
  for (c=0;c<12;c++) {
    Grab_Sprite(&ships,290,c*13,c,&number_pointers);
    cout << ".";
  }

  cout << "check.\n WEAPON ICONS : ";
  Init_Sprite(&weapons,12,12,12);
  for (c=0;c<12;c++) {
    Grab_Sprite(&ships,300,c*12,c,&weapons);
    cout << ".";
  }

  cout << "check.\n INPUT ICONS : ";
  Init_Sprite(&ship_input,42,7,1);
  Grab_Sprite(&ships,240,42,0,&ship_input);

  cout << "check.\n OPENING ANIMATION: ";
  Init_Sprite(&opening,61,47,4);
  for (c=0;c<4;c++) {
    Grab_Sprite(&ships,c*61,135,c,&opening);
    cout << ".";
  }

  cout << "check.\nSAVING:\n";
  datafile=fopen("gravwhip.dat","wb");
  cout << " ALPHABET : ";
  Save_Alph(datafile,&alphabet);
  cout << "check.\n SHIPS : ";
  for (c=0;c<6;c++) {
    Save_Sprite(datafile,&ship[c]);
    cout << ".";
  }
  cout << "check.\n EXPLOSIONS : ";
  Save_Sprite(datafile,&explosion);
  cout << "check.\n LOSS COUNTER : ";
  Save_Sprite(datafile,&checks);
  cout << "check.\n KILL COUNTER : ";
  Save_Sprite(datafile,&tiks);
  cout << "check.\n LOGO : ";
  Save_Sprite(datafile,&logo);
  cout << "check.\n BOXED ICONS : ";
  Save_Sprite(datafile,&boxed_icons);
  cout << "check.\n LITTLE ICONS : ";
  Save_Sprite(datafile,&little_icons);
  cout << "check.\n SLIDER BAR : ";
  Save_Sprite(datafile,&slider_bar);
  cout << "check.\n NUMBER POINTERS : ";
  Save_Sprite(datafile,&number_pointers);
  cout << "check.\n WEAPON ICONS : ";
  Save_Sprite(datafile,&weapons);
  cout << "check.\n INPUT ICONS : ";
  Save_Sprite(datafile,&ship_input);
  cout << "check.\n OPENING ANIMATION : ";
  Save_Sprite(datafile,&opening);
  cout << "check.\n PALLETTE : ";
  for (c=144;c<256;c++)
    Save_Color(datafile,ships.pal[c]);

  cout << "check.\n DONE";
  fclose(datafile);
} // end main
