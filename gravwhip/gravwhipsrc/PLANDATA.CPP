////////////////////////////////////////////////////////////////////////////
//         PlanDat : Gravity Whip planet data file maker.                 //
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

// F U N C T I O N S //////////

int getnumber(FILE *fp) {
char ch;

  int num=0;
  if ((ch=getc(fp))==EOF) {
    cout << "ERROR-END of FILE";
    exit(2);
  }
  while (ch=='#') {
    while (ch!='\n')
      if ((ch=getc(fp))==EOF) {
        cout << "ERROR-END of FILE";
        exit(2);
      }
    if ((ch=getc(fp))==EOF) {
      cout << "ERROR-END of FILE";
      exit(2);
    }
  }
  while ((ch>='0')&&(ch<='9')) {
    num=num*10+ch-'0';
    if ((ch=getc(fp))==EOF) {
      cout << "ERROR-END of FILE";
      exit(2);
    }
  }
  return num;
}
////////////////////////////////
float getfloat(FILE *fp) {
char ch;

  float absolute=0.0,fractional=0.0;
  if ((ch=getc(fp))==EOF) {
    cout << "ERROR-END of FILE";
    exit(2);
  }

  while (ch=='#') {
    while (ch!='\n')
      if ((ch=getc(fp))==EOF) {
        cout << "ERROR-END of FILE";
        exit(2);
      }

    if ((ch=getc(fp))==EOF) {
      cout << "ERROR-END of FILE";
      exit(2);
    }

  }
  while ((ch>='0')&&(ch<='9')) {
    absolute=absolute*10.0+ch-'0';
    if ((ch=getc(fp))==EOF) {
      cout << "ERROR-END of FILE";
      exit(2);
    }
  }
  if (ch=='.') {
    if ((ch=getc(fp))==EOF) {
      cout << "ERROR-END of FILE";
      exit(2);
    }
    while ((ch>='0')&&(ch<='9')) {
      fractional=(float)(fractional/10.0+ch-'0');
      if ((ch=getc(fp))==EOF) {
        cout << "ERROR-END of FILE";
        exit(2);
      }
    }
    fractional=fractional/10.0;
  }
  return (absolute+fractional);
}

// M A I N ////////////////////

void main(int argc, char *argv[]) {
pcx_picture planets;
sprite far *planet;
FILE *datafile, *planet_in;
float *table;
char *table2;
int c;
int num_planets,diameter,x,y;
char planet_name[80];


  if (argc<2) {
    cout << "You must input a file name. Example:\n"
         << "WHIPDAT planets.txt";
  } else {
    if ((planet_in=fopen(argv[1],"r"))==NULL) {
      cout << "INVALID FILE NAME : " << argv[1];
    } else {
      cout << "LOADING : ";
      do {
        if (fgets(planet_name,80,planet_in)==NULL) {
          cout << "INVALID FILE NAME : " << planet_name;
          exit(2);
        }
      } while (planet_name[0]=='#');
      planet_name[strlen(planet_name)-1]=NULL;
      if (PCX_Load(planet_name,&planets)!=0) {
        exit(2);
      }
      cout << "check.\nGRABING : ";

      num_planets=getnumber(planet_in);
      cout << num_planets << " planets : ";
      table=new float [num_planets];
      table2=new char [num_planets];
      planet=new sprite [num_planets];

      for (c=0;c<num_planets;c++) {
        diameter=getnumber(planet_in);
        x=getnumber(planet_in);
        y=getnumber(planet_in);
        Init_Sprite(&planet[c],diameter,diameter,1);
        Grab_Sprite(&planets,x,y,0,&planet[c]);
        cout << ".";

        table[c]=getfloat(planet_in);
        table2[c]=getnumber(planet_in);
      }
      fclose(planet_in);

      cout << "check.\nSAVING :\n";
      datafile=fopen("planets.dat","wb");
      cout << " PLANETS : ";
      putc(num_planets,datafile);
      for (c=0;c<num_planets;c++) {
        Save_Sprite(datafile,&planet[c]);
        cout << ".";
      }
      cout << "check.\n TYPES : ";
      fwrite(table2,sizeof(char),num_planets,datafile);
      cout << "check.\n DENSITIES : ";
      fwrite(table,sizeof(float),num_planets,datafile);
      cout << "check.\n PALLETTE : ";
      for (c=0;c<144;c++)
        Save_Color(datafile,planets.pal[c]);
      cout << "check.\n DONE";
      fclose(datafile);
    }
  }
} // end main
