// I N C L U D E S ////////

#include <dos.h>
#include <conio.h>
#include <io.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <malloc.h>
#include <values.h>

#include "joegraph.h"

// C O N S T A N T S //////

const unsigned long WIDTH=320;
const unsigned long HEIGHT=200;

// G L O B A L S //////////

// F U N C T I O N S //////

void Set_Mode(int mode) {
  asm {
    mov ah, 0
    mov al, BYTE PTR mode
    int 10h
  }
}

void Fill_Screen(int color) {
  asm {
    mov ax, 0A000h
    mov es, ax
    xor di, di

    mov cx, HEIGHT*WIDTH/2

    mov al, BYTE PTR color
    mov ah, al
    rep stosw
  }
}

void Set_Pal_Reg(int index, RGB_tripple *color) {
  outp(PAL_MASK,0xFF);               // Make sure the palet reg is unmasked.
  outp(PAL_WRITE, index);             // Set up to write to chosen register.

  outp(PAL_DATA,color->red);                                     // The red,
  outp(PAL_DATA,color->green);                                     // green,
  outp(PAL_DATA,color->blue);                                   // and blue.
}

void Get_Pal_Reg(int index, RGB_tripple *color) {

  outp(PAL_MASK,0xFF);
  outp(PAL_READ, index);

  color->red=inport(PAL_DATA);
  color->green=inport(PAL_DATA);
  color->blue=inport(PAL_DATA);
}

void Wait_Retrace(void) {

while (inport(0x03DA)&0x08) {}         // wait till VGA is done with retrace.
while (!(inport(0x03DA)&0x08)) {}        // wait for start of next retrace.
}

int PCX_Load(char *filename, pcx_picture *picture) {
// I cannot take credit for writing this logrythm. I just tweaked it into
// working order.

FILE *fp;
static unsigned char outbyte,bytecount;
int index;
const int BYTEMODE=0, RUNMODE=1;
int mode=BYTEMODE;
unsigned long i;

  picture->image=new unsigned char[WIDTH*HEIGHT];

  if ((fp=fopen(filename, "rb"))==NULL)
    return (1);

  fread((char *)&picture->header,sizeof(pcx_header),1,fp);

  fseek(fp,128L,SEEK_SET);
  for(i=0;i<WIDTH*HEIGHT;i++){
    if (mode==BYTEMODE) {
      outbyte=(unsigned char)fgetc(fp);
      if (outbyte>0xbf) {
	bytecount=(int)((int)outbyte&0x3f);
	outbyte=(unsigned char)getc(fp);
	if (--bytecount>0) mode=RUNMODE;
      }
    } else if (--bytecount==0) mode=BYTEMODE;
    picture->image[i]=outbyte;
  }

  fseek(fp,-768L,SEEK_END);
  for (index=0; index<256; index++) {
    picture->pal[index].red=(fgetc(fp)>>2);
    picture->pal[index].green=(fgetc(fp)>>2);
    picture->pal[index].blue=(fgetc(fp)>>2);
  } // end for

  fclose(fp);
  return(0);
}

void PCX_Show(pcx_picture *picture) {
int index;
unsigned long x;

// First, make the the palette...
  for (index=0; index<256; index++)
    Set_Pal_Reg(index,(RGB_tripple *)&picture->pal[index]);
// then put the graphic up.
  memcpy((unsigned char far *)screen,(unsigned char far *)picture->image,WIDTH*HEIGHT);
}

void Free_PCX(pcx_picture *picture) {

  delete picture->image;
}

void Init_Sprite(sprite *pict, unsigned char xsize, unsigned char ysize, unsigned char num_cels) {

  pict->x=0;
  pict->y=0;
  pict->flip=0;
  pict->cel=0;
  pict->xsize=xsize;
  pict->ysize=ysize;
  pict->num_cels=num_cels;
  pict->size=new unsigned int [num_cels];
  pict->image=new unsigned char far * [num_cels];
  pict->behind=new unsigned char far [ysize*xsize];
}

void Free_Sprite(sprite *pict) {
register int c;

  delete pict->size;
  for (c=0;c<pict->num_cels;c++)
    delete pict->image[c];
  delete pict->image;
  delete pict->behind;
}

void Grab_Sprite(pcx_picture *source, int x1, int y1, unsigned char cel, sprite *dest) {
unsigned char far *buffer, far *lastbit, far *linestart, far *lastline,
	      far *runstart, far *datain, far *runsize, far *ptr1, far *ptr2;
unsigned int size, sizerun;

  buffer=new unsigned char far [dest->xsize*dest->ysize*2];
	 // Potental maximum size (if someone is realy trying to destroy this
      // logrythm) is realy 1.5 times (x*y) but since this is a temp buffer I
				    // decided to make it 2 times to be safe.
  size=0;
  runstart=&source->image[y1*WIDTH+x1];
  datain=runstart;
  lastline=&source->image[(dest->ysize+y1)*WIDTH+x1];
  for (linestart=&source->image[y1*WIDTH+x1];linestart<lastline;
      linestart+=WIDTH) {
    lastbit=linestart+dest->xsize;
    lastbit--;
    while ((*lastbit==0)&&(lastbit>linestart))   // point lastbit to the last
      lastbit--;                                 // non-zero bit in the line.
    if (lastbit>linestart) {                                    // code line.
      lastbit++;
      datain=linestart;
      while (*datain==0) datain++;
      buffer[size++]=datain-linestart;
      runstart=datain;
      runsize=&buffer[size++];
      while (datain<lastbit) {
	if (*datain!=0) {
	  buffer[size++]=*datain++;
	  }
	else {
	  *runsize=datain-runstart;
	  runstart=datain++;
	  while (datain==0) datain++;
	  buffer[size++]=datain-runstart;
	  runstart=datain;
	  runsize=&buffer[size++];
	}
      }
    }
    buffer[size++]=255;
    *runsize=datain-runstart;
  }                                                            // Next line.
  dest->size[cel]=(--size);
  dest->image[cel]=new unsigned char far [size];
  ptr1=dest->image[cel];
  for (ptr2=buffer;ptr2<buffer+size;ptr2++) {
    *ptr1++=*ptr2;
  }
  delete buffer;
} // end Grab_Sprite

void Save_Behind(sprite *pict) {
unsigned char far *background, far *offset, far *endline, far *workback;

  workback=pict->behind;
  endline=screen+((pict->y+pict->ysize)<<8)+((pict->y+pict->ysize)<<6)+pict->x;

  for (offset=screen+(pict->y<<8)+(pict->y<<6)+pict->x;
      offset<endline;offset+=320) {
    _fmemcpy(workback, offset, pict->xsize);
    workback+=pict->xsize;
  }
}

void Erase_Sprite(sprite *pict) {
unsigned char far *background, far *offset, far *endline, far *workback;

  workback=pict->behind;
  endline=screen+((pict->oldy+pict->ysize)<<8)+((pict->oldy+pict->ysize)<<6)+pict->oldx;

  for (offset=screen+(pict->oldy<<8)+(pict->oldy<<6)+pict->oldx;
      offset<endline;offset+=320) {
    _fmemcpy(offset, workback, pict->xsize);
    workback+=pict->xsize;

  pict->oldx=pict->x;
  pict->oldy=pict->y;
  }
}

void Draw_Sprite(sprite *pict) {
// This routine decodes sprite to screen. It is complacated (such that I get
// lost) but I will endevor to comment it to death.

int x, y, xflip, yflip, length;
unsigned char far *data ,far *runend, far *dataend,
	      far *dataout, far *nextline;

  xflip=pict->flip&(0x01);            // If bit 1 is off, xflip will be zero.
  yflip=pict->flip&(0x02);            // If bit 2 is off, yflip will be zero.
  data=pict->image[pict->cel];
  dataend=data+pict->size[pict->cel];

  // Set the y to the start.
  if (!yflip)
    dataout=screen+(pict->y<<8)+(pict->y<<6);
  else
    dataout=screen+((pict->y+pict->ysize-1)<<8)+((pict->y+pict->ysize-1)<<6);
  // and the x.
  if (!xflip)
    nextline=(dataout+=pict->x);
  else
    nextline=(dataout+=pict->x+pict->xsize-1);

  while (data<dataend) {              // count is the index to the pict data.
    if (*data==255) {                                   // 255 = end of line.
      if (!yflip)                                         // advance y and...
	dataout=(nextline+=320);
      else
	dataout=(nextline-=320);
      data++;
    } else {
						      // advance past blanks.
      if (!xflip)
	dataout+=*data++;
      else
	dataout-=*data++;
					       // and put info run to screen.
      length=*data++;
      runend=length+data;
      while (data<runend)
	if (!xflip)
	  *dataout++=*data++;
	else
	  *dataout--=*data++;
    } // end if/else
  } // end while
} // End Draw_Sprite


void Init_Alph(alph *alphabet, char height) {
// The Height is constant for the whole alphabet. The width is variable so
// that letters I and W can be different sized.
char c;

  for (c=0;c<26;c++)
    alphabet->upper[c]=new unsigned char far * [height];

  for (c=0;c<26;c++)
    alphabet->lower[c]=new unsigned char far * [height];

  for (c=0;c<10;c++)
    alphabet->numbers[c]=new unsigned char far * [height];

  for (c=0;c<9;c++)
    alphabet->other[c]=new unsigned char far * [height];

  alphabet->height=height;
}

void Grab_Alph(char type, pcx_picture *source, alph *alphabet, char index, int x1,int y1,int width) {
// At this point the data is simply copied with no transparences. This will
// require major change eventualy, but for a routine writen under duress,
// you can't ask too much.
char c, len, longest;
unsigned char far **buffer;
unsigned char far **dest;

  buffer=new unsigned char far * [alphabet->height];
  for (c=0;c<alphabet->height;c++)
    buffer[c]=new unsigned char far [width+1];

  longest=0;

  for (c=0;c<alphabet->height;c++) {                 // dump char to buffer.
    _fmemcpy(buffer[c],&source->image[(y1+c)*320+x1],width);
    len=width-1;
    while (buffer[c][len]==0) len--;   // count 0s from end so that I and W
    len++;                           // and the like can be different sized.
    if (len>longest) longest=len;
  }

  switch (type) {
    case 0:       dest=alphabet->upper[index];
                  break;
    case 1:       dest=alphabet->lower[index];
                  break;
    case 2:       dest=alphabet->numbers[index];
                  break;
    case 3:       dest=alphabet->other[index];
  }
  for (c=0;c<alphabet->height;c++) {
    dest[c]=new unsigned char far [longest+1];
    dest[c][0]=longest;
    _fmemcpy(&dest[c][1],buffer[c],longest);
  }

  for (c=0;c<alphabet->height;c++)
    delete buffer[c];
  delete buffer;
}

void Alph_Write(char *text, int x, int y, alph *font) {
char c;
int column;
unsigned char far **source;

  column=x;
  while (*text!=NULL) {
    if (*text!=' ') {
      if ((*text>='A')&(*text<='Z'))
        source=font->upper[*text-'A'];
      else if ((*text>='a')&(*text<='z'))
        source=font->lower[*text-'a'];
      else if ((*text>='0')&(*text<='9'))
        source=font->numbers[*text-'0'];
      else if (*text=='.')
        source=font->other[1];
      else if (*text=='!')
        source=font->other[2];
      else if (*text=='?')
        source=font->other[3];
      else if (*text==',')
        source=font->other[4];
      else if (*text=='\'')
        source=font->other[5];
      else if (*text=='\"')
        source=font->other[6];
      else if (*text=='-')
        source=font->other[7];
      else if ((*text=='/')||(*text=='\\'))
        source=font->other[8];
      else if (*text==':')
        source=font->other[9];
      else
        source=font->other[0];                                        // '*'
      for (c=0;c<font->height;c++)
        _fmemcpy(&screen[column+((y+c)<<8)+((y+c)<<6)],&source[c][1],source[c][0]);
      column+=source[0][0]+1;
      text++;
    } else {
      column+=font->height;
      text++;
    }
  }
}

void Alph_Write(int num, int x, int y, alph *font) {
char c;
int column;
char power;
int digit;
unsigned char far **source;

  digit=0;
  column=x;
  power=0;
  digit=num;
  while (digit>=10) {
    power++;
    digit=digit/10;
  }
  while (power>=0) {
    digit=num/pow(10,power);
    source=font->numbers[digit];
    for (c=0;c<font->height;c++)
      _fmemcpy(&screen[column+((y+c)<<8)+((y+c)<<6)],&source[c][1],source[c][0]);
    column+=source[0][0]+1;
    num-=(digit*pow(10,power));
    power--;
  }
}

void Alph_In(char *text, int max_len, int x, int y, alph *font) {
int column, index, c;
char *temp;
int *columns;
unsigned char far **source;

  temp=new char [max_len];
  columns=new int [max_len];
  max_len--;
  index=0;
  columns[index]=x;
  while ((temp[index]=getch())!=13) {  // 13=enter
    if (temp[index]==27) { // 27=esc. Clear line.
      Draw_Box(x,y,columns[index]-x,font->height,255);
      index=0;
      columns[index]=x;
    } else if (temp[index]==8) { // 8=backspace
      index--;
      if (index<0) index=0;
      Draw_Box(columns[index],y,columns[index+1]-columns[index],font->height,255);
    } else if (index<=max_len) {
      if (temp[index]!=' ') {
        if ((temp[index]>='A')&(temp[index]<='Z'))
          source=font->upper[temp[index]-'A'];
        else if ((temp[index]>='a')&(temp[index]<='z'))
          source=font->lower[temp[index]-'a'];
        else if ((temp[index]>='0')&(temp[index]<='9'))
          source=font->numbers[temp[index]-'0'];
        else if (temp[index]=='.')
          source=font->other[1];
        else if (temp[index]=='!')
          source=font->other[2];
        else if (temp[index]=='?')
          source=font->other[3];
        else if (temp[index]==',')
          source=font->other[4];
        else if (temp[index]=='\'')
          source=font->other[5];
        else if (temp[index]=='\"')
          source=font->other[6];
        else if (temp[index]=='-')
          source=font->other[7];
        else if ((temp[index]=='/')||(temp[index]=='\\'))
          source=font->other[8];
        else if (*text==':')
          source=font->other[9];
        else
          source=font->other[0];                                        // '*'
        for (c=0;c<font->height;c++)
          _fmemcpy(&screen[columns[index]+((y+c)<<8)+((y+c)<<6)],&source[c][1],source[c][0]);
        columns[index+1]=columns[index]+source[0][0]+1;
        index++;
      } else {
        columns[index+1]=columns[index]+font->height;
        index++;
      }
    }
  }
  delete text;
  delete columns;
  temp[index]=NULL;
  text=new char [index+1];
  memcpy(text,temp,index);
  delete temp;
}

void Alph_In(int *num, int max_len, int x, int y, alph *font) {
int column, index, c;
int *columns;
char temp;
unsigned char far **source;

  *num=0;
  columns=new int [max_len];
  max_len--;
  index=0;
  columns[index]=x;
  while ((temp=getch())!=13) {  // 13=enter
    if (temp==27) { // 27=esc. Clear line.
      Draw_Box(x,y,columns[index]-x,font->height,255);
      index=0;
      columns[index]=x;
    } else if (temp==8) { // 8=backspace
      index--;
      if (index<0) index=0;
      Draw_Box(columns[index],y,columns[index+1]-columns[index],font->height,255);
    } else if (index<=max_len) {
      if ((temp>='0')&&(temp<='9'))
        if ((*num*10)<(MAXINT-10)) {
          source=font->numbers[temp-'0'];
          for (c=0;c<font->height;c++)
            _fmemcpy(&screen[columns[index]+((y+c)<<8)+((y+c)<<6)],&source[c][1],source[c][0]);
          columns[index+1]=columns[index]+source[0][0]+1;
          *num=*num*10+temp-'0';
          index++;
        }
    }
  }
  delete columns;
}

void Free_Alph(alph *alphabet) {
char c,s;

  for (c=0;c<26;c++) {
    for (s=0;s<alphabet->height;s++) {
      delete alphabet->upper[c][s];
      delete alphabet->lower[c][s];
    }
    delete alphabet->upper[c];
    delete alphabet->lower[c];
  }
  for (c=0;c<10;c++) {
    for (s=0;s<alphabet->height;s++)
      delete alphabet->numbers[c][s];
    delete alphabet->numbers[c];
  }
  for (c=0;c<10;c++) {
    for (s=0;s<alphabet->height;s++)
      delete alphabet->other[c][s];
    delete alphabet->other[c];
  }

}

void Fade_To(RGB_tripple *dest, char step) {
int i;
RGB_tripple source[256];
long greatest;
register int compare;

  for (i=0;i<256;i++)
    Get_Pal_Reg(i,&source[i]);
  greatest=63/step;                        // number of steps to finish fade.
  do {
    for (i=0;i<256;i++) {
      compare=dest[i].red-source[i].red;
      if (compare>step)
        source[i].red+=step;
      else if (-compare>step)
        source[i].red-=step;
      else source[i].red=dest[i].red;
        compare=dest[i].green-source[i].green;
      if (compare>step)
        source[i].green+=step;
      else if (-compare>step)
        source[i].green-=step;
      else source[i].green=dest[i].green;
        compare=dest[i].blue-source[i].blue;
      if (compare>step)
        source[i].blue+=step;
      else if (-compare>step)
        source[i].blue-=step;
      else source[i].blue=dest[i].blue;
        Set_Pal_Reg(i,&source[i]);
    }
    Wait_Retrace();
  }
  while (greatest--);
}

void Pal_Rot(char start, char end) {
RGB_tripple store, temp;

  if (start<end) {                                          // Rotate Down.
    Get_Pal_Reg(end,&store);
    do {
      Get_Pal_Reg(end-1,&temp);
      Set_Pal_Reg(end--,&temp);
    } while (start<end);
    Set_Pal_Reg(end,&store);
  } else {
    Get_Pal_Reg(start,&store);
    do {
      Get_Pal_Reg(start-1,&temp);
      Set_Pal_Reg(start--,&temp);
    } while (start>end);
    Set_Pal_Reg(end,&store);
  }
}

void Draw_Box(int x, int y, int sizex, int sizey, char color) {
unsigned char far *linestart, far *lineend;

  lineend=&screen[x+((y+sizey)<<6)+((y+sizey)<<8)];
  for (linestart=&screen[x+(y<<6)+(y<<8)];
       linestart<lineend; linestart+=WIDTH)
    setmem(linestart, sizex, color);
}

void Save_Sprite(FILE *fp, sprite *pict) {
unsigned char far *data;
unsigned char c;

  putc(pict->xsize,fp);
  putc(pict->ysize,fp);
  putc(pict->num_cels,fp);
  for (c=0;c<pict->num_cels;c++)
    fwrite(&pict->size[c],sizeof(unsigned int),1,fp);
  for (c=0;c<pict->num_cels;c++) {
    for (data=pict->image[c];data<pict->image[c]+pict->size[c];data++)
      putc(*data,fp);
  }
}

void Load_Sprite(FILE *fp, sprite *pict) {
unsigned char far *data;
unsigned char c;

  pict->xsize=getc(fp);
  pict->ysize=getc(fp);
  pict->num_cels=getc(fp);
  Init_Sprite(pict,pict->xsize,pict->ysize,pict->num_cels);
  for (c=0;c<pict->num_cels;c++)
    fread(&pict->size[c],sizeof(unsigned int),1,fp);
  for (c=0;c<pict->num_cels;c++) {
    pict->image[c]=new unsigned char far [pict->size[c]];
    for (data=pict->image[c];data<pict->image[c]+pict->size[c];data++)
      *data=getc(fp);
  }
}

void Save_Alph(FILE *fp, alph *alphabet) {
unsigned char c,s;

  putc(alphabet->height,fp);

  for (c=0;c<26;c++)                                // store the characters.
    for (s=0;s<alphabet->height;s++) {
      putc(alphabet->upper[c][s][0],fp);      // put the length of the line.
      fwrite(alphabet->upper[c][s],alphabet->upper[c][s][0]+1,1,fp);
    }
  for (c=0;c<26;c++)                                // store the characters.
    for (s=0;s<alphabet->height;s++) {
      putc(alphabet->lower[c][s][0],fp);      // put the length of the line.
      fwrite(alphabet->lower[c][s],alphabet->lower[c][s][0]+1,1,fp);
    }
  for (c=0;c<10;c++)                                   // store the numbers.
    for (s=0;s<alphabet->height;s++) {
      putc(alphabet->numbers[c][s][0],fp);    // put the length of the line.
      fwrite(alphabet->numbers[c][s],alphabet->numbers[c][s][0]+1,1,fp);
    }
  for (c=0;c<10;c++)                                      // store the other.
    for (s=0;s<alphabet->height;s++) {
      putc(alphabet->other[c][s][0],fp);      // put the length of the line.
      fwrite(alphabet->other[c][s],alphabet->other[c][s][0]+1,1,fp);
    }
}

void Load_Alph(FILE *fp, alph *alphabet) {
unsigned char c, s, size;

  alphabet->height=getc(fp);

  for (c=0;c<26;c++) {                               // read the characters.
    alphabet->upper[c]=new unsigned char far * [alphabet->height];
    for (s=0;s<alphabet->height;s++) {
      size=getc(fp)+1;                        // get the length of the line.
      alphabet->upper[c][s]=new unsigned char far [size];
      fread(alphabet->upper[c][s],size,1,fp);
    }
  }
  for (c=0;c<26;c++) {                               // read the characters.
    alphabet->lower[c]=new unsigned char far * [alphabet->height];
    for (s=0;s<alphabet->height;s++) {
      size=getc(fp)+1;                        // get the length of the line.
      alphabet->lower[c][s]=new unsigned char far [size];
      fread(alphabet->lower[c][s],size,1,fp);
    }
  }
  for (c=0;c<10;c++) {                                  // read the numbers.
    alphabet->numbers[c]=new unsigned char far * [alphabet->height];
    for (s=0;s<alphabet->height;s++) {
      size=getc(fp)+1;                        // get the length of the line.
      alphabet->numbers[c][s]=new unsigned char far [size];
      fread(alphabet->numbers[c][s],size,1,fp);
    }
  }
  for (c=0;c<10;c++) {                                    // store the other.
    alphabet->other[c]=new unsigned char far * [alphabet->height];
    for (s=0;s<alphabet->height;s++) {
      size=getc(fp)+1;                        // get the length of the line.
      alphabet->other[c][s]=new unsigned char far [size];
      fread(alphabet->other[c][s],size,1,fp);
    }
  }
}

void Save_Color(FILE *fp, RGB_tripple color) {

  fwrite(&color,sizeof(RGB_tripple),1,fp);
}

RGB_tripple Load_Color(FILE *fp) {
RGB_tripple send;

  fread(&send,sizeof(RGB_tripple),1,fp);
  return send;
}
