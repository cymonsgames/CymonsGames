// JoeGraph.h - Header file for Joseph Larson's programs including graphic
// handling routines, 1996. Any use without express permission of author is
// forbiden. Any spelling errors are the express property of the author. Any
// programming errors can be blamed on others messing with my code. :)

// D E F I N E S ///////////////

#define MCGA            0x13
#define TEXT            0x03

#define PAL_MASK        0x3c6
#define PAL_READ        0x3c7
#define PAL_WRITE       0x3c8
#define PAL_DATA        0x3c9

// M A C R O S /////////////////

#define PLOT(x,y,color) screen[(y<<8)+(y<<6)+x]=color

// S T R U C T U R E S /////////

typedef struct RGB_tripple_type {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} RGB_tripple;

typedef struct pcx_header_type {
  char manufacturer;
  char ver;
  char encoding;
  char bits_per_pixel;
  int x,y;
  int width, heigth;
  int horz_res;
  int vert_res;
  char ega_palette[48];
  char reserved;
  char num_color_planes;
  int bytes_per_line;
  int palette_type;
  char padding[58];
} pcx_header;

typedef struct pcx_picture_type {
  pcx_header header;
  unsigned char *image;
  RGB_tripple pal[256];
} pcx_picture;

typedef struct sprite_type {
  unsigned int x,y;
  unsigned int oldx,oldy;
  unsigned char flip;          //0=no flip. 1=x flip, 2=y flip, 3=both flip.
  unsigned char cel;
  unsigned char xsize,ysize;
  unsigned char num_cels;
  unsigned int *size;                        // array of size of the images.
  unsigned char far **image;         // array of pointers to encoded images.
  unsigned char far *behind;                 // image of area behind sprite.
  // Current version forces palette info to be handled elsewhere.
} sprite;
// SPRITE ENCODING SCHEME:
// The encoding scheme for sprites are for the purpose of speeding up sprite
// drawing allowing for transparencies. The method is a sort of variation on
// the RLE scheme. Each line is terminated with a 255. The data is in the
// form: <length of zero run>,<length of data run>,<data>,<data>...<data>,
// <length zeros>,<length data>,<data>...,256,...<next line>....

typedef struct alph_type {
  unsigned char far **upper[26];                // To store the letters A-Z.
  unsigned char far **lower[26];                // To store the letters a-z.
  unsigned char far **numbers[10];                           // numbers 0-9.
  unsigned char far **other[10];                      // * . ! ? , ' " - / :
                                           // all others will be shown as *.
  unsigned char height;
} alph;

// G L O B A L S ///////////////

unsigned char far *screen = (unsigned char far*)0xa0000000L;

// P R O T O T Y P E S /////////

void Set_Mode(int mode);
// This function sets the graphics in the selected mode. It is recomended
// that you use either "MCGA" or "TEXT" as those are both #define-d above.

void Fill_Screen(char color);
// Simply fills the screen quickly with one color.

void Set_Pal_Reg (int index, RGB_tripple *color);
// This sets a single color to the specified RGB color tripple.
// Future improvements will do a whole lot of colors at once.

void Get_Pal_Reg (int index, RGB_tripple *color);
// Gets a single color from the current palette.

void Wait_Retrace(void);
// Waits for the Vertical Retrace.

int PCX_Load(char *filename, pcx_picture *pict);
// Loads specified pcx file.

void PCX_Show(pcx_picture *picture);
// Copies the picture and it's palette to the screen.

void Free_PCX(pcx_picture *picture);
// Frees allocated blocks for sprites.

void Init_Sprite(sprite *pict, unsigned char xsize, unsigned char ysize, unsigned char num_cels);
// Initalizes a sprite. Requires xsize, ysize and number of cels.

void Free_Sprite(sprite *pict);
// Frees all the allocated sprites for a sprite allowing it to be reused.

void Grab_Sprite(pcx_picture *source,int x1,int y1, unsigned char cel, sprite *dest);
// Loads a sprite from a PCX picture starting at (x1,y1).

void Free_Sprite(sprite *pict);
// Frees all the allocated sprites for a sprite allowing it to be reused.

void Save_Behind(sprite *pict);
// Saves what will be in the sprites current position. (Call before drawing)

void Replace_Behind(sprite *pict);
// Erases the sprite's old position. (Call before saving behind)

void Draw_Sprite(sprite *pict);
// Draws the current sprite on the screen.

void Init_Alph(alph *alphabet, char height);
// Initalizes alphabet to height given.

void Grab_Alph(char type, pcx_picture *source, alph *alphabet, char index, int x1,int y1,int width);
// Grabs one char. Type: 0=upper case char, 1=lower case, 2=numerical char
// 3=o5h34.

void Alph_Write(char *text, int x, int y, alph *font);
// Puts null terminated string on screen starting at (x,y) in font specified.

void Alph_Write(int num, int x, int y, alph *font);
// Puts int on screen.

void Alph_In(char *text, int max_len, int x, int y, alph *font);
// Reads a string into *text of maximum max_len and echos to (x,y).

void Alph_In(int *num, int max_len, int x, int y, alph *font);
// Reads int in.

void Free_Alph(alph *alphabet);
// Frees allocated space for sprite.

void Fade_To(RGB_tripple *dest, char step);
// recieves array[256] of RGB_tripple and fades current palette to it. Step
// is the length of each color jump. Smaller steps are longer fades.

void Pal_Rot(char start, char end);
// Rotates the current palette fron start to end once.

void Draw_Box(int x, int y, int sizex, int sizey, char color);
// Draws a box at coords (x,y) that is xsize and ysize, and color passed.

void Save_Sprite(FILE *fp, sprite *pict);
// Saves crutal sprite data to file. Read with...

void Load_Sprite(FILE *fp, sprite *pict);
// Loads a sprite from file that was saved with Save_Sprite.

void Save_Alph(FILE *fp, alph *alphabet);
// Guess.

void Load_Alph(FILE *fp, alph *alphabet);
// I'm sure you can figure this one out too.

void Save_Color(FILE *fp, RGB_tripple color);
// Saves a single RGB_tripple to specified file.

RGB_tripple Load_Color(FILE *fp);
// Loads a single RGB_tripple.

