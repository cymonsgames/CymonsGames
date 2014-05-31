#include <dos.h>
#include<stdio.h>

void main (int argc, char *argv[]) {
FILE *input, *output;
int ch;

  if (argc>1) {
    input=fopen(argv[1],"r");
    output=fopen(argv[2],"w");
    while ((ch=fgetc(input))!=EOF) {
      if (ch==9) {
        fputc(' ',output);
        fputc(' ',output);
        fputc(' ',output);
        fputc(' ',output);
        fputc(' ',output);
        fputc(' ',output);
        fputc(' ',output);
        fputc(' ',output);
      } else
        fputc(ch,output);
    }
    fclose(input);
    fclose(output);      
  }
}