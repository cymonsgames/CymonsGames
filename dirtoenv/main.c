/* dirtoenv by Joseph Larson ver3Sept2008*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <curses.h>
#include <stdlib.h>

#define MAXNUMFILES 1000

int getint (void) {
    int t, in, x, y;

    t = 0;
    while (1) {
      in = getch(); /*printw(" %d ",in);*/
      if ((in >= '0') && (in <= '9')) {
          t = t * 10 + (in - '0');
          addch (in);
      }
      if ((in == 8) && (t > 0)) {
          t = t / 10;
          getyx(stdscr, y, x);
          move (y, x-1);
          delch ();
      }
      if ((in == 459) || (in == 10)) return t;
    }
}

int main (int argc, char *argv[]) {
    DIR *dp;
    struct dirent *ep;
    char envvar[255];
    char prompt[255];
    char files[MAXNUMFILES][255];
    int numfiles;
    int longest, numcols, maxonscreen;
    int c, page; /* c is a generic counter variable */

    if (argc < 2) {
        perror ("dirtoenv\n-------\n"
        "A utility to display a list of all files and directories and assign the selected\n"
        "directory or file to the environment variable passed to it.\n\n"
        "Usage:\n"
        "dirtoenv EnvironmentVariable [Prompt]\n"
        "EnvironmentVariable - The environment variable you want to assign.\n"
        "Prompt - An optional prompt to display.\n\n");
        return (1);
    }
    strcpy (envvar, "SET "); /* SET added to get around local environment issues */
    strcat (envvar, argv[1]);
    if (argc > 2) {
        strcpy (prompt, argv[2]);
        for (c = 3; c < argc; c++) {
            strcat(prompt, " ");
            strcat(prompt, argv[c]);
        }
    } else strcpy (prompt, "Choose a file/directory:");
    dp = opendir ("./");
    if (dp != NULL) {
        numfiles = 0;
        longest = 0;
        while ((ep = readdir (dp)) && (numfiles < MAXNUMFILES)) {
            strcpy (files[numfiles], ep->d_name);
            if (strlen(files[numfiles]) > longest)
                longest = strlen(files[numfiles]);
            numfiles++;
       }
        (void) closedir (dp);
    } else {
        perror ("Couldn't open the directory");
        return (1);
    }
    /* all the variables are set. Let's rock and roll */

    initscr (); keypad(stdscr,1); noecho(); /* Now we're in Curses mode */
    longest += 6;
    numcols = COLS / longest;
    maxonscreen = (LINES - 5) * numcols;
    page = 0;

    while (1) { /* I'm making some formatting assumptions. Maybe I'll fix it later. If you are reading this, maybe not */
        clear();
        mvprintw (0,0, prompt);

        for (c = 0; (c < maxonscreen) && ((c + page * maxonscreen) < numfiles); c++)
            mvprintw (c % (LINES - 5) + 2, (COLS / numcols) * (c / (LINES - 5))
                , "%d) %s", c + 1, files[c + page * maxonscreen]);
        if (page > 0) {
            mvprintw (LINES - 2, 0, "%d) First Page", maxonscreen + 1);
            mvprintw (LINES - 2, 20, "%d) Previous Page", maxonscreen + 2);
        }
        if (page < (numfiles / maxonscreen)) {
            mvprintw (LINES - 2,40, "%d) Next Page", maxonscreen + 3);
            mvprintw (LINES - 2,60, "%d) Last Page", maxonscreen + 4);
        }
        mvprintw (LINES - 1, 0, "Type the number of your selection : ");
        refresh ();

        c = getint ();
        if ((c > 0) && (c <= maxonscreen) && ((c - 1 + page * maxonscreen) < numfiles)) {
            endwin ();
            strcat(envvar, "=");
            strcat(envvar, files[c - 1 + page * maxonscreen]);
/*            putenv (envvar); This does nothing outside of the program, hence the following: */
            FILE *fp;

            fp = fopen ("c:\\temp.bat", "w");
            if (fp != NULL) {
                fprintf(fp, "%s\n", envvar);
                fclose(fp);
            }
            printf ("You must not run c:\\temp.bat to set the environment variable.\n");

            return (0);
        }
        if ((c == maxonscreen + 1) && (page > 0)) page = 0;
        if ((c == maxonscreen + 2) && (page > 0)) page--;
        if ((c == maxonscreen + 3) && ((page + 1) * maxonscreen) < numfiles) page ++;
        if ((c == maxonscreen + 4) && ((page + 1) * maxonscreen) < numfiles) page = numfiles / maxonscreen;
    }
}
