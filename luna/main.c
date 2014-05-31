/* luna by whitespace
 * Made for the TIGSource cockpit compo.
 */
#include <curses.h>
#include <string.h>

int main() {
    while (1) {
        int row,col;
        int y=0;
        int t=-1;
        int h=550;
        int v=45;
        int f=150;
        int e=48;

        char tit[] = "      ";
        char title[] = " LUNA ";
        char author[] = "by Whitespace";
        char cockpit[] ="Tigsource Cockpit Competition 2009";
        initscr();
        curs_set(0);
        raw();
        noecho();
        clear();
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        attron(COLOR_PAIR(1));
        getmaxyx(stdscr,row,col);
        mvprintw(row/2-1,(col-strlen(tit))/2,"%s",tit);
        mvprintw(row/2,(col-strlen(title))/2,"%s",title);
        mvprintw(row/2+1,(col-strlen(tit))/2,"%s",tit);
        attroff(COLOR_PAIR(1));
        mvprintw(row/2+3,(col-strlen(author))/2,"%s",author);
        mvprintw(row/2+4,(col-strlen(cockpit))/2,"%s\n",cockpit);
        getch();
        clear();
        while (h>0) {
            clear();
            y=e-48;
            t+=1;
            f-=y;
            if (f<0) {
                f=0;
                y=0;
            };
            v+=(5-y);
            h-=v;
            if (h<0) {
                h=0;
            };
            mvprintw(10,35,"Time:%d",t);
            mvprintw(11,35,"Height:%d",h);
            mvprintw(12,35,"Speed:%d",v);
            mvprintw(13,35,"Fuel:%d",f);
            attron(COLOR_PAIR(1));
            mvprintw(15,25," Burn fuel units:(press 0-9) ");
            attroff(COLOR_PAIR(1));

            if (h==0 && v>5) {
                clear();
                char fail1[] = "You are on the moon.";
                char fail2[] = "You landed too fast.";
                char fail3[] = "Now you're dead.";
                mvprintw(row/2-1,(col-strlen(fail1))/2,"%s",fail1);
                mvprintw(row/2,(col-strlen(fail2))/2,"%s",fail2);
                mvprintw(row/2+1,(col-strlen(fail3))/2,"%s",fail3);
                break;
            };
            if (h==0 && v<6) {
                clear();
                char succ1[] = "You succesfully landed on the moon.";
                char succ2[] = "I wonder what you want to do here.";
                mvprintw(row/2-1,(col-strlen(succ1))/2,"%s",succ1);
                mvprintw(row/2,(col-strlen(succ2))/2,"%s",succ2);
                break;
            };
            e = getch();
            while (e<48 || e>57) {
                e = getch();
            }

        }
        getch();
    }
    endwin();
    return 0;
}
