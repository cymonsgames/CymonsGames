/*
 * wumpus.c --- a faithful translation of the classic "Hunt The Wumpus" game.
 *
 * Translator: Eric S. Raymond <esr@snark.thyrsus.com>
 * Version: $Id: wumpus.c,v 1.3 1993/11/07 19:19:27 esr Exp $
 *
 * This was the state of the art 20 years ago, in 1972.  We've come a long
 * way, baby.
 *
 * The BASIC source is that posted by Magnus Olsson in USENET article
 * <9207071854.AA21847@thep.lu.se>: he wrote
 *
 * >Below is the source code for _one_ (rather simple) Wumpus version,
 * >which I found in the PC-BLUE collection on SIMTEL20. I believe this is
 * >pretty much the same version that was published in David Ahl's "101
 * >Basic Computer Games" (or, possibly, in the sequel). 
 *
 * I have staunchly resisted the temptation to "improve" this game.  It
 * is functionally identical to the BASIC version (source for which
 * appears in the comments).  I fixed some typos in the help text.
 *
 * Language hackers may be interested to know that he most difficult thing
 * about the translation was tracking the details required to translate from
 * 1-origin to 0-origin array indexing.
 *
 * The only enhancement is a an -s command-line switch for setting the
 * random number seed.
 *
 * So, pretend for a little while that your workstation is an ASR-33 and
 * limber up your fingers for a trip to nostalgia-land...
 */

#include <stdio.h>

/* 5 REM *** HUNT THE WUMPUS ***                    */

/* 10 DIM P(5)                                */
static int path[5];

static int j, k, arrows, scratchloc;
static char inp[BUFSIZ];        /* common input buffer */

#define YOU    0
#define WUMPUS    1
#define PIT1    2
#define PIT2    3
#define BATS1    4
#define BATS2    5
#define LOCS    6
static int loc[LOCS], save[LOCS];    /* locations */

#define NOT    0
#define WIN    1
#define LOSE    -1
static int finished;

/* 80 REM *** SET UP CAVE (DODECAHEDRAL NODE LIST) ***        */
/* 85 DIM S(20,3)                            */
/* 90 FOR J=1 TO 20                            */
/* 95 FOR K=1 TO 3                            */
/* 100 READ S(J,K)                            */
/* 105 NEXT K                            */
/* 110 NEXT J                            */
/* 115 DATA 2,5,8,1,3,10,2,4,12,3,5,14,1,4,6            */
/* 120 DATA 5,7,15,6,8,17,1,7,9,8,10,18,2,9,11            */
/* 125 DATA 10,12,19,3,11,13,12,14,20,4,13,15,6,14,16        */
/* 130 DATA 15,17,20,7,16,18,9,17,19,11,18,20,13,16,19        */
static int cave[20][3] =
{
    {1,4,7},
    {0,2,9},
    {1,3,11},
    {2,4,13},
    {0,3,5},
    {4,6,14},
    {5,7,16},
    {0,6,8},
    {7,9,17},
    {1,8,10},
    {9,11,18},
    {2,10,12},
    {11,13,19},
    {3,12,14},
    {5,13,15},
    {14,16,19},
    {6,15,17},
    {8,16,18},
    {10,17,19},
    {12,15,18},
};

/* 135 DEF FNA(X)=INT(20*RND(1))+1                    */
#define FNA() (rand() % 20) 

/* 140 DEF FNB(X)=INT(3*RND(1))+1                    */
#define FNB() (rand() % 3) 

/* 145 DEF FNC(X)=INT(4*RND(1))+1                    */
#define FNC() (rand() % 4) 

int getnum(prompt)
char *prompt;
{
    (void) printf("%s\n?", prompt);
    (void) fgets(inp, sizeof(inp), stdin);
    return(atoi(inp));
}

int getlet(prompt)
char *prompt;
{
    (void) printf("%s\n?", prompt);
    (void) fgets(inp, sizeof(inp), stdin);
    return(inp[0]);    
}

void print_instructions()
{
    char ebuf[BUFSIZ];

/* 375 REM *** INSTRUCTIONS ***                        */
/* 380 PRINT "WELCOME TO 'HUNT THE WUMPUS'"                */
    puts("WELCOME TO 'HUNT THE WUMPUS'");
/* 385 PRINT "  THE WUMPUS LIVES IN A CAVE OF 20 ROOMS. EACH ROOM"    */
    puts("  THE WUMPUS LIVES IN A CAVE OF 20 ROOMS. EACH ROOM");
/* 390 PRINT "HAS 3 TUNNELS LEADING TO OTHER ROOMS. (LOOK AT A"        */
    puts("HAS 3 TUNNELS LEADING TO OTHER ROOMS. (LOOK AT A");
/* 395 PRINT "DODECAHEDRON TO SEE HOW THIS WORKS-IF YOU DON'T KNOW"    */
    puts("DODECAHEDRON TO SEE HOW THIS WORKS-IF YOU DON'T KNOW");
/* 400 PRINT "WHAT A DODECAHEDRON IS, ASK SOMEONE)"            */
    puts("WHAT A DODECAHEDRON IS, ASK SOMEONE)");
/* 405 PRINT                                */
    puts("");
/* 410 PRINT "     HAZARDS:"                        */
    puts("     HAZARDS:");
/* 415 PRINT " BOTTOMLESS PITS - TWO ROOMS HAVE BOTTOMLESS PITS IN THEM   
 */
    puts(" BOTTOMLESS PITS - TWO ROOMS HAVE BOTTOMLESS PITS IN THEM");
/* 420 PRINT "     IF YOU GO THERE, YOU FALL INTO THE PIT (& LOSE!)"    */
    puts("     IF YOU GO THERE, YOU FALL INTO THE PIT (& LOSE!)");
/* 425 PRINT " SUPER BATS - TWO OTHER ROOMS HAVE SUPER BATS. IF YOU"    */
    puts(" SUPER BATS - TWO OTHER ROOMS HAVE SUPER BATS. IF YOU");
/* 430 PRINT "     GO THERE, A BAT GRABS YOU AND TAKES YOU TO SOME OTHER" 
   */
    puts("     GO THERE, A BAT GRABS YOU AND TAKES YOU TO SOME OTHER");
/* 435 PRINT "     ROOM AT RANDOM. (WHICH MAY BE TROUBLESOME)"        */
    puts("     ROOM AT RANDOM. (WHICH MAY BE TROUBLESOME)");
/* 440 INPUT "TYPE AN E THEN RETURN ";W9                */
    (void) getlet("TYPE AN E THEN RETURN ");
/* 445 PRINT "     WUMPUS:"                        */
    puts("     WUMPUS:");
/* 450 PRINT " THE WUMPUS IS NOT BOTHERED BY HAZARDS (HE HAS SUCKER"    */
    puts(" THE WUMPUS IS NOT BOTHERED BY HAZARDS (HE HAS SUCKER");
/* 455 PRINT " FEET AND IS TOO BIG FOR A BAT TO LIFT).  USUALLY"    */
    puts(" FEET AND IS TOO BIG FOR A BAT TO LIFT).  USUALLY");
/* 460 PRINT " HE IS ASLEEP.  TWO THINGS WAKE HIM UP: YOU SHOOTING AN"    */
    puts(" HE IS ASLEEP.  TWO THINGS WAKE HIM UP: YOU SHOOTING AN");
/* 465 PRINT "ARROW OR YOU ENTERING HIS ROOM."                */
    puts("ARROW OR YOU ENTERING HIS ROOM.");
/* 470 PRINT "     IF THE WUMPUS WAKES HE MOVES (P=.75) ONE ROOM"    */
    puts("     IF THE WUMPUS WAKES HE MOVES (P=.75) ONE ROOM");
/* 475 PRINT " OR STAYS STILL (P=.25).  AFTER THAT, IF HE IS WHERE YOU"   
 */
    puts(" OR STAYS STILL (P=.25).  AFTER THAT, IF HE IS WHERE YOU");
/* 480 PRINT " ARE, HE EATS YOU UP AND YOU LOSE!"            */
    puts(" ARE, HE EATS YOU UP AND YOU LOSE!");
/* 485 PRINT                                */
    puts("");
/* 490 PRINT "     YOU:"                        */
    puts("     YOU:");
/* 495 PRINT " EACH TURN YOU MAY MOVE OR SHOOT A CROOKED ARROW"        */
    puts(" EACH TURN YOU MAY MOVE OR SHOOT A CROOKED ARROW");
/* 500 PRINT "   MOVING:  YOU CAN MOVE ONE ROOM (THRU ONE TUNNEL)"    */
    puts("   MOVING:  YOU CAN MOVE ONE ROOM (THRU ONE TUNNEL)");
/* 505 PRINT "   ARROWS:  YOU HAVE 5 ARROWS.  YOU LOSE WHEN YOU RUN OUT   
 */
    puts("   ARROWS:  YOU HAVE 5 ARROWS.  YOU LOSE WHEN YOU RUN OUT");
/* 510 PRINT "   EACH ARROW CAN GO FROM 1 TO 5 ROOMS. YOU AIM BY TELLING*/
    puts("   EACH ARROW CAN GO FROM 1 TO 5 ROOMS. YOU AIM BY TELLING");
/* 515 PRINT "   THE COMPUTER THE ROOM#S YOU WANT THE ARROW TO GO TO."    */
    puts("   THE COMPUTER THE ROOM#S YOU WANT THE ARROW TO GO TO.");
/* 520 PRINT "   IF THE ARROW CAN'T GO THAT WAY (IF NO TUNNEL) IT MOVES"*/
    puts("   IF THE ARROW CAN'T GO THAT WAY (IF NO TUNNEL) IT MOVES");
/* 525 PRINT "   AT RANDOM TO THE NEXT ROOM."                */
    puts("   AT RANDOM TO THE NEXT ROOM.");
/* 530 PRINT "     IF THE ARROW HITS THE WUMPUS, YOU WIN."        */
    puts("     IF THE ARROW HITS THE WUMPUS, YOU WIN.");
/* 535 PRINT "     IF THE ARROW HITS YOU, YOU LOSE."            */
    puts("     IF THE ARROW HITS YOU, YOU LOSE.");
/* 540 INPUT "TYPE AN E THEN RETURN ";W9                */
    (void) getlet("TYPE AN E THEN RETURN ");
/* 545 PRINT "    WARNINGS:"                        */
    puts("    WARNINGS:");
/* 550 PRINT "     WHEN YOU ARE ONE ROOM AWAY FROM A WUMPUS OR HAZARD,"   
 */
    puts("     WHEN YOU ARE ONE ROOM AWAY FROM A WUMPUS OR HAZARD,");
/* 555 PRINT "     THE COMPUTER SAYS:"                    */
    puts("     THE COMPUTER SAYS:");
/* 560 PRINT " WUMPUS:  'I SMELL A WUMPUS'"                */
    puts(" WUMPUS:  'I SMELL A WUMPUS'");
/* 565 PRINT " BAT   :  'BATS NEARBY'"                    */
    puts(" BAT   :  'BATS NEARBY'");
/* 570 PRINT " PIT   :  'I FEEL A DRAFT'"                */
    puts(" PIT   :  'I FEEL A DRAFT'");
/* 575 PRINT                                */
    puts("");
/* 580 RETURN                                */
}

void check_hazards()
{
    /* 585 REM *** PRINT LOCATION & HAZARD WARNINGS ***            */
    /* 590 PRINT                            */
    (void) puts("");

    /* 595 FOR J=2 TO 6                            */
    /* 600 FOR K=1 TO 3                            */
    /* 605 IF S(L(1),K)<>L(J) THEN 640                    */
    /* 610 ON J-1 GOTO 615,625,625,635,635                */
    /* 615 PRINT "I SMELL A WUMPUS!"                    */
    /* 620 GOTO 640                            */
    /* 625 PRINT "I FEEL A DRAFT"                    */
    /* 630 GOTO 640                            */
    /* 635 PRINT "BATS NEARBY!"                        */
    /* 640 NEXT K                            */
    /* 645 NEXT J                            */
    for (k = 0; k < 3; k++)
    {
    int room = cave[loc[YOU]][k];

    if (room == loc[WUMPUS])
        (void) puts("I SMELL A WUMPUS!");
    else if (room == loc[PIT1] || room == loc[PIT2])
        (void) puts("I FEEL A DRAFT");
    else if (room == loc[BATS1] || room == loc[BATS2])
        (void) puts("BATS NEARBY!");
    }

    /* 650 PRINT "YOU ARE IN ROOM "L(1)                    */
    (void) printf("YOU ARE IN ROOM %d\n", loc[YOU]+1);

    /* 655 PRINT "TUNNELS LEAD TO "S(L,1);S(L,2);S(L,3)            */
    (void) printf("TUNNELS LEAD TO %d %d %d\n",
          cave[loc[YOU]][0]+1, cave[loc[YOU]][1]+1, cave[loc[YOU]][2]+1);

    /* 660 PRINT                            */
    (void) puts("");

    /* 665 RETURN                            */
}

int move_or_shoot()
{
    int c;

    /* 670 REM *** CHOOSE OPTION ***                    */

badin:
    /* 675 PRINT "SHOOT OR MOVE (S-M)";                    */
    /* 680 INPUT I$                            */
    c = getlet("SHOOT OR MOVE (S-M)");

    /* 685 IF I$<>"S" THEN 700                        */
    /* 690 O=1                                */
    /* 695 RETURN                            */
    /* 700 IF I$<>"M" THEN 675                        */
    /* 705 O=2                                */
    /* 710 RETURN                            */
    if (c == 'S')
    return(1);
    else if (c == 'M')
    return(0);
    else
    goto badin;
}



void shoot()
{
    extern void check_shot(), move_wumpus();
    int    j9;

    /* 715 REM *** ARROW ROUTINE ***                    */
    /* 720 F=0                                */
    finished = NOT;

    /* 725 REM *** PATH OF ARROW ***                    */
badrange:
    /* 735 PRINT "NO. OF ROOMS (1-5)";                    */
    /* 740 INPUT J9                            */
    j9 = getnum("NO. OF ROOMS (1-5)");

    /* 745 IF J9<1 THEN 735                        */
    /* 750 IF J9>5 THEN 735                        */
    if (j9 < 1 || j9 > 5)
    goto badrange;

    /* 755 FOR K=1 TO J9                        */
    for (k = 0; k < j9; k++)
    {
    /* 760 PRINT "ROOM #";                        */
    /* 765 INPUT P(K)                        */
    path[k] = getnum("ROOM #") - 1;

    /* 770 IF K<=2 THEN 790                        */
    if (k <= 1)
        continue;

    /* 775 IF P(K)<>P(K-2) THEN 790                    */
    if (path[k] != path[k - 2])
        continue;

    /* 780 PRINT "ARROWS AREN'T THAT CROOKED - TRY ANOTHER ROOM"    */
    (void) puts("ARROWS AREN'T THAT CROOKED - TRY ANOTHER ROOM");
    /* 785 GOTO 760                            */
    k--;

    /* 790 NEXT K                            */
    }

    /* 795 REM *** SHOOT ARROW ***                    */
    /* 800 L=L(1)                            */
    scratchloc = loc[YOU];

    /* 805 FOR K=1 TO J9                        */
    for (k = 0; k < j9; k++)
    {
    int    k1;

    /* 810 FOR K1=1 TO 3                        */
    for (k1 = 0; k1 < 3; k1++)
    {
        /* 815 IF S(L,K1)=P(K) THEN 895                */
        if (cave[scratchloc][k1] == path[k])
        {
        /*
         * This is the only bit of the translation I'm not sure
         * about.  It requires the trajectory of the arrow to
         * be a path.  Without it, all rooms on the trajectory
         * would be required by the above to be adjacent to the
         * player, making for a trivial game --- just move to where
         * you smell a wumpus and shoot into all adjacent passages!
         * However, I can't find an equivalent in the BASIC.
         */
        scratchloc = path[k];

        /* this simulates logic at 895 in the BASIC code */
        check_shot();
        if (finished != NOT)
            return;
        }

        /* 820 NEXT K1                        */
    }

    /* 825 REM *** NO TUNNEL FOR ARROW ***                */
    /* 830 L=S(L,FNB(1))                        */
    scratchloc = cave[scratchloc][FNB()];

    /* 835 GOTO 900                            */
    check_shot();

    /* 840 NEXT K                            */
    }

ammo:
    if (finished == NOT)
    {
    /* 845 PRINT "MISSED"                        */
    (void) puts("MISSED");

    /* 850 L=L(1)                            */
    scratchloc = loc[YOU];

    /* 855 REM *** MOVE WUMPUS ***                    */
    /* 860 GOSUB 935                        */
    move_wumpus();

    /* 865 REM *** AMMO CHECK ***                    */
    /* 870 A=A-1                            */
    /* 875 IF A>0 THEN 885                        */
    /* 880 F=-1                            */
    if (--arrows <= 0)
        finished = LOSE;
    }

    /* 885 RETURN                            */
}

void check_shot()
{
    /* 890 REM *** SEE IF ARROW IS AT L(1) OR AT L(2)            */
    /* 895 L=P(K)                            */

    /* 900 IF L<>L(2) THEN 920                        */
    /* 905 PRINT "AHA! YOU GOT THE WUMPUS!"                */
    /* 910 F=1                                */
    /* 915 RETURN                            */
    if (scratchloc == loc[WUMPUS])
    {
    (void) puts("AHA! YOU GOT THE WUMPUS!");
    finished = WIN;
    }

    /* 920 IF L<>L(1) THEN 840                        */
    /* 925 PRINT "OUCH! ARROW GOT YOU!"                    */
    /* 930 GOTO 880                            */
    else if (scratchloc == loc[YOU])
    {
    (void) puts("OUCH! ARROW GOT YOU!");
    finished = LOSE;
    }
}

void move_wumpus()
{
    /* 935 REM *** MOVE WUMPUS ROUTINE ***                */
    /* 940 K=FNC(0)                            */
    k = FNC();

    /* 945 IF K=4 THEN 955                        */
    /* 950 L(2)=S(L(2),K)                        */
    if (k < 3)
    loc[WUMPUS] = cave[loc[WUMPUS]][k];

    /* 955 IF L(2)<>L THEN 970                        */
    if (loc[WUMPUS] != loc[YOU])
    return;

    /* 960 PRINT "TSK TSK TSK - WUMPUS GOT YOU!"            */
    (void) puts("TSK TSK TSK - WUMPUS GOT YOU!");

    /* 965 F=-1                                */
    finished = LOSE;

    /* 970 RETURN                            */
}

void move()
{
    /* 975 REM *** MOVE ROUTINE ***                    */
    /* 980 F=0                                */
    finished = NOT;

badmove:
    /* 985 PRINT "WHERE TO";                        */
    /* 990 INPUT L                            */
    scratchloc = getnum("WHERE TO");

    /* 995 IF L<1 THEN 985                        */
    /* 1000 IF L>20 THEN 985                        */
    if (scratchloc < 1 || scratchloc > 20)
    goto badmove;
    scratchloc--;

    /* 1005 FOR K=1 TO 3                        */
    for (k = 0; k < 3; k++)
    {
    /* 1010 REM *** CHECK IF LEGAL MOVE ***                */
    /* 1015 IF S(L(1),K)=L THEN 1045                */
    if (cave[loc[YOU]][k] == scratchloc)
        goto goodmove;

    /* 1020 NEXT K                            */
    }

    /* 1025 IF L=L(1) THEN 1045                        */
    if (scratchloc != loc[YOU])
    {
    /* 1030 PRINT "NOT POSSIBLE -";                    */
    (void) puts("NOT POSSIBLE -");

    /* 1035 GOTO 985                        */
    goto badmove;
    }

goodmove:
    /* 1040 REM *** CHECK FOR HAZARDS ***                */
    /* 1045 L(1)=L                            */
    loc[YOU] = scratchloc;

    if (scratchloc == loc[WUMPUS])
    {
    /* 1050 REM *** WUMPUS ***                    */
    /* 1055 IF L<>L(2) THEN 1090                    */
    /* 1060 PRINT "... OOPS! BUMPED A WUMPUS!"            */
    /* 1065 REM *** MOVE WUMPUS ***                    */
    /* 1070 GOSUB 940                        */
    /* 1075 IF F=0 THEN 1090                    */
    /* 1080 RETURN                            */
    (void) puts("... OOPS! BUMPED A WUMPUS!");
    move_wumpus();
    }
    else if (scratchloc == loc[PIT1] || scratchloc == loc[PIT2])
    {
    /* 1085 REM *** PIT ***                        */
    /* 1090 IF L=L(3) THEN 1100                    */
    /* 1095 IF L<>L(4) THEN 1120                    */
    /* 1100 PRINT "YYYYIIIIEEEE . . . FELL IN PIT"            */
    /* 1105 F=-1                            */
    /* 1110 RETURN                            */
    (void) puts("YYYYIIIIEEEE . . . FELL IN PIT");
    finished = LOSE;
    }
    else if (scratchloc == loc[BATS1] || scratchloc == loc[BATS2])
    {
    /* 1115 REM *** BATS ***                    */
    /* 1120 IF L=L(5) THEN 1130                    */
    /* 1125 IF L<>L(6) THEN 1145                    */
    /* 1130 PRINT "ZAP--SUPER BAT SNATCH! ELSEWHEREVILLE FOR YOU!"    */
    /* 1135 L=FNA(1)                        */
    /* 1140 GOTO 1045                        */
    /* 1145 RETURN                            */
    /* 1150 END                            */
    (void) puts("ZAP--SUPER BAT SNATCH! ELSEWHEREVILLE FOR YOU!");
    scratchloc = loc[YOU] = FNA();
    goto goodmove;
    }
}

main(argc, argv)
int argc;
char *argv[];
{
    int    c;

    if (argc >= 2 && strcmp(argv[1], "-s") == 0)
    srand(atoi(argv[2]));
    else
    srand((int)time((long *) 0));

    /* 15 PRINT "INSTRUCTIONS (Y-N)";                    */
    /* 20 INPUT I$                            */
    c = getlet("INSTRUCTIONS (Y-N)");

    /* 25 IF I$="N" THEN 35                        */
    /* 30 GOSUB 375                            */
    /* 35 GOTO 80                            */
    if (c == 'Y')
    print_instructions();

    /* 150 REM *** LOCATE L ARRAY ITEMS ***                */
    /* 155 REM *** 1-YOU, 2-WUMPUS, 3&4-PITS, 5&6-BATS ***        */
    /* 160 DIM L(6)                            */
    /* 165 DIM M(6)                            */
badlocs:
    /* 170 FOR J=1 TO 6                            */
    /* 175 L(J)=FNA(0)                            */
    /* 180 M(J)=L(J)                            */
    /* 185 NEXT J                            */
    for (j = 0; j < LOCS; j++)
    loc[j] = save[j] = FNA();

    /* 190 REM *** CHECK FOR CROSSOVERS (IE L(1)=L(2), ETC) ***        */
    /* 195 FOR J=1 TO 6                            */
    /* 200 FOR K=1 TO 6                            */
    /* 205 IF J=K THEN 215                        */
    /* 210 IF L(J)=L(K) THEN 170                    */
    /* 215 NEXT K                            */
    /* 220 NEXT J                            */
    for (j = 0; j < LOCS; j++)
    for (k = 0; k < LOCS; k++)
        if (j == k)
        continue;
            else if (loc[j] == loc[k])
        goto badlocs;

    /* 225 REM *** SET NO. OF ARROWS ***                */
newgame:    
    /* 230 A=5                                */
    /* 235 L=L(1)                            */
    arrows = 5;
    scratchloc = loc[YOU];

    /* 240 REM *** RUN THE GAME ***                    */
    /* 245 PRINT "HUNT THE WUMPUS"                    */
    (void) puts("HUNT THE WUMPUS");

#ifdef DEBUG
    (void) printf("Wumpus is at %d, pits at %d & %d, bats at %d & %d\n",
          loc[WUMPUS]+1,
          loc[PIT1]+1, loc[PIT2]+1,
          loc[BATS1]+1, loc[BATS2]+1);
#endif

nextmove:    
    /* 250 REM *** HAZARD WARNING AND LOCATION ***            */
    /* 255 GOSUB 585                            */
    check_hazards();

    /* 260 REM *** MOVE OR SHOOT ***                    */
    /* 265 GOSUB 670                            */
    /* 270 ON O GOTO 280,300                        */
    if (move_or_shoot())
    {
    /* 275 REM *** SHOOT ***                    */
    /* 280 GOSUB 715                        */
    shoot();

    /* 285 IF F=0 THEN 255                        */
    if (finished == NOT)
        goto nextmove;

    /* 290 GOTO 310                            */
    }
    else
    {
    /* 295 REM *** MOVE ***                        */
    /* 300 GOSUB 975                        */
    move();

    /* 305 IF F=0 THEN 255                        */
    if (finished == NOT)
        goto nextmove;
    }

    /* 310 IF F>0 THEN 335                        */
    if (finished == LOSE)
    {
    /* 315 REM *** LOSE ***                        */
    /* 320 PRINT "HA HA HA - YOU LOSE!"                */
    /* 325 GOTO 340                            */
    (void) puts("HA HA HA - YOU LOSE!");
    }
    else
    {
    /* 330 REM *** WIN ***                        */
    /* 335 PRINT "HEE HEE HEE - THE WUMPUS'LL GET YOU NEXT TIME!!"    */
    (void) puts("HEE HEE HEE - THE WUMPUS'LL GET YOU NEXT TIME!!");
    }

    /* 340 FOR J=1 TO 6                            */
    /* 345 L(J)=M(J)                            */
    /* 350 NEXT J                            */
    for (j = YOU; j < LOCS; j++)
    loc[j] = save[j];

    /* 355 PRINT "SAME SETUP (Y-N)";                    */
    /* 360 INPUT I$                            */
    c = getlet("SAME SETUP (Y-N)");

    /* 365 IF I$<>"Y"THEN 170                        */
    /* 370 GOTO 230                            */
    if (c != 'Y')
    goto badlocs;
    else
    goto newgame;
}

/* wumpus.c ends here */
