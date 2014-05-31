/*
	starports.h - Starport structures and listings
	Released under the GNU General Public License V2
	Author: Devin Watson

*/
#ifndef __STARPORTS_H
#define __STARPORTS_H

#define MAX_STARPORTS 11

typedef struct sStarport {
    const char *port_name;
    const char *classification;
    float X1;
    float Y1;
    float distance;  /* This is calculated and then stored here with each "jump" */
    int direction;   /* same for this one as well */
    int trade;
    const char *spectral_type;
} STARPORT;

STARPORT starports[MAX_STARPORTS] = {
    {"Lalande 21185","NI,P", 2.83, -7.36, 0.00, 0, 20, "M2V"},
    {"Alpha Centauri","NA,I", -2.51, -3.57, 0.00, 0, 9, "G2V"},
    {"Sirius","A", 8.38, 9.93, 0.00, 0, 2, "A1V"},
    {"Barnards Star","I,P", -6.1, 0, 0.00, 0, 24, "M4V"},
    {"Sol","R",0, 0, 0.00, 0, 32, "G2V"},
    {"Ross 154","NI,NA", -8.87, 2.05, 0.00, 0, 5, "M3.5V"},
    {"Epsilon Eridani","A,P", 8.45, 6.65, 0.00, 0, 18, "K2V"},
    {"Luyten 726-8","NA", 2.99, 7.42, 0.00, 0, 1, "M6V"},
    {"Luyten 789-6","A,NI,P", -4.43, 9.3, 0.00, 0, 22, "M5V"},
    {"Ross 248","A,I", -0.89, 10.26, 0.00, 0, 10, "M5.5V"},
    {"Gliese 876","R,I", 5.38, -14.33, 0.00, 0, 25, "M3.5V"}
};

int trade_factors[6] = {0,0,0,0,0,0};

#endif
