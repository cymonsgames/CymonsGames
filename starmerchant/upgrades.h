/*
	upgrades.h - Ship upgrades
	Released under the GNU General Public License V2
	Author: Devin Watson

*/

#ifndef __UPGRADES_H
#define __UPGRADES_H

#define MAX_UPGRADES 7

typedef struct sShipUpgrade {
    const char *package_name;
    float engine_speed;
    long upgrade_cost;
    short tonnage;
} SHIPUPGRADE;

SHIPUPGRADE upgrades[MAX_UPGRADES] = {
    {"50XFTL",0.02,500000,200},
    {"67XFTL",0.015,1500000,210},
    {"Einsteinium Drive Mk. I",0.01,3750000,220},
    {"Einsteinium Drive Mk. II",0.00525,65000000, 250},
    {"Terradrive Mk. I",0.005,100000000,300},
    {"Terradrive Mk. II",0.0045,275000000,400},
    {"Infinidrive",0.003,500000000,600}
};

#endif
