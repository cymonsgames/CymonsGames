/*
	cargo.h - Cargo definitions
	Released under the GNU General Public License V2
	Author: Devin Watson
*/
#ifndef __CARGO_H
#define __CARGO_H

#define MAX_CARGO_PARTITIONS 20
#define MAX_CARGO_FOR_SALE 8
#define MAX_CARGOS 38
#define MAX_TONS 200

#ifndef FALSE
	#define FALSE 0
#endif

typedef struct sCargoPartition {
    int cargo_type;
    int amt;
    long price;
    int pct;
    int purchase_loc;
    int in_port;
} CARGOPARTITION;


CARGOPARTITION cargo_bays[MAX_CARGO_PARTITIONS] = {
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE}
};

CARGOPARTITION starport_cargo[MAX_CARGO_FOR_SALE] = {
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE},
    {-1,0,0,0,0,FALSE}
};

typedef struct sCargoType {
    const char *cargo_name;
    int price_factors[6];
    long long basePrice;
    short amt_multiplier;
} CARGOTYPE;

CARGOTYPE cargos[MAX_CARGOS] = {
    {"Crystals", {3, -2, 2, -2, 0, -4}, 20000, 1},
    {"Radioactives", {0, 1, 4, -3, 0, -2},1000000, 1},
    {"Alloys", {-2, 0, -4, 6, 1, -2}, 200000, 1},
    {"Medicine", {-1, 4, -4, 3, -2, 0},100000, 1},
    {"Gems", {4, -2, 4, -4, -1, 1}, 1000000, 1},
    {"Aircraft", {-2, 4, -3, 4, 1, -1}, 1000000, 1},
    {"Grav Sleds", {2, 0, -1, 1, 0, 0}, 6000000, 1},
    {"Neurocomputers", {1, 0 ,-2, 0, 0, 0},  10000000, 1},
    {"Terraformers", {1, 0 ,-1, 0, 0, 0},  25000000, 1},
    {"Planetary Matrix", {1, 0, -1, 0, 0}, 75500000, 1},
    {"ATV", {-2, 2, -2, 1, 1, 0}, 300000, 1},
    {"AFV", {0, 2, -2, 0, 0, 1}, 700000, 1},
    {"Firearms", {-2, 6, -4, 1, 0, 0}, 30000, 2},
    {"Ammunition", {-1, 6, -5, 2, 0, 0}, 30000, 2},
    {"Plasma Guns", {-1, 3, -2, 0, 0, 0}, 200000, 2},
    {"Tools", {-4, 7, -8, 4, 5, 0}, 10000, 2},
    {"Body Armor", {-3, 6, -4, 1, 0, 0}, 50000, 2},
    {"Farm Machinery", {-2, 2, -6, 0, 6, -4}, 150000, 1},
    {"Liquor", {3, 3, -1, 0, -3, 0}, 10000, 1},
    {"Silver", {3, -1, 3, -1, 0, -2}, 70000, 1},
    {"Spices", {4, -2, 3, -1, -5, 2}, 6000, 1},
    {"Electronics", {0, 0, -4, 4, 1, 1}, 100000, 1},
    {"Mechanical Parts", {0, 1, -3, 3, 2, 1}, 75000, 1},
    {"Cybernetic Parts", {1, 0, -4, 2, 1, 0}, 250000, 1},
    {"Computer Parts", {-1, 0, -2, 1, 0}, 250000, 1},
    {"Machine Tools", {1, 0, -2, 1, 0, 0}, 750000, 1},
    {"Space Suits", {-1, 2, -3, 2, 2, 0}, 400000, 1},
    {"Fruit", {1, 2, 3, 3, -4, -6}, 1000, 2},
    {"Textiles", {3, 0, -3, 1, -4, -3}, 3000, 3},
    {"Polymers", {-2, 0 ,3, 3, 0, 0}, 7000, 4},
    {"Meat", {0, 0, 5, 2, -5, 5}, 1500, 4},
    {"Petrochemicals", {2, 0, 4, -2, 3, 0}, 10000, 6},
    {"Grain", {0, 0, 1, 3, -5, 6}, 300, 8},
    {"Wood", {0, 0, 1, 2, -7, 3}, 1000, 2},
    {"Copper", {2, 2, 3, -2, -1, -3}, 2000, 2},
    {"Tin", {2, 2, 3, -4, -1, -2}, 9000, 3},
    {"Steel", {-1, 2, 6, 0, 0, 0}, 500, 4},
    {"Aluminum", {-1, 1, 3, -2, 0, -2}, 1000, 5}
};

#define PRICE_DATA_MAX 14
float price_data[PRICE_DATA_MAX] = {
    0.400,
    0.500,
    0.700,
    0.800,
    0.900,
    1.000,
    1.100,
    1.200,
    1.300,
    1.500,
    1.700,
    2.000,
    3.000,
    4.000
};

#endif

