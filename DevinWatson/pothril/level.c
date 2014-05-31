#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "level.h"
#include "dice.h"

void init_world() {
    numlevels = 0;
    game_world = NULL;
    cur_map = NULL;
}

void stripout(char *s,const char c) {   /* Remove all c's from string s */
    size_t i=0,j=0;
    while(i < strlen(s)) {
        if(s[i]!=c) { s[j]=s[i]; j++;}
        i++;
    }
    s[j]=0;
}

/* Generates a random name for a level */
char *makeLevelName() {
    const char *letpairs = "..LEXEGEZACEBISOUSESARMAINDIREA.ERATENBERALAVETIEDORQUANTEISRION";
    char *buf;
    int i;

    buf = (char *)malloc(sizeof(char) * 9);
    memset(buf,0,sizeof(buf));

    for (i = 0; i < 8; i++) {
        buf[i] = letpairs[randMap(0,strlen(letpairs)-1)];
    }
    buf[8] = 0;
    stripout(buf,'.');
    for (i = 1; i < 8; i++) {
        buf[i] = tolower(buf[i]);
    }
    return buf;
}

/* Creates a map for a level in the world list */
LEVELMAP* create_map(char *level_name, int level_id, int width, int height, boolean has_shops, int level_type) {
    LEVELMAP *new_element;
    generation_params *params;
    generation_params *params_set;
    int xi, yi, ii, jj;
    int generations = 1;

    if ( (new_element = (LEVELMAP *)malloc(sizeof(LEVELMAP))) == NULL) return NULL;
    memset(new_element, 0, sizeof(LEVELMAP));

    new_element->level_name = (char *)malloc(sizeof(char)*strlen(level_name)+1);
    memset(new_element->level_name, 0, strlen(level_name) + 1);
    if (level_name != NULL)
        strcpy(new_element->level_name,level_name);

    new_element->level_id = level_id;
    if (width < MAP_DISPLAY_WIDTH * 3) width = MAP_DISPLAY_WIDTH * 3;
    if (height < MAP_DISPLAY_HEIGHT * 3) height = MAP_DISPLAY_HEIGHT * 3;

    new_element->width = width;
    new_element->height = height;
    new_element->has_shops = has_shops;
    new_element->level_type = level_type;
    new_element->portal_up_loc.x = new_element->portal_up_loc.y = new_element->portal_down_loc.x = new_element->portal_down_loc.y = new_element->shop_loc.x = new_element->shop_loc.y = 0;
    new_element->next = NULL;

    /*
        Now we build up the level
    */
	new_element->grid  = (int**)malloc(sizeof(int*) * height);

	/* Allocate for the scratch area */
	grid2 = (int**)malloc(sizeof(int*) * height);

	for(yi=0; yi < height; yi++) {
		new_element->grid[yi] = (int*)malloc(sizeof(int) * width);
		grid2[yi] = (int*)malloc(sizeof(int) * width);
	}

	for(yi=1; yi < height-1; yi++)
        for(xi=1; xi < width-1; xi++)
            new_element->grid[yi][xi] = mapPick(35);

	for(yi=0; yi < height; yi++)
        for(xi=0; xi < width; xi++)
            grid2[yi][xi] = OUTERWALL;

	for(yi=0; yi < height; yi++)
		new_element->grid[yi][0] = new_element->grid[yi][new_element->width-1] = OUTERWALL;
	for(xi=0; xi < width; xi++)
		new_element->grid[0][xi] = new_element->grid[new_element->height-1][xi] = OUTERWALL;

    params = params_set = (generation_params*)malloc( sizeof(generation_params) * generations);

    params->r1_cutoff = 30;
    params->r2_cutoff = 5;
    params->reps = 5;
/*
    params++;
    params->r1_cutoff = 20;
    params->r2_cutoff = 3;
    params->reps = 1;
    params++;
*/
 	for(ii=0; ii < generations; ii++) {
 		params = &params_set[ii];
 		for(jj=0; jj < params->reps; jj++)
 			generation(new_element,params);
 	}

    free(params);
    free(params_set);

    for (ii = 0; ii < new_element->height; ii++) {
        free(grid2[ii]);
    }

    /* We're done with the secondary grid, deallocate it */
    free(grid2);

    /* If a shop needs to be placed place it */
    if (new_element->has_shops) {
            placeShop(new_element);
    }

    placePortal(new_element);

    /* return the newly created element in memory */
    return new_element;
}

int add_level(char *level_name, int level_id, int width, int height, boolean has_shops, int level_type) {
    LEVELMAP *new_element;
    LEVELMAP *current;

    new_element = create_map(level_name, level_id, width, height, has_shops, level_type);

    if (new_element == NULL) return 1;

    if (game_world == NULL) {
        game_world = new_element;
        if (game_world == NULL) return 1;
        numlevels++;
        return 0;
    } else {
        current = game_world;
        while (current->next != NULL)
            current = current->next;
    }

    current->next = new_element;
    numlevels++;

    return 0;
}

LEVELMAP* get_map(int id) {
    LEVELMAP *elt;

    elt = game_world;
    while (elt != NULL) {
        if (elt->level_id == id) return elt;
        elt = elt->next;
    }

    return NULL;
}

/*
    Deallocate every map in memory
*/
void world_destroy() {
    LEVELMAP *current;
    LEVELMAP *temp;
    int yi;

    current = game_world;
    while (current != NULL) {
        temp = current->next;

        for (yi = 0; yi < current->height; yi++) {
            free(current->grid[yi]);
        }
        free(current->grid);

        if (current->level_name != NULL)
            free(current->level_name);

        free(current);
        current = temp;
        numlevels--;
    }

/*    free(game_world);*/
    game_world = NULL;
    numlevels = 0;
}


/*
    Computes a "generation" of a map
    using cellular automata
*/
void generation(LEVELMAP *map, generation_params *params) {
    int xi, yi, ii, jj;

    for(yi=1; yi < map->height-1; yi++)
        for(xi=1; xi < map->width-1; xi++) {
            int adjcount_r1 = 0, adjcount_r2 = 0;
            for(ii=-1; ii<=1; ii++)
                for(jj=-1; jj<=1; jj++) {
                    if(map->grid[yi+ii][xi+jj] != GRAVEL)
                        adjcount_r1++;
                }
                for(ii=yi-2; ii<= yi+2; ii++)
                    for(jj=xi-2; jj <= xi+2; jj++) {
                        if(abs(ii-yi)== 2 && abs(jj-xi) == 2) continue;
                        if(ii<0 || jj<0 || ii>=map->height || jj>= map->width) continue;
                        if(map->grid[ii][jj] != GRAVEL)
                            adjcount_r2++;
                    }

                    if(adjcount_r1 >= params->r1_cutoff || adjcount_r2 <= params->r2_cutoff)
                        grid2[yi][xi] = WALL;
                    else
                        grid2[yi][xi] = GRAVEL;
        }

        for(yi=1; yi < map->height-1; yi++)
            for(xi=1; xi < map->width-1; xi++)
                map->grid[yi][xi] = grid2[yi][xi];
}

boolean isUsable(LEVELMAP *map, int x, int y) {
    return ( map->grid[y][x] == GRAVEL);
}

boolean isPassable(LEVELMAP *map, int x, int y) {
    return ( map->grid[y][x] != WALL && map->grid[y][x] != OUTERWALL);
}

boolean isLineClear(LEVELMAP *map, int x, int y) {
    int i;

    for (i = y; i < y + 5; i++) {
        if (!isPassable(map,x,i)) return 0;
    }
    return 1;
}

int randMap(int min, int max) {
    double r;
    double x;
    r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
	x = (r * max);
    return (int)x + min;
}

void placePortal(LEVELMAP *map) {
    int portalX;
    int portalY;
    int yi;

    portalX = randMap(2,map->width - 10);
    portalY = randMap(2,map->height - 16);

    map->portal_up_loc.x = portalX;
    map->portal_up_loc.y = portalY;
    map->grid[portalY][portalX] = PORTAL;

    for (yi = portalY + 1; yi < portalY + 15; yi++) {
        if (map->grid[yi][portalX] != OUTERWALL && map->grid[yi][portalX] != SHOP && map->grid[yi][portalX] != PORTAL)
            map->grid[yi][portalX] = GRAVEL;
    }

    portalX = randMap(2,map->width - 10);
    portalY = randMap(2,map->height - 16);

    map->portal_down_loc.x = portalX;
    map->portal_down_loc.y = portalY;
    map->grid[portalY][portalX] = PORTAL;

    for (yi = portalY + 1; yi < portalY + 15; yi++) {
        if (map->grid[yi][portalX] != OUTERWALL && map->grid[yi][portalX] != SHOP && map->grid[yi][portalX] != PORTAL)
            map->grid[yi][portalX] = GRAVEL;
    }

}

/* Place a shop on level */
void placeShop(LEVELMAP *map) {
    int shopX;
    int shopY;
    int yi;

    shopX = randMap(2,map->width - 10);
    shopY = randMap(2,map->height - 16);

    map->grid[shopY][shopX+1] = OUTERWALL;
    map->grid[shopY][shopX-1] = OUTERWALL;
    map->grid[shopY-1][shopX] = OUTERWALL;
    map->grid[shopY-1][shopX-1] = OUTERWALL;
    map->grid[shopY-1][shopX+1] = OUTERWALL;

    map->grid[shopY+1][shopX-1] = WATER;
    map->grid[shopY+1][shopX+1] = WATER;
    map->grid[shopY][shopX] = SHOP;

    for (yi = shopY + 1; yi < shopY + 15; yi++) {
        map->grid[yi][shopX] = GRAVEL;
    }

    map->shop_loc.x = shopX;
    map->shop_loc.y = shopY;

}

void buildMaps(int map_count) {
    int i;

    if (map_count > MAX_NUMLEVELS) map_count = MAX_NUMLEVELS;
    for (i = 0; i < map_count; i++) {
        add_level(makeLevelName(),i,300,200,1,LEVEL_RUINS);
    }
}

void dumpMapList(FILE *f) {
    LEVELMAP *map;

    map = game_world;
    while (map != NULL) {
        fprintf(f,"Map %i - %p\tNext: %p\n",map->level_id,map,map->next);
        map = map->next;
        fflush(f);
    }
}
