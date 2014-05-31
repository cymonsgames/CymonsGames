#include "tiles.h"

struct coord tilecoord[numregiondata][numtiles] = {
{ /* region 1 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {0, 0}, {0, 1}, {0, 2}, {0, 3},
/* tpath_1, tpath_2, tpath_3, */
   {1, 0},  {2, 0},  {3, 0},
/* t1001,  t0001,  t0011, */
   {1, 1}, {2, 1}, {3, 1},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {1, 2}, {2, 2},  {0, 2},  {0, 3},  {3, 2},
/* t1100,  t0100,  t0110, */
   {1, 3}, {2, 3}, {3, 3},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {4, 1},{5, 1},{4, 2},{5, 2},{4, 3}, {5, 3},
},
{ /* region 2 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {0, 4}, {0, 5}, {0, 6}, {0, 7},
/* tpath_1, tpath_2, tpath_3, */
   {1, 4},  {2, 4},  {3, 4},
/* t1001,  t0001,  t0011, */
   {1, 5}, {2, 5}, {3, 5},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {1, 6}, {2, 6},  {4, 4},  {5, 4},  {3, 6},
/* t1100,  t0100,  t0110, */
   {1, 7}, {2, 7}, {3, 7},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {4, 5},{5, 5},{4, 6},{5, 6},{4, 7}, {5, 7},
},
{ /* region 3 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {0, 8}, {0, 9}, {0,10}, {0,11},
/* tpath_1, tpath_2, tpath_3, */
   {1, 8},  {2, 8},  {3, 8},
/* t1001,  t0001,  t0011, */
   {1, 9}, {2, 9}, {3, 9},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {1,10}, {2,10},  {4, 8},  {5, 8},  {3,10},
/* t1100,  t0100,  t0110, */
   {1,11}, {2,11}, {3,11},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {4, 9},{5, 9},{4,10},{5,10},{4,11}, {5,11},
},
{ /* region 4 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {0,12}, {0,13}, {0,14}, {0,15},
/* tpath_1, tpath_2, tpath_3, */
   {1,12},  {2,12},  {3,12},
/* t1001,  t0001,  t0011, */
   {1,13}, {2,13}, {3,13},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {1,14}, {2,14},  {4,12},  {5,12},  {3,14},
/* t1100,  t0100,  t0110, */
   {1,15}, {2,15}, {3,15},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {4,13},{5,13},{4,14},{5,14},{4,15}, {5,15},
},
{ /* region 5 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {20, 0}, {20, 1}, {20, 2}, {20, 3},
/* tpath_1, tpath_2, tpath_3, */
   {21, 0},  {22, 0},  {23, 0},
/* t1001,  t0001,  t0011, */
   {21, 1}, {22, 1}, {23, 1},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {21, 2}, {22, 2},  {24, 0},  {25, 0},  {23, 2},
/* t1100,  t0100,  t0110, */
   {21, 3}, {22, 3}, {23, 3},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {24, 1},{25, 1},{24, 2},{25, 2},{24, 3}, {25, 3},
},
{ /* region 6 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {20, 4}, {20, 5}, {20, 6}, {20, 7},
/* tpath_1, tpath_2, tpath_3, */
   {21, 4},  {22, 4},  {23, 4},
/* t1001,  t0001,  t0011, */
   {21, 5}, {22, 5}, {23, 5},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {21, 6}, {22, 6},  {24, 4},  {25, 4},  {23, 6},
/* t1100,  t0100,  t0110, */
   {21, 7}, {22, 7}, {23, 7},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {24, 5},{25, 5},{24, 6},{25, 6},{24, 7}, {25, 7},
},
{ /* region 7 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {20, 8}, {20, 9}, {20,10}, {20,11},
/* tpath_1, tpath_2, tpath_3, */
   {21, 8},  {22, 8},  {23, 8},
/* t1001,  t0001,  t0011, */
   {21, 9}, {22, 9}, {23, 9},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {21,10}, {22,10},  {24, 8},  {25, 8},  {23,10},
/* t1100,  t0100,  t0110, */
   {21,11}, {22,11}, {23,11},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {24, 9},{25, 9},{24,10},{25,10},{24,11}, {25,11},
},
{ /* region 8 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {20,12}, {20,13}, {20,14}, {20,15},
/* tpath_1, tpath_2, tpath_3, */
   {21,12},  {22,12},  {23,12},
/* t1001,  t0001,  t0011, */
   {21,13}, {22,13}, {23,13},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {21,14}, {22,14},  {24,12},  {25,12},  {23,14},
/* t1100,  t0100,  t0110, */
   {21,15}, {22,15}, {23,15},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {24,13},{25,13},{24,14},{25,14},{24,15}, {25,15},
},
{ /* region 9 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {40, 0}, {40, 1}, {40, 2}, {40, 3},
/* tpath_1, tpath_2, tpath_3, */
   {41, 0},  {42, 0},  {43, 0},
/* t1001,  t0001,  t0011, */
   {41, 1}, {42, 1}, {43, 1},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {41, 2}, {42, 2},  {44, 0},  {45, 0},  {43, 2},
/* t1100,  t0100,  t0110, */
   {41, 3}, {42, 3}, {43, 3},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {44, 1},{45, 1},{44, 2},{45, 2},{44, 3}, {45, 3},
},
#if 0
{ /* region 10 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {40, 4}, {40, 5}, {40, 6}, {40, 7},
/* tpath_1, tpath_2, tpath_3, */
   {41, 4},  {42, 4},  {43, 4},
/* t1001,  t0001,  t0011, */
   {41, 5}, {42, 5}, {43, 5},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {41, 6}, {42, 6},  {44, 4},  {45, 4},  {43, 6},
/* t1100,  t0100,  t0110, */
   {41, 7}, {42, 7}, {43, 7},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {44, 5},{45, 5},{44, 6},{45, 6},{44, 7}, {45, 7},
},
{ /* region 11 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {40, 8}, {40, 9}, {40,10}, {40,11},
/* tpath_1, tpath_2, tpath_3, */
   {41, 8},  {42, 8},  {43, 8},
/* t1001,  t0001,  t0011, */
   {41, 9}, {42, 9}, {43, 9},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {41,10}, {42,10},  {44, 8},  {45, 8},  {43,10},
/* t1100,  t0100,  t0110, */
   {41,11}, {42,11}, {43,11},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {44, 9},{45, 9},{44,10},{45,10},{44,11}, {45,11},
},
{ /* region 12 */
/* tdot_1, tdot_2, tdot_3, tdot_4, */
   {40,12}, {40,13}, {40,14}, {40,15},
/* tpath_1, tpath_2, tpath_3, */
   {41,12},  {42,12},  {43,12},
/* t1001,  t0001,  t0011, */
   {41,13}, {42,13}, {43,13},
/* t1000,  t0000_1, t0000_2, t0000_3, t0010, */
   {41,14}, {42,14},  {44,12},  {45,12},  {43,14},
/* t1100,  t0100,  t0110, */
   {41,15}, {42,15}, {43,15},
/* tcor1, tcor2, tcor3, tcor4, tdiag1, tdiag2, */
   {44,13},{45,13},{44,14},{45,14},{44,15}, {45,15},
},
#endif
};

struct coord charcoord[numsprites][numcharframes] = {
{ /* Hansel */
/* cwest0, cwest1, cwest2, csouth0, csouth1, csouth2, */
   {0, 0},   {0, 1},   {0, 2},   {1, 0},    {1, 1},    {1, 2},
/* ceast0, ceast1, ceast2, cnorth0, cnorth1, cnorth2, */
   {2, 0},   {2, 1},   {2, 2},   {3, 0},    {3, 1},    {3, 2},
},
{ /* Gretel */
/* cwest0, cwest1, cwest2, csouth0, csouth1, csouth2, */
   {0, 3},   {0, 4},   {0, 5},   {1, 3},    {1, 4},    {1, 5},
/* ceast0, ceast1, ceast2, cnorth0, cnorth1, cnorth2, */
   {2, 3},   {2, 4},   {2, 5},   {3, 3},    {3, 4},    {3, 5},
},
};

struct coord blankcoord = {0, 0};

void load_tiles(struct gfxstruct *gfx) {
    BITMAP *spritesheet, *tilesheet;
    PALETTE pal;
    int c, d, xx, yy;

    gfx->tilesize = 16;
    gfx->h = 8;
    gfx->tileoverlap = 16;  // Tiles overlap only to the north.
    gfx->spriteoverlap = 8; // Sprites overlap in all 4 directions.

    spritesheet = load_bitmap("images\\spritesheet.tga", pal);
    tilesheet = load_bitmap("images\\tilesheet.tga", pal);
    if (!spritesheet)
        exit(80080);
    xx = blankcoord.x * gfx->tilesize;
    yy = blankcoord.y * (gfx->tilesize + gfx->tileoverlap);
    gfx->blank = create_bitmap(gfx->tilesize, gfx->tilesize + gfx->tileoverlap);
    blit(tilesheet, gfx->blank, xx, yy, 0, 0, gfx->tilesize, gfx->tilesize + gfx->tileoverlap);
    for (d = 0; d < numregiondata; d++)
        for (c = 0; c < numtiles; c++) {
            int t = d * numtiles + c;

            gfx->tiles[t] = create_bitmap(gfx->tilesize, gfx->tilesize + gfx->tileoverlap);
            if (!gfx->tiles[t])
                exit(80081 + c + 10 * d);
            xx = tilecoord[d][c].x * gfx->tilesize;
            yy = tilecoord[d][c].y * (gfx->tilesize + gfx->tileoverlap);
            blit(tilesheet, gfx->tiles[t], xx, yy, 0, 0, gfx->tilesize, gfx->tilesize + gfx->tileoverlap);
        }
    for (d = 0; d < numsprites; d++)
        for (c = 0; c < numcharframes; c++) {
            gfx->sprites[d][c] = create_bitmap(gfx->tilesize + 2 * gfx->spriteoverlap, gfx->tilesize + 2 * gfx->spriteoverlap);
            if (!gfx->sprites[c])
                exit(80081 + c);
            xx = charcoord[d][c].x * (gfx->tilesize + 2 * gfx->spriteoverlap);
            yy = charcoord[d][c].y * (gfx->tilesize + 2 * gfx->spriteoverlap);
            blit(spritesheet, gfx->sprites[d][c], xx, yy, 0, 0, gfx->tilesize + 2 * gfx->spriteoverlap, gfx->tilesize + 2 * gfx->spriteoverlap);
        }
    destroy_bitmap(spritesheet);
    destroy_bitmap(tilesheet);
}

void unload_tiles(struct gfxstruct *gfx) {
    int c, d;

    destroy_bitmap(gfx->blank);

    for (d = 0; d < numregiondata; d++)
        for (c = 0; c < numtiles; c++)
            destroy_bitmap(gfx->tiles[d * numtiles + c]);
    for (d = 0; d < numsprites; d++)
        for (c = 0; c < numcharframes; c++)
            destroy_bitmap(gfx->sprites[d][c]);
}
