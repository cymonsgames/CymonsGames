#include "gfxwrapper.h"
#include "tricule.h"
#include "mapgen.h"
#include "tiles.h"
#include "draw.h"
#include "game.h"

extern BITMAP *scrbuf;
BITMAP *loadingscreen;

int mainmenu(void *ignore) {
    return 0;
}

int pausemenu(void *ignore) {
    return 0;
}

int nextactor(struct logicstruct *stage) {
    int c;

    if (stage->lastactor < MAXACTORS) {
        stage->lastactor++;
    } else {
        stage->lastactor = 0;
        for (c = 0; c < MAXACTORS; c++) {
            if (stage->actors[c].active) {
                if (c > stage->lastactor) {
                    stage->actors[stage->lastactor] = stage->actors[c];
qdebug(0, 0, 0,"alloc new actor");
                    memset(&stage->actors[c], 0, 1);
                }
                stage->lastactor++;
            }
        }
    }
    return stage->lastactor - 1;
}

int newactor(struct gamestruct *game, int sprite, int xx, int yy) {
    int c = nextactor(&(game->stage));
    int h = game->gfx.tilesize / 2;
    game->stage.actorcollision[xx][yy] = c;
    game->stage.actors[c].x = game->gfx.tilesize * xx + h;
    game->stage.actors[c].y = game->gfx.tilesize * yy + h;
    game->stage.actors[c].tx = xx;
    game->stage.actors[c].ty = yy;
    game->stage.actors[c].rx = toxreg(xx);
    game->stage.actors[c].ry = toyreg(yy);
    game->stage.actors[c].dir = SOUTH;
    game->stage.actors[c].sprite = sprite;
    game->stage.actors[c].frame = fsouth0;
    game->stage.actors[c].active = 1;

    return c;
}

void freeactor(struct logicstruct *stage, int n) {
    stage->actorcollision[stage->actors[n].x][stage->actors[n].x] = -1;
    memset(&stage->actors[n], 0, 1);
}

void initgame(struct gamestruct *game) {
    int xx, yy, c;

    memset(game->stage.dots, 0, sizeof *game->stage.dots);
    for (yy = 0; yy < YBIG; yy++)
        for (xx = 0; xx < XBIG; xx++)
            if ((game->map.map[xx][yy] == 1) && ((xx + yy) % 2))
                game->stage.dots[xx][yy] = 1;
    memset(game->stage.actorcollision, -1, sizeof *game->stage.actorcollision);
    memset(game->stage.actors, 0, sizeof *game->stage.actors);
    game->stage.lastactor = 0;
    c = newactor(game, shansel, XBIG / 2, YBIG / 2);
    game->stage.actors[c].isplayer = 1;
    game->stage.playerindex[0] = c;
    c = newactor(game, sgretel, 3 * XBIG / 4, 3 * YBIG / 4);
    game->stage.actors[c].isplayer = 1;
    game->stage.actors[c].active = 0;
    game->stage.playerindex[1] = c;
}

void moveactor(struct gamestruct *g, int a, int dir) {
    int olddir = g->stage.actors[a].dir;
    /* [ ] collision detection */

    g->stage.actorcollision[g->stage.actors[a].tx][g->stage.actors[a].ty] = -1;
    g->stage.actors[a].dir = dir;
    g->stage.actors[a].x += 2 * DX(dir);
    g->stage.actors[a].y += 2 * DY(dir);
    g->stage.actors[a].tx = g->stage.actors[a].x / g->gfx.tilesize;
    g->stage.actors[a].ty = (g->stage.actors[a].y) / g->gfx.tilesize;
    g->stage.actors[a].rx = toxreg(g->stage.actors[a].tx);
    g->stage.actors[a].rx = toyreg(g->stage.actors[a].ty);
    if ((dir != olddir) || !(g->stage.step % 10)) {
        g->stage.actors[a].frame %= 3;
        if (g->stage.actors[a].frame == feast1)
            g->stage.actors[a].frame = feast2;
        else g->stage.actors[a].frame = feast1;
        g->stage.actors[a].frame += dir * 3;
    }
    g->stage.actorcollision[g->stage.actors[a].tx][g->stage.actors[a].ty] = a;
}

int mainloop() {
    int running = 1;
    int x, y;
    struct gamestruct game;

    PALETTE pal;
    loadingscreen = load_bitmap("images\\loading.tga", pal);
    draw_sprite(scrbuf, loadingscreen, 0, 0);
    refresh(scrbuf);
    showconsole();
    consolemsg("Loading Tiles");
    load_tiles(&game.gfx);
    consolemsg("Generating Map");
    generatemap(&game.map);
    x = 0;
    y = 0;
    consolemsg("Init Game");
    initgame(&game);
    consolemsg("Init Draw");
    initdraw(&game);
    //hideconsole();
    begintimer();
    consolemsg("Beginning Game");
    while (running) {
        if (key[KEY_UP])
            moveactor(&game, 0, NORTH);
        if (key[KEY_DOWN])
            moveactor(&game, 0, SOUTH);
        if (key[KEY_LEFT])
            moveactor(&game, 0, WEST);
        if (key[KEY_RIGHT])
            moveactor(&game, 0, EAST);
        if (key[KEY_Q])
            running = 0;
        if (key[KEY_ESC])
            running = 0;
        game.stage.step++;

        draw(scrbuf, &game);

        refresh(scrbuf);
        if (running)
            running = wait();
    }
    unload_tiles(&game.gfx);
    deinitdraw(&game);
    return 1;
}
