#include "draw.h"

#define SQR(x) (x)*(x)

void tilezoomview(struct gamestruct *game, int xx, int yy) {
    int x = xx * game->gfx.tilesize;
    int y = yy * game->gfx.tilesize - game->gfx.tileoverlap;
    int t = game->map.tiles[xx][yy];
    draw_sprite(game->gfx.zoomview, game->gfx.tiles[t], x, y);
}

void initdraw (struct gamestruct *game) {
    int xx, yy;

    game->gfx.zmap = create_bitmap(SCREEN_W, SCREEN_H);
    game->gfx.mapview = create_bitmap(XBIG, YBIG);
    game->gfx.zoomview = create_bitmap(XBIG * game->gfx.tilesize, YBIG * game->gfx.tilesize);
    for (xx = 0; xx < XBIG; xx++)
        for (yy = 0; yy < YBIG; yy++) {
            tilezoomview(game, xx, yy);
        }
    game->gfx.mapzoom = trunc(SCREEN_W / XBIG);
    if (game->gfx.mapzoom < trunc(SCREEN_H / YBIG))
        game->gfx.mapzoom = trunc(SCREEN_H / YBIG);
}

void deinitdraw (struct gamestruct *game) {
    destroy_bitmap(game->gfx.zoomview);
    destroy_bitmap(game->gfx.mapview);
    destroy_bitmap(game->gfx.zmap);
}

void clearsprites(struct gamestruct *game) {
    /* Ah, good ol' dirty rectagle... cause the zoom map be so big */
    int a, xx, yy;

    for (a = 0; a < MAXACTORS; a++) {
        if (!game->stage.actors[a].active)
            continue;
        for (xx = -1; xx <= 1; xx++)
            for (yy = -1; yy <= 0; yy++)
                tilezoomview(game, game->stage.actors[a].tx + xx, game->stage.actors[a].ty + yy);
    }
}

void drawsprites(struct gamestruct *game) {
    int a;

    for (a = 0; a < MAXACTORS; a++) {
        if (game->stage.actors[a].active <= 0)
            continue;
        draw_sprite(game->gfx.zoomview, game->gfx.sprites[game->stage.actors[a].sprite][game->stage.actors[a].frame],
                    game->stage.actors[a].x - game->gfx.h - game->gfx.spriteoverlap,
                    game->stage.actors[a].y - game->gfx.tilesize - 2 * game->gfx.spriteoverlap);
    }
}

void draw(BITMAP *scrbuf, struct gamestruct *game) {
    int xx, yy, offx, offy;
    int orgx, orgy, orgtx, orgty;
    int p;

tilesdebugoutput(game->gfx.tiles, 0,0,1,"");
    clear_to_color(game->gfx.mapview, makecol(128, 0, 128));
    clear_to_color(scrbuf, makecol(0, 0, 0));
    clear_to_color(game->gfx.zmap, 0); /* this is an abuse of the BITMAP*, and I don't care */
    orgx = (XSCREEN - (XBIG * game->gfx.mapzoom)) / 2;
    orgy = (YSCREEN - (YBIG * game->gfx.mapzoom)) / 2;
    for (yy = 0; yy < YBIG; yy++) {
        for (xx = 0; xx < XBIG; xx++) {
            int t = game->map.tiles[xx][yy];
            if (game->stage.dots[xx][yy]) {
                int r = game->map.regs[toxreg(xx)][toyreg(yy)];
                int d = r * numtiles + (tdot_1 + (xx + yy + game->stage.step) % 4); /* animate the dot */
                putpixel(game->gfx.mapview, xx, yy, getpixel(game->gfx.tiles[d], game->gfx.h, game->gfx.tileoverlap));
            } else if (game->stage.actorcollision[xx][yy] >= 0) {
                int s = game->stage.actors[game->stage.actorcollision[xx][yy]].sprite;
                int f = game->stage.actors[game->stage.actorcollision[xx][yy]].frame;
                putpixel(game->gfx.mapview, xx, yy, getpixel(game->gfx.sprites[s][f], game->gfx.h + game->gfx.spriteoverlap, game->gfx.h + game->gfx.spriteoverlap));
            } else {
                putpixel(game->gfx.mapview, xx, yy, getpixel(game->gfx.tiles[t], game->gfx.h, game->gfx.h + game->gfx.tileoverlap));
            }
qdebug(0,0,0,"");
        }
    }
    stretch_blit (game->gfx.mapview, scrbuf, 0, 0, XBIG, YBIG, orgx, orgy, XBIG * game->gfx.mapzoom, YBIG * game->gfx.mapzoom);
consolemsg("%d, h = %d", game->gfx.mapzoom, game->gfx.h);
qdebug(0,0,1,"");
    drawsprites(game);
consolemsg("sprites drawn");
    offx = (game->stage.actors[0].x) % (game->gfx.tilesize * game->gfx.mapzoom);
    offy = (game->stage.actors[0].y) % (game->gfx.tilesize * game->gfx.mapzoom);
    orgtx = game->stage.actors[0].tx - playerdata[0].tilerange;
    orgty = game->stage.actors[0].ty - playerdata[0].tilerange;
    for (p = 0; p < numplayers; p++) {
        int cx, cy;
        int pi = game->stage.playerindex[p];
        if (pi < 0 || !game->stage.actors[pi].active)
            continue;

        cx = game->stage.actors[pi].x;
        cy = game->stage.actors[pi].y - game->gfx.h - game->gfx.spriteoverlap;
        for (xx = -game->gfx.mapzoom * playerdata[p].tilerange; xx < game->gfx.mapzoom * playerdata[p].tilerange; xx++)
            for (yy = -game->gfx.mapzoom * playerdata[p].tilerange; yy < game->gfx.mapzoom * playerdata[p].tilerange; yy++) {
                double d;
                int zoom;
                int sx, sy, dx, dy;
                int d2 = SQR(xx) + SQR(yy);
                if (d2 > SQR(game->gfx.mapzoom * playerdata[p].tilerange))
                    continue;

                d = sqrt(d2);
                zoom = (int)d;
                dx = (xx * zoom);
                dy = (yy * zoom);
                sx = orgx + xx + game->stage.actors[pi].x * game->gfx.mapzoom / game->gfx.tilesize;
                sy = orgy + yy + game->stage.actors[pi].y * game->gfx.mapzoom / game->gfx.tilesize - game->gfx.spriteoverlap - game->gfx.h;
                if (getpixel(game->gfx.zmap, sx, sy) <= (int)(zoom * 100.0)) {
                    putpixel(game->gfx.zmap, sx, sy, (int)(zoom * 100.0));
                    putpixel(scrbuf, sx, sy, getpixel(game->gfx.zoomview, cx + dx, cy + dy));
                }
            }
    }
    clearsprites(game);
consolemsg("Sprites cleared");
}
