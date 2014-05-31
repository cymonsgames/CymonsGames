#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "tricule.h"

void initdraw (struct gamestruct *game);
void deinitdraw (struct gamestruct *game);
void clearsprites(struct gamestruct *game);
void drawsprites(struct gamestruct *game);
void draw(BITMAP *scrbuf, struct gamestruct *game);

#endif // DRAW_H_INCLUDED
