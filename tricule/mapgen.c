#include "mapgen.h"

int regioncolor(int c) { /* for debugging purposes */
    int colors[numregiondata] = {
        makecol(0,0,0),
        makecol(128,128,128),
        makecol(255,000,000),
        makecol(255,128,000),
        makecol(128,128,000),
        makecol(000,255,000),
        makecol(000,255,255),
        makecol(000,000,255),
        makecol(128,000,255),
    };

    return colors[c];
}

void bigmapdebugoutput (int m[XBIG][YBIG], int r[XREG][YREG], int xx, int yy, int pause, char *fmt, ...) {
    BITMAP *debug;
    int x, y;
	char buf[4096];

	va_list ap;
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

    debug = create_bitmap(XBIG, YBIG);
    clear_to_color(debug, makecol(0, 0, 0));

    for (x = 0; x < XBIG; x++)
        for (y = 0; y < YBIG; y++) {
            if (m[x][y] == 0)
                putpixel(debug, x, y, regioncolor(r[x/(XBIG/XREG)][y/(YBIG/YREG)]));
            else if (m[x][y] <= 5)
                putpixel(debug, x, y, makecol(255,255,255));
            else
                putpixel(debug, x, y, makecol(m[x][y], 255, 255));
        }
    acquire_screen();
    stretch_blit(debug, screen, 0, 0, XBIG, YBIG, XMINI, YMINI, XBIG * 2, YBIG * 2);
    textprintf_ex(screen, font, xx, yy, makecol(255, 100, 200), -1, "%s", buf);
    release_screen();
    if (pause)
        if (readkey()>>8 ==KEY_ESC) exit(1);
    if(key[KEY_ESC]) exit(1);
}

void minimapdebugoutput (int m[XMINI][YMINI], int xx, int yy, int pause, char *fmt, ...) {
    BITMAP *debug;
    int x, y;
	char buf[4096];

	va_list ap;
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

    debug = create_bitmap(XMINI, YMINI);
    clear_to_color(debug, makecol(0, 0, 0));

    for (x = 0; x < XMINI; x++)
        for (y = 0; y < YMINI; y++) {
            if (!m[x][y])
                putpixel(debug, x, y, makecol(0,0,0));
            else if (m[x][y] == 1)
                putpixel(debug, x, y, makecol(255,255,255));
            else
                putpixel(debug, x, y, makecol(m[x][y] % 255,255,255));
        }
    acquire_screen();
    stretch_blit(debug, screen, 0, 0, XMINI, YMINI, 0, 0, 800, 600);
    textprintf_ex(screen, font, xx, yy, makecol(255, 100, 200), -1, "%s", buf);
    release_screen();
    if (pause)
        if (readkey()>>8 ==KEY_ESC) exit(1);
    if(key[KEY_ESC]) exit(1);
}

void regsdebugoutput (int m[XREG][YREG], int xx, int yy, int pause, char *fmt, ...) {
    BITMAP *debug;
    int x, y;
	char buf[4096];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

    debug = create_bitmap(XREG, YREG);
    clear_to_color(debug, makecol(0, 0, 0));

    for (x = 0; x < XREG; x++)
        for (y = 0; y < YREG; y++) {
            if (!m[x][y])
                putpixel(debug, x, y, makecol(0,0,0));
            else if (m[x][y] < numregiondata)
                putpixel(debug, x, y, regioncolor(m[x][y]));
            else
                putpixel(debug, x, y, makecol(m[x][y] % 255,255,255));
        }
    acquire_screen();
    stretch_blit(debug, screen, 0, 0, XREG, YREG, 0, 0, 800, 600);
    textprintf_ex(screen, font, xx, yy, makecol(255, 100, 200), -1, "%s", buf);
    release_screen();
    if (pause)
        if (readkey()>>8 ==KEY_ESC) exit(1);
    if(key[KEY_ESC]) exit(1);
}

void tilesdebugoutput (int t[XBIG][YBIG], int xx, int yy, int pause, char *fmt, ...) {
    BITMAP *debug;
    int x, y;
	char buf[4096];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

    debug = create_bitmap(XBIG, YBIG);
    clear_to_color(debug, makecol(0, 0, 0));

    for (x = 0; x < XBIG; x++)
        for (y = 0; y < YBIG; y++) {
            putpixel(debug, x, y, makecol(t[x][y] * 4, t[x][y] * 4, t[x][y] * 4));
        }
    acquire_screen();
    stretch_blit(debug, screen, 0, 0, XBIG, YBIG, 0, 0, 800, 600);
    textprintf_ex(screen, font, xx, yy, makecol(255, 100, 200), -1, "%s", buf);
    release_screen();
    if (pause)
        if (readkey()>>8 ==KEY_ESC) exit(1);
    if(key[KEY_ESC]) exit(1);
}

void carvemap (int m[XMINI][YMINI], int xx, int yy, int reflect, int val) {
    m[xx][yy] = val;
    if (reflect & HOR) {
        int ny = YMINI - yy - 1;
        m[xx][ny] = val;
    }
    if (reflect & VERT) {
        int nx = XMINI - xx - 1;
        m[nx][yy] = val;
    }
    if ((reflect & VERT) && (reflect & HOR)) {
        int nx = XMINI - xx - 1;
        int ny = YMINI - yy - 1;
        m[nx][ny] = val;
    }
}

int walkpath(int m[XMINI][YMINI], int xx, int yy, int dist, int dir, struct regions *rd) {
    /* recursion HO! */
    int check = 0;

    xx += DX(dir);
    yy += DY(dir);

    if ((xx < 2) || (xx >= XMINI - 2) || (yy < 2) || (yy >= YMINI - 2))
        return 0;

    carvemap(m, xx, yy, rd->reflect, 1);

    if (dist > 0) {
        if ((xx + DY(dir) >= 0) && (xx + DY(dir) < XMINI)
          && (yy + DX(dir) >= 0) && (yy + DX(dir) < YMINI))
            if (m[xx + DY(dir)][yy + DX(dir)]) {
                carvemap(m, xx, yy, rd->reflect, 0);
                return 0;
            }
        if ((xx + 2 * DY(dir) >= 0) && (xx + 2 * DY(dir) < XMINI)
          && (yy + 2 * DX(dir) >= 0) && (yy + 2 * DX(dir) < YMINI))
            if (m[xx + 2 * DY(dir)][yy + 2 * DX(dir)]) {
                carvemap(m, xx, yy, rd->reflect, 0);
                return 0;
            }
        if ((xx - DY(dir) >= 0) && (xx - DY(dir) < XMINI)
          && (yy - DX(dir) >= 0) && (yy - DX(dir) < YMINI))
            if (m[xx - DY(dir)][yy - DX(dir)]) {
                carvemap(m, xx, yy, rd->reflect, 0);
                return 0;
            }
        if ((xx - 2 * DY(dir) >= 0) && (xx - 2 * DY(dir) < XMINI)
          && (yy - 2 * DX(dir) >= 0) && (yy - 2 * DX(dir) < YMINI))
            if (m[xx - 2 * DY(dir)][yy - 2 * DX(dir)]) {
                carvemap(m, xx, yy, rd->reflect, 0);
                return 0;
            }
    }

    if ((xx + DX(dir) < 2) || (xx + DX(dir) >= XMINI - 2) ||
        (yy + DY(dir) < 2) || (yy + DY(dir) >= YMINI - 2))
        dist = 0;

    if (dist <= 0) { /* choose a new heading */
        int dirarr[MAXCOORD];
        int c;

        for (c = 0; c < MAXCOORD; c++) { /* fill */
            if (m[xx + DX(c)][yy + DY(c)])
                dirarr[c] = -1;
            else
                dirarr[c] = c;
        }
        for (c = 0; c < MAXCOORD; c++) { /* shuffle */
            int r = randint(0, MAXCOORD - 1);
            int temp = dirarr[r];
            dirarr[r] = dirarr[c];
            dirarr[c] = temp;
        }
        dist = randint(rd->minlen, rd->maxlen);
        for (c = 0; c < MAXCOORD; c++) { /* try */
            if (dirarr[c] < 0)
                continue;
            check = walkpath(m, xx, yy, dist, dirarr[c], rd);
            if (check < 1)
                continue;
            return 1;
        }
        carvemap(m, xx, yy, rd->reflect, 0);
        return 0;
    }
    if (m[xx + DX(dir)][yy + DY(dir)]) {
        return 1;
    }
    check = walkpath (m, xx, yy, dist - 1, dir, rd);
    if (check <= 0) {
        carvemap(m, xx, yy, rd->reflect, 0);
        return 0;
    }
    return 1;
}

int finddeadend(int m[XMINI][YMINI]) {
    int dir, xx, yy, found = 0;

    for (xx = 1; xx < XMINI - 1; xx++)
        for (yy = 1; yy < YMINI - 1; yy++) {

            if (!m[xx][yy]) {
                int deadend = 0;
                int c;

                for (c = 0; c < 9; c++) {
                    if (m[xx + (c % 3) - 1][yy + (c / 3) - 1])
                        deadend++;
                }
                if (deadend == 7) {
                    carvemap(m, xx, yy, 0, 1);
                    found = 1;
                    continue;
                }
                if (((m[xx + DX(EAST)][yy] && m[xx + DX(WEST)][yy])) ||
                  ((m[xx][yy + DY(NORTH)] && m[xx][yy + DY(SOUTH)]))) {
                      carvemap(m, xx, yy, 0, 1);
                    found = 1;
                }
            } else {
                int surrounded = 0;

                for (dir = 0; dir < MAXCOORD; dir++) {
                    if (m[xx + DX(dir)][yy + DY(dir)])
                        surrounded++;
                }
                if (surrounded <= 1) {
                    carvemap(m, xx, yy, 0, 0);
                    found = 1;
                }
            }
        }

    return found;
}

void pathfill(int a[XMINI][YMINI], int b[XMINI][YMINI], int xx, int yy) {
    int d;

    a[xx][yy] = b[xx][yy];
    for (d = 0; d < MAXCOORD; d++)
        if ((xx + DX(d) >= 0) && (xx + DX(d) < XMINI) &&
          (yy + DY(d) >= 0) && (yy + DY(d) < YMINI) &&
          !a[xx + DX(d)][yy + DY(d)] && b[xx + DX(d)][yy + DY(d)]) {
            pathfill(a, b, xx + DX(d), yy + DY(d));
        }
}

int checkclosure(int m[XMINI][YMINI]) {
    int x, y, xx, yy, check[XMINI][YMINI];

    xx = yy = 0;
    //memset (check, 0, sizeof *check * XMINI * YMINI);
    for (x = 0; x < XMINI; x++)
        for (y = 0; y < YMINI; y++) {
            if (m[x][y]) {
                xx = x;
                yy = y;
            }
            check[x][y] = 0;
        }
    x = xx;
    y = yy;
    pathfill(check, m, xx, yy);
    for (xx = 0; xx < XMINI; xx++)
        for (yy = 0; yy < YMINI; yy++)
            if (m[xx][yy] != check[xx][yy]) {
                return 0;
            }
    return 1;
}

int verifymap(int m[XMINI][YMINI]) {
    int xx, yy;
    int edge[4] = {0, 0, 0, 0};
    int area = 0;
    int closed;

    while (finddeadend(m)) ;
    closed = checkclosure(m);

    //memset(edge, 0, sizeof *edge * 4);

    for (xx = 0; xx < XMINI; xx++)
        for (yy = 0; yy < YMINI; yy++) {
            if (m[xx][yy]) {
                area++;
                if (xx <= 3)
                    edge[0] = 1;
                if (xx >= XMINI - 3)
                    edge[1] = 1;
                if (yy <= 3)
                    edge[2] = 1;
                if (yy >= YMINI - 3)
                    edge[3] = 1;
            }
        }

//    if (!(closed && (area > (XMINI * YMINI / 6)) && edge[0] && edge[1] && edge[2] && edge[3])) {
//        consolemsg("  o verify failed: closed = %d, area = %d/%d, edge = {%d,%d,%d,%d}", closed, area, XMINI * YMINI / 6, edge[0], edge[1], edge[2], edge[3]);
//    }
    return (closed && (area > (XMINI * YMINI / 6)) && edge[0] && edge[1] && edge[2] && edge[3]);
}

void generateminimap(int m[XMINI][YMINI], struct regions *rd) {
    int xx, yy;
    int running = rd->numpaths;

    // memset(m, 0, sizeof *m * XMINI * YMINI);
    for (xx = 0; xx < XMINI; xx++)
        for (yy = 0; yy < YMINI; yy++)
            m[xx][yy] = 0;
    do {
        xx = randint((XMINI / 4), (XMINI / 4) * 3);
        yy = randint((YMINI / 4), (YMINI / 4) * 3);
    } while (walkpath(m, xx, yy, 0, randint(0, MAXCOORD), rd) <= 0);
    while (running) {
        int i, n = 0, hallarr[XMINI * YMINI];

        for (xx = 0; xx < XMINI; xx++) /* fill */
            for (yy = 0; yy < YMINI; yy++)
                if (m[xx][yy] == 1) {
                    hallarr[n] = xx + yy * XMINI;
                    n++;
                }
        for (i = 0; i < n; i++) { /* shuffle */
            int temp;
            int  r = randint (0, n - 1);

            temp = hallarr[r];
            hallarr[r] = hallarr[i];
            hallarr[i] = temp;
        }
        for (i = 0; i < n; i++) { /* try */
            xx = hallarr[i] % XMINI;
            yy = hallarr[i] / XMINI;
            int check;
            int dir = 0;

            while ((dir < MAXCOORD) && (m[xx + DX(dir)][yy + DY(dir)]))
                dir++;
            if (dir == MAXCOORD)
                continue;

            check = walkpath(m, xx, yy, 0, dir, rd);
            if (check == 1) {
                break;
            }
        }
        running--;
        if (i == n)
            running = 0;
    }
//    minimapdebugoutput(m, 0, 0, 0, "%d",__LINE__);
}

void generateregions (int regs[XREG][YREG]) {
    int xx, yy, c, zeros;
    int chosenregs[numregiondata + 1];
    int area[numregiondata];
    int minarea, mintype;

    memset(area, 0, sizeof *area * numregiondata);
    memset(regs, 0, sizeof *regs * (XREG * YREG));
    memset(chosenregs, 0, sizeof *chosenregs * (numregiondata + 1));
    for (c = 0; c < numregions; c++) {
        int type, xx, yy, theta;

        if (c == 0)
            type = 1;
        else do {
            type = randint(1, numregiondata);
        } while (chosenregs[type]);
        chosenregs[type] = 1;
        theta = c * 2 * M_PI / numregions;
        xx = (XREG / 2) + (XREG / 3) * sin(theta);
        yy = (YREG / 2) + (YREG / 3) * cos(theta);
        regs[xx][yy] = type;
        area[type]++;
    }
    zeros = XREG * YREG - numregions;
    while (zeros) {
        struct coord coords[XREG * YREG];

        //regsdebugoutput(regs, 0, 0, 0, "Zeros = %d", zeros);
        zeros = 0;
        memset(coords, -1, sizeof *coords * XREG * YREG);
        for (xx = 0; xx < XREG; xx++)
            for (yy = 0; yy < YREG; yy++) /* fill with the zero regions */
                if (!regs[xx][yy]) {
                    coords[xx + yy * XREG].x = xx;
                    coords[xx + yy * XREG].y = yy;
                }
        for (c = 0; c < XREG * YREG; c++) { /* shuffle */
            int r = randint(0, XREG * YREG - 1);
            struct coord temp;
            temp.x = coords[c].x;
            temp.y = coords[c].y;
            coords[c].x = coords[r].x;
            coords[c].y = coords[r].y;
            coords[r].x = temp.x;
            coords[r].y = temp.y;
        }
        for (c = 0; c < XREG * YREG; c++) { /* try */
            minarea = XREG*YREG;
            mintype = 0;
            int d, nbors[MAXCOORD];

            if (coords[c].x == -1)
                continue;
            xx = coords[c].x;
            yy = coords[c].y;
            for (d = 0; d < MAXCOORD; d++) {
                if ((xx + DX(d) >= 0) && (xx + DX(d) < XREG)
                    && (yy + DY(d) >= 0) && (yy + DY(d) < YREG)) {
                    nbors[d] = regs[xx + DX(d)][yy + DY(d)];
                    if (nbors[d]) {
                        if (area[nbors[d]] == minarea) {
                            mintype = (randint(0, 1))? mintype : nbors[d];
                        }
                        if (area[nbors[d]] < minarea) {
                            minarea = area[nbors[d]];
                            mintype = nbors[d];
                        }
                    }
                } else
                    nbors[d] = 0;
            }
            for (d = 0; (d < MAXCOORD) && (!nbors[d]); d++) ;
            if (d == MAXCOORD) {
                zeros++;
                continue;
            }
            regs[xx][yy] = mintype;
            area[mintype]++;
        }
    }
}

int isclear(int map[XBIG][YBIG], int xx, int yy, int dir) {
    if ((xx + DY(dir) >= 0) && (xx + DY(dir) < XBIG)
      && (yy + DX(dir) >= 0) && (yy + DX(dir) < YBIG))
        if (map[xx + DY(dir)][yy + DX(dir)])
            return 0;
    if ((xx + 2 * DY(dir) >= 0) && (xx + 2 * DY(dir) < XBIG)
      && (yy + 2 * DX(dir) >= 0) && (yy + 2 * DX(dir) < YBIG))
        if (map[xx + 2 * DY(dir)][yy + 2 * DX(dir)])
            return 0;
    if ((xx - DY(dir) >= 0) && (xx - DY(dir) < XBIG)
      && (yy - DX(dir) >= 0) && (yy - DX(dir) < YBIG))
        if (map[xx - DY(dir)][yy - DX(dir)])
            return 0;
    if ((xx - 2 * DY(dir) >= 0) && (xx - 2 * DY(dir) < XBIG)
      && (yy - 2 * DX(dir) >= 0) && (yy - 2 * DX(dir) < YBIG))
        if (map[xx - 2 * DY(dir)][yy - 2 * DX(dir)])
            return 0;
    return 1;
}

int runconnect(int map[XBIG][YBIG], int sx, int sy, int d, int v) {
    int x, y;

    x = sx;
    y = sy;
    while (!map[x][y]) { /* find the start */
        x -= DX(d);
        y -= DY(d);
    }
    x += DX(d);
    y += DY(d);
    while (!map[x][y]) { /* scrub to end checking if clear */
        if (!isclear(map, x, y, d))
            return 0;
        x += DX(d);
        y += DY(d);
    }
    x -= DX(d);
    y -= DY(d);
    while (!map[x][y]) { /* and scrub back filling in the path */
        map[x][y] = v;
        x -= DX(d);
        y -= DY(d);
    }
    return 1;
}

void connectregion(int m[XBIG][YBIG], int r[XREG][YREG], int rx, int ry, int d, int b) {
    int i, n, x, y, check = 0;

    n = 5;
    if (DX(d)) {
        if (regiondata[r[rx][ry]].reflect & HOR)
            n = 2;
        for (i = 0; i < n; i++) {
            x = rx * XMINI;
            if (DX(d) > 0)
                x += XMINI;
            y = 5 + (randint(0, YMINI - 10));
            check = runconnect(m, x, ry * YMINI + y, d, b);
            if (regiondata[r[rx][ry]].reflect & VERT) {
                y = YMINI - y - 1;
                check |= runconnect(m, x, ry * YMINI + y, d, b);
            }
            if (!check)
                i--;
        }
    } else {
        if (regiondata[r[rx][ry]].reflect & VERT)
            n = 2;
        for (i = 0; i < n; i++) {
            y = ry * YMINI;
            if (DY(d) > 0)
                y += YMINI;
            x = 5 + (randint(0, XMINI - 10));
            check = runconnect(m, rx * XMINI + x, y, d, b);
            if (regiondata[r[rx][ry]].reflect & VERT) {
                x = XMINI - x - 1;
                check |= runconnect(m, rx * XMINI + x, y, d, b);
            }
            if (!check)
                i--;
        }
    }
}

void connectregions(int map[XBIG][YBIG], int r[XREG][YREG]) {
    int xx, yy, c;
    struct coord coords[XREG * YREG];
    int visited[XREG][YREG];
    int connected[numregiondata][numregiondata];

    consolemsg("- Connecting Regions");
    //memset(connected, 0, sizeof *connected * numregiondata * numregiondata);
    for (xx = 0; xx < numregiondata; xx++)
        for (yy = 0; yy < numregiondata; yy++)
          connected[xx][yy] = 0;
    //memset(visited, 0, sizeof *visited);// * XREG * YREG);
    for (xx = 0; xx < XREG; xx++)
        for (yy = 0; yy < YREG; yy++)
          visited[xx][yy] = 0;
    for (xx = 0; xx < XREG; xx++) {
        for (yy = 0; yy < YREG; yy++) { /* fill */
            coords[xx + yy * XREG].x = xx;
            coords[xx + yy * XREG].y = yy;
        }
    }
    for (c = 0; c < XREG * YREG; c++) { /* shuffle */
        int r = randint(0, XREG * YREG - 1);
        struct coord temp;
        temp.x = coords[c].x;
        temp.y = coords[c].y;
        coords[c].x = coords[r].x;
        coords[c].y = coords[r].y;
        coords[r].x = temp.x;
        coords[r].y = temp.y;
    }
    for (c = 0; c < XREG * YREG; c++) { /* try */
        int d;
        xx = coords[c].x;
        yy = coords[c].y;
        for (d = 0; d < MAXCOORD; d++) {
            if ((xx + DX(d) < 0) || (xx + DX(d) >= XREG) ||
                (yy + DY(d) < 0) || (yy + DY(d) >= YREG))
                continue;
            if (visited[xx + DX(d)][yy + DY(d)])
                continue;
            if ((r[xx][yy] == r[xx + DX(d)][yy + DY(d)]) ||
                (!connected[r[xx][yy]][r[xx + DX(d)][yy + DY(d)]])) {
                    /* connect 'em */
                    int bridge = 2;

                    if (!connected[r[xx][yy]][r[xx + DX(d)][yy + DY(d)]])
                        bridge = 3;
                    connectregion(map, r, xx, yy, d, bridge);
                    connected[r[xx][yy]][r[xx + DX(d)][yy + DY(d)]]++;
                }
        }
        visited[xx][yy] = 1;
    }
}

void calculatetile(int t[XBIG][YBIG], int m[XBIG][YBIG], int reg, int x, int y) {
    int c;
    int mask = 0;

    for (c = 0; c < 9; c++) {
        int xx = x + (c % 3) - 1;
        int yy = y + (c / 3) - 1;

        if ((xx < 0) || (xx >= XBIG) || (yy < 0) || (yy >= YBIG))
            continue;
        if (m[xx][yy])
            mask += 1 << c;
    }
    switch (mask) {
        case 0x004F: /* 111100100 */
        case 0x004B: /* 110100100 */
        case 0x000F: /* 111100000 */
        case 0x000B: /* 110100000 */
        case 0x004E: /* 011100100 */
        case 0x0046: /* 011000100 */
        case 0x004C: /* 001100100 */
            t[x][y] = t1001;
            break;
        case 0x0007: /* 111000000 */
        case 0x0006: /* 011000000 */
        case 0x0003: /* 110000000 */
        case 0x0002: /* 010000000 */
            t[x][y] = t0001;
            break;
        case 0x0127: /* 111001001 */
        case 0x0126: /* 011001001 */
        case 0x0027: /* 111001000 */
        case 0x0026: /* 011001000 */
        case 0x0121: /* 100001001 */
        case 0x0123: /* 110001001 */
        case 0x0103: /* 110000001 */
            t[x][y] = t0011;
            break;
        case 0x0049: /* 100100100 */
        case 0x0048: /* 000100100 */
        case 0x0009: /* 100100000 */
        case 0x0008: /* 000100000 */
            t[x][y] = t1000;
            break;
        case 0x0124: /* 001001001 */
        case 0x0024: /* 001001000 */
        case 0x0120: /* 000001001 */
        case 0x0020: /* 000001000 */
            t[x][y] = t0010;
            break;
        case 0x01C9: /* 100100111 */
        case 0x01C8: /* 000100111 */
        case 0x00C9: /* 100100110 */
        case 0x00C8: /* 000100110 */
        case 0x0901: /* 100100001 */
        case 0x0189: /* 100100011 */
        case 0x0181: /* 100000011 */
        case 0x0109: /* 100100001 */
            t[x][y] = t1100;
            break;
        case 0x01C0: /* 000000111 */
        case 0x0180: /* 000000011 */
        case 0x00C0: /* 000000110 */
        case 0x0080: /* 000000010 */
            t[x][y] = t0100;
            break;
        case 0x01E4: /* 001001111 */
        case 0x01E0: /* 000001111 */
        case 0x01A4: /* 001001011 */
        case 0x01A0: /* 000001011 */
        case 0x0064: /* 001001100 */
        case 0x00E4: /* 001001110 */
        case 0x00C4: /* 001000110 */
            t[x][y] = t0110;
            break;
        case 0x0001: /* 100000000 */
            t[x][y] = tcor1;
            break;
        case 0x0004: /* 001000000 */
            t[x][y] = tcor2;
            break;
        case 0x0040: /* 000000100 */
            t[x][y] = tcor3;
            break;
        case 0x0100: /* 000000001 */
            t[x][y] = tcor4;
            break;
        case 0x0101: /* 100000001 */
            t[x][y] = tdiag1;
            break;
        case 0x0044: /* 001000100 */
            t[x][y] = tdiag2;
            break;
        default:
            if (m[x][y])
                t[x][y] = tpath_1 + rand() % 3;
            else
                t[x][y] = t0000_1 + rand() % 3;
    }
    t[x][y] += reg * numtiles;
}

void calculatealltiles(struct mapstruct *m) {
    int x, y;

    consolemsg("- Tile Calculation");
    for (x = 0; x < XBIG; x++)
        for (y = 0; y < YBIG; y++) {
            calculatetile(m->tiles, m->map, m->regs[toxreg(x)][toyreg(y)], x, y);
        }
}

void generatemap(struct mapstruct *m) {
    int rx, ry;

    consolemsg("- Region Generation");
    generateregions(m->regs);

    consolemsg("- Sub-Map Generations");
    for (rx = 0; rx < XREG; rx++) {
        for (ry = 0; ry < YREG; ry++) {
            int x = rx * XMINI;
            int y = ry * YMINI;
            int xx, yy;
            int tempmap[XMINI][YMINI];

            do
                generateminimap(tempmap, &regiondata[m->regs[rx][ry]]);
            while (!verifymap(tempmap));
            for (xx = 0; xx < XMINI; xx++)
                for (yy = 0; yy < YMINI; yy++) {
                    m->map[x + xx][y + yy] = tempmap[xx][yy];
                }
        }
    }
    connectregions(m->map, m->regs);
//    bigmapdebugoutput(m, r, 0, 0, 0, "Woot!");
    calculatealltiles(m);
//    tilesdebugoutput(m->tiles, 0,0,1,"");
}
