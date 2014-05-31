#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <assert.h>

template <class T>
class Array
{
public:
    Array(int width, int height)
        : m_Width(width)
        , m_Height(height)
    {
        m_Data = new T[m_Width * m_Height];
    }

    ~Array()
    {
        delete [] m_Data;
    }

    int getWidth() const { return m_Width; }
    int getHeight() const { return m_Height; }

    T& operator()(int x, int y) { assert(valid(x, y)); return m_Data[y * m_Width + x]; }
    const T& operator()(int x, int y) const { assert(valid(x, y)); return m_Data[y * m_Width + x]; }

    bool valid(int x, int y) const
    {
        return x >= 0 && x < getWidth()
            && y >= 0 && y < getHeight();
    }

    const T* get() const { return m_Data; }

private:
    Array(const Array& o);
    Array& operator=(const Array& o);

    int m_Width;
    int m_Height;
    T* m_Data;
};

void rand_init() { srand( (unsigned)time( NULL ) ); }

int rand(int x)
{
    int r = rand();
    double rp = (double) r / (RAND_MAX + 1);
    return (int) (rp * x);
}

enum dir {
    DIR_NORTH,
    DIR_EAST,
    DIR_SOUTH,
    DIR_WEST,
	DIR_NONE,
};

enum piece {
    B_EMPTY,
    B_LASER_H,
    B_LASER_V,
    B_LASER_P,
    B_MIRROR1,
    B_MIRROR2,
    B_HIDDEN,
};

struct board
{
    piece mirror;
    piece hidden;
};

enum board_color {
    C_DEFAULT = 1,
    C_SELECTED,
    C_LASER,
    C_MIRROR_RIGHT,
    C_MIRROR_WRONG,
    C_HIDDEN,
    C_MISSED,
};

struct point
{
    point()
    {
    }

    point(int sx, int sy)
        : x(sx), y(sy)
    {
    }

    void move(dir d)
    {
        switch (d)
        {
        case DIR_NORTH:
            --y;
            break;
        case DIR_EAST:
            ++x;
            break;
        case DIR_SOUTH:
            ++y;
            break;
        case DIR_WEST:
            --x;
            break;
        };
    }

    int x;
    int y;
};

enum draw_mode
{
    DRAW_ERASE,
    DRAW_LASER,
    DRAW_CLEAR,
};

point draw_laser(Array<board>& a, const point& l, draw_mode mode, int* length_reveal)
{
    point p = l;
    dir d;

    *length_reveal = 0;

    if (l.x == -1)
        d = DIR_EAST;
    else if (l.x == a.getWidth())
        d = DIR_WEST;
    else if (l.y == -1)
        d = DIR_SOUTH;
    else if (l.y == a.getHeight())
        d = DIR_NORTH;
    else
        return p;

    p.move(d);
    do {
        board& c = a(p.x, p.y);
        piece pb = mode == DRAW_CLEAR ? pb = c.hidden : c.mirror;
        switch(pb)
        {
        case B_MIRROR1:
            switch (d)
            {
            case DIR_NORTH:
                d = DIR_EAST;
                break;
            case DIR_EAST:
                d = DIR_NORTH;
                break;
            case DIR_SOUTH:
                d = DIR_WEST;
                break;
            case DIR_WEST:
                d = DIR_SOUTH;
                break;
            };
            break;

        case B_MIRROR2:
            switch (d)
            {
            case DIR_NORTH:
                d = DIR_WEST;
                break;
            case DIR_EAST:
                d = DIR_SOUTH;
                break;
            case DIR_SOUTH:
                d = DIR_EAST;
                break;
            case DIR_WEST:
                d = DIR_NORTH;
                break;
            };
            break;

        case B_LASER_H:
        case B_LASER_V:
        case B_LASER_P:
            switch(mode)
            {
            case DRAW_ERASE:
                c.mirror = B_EMPTY;
                break;

            case DRAW_LASER:
                c.mirror = B_LASER_P;
                break;
            }
            break;

        case B_HIDDEN:
            if (mode == DRAW_CLEAR)
            {
                c.hidden = B_EMPTY;
                if (c.mirror == B_MIRROR1 || c.mirror == B_MIRROR2)
                    *length_reveal = -1;
                if (*length_reveal >= 0)
                    ++*length_reveal;
            }
            break;

        case B_EMPTY:
            switch(mode)
            {
            case DRAW_LASER:
                switch (d)
                {
                case DIR_NORTH:
                case DIR_SOUTH:
                    c.mirror = B_LASER_V;
                    break;
                case DIR_EAST:
                case DIR_WEST:
                    c.mirror = B_LASER_H;
                    break;
                };
                break;

            case DRAW_CLEAR:
                c.hidden = B_EMPTY;
                break;
            }
            break;
        }
        p.move(d);
        if (mode != DRAW_CLEAR)
            ++*length_reveal;
    } while (a.valid(p.x, p.y));
    return p;
}

void init_game(Array<board>& a, int mirror_count)
{
    for (int y = 0; y < a.getHeight(); ++y)
    {
        for (int x = 0; x < a.getWidth(); ++x)
        {
            a(x, y).mirror = B_EMPTY;
            a(x, y).hidden = B_HIDDEN;
        }
    }

    for (int i = 0; i < mirror_count; ++i)
    {
        int x = rand(a.getWidth());
        int y = rand(a.getHeight());
        board& c = a(x, y);
        if (c.mirror == B_EMPTY)
            c.mirror = (piece) (rand(2) + B_MIRROR1);
        else
            --i;
    }
}

int fix_range(int c, int min, int max)
{
    if (c < min)
        return min;
    else if (c > max)
        return max;
    else
        return c;
}

void control_key() {
    int y = LINES - 8;
    mvprintw(y, 40, "In-game controls:");
    y += 1;
    mvprintw(y, 40, " arrow/wasd - move the cursor");
    y += 1;
    mvprintw(y, 40, " spacebar   - reveal space/place mirror");
    y += 1;
    mvprintw(y, 40, " z/x        - change mirror guess");
    y += 1;
    mvprintw(y, 40, " c          - clear along laser path");
    y += 1;
    mvprintw(y, 40, " r          - restart");
    y += 1;
    mvprintw(y, 40, " q          - quit");
}

dir get_edge(const Array<board>& a, int x, int y)
{
    if (x == -1 && y >= 0 && y < a.getHeight())
		return DIR_WEST;
	if (x == a.getWidth() && y >= 0 && y < a.getHeight())
		return DIR_EAST;
	if (y == -1 && x >= 0 && x < a.getWidth())
		return DIR_NORTH;
	if (y == a.getHeight() && x >= 0 && x < a.getWidth())
		return DIR_SOUTH;
	return DIR_NONE;
}

void do_game(int sx, int sy)
{
    curs_set(2);
    attr_set(0, C_DEFAULT, NULL);
    erase();
    point board_pos(2, 2);
    Array<board> a(sx, sy);
    int mirror_count = a.getWidth() * a.getHeight() / 10;
    init_game(a, mirror_count);

    point laser_begin;

    switch (rand(4))
    {
    case 0: // TOP
        laser_begin.x = rand(a.getWidth());
        laser_begin.y = -1;
        break;

    case 1: // BOTTOM
        laser_begin.x = rand(a.getWidth());
        laser_begin.y = a.getHeight();
        break;

    case 2: // LEFT
        laser_begin.x = -1;
        laser_begin.y = rand(a.getHeight());
        break;

    case 3: // RIGHT
        laser_begin.x = a.getWidth();
        laser_begin.y = rand(a.getHeight());
        break;

    default:
        assert(false);
        break;
    }

    int laser_length = 0;
    point laser_end = draw_laser(a, laser_begin, DRAW_LASER, &laser_length);

    bool error = false;
    point cc(0, 0);
#ifdef PDCURSES
    bool mouse_down = false;
#endif
    bool cont = true;
    while (cont)
    {
        int hcount = 0;
        int mcount = 0;
        int mguess = 0;

        for (int x = 0; x < a.getWidth(); ++x)
        {
            mvaddch(-1 + board_pos.y, x + board_pos.x, COLOR_PAIR(C_DEFAULT) | 249);
            mvaddch(a.getHeight() + board_pos.y, x + board_pos.x, COLOR_PAIR(C_DEFAULT) | 249);
        }

        for (int y = 0; y < a.getHeight(); ++y)
        {
            mvaddch(y + board_pos.y, -1 + board_pos.x, COLOR_PAIR(C_DEFAULT) | 249);
            mvaddch(y + board_pos.y, a.getWidth() + board_pos.x, COLOR_PAIR(C_DEFAULT) | 249);
        }

        mvaddch(-1 + board_pos.y, -1 + board_pos.x, COLOR_PAIR(C_DEFAULT) | ' ');
        mvaddch(-1 + board_pos.y, a.getWidth() + board_pos.x, COLOR_PAIR(C_DEFAULT) | ' ');
        mvaddch(a.getHeight() + board_pos.y, -1 + board_pos.x, COLOR_PAIR(C_DEFAULT) | ' ');
        mvaddch(a.getHeight() + board_pos.y, a.getWidth() + board_pos.x, COLOR_PAIR(C_DEFAULT) | ' ');

        for (int y = 0; y < a.getHeight(); ++y)
        {
            for (int x = 0; x < a.getWidth(); ++x)
            {
                board& cb = a(x, y);
                chtype c = COLOR_PAIR(C_DEFAULT) | '?';

                if (cb.hidden != B_EMPTY && cb.mirror != B_MIRROR1 && cb.mirror != B_MIRROR2)
                    ++hcount;
				if (cb.mirror == B_MIRROR1 || cb.mirror == B_MIRROR2)
					++mcount;
				if (cb.hidden == B_MIRROR1 || cb.hidden == B_MIRROR2)
					++mguess;

                switch(cb.mirror)
                {
                case B_EMPTY:
                    if (cb.hidden == B_HIDDEN)
                        c = COLOR_PAIR(C_MISSED) | A_BOLD | 254;
                    else
                        c = COLOR_PAIR(C_DEFAULT) | ' ';
                    break;
                case B_LASER_H:
                    c = COLOR_PAIR(C_LASER) | A_BOLD | ACS_HLINE;
                    break;
                case B_LASER_V:
                    c = COLOR_PAIR(C_LASER) | A_BOLD | ACS_VLINE;
                    break;
                case B_LASER_P:
                    c = COLOR_PAIR(C_LASER) | A_BOLD | ACS_PLUS;
                    break;
                case B_MIRROR1:
                    if (cb.mirror == cb.hidden)
                        c = COLOR_PAIR(C_MIRROR_RIGHT) | A_BOLD | '/';
                    else
                        c = COLOR_PAIR(C_MIRROR_WRONG) | A_BOLD | '/';
                    break;
                case B_MIRROR2:
                    if (cb.mirror == cb.hidden)
                        c = COLOR_PAIR(C_MIRROR_RIGHT) | A_BOLD | '\\';
                    else
                        c = COLOR_PAIR(C_MIRROR_WRONG) | A_BOLD | '\\';
                    break;
                }
                if (!error)
                {
                    switch(cb.hidden)
                    {
                    case B_MIRROR1:
                        c = COLOR_PAIR(C_HIDDEN) | '/';
                        break;
                    case B_MIRROR2:
                        c = COLOR_PAIR(C_HIDDEN) | '\\';
                        break;
                    case B_HIDDEN:
                        c = COLOR_PAIR(C_HIDDEN) | 254;
                        break;
                    }
                }
                mvaddch(y + board_pos.y, x + board_pos.x, c);
            }
        }

		int c = '*';
		switch (get_edge(a, laser_begin.x, laser_begin.y))
		{
		case DIR_NORTH: c = ACS_DARROW; break;
		case DIR_SOUTH: c = ACS_UARROW; break;
		case DIR_EAST:  c = ACS_LARROW; break;
		case DIR_WEST:  c = ACS_RARROW; break;
		}
        mvaddch(laser_begin.y + board_pos.y, laser_begin.x + board_pos.x, COLOR_PAIR(C_LASER) | A_BOLD | c);
        mvaddch(laser_end.y + board_pos.y, laser_end.x + board_pos.x, COLOR_PAIR(C_LASER) | A_BOLD | '*');

        int x = 45;
        int y = 1;
        mvprintw(y, x, " %d empty spaces to find ", hcount);
        y += 2;
        mvprintw(y, x, " %d hidden mirrors ", mirror_count);
        y += 2;
        mvprintw(y, x, " %d guessed mirrors ", mguess);
        y += 2;
        mvprintw(y, x, " %d length of laser ", laser_length);
        y += 2;
        if (hcount == 0)
            mvprintw(y, x, " You won!              ");
        else if (error)
            mvprintw(y, x, " You lost... try again ");
        else
            mvprintw(y, x, "                       ");
        y += 2;
        if (hcount == 0 || error)
            mvprintw(y, x, " %d mirrors correct ", mcount);
        else
            mvprintw(y, x, "                    ", mcount);

        if (sx < 37)
            control_key();
        refresh();
        move(cc.y + board_pos.y, cc.x + board_pos.x);

        int ch = getch();
#ifdef PDCURSES
        if (ch == KEY_MOUSE)
        {
            request_mouse_pos();
            cc.x = MOUSE_X_POS - board_pos.x;
            cc.y = MOUSE_Y_POS - board_pos.y;

            if (BUTTON_STATUS(1) == BUTTON_PRESSED)
                mouse_down = true;
            else if (BUTTON_STATUS(1) == BUTTON_RELEASED)
                mouse_down = false;
            if (BUTTON_STATUS(1) == BUTTON_CLICKED || mouse_down)
                ch = ' ';
            if (BUTTON_STATUS(3) == BUTTON_CLICKED)
                ch = 'x';
        }
#endif
        switch (ch)
        {
        case 'q':
            cont = false;
            break;

        case 'd':
        case KEY_RIGHT:
            ++cc.x;
            break;

        case 'a':
        case KEY_LEFT:
            --cc.x;
            break;

        case 's':
        case KEY_DOWN:
            ++cc.y;
            break;

        case 'w':
        case KEY_UP:
            --cc.y;
            break;

#ifdef _DEBUG
        case 'e':
            error = !error;
            break;
#endif
        case 'r':
            init_game(a, mirror_count);
            laser_end = draw_laser(a, laser_begin, DRAW_LASER, &laser_length);
            hcount = -1;
            error = false;
            break;

        case 'z':
            if (!error && a.valid(cc.x, cc.y))
            {
                board& c = a(cc.x, cc.y);
                switch (c.hidden)
                {
                case B_HIDDEN:  c.hidden = B_MIRROR2; break;
                case B_MIRROR1: c.hidden = B_HIDDEN;  break;
                case B_MIRROR2: c.hidden = B_MIRROR1; break;
                }
            }
            break;

        case 'x':
            if (!error && a.valid(cc.x, cc.y))
            {
                board& c = a(cc.x, cc.y);
                switch (c.hidden)
                {
                case B_HIDDEN:  c.hidden = B_MIRROR1; break;
                case B_MIRROR1: c.hidden = B_MIRROR2; break;
                case B_MIRROR2: c.hidden = B_HIDDEN;  break;
                }
            }
            break;

        case 'c':
            if (!error)
            {
                int reveal_count = 0;
                draw_laser(a, laser_begin, DRAW_CLEAR, &reveal_count);
                if (reveal_count < 0)
                    error = true;
                else
                    hcount -= reveal_count;
            }
            break;

        case ' ':
            if (!error && a.valid(cc.x, cc.y))
            {
                board& c = a(cc.x, cc.y);
                if (c.hidden == B_HIDDEN)
                {
                    if (c.mirror == B_MIRROR1 || c.mirror == B_MIRROR2)
                        error = true;
                    else
                    {
                        c.hidden = B_EMPTY;
                        --hcount;
                    }
                }
            }
            else if (get_edge(a, cc.x, cc.y) != DIR_NONE)
            {
                laser_end = draw_laser(a, laser_begin, DRAW_ERASE, &laser_length);
                laser_begin = cc;
                laser_end = draw_laser(a, laser_begin, DRAW_LASER, &laser_length);
            }
            break;

        default:
            //printf("%d\n", ch);
            break;
        }

        if (hcount == 0)
            error = true;

        cc.x = fix_range(cc.x, -1, a.getWidth());
        cc.y = fix_range(cc.y, -1, a.getHeight());
    }
}

void do_menu()
{
    curs_set(0);
    erase();
    int select = 0;
    bool cont = true;
    while (cont)
    {
        int y = 1;
        attr_set(A_BOLD, C_DEFAULT, NULL);
        mvprintw(y, 40, "The objective is to find all of the");
        y += 1;
        mvprintw(y, 40, "hidden mirrors. To help you find them");
        y += 1;
        mvprintw(y, 40, "you have a laser which you can place");
        y += 1;
        mvprintw(y, 40, "along one of the edges. The laser");
        y += 1;
        mvprintw(y, 40, "bounces off the mirrors. Use your logic");
        y += 1;
        mvprintw(y, 40, "skills to deduce where the mirrors are.");
        y += 1;
        mvprintw(y, 40, "If you reveal a mirror the game is over.");
        y += 1;
        mvprintw(y, 40, "Good luck!");

        control_key();

        y = 1;
        attr_set(A_BOLD, C_LASER, NULL);
        mvprintw(y, 1, "*Laser*");
        y += 2;
        attr_set(A_BOLD, select == 0 ? C_SELECTED : C_DEFAULT, NULL);
        int menu_begin = y;
        mvprintw(y, 1, " Easy   ");
        y += 2;
        attr_set(A_BOLD, select == 1 ? C_SELECTED : C_DEFAULT, NULL);
        mvprintw(y, 1, " Medium ");
        y += 2;
        attr_set(A_BOLD, select == 2 ? C_SELECTED : C_DEFAULT, NULL);
        mvprintw(y, 1, " Hard   ");
        y += 2;
        attr_set(A_BOLD, select == 3 ? C_SELECTED : C_DEFAULT, NULL);
        mvprintw(y, 1, " Quit   ");

        int ch = getch();
#ifdef PDCURSES
        if (ch == KEY_MOUSE)
        {
            request_mouse_pos();
            if (MOUSE_X_POS >= 1 && MOUSE_X_POS < 8)
            {
                if ((MOUSE_Y_POS - menu_begin) % 2 == 0)
                    select = (MOUSE_Y_POS - menu_begin) / 2;
                if (BUTTON_STATUS(1) == BUTTON_CLICKED)
                    ch = ' ';
            }
        }
#endif
        switch (ch)
        {
        case 'w':
        case KEY_UP:
            --select;
            break;

        case 's':
        case KEY_DOWN:
            ++select;
            break;

        case 'q':
            cont = false;
            break;

        case '\r':
        case '\n':
        case ' ':
            switch (select)
            {
            case 0:
                do_game(10, 10);
                break;

            case 1:
                do_game(20, 20);
                break;

            case 2:
                do_game(40, 20);
                break;

            case 3:
                cont = false;
                break;
            }
            curs_set(0);
            erase();
            break;
        }

        select = fix_range(select, 0, 3);
    }
}

int main()
{
    WINDOW *s = initscr();
    PDC_set_title("Laser");
    //nodelay(s, TRUE);
    noecho();
    keypad(s, TRUE);
#ifdef PDCURSES
    mouse_on(ALL_MOUSE_EVENTS);
#endif
    start_color();

    init_pair(C_DEFAULT, COLOR_WHITE, COLOR_BLACK);
    init_pair(C_SELECTED, COLOR_BLACK, COLOR_WHITE);
    init_pair(C_LASER, COLOR_RED, COLOR_BLACK);
    init_pair(C_MIRROR_RIGHT, COLOR_BLUE, COLOR_BLACK);
    init_pair(C_MIRROR_WRONG, COLOR_WHITE, COLOR_BLACK);
    init_pair(C_HIDDEN, COLOR_WHITE, COLOR_BLACK);
    init_pair(C_MISSED, COLOR_BLACK, COLOR_BLACK);

    rand_init();

    do_menu();

    endwin();

    return 0;
}
