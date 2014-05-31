#ifndef SNAKE_H
#define SNAKE_H
#include <allegro.h>
#include <string>
#include <stdio.h>

//Oh god two classes to comment.
class Snake //The snake's head. I think most of this is fairly self-explanatory.
{
    int x, y, oldx, oldy;
    public:
    int score;
    Snake(); //CONSTRUCTAH
    void move();
    int getOldX();
    int getOldY();
    int getX();
    int getY();
    void reset();
    void setX(int X);
    void setY(int Y);
    void draw(BITMAP *destination);
    void check();
    std::string direction;
};

class SnakeTail //This is also self-explanatory. Really, there's not much of a difference between the classes.
{
    int x, y;
    int oldx, oldy;
    bool active;
    public:
    SnakeTail();
    int getOldX();
    int getOldY();
    int getX();
    int getY();
    void setXY(int X, int Y);
    void draw(BITMAP *destination);
    bool getActive();
    void setActive(bool stat);
};
#endif
