#include "snake.h"

Snake::Snake()
{
    x = 10;
    y = 10;
    oldx = 9;
    oldy = 10;
    score = 0;
    direction = "right";
}

void Snake::move()
{
    if (key[KEY_LEFT] && (direction != "right")) direction = "left"; //Move head, and check so you don't move back into yourself
    else if (key[KEY_RIGHT] && (direction != "left")) direction = "right";
    else if (key[KEY_UP] && (direction != "down")) direction = "up";
    else if (key[KEY_DOWN] && (direction != "up")) direction = "down";

    oldx = x;
    oldy = y;

    if (direction == "left") x--;
    else if (direction == "right") x++;
    else if (direction == "up") y--;
    else if (direction == "down") y++;
}

void Snake::draw(BITMAP *destination)
{
   putpixel(destination, x, y, makecol(255, 255, 0)); //Draw a YELLOW PIXEL.
}

//Most of this stuff is very self-explanatory. Have fun reading.

int Snake::getOldX()
{
    return oldx;
}

int Snake::getOldY()
{
    return oldy;
}

int Snake::getX()
{
    return x;
}

int Snake::getY()
{
    return y;
}

void Snake::setX(int X)
{
    x = X;
}

void Snake::setY(int Y)
{
    y = Y;
}

int SnakeTail::getX()
{
    return x;
}

int SnakeTail::getY()
{
    return y;
}

void SnakeTail::draw(BITMAP *destination)
{
    int randColor = (rand()%150) + 60;
    putpixel(destination, x, y, makecol(randColor, 0, 0));
}

bool SnakeTail::getActive()
{
    return active;
}

void SnakeTail::setActive(bool stat)
{
    active = stat;
}

void SnakeTail::setXY(int X, int Y)
{
    oldx = x;
    oldy = y;
    x = X;
    y = Y;
}

int SnakeTail::getOldX()
{
    return oldx;
}

int SnakeTail::getOldY()
{
    return oldy;
}

SnakeTail::SnakeTail()
{
    x = -1;
    y = -1;
}

void Snake::reset()
{
    direction = "right";
    score = 0;
    x = 10;
    y = 10;
}
