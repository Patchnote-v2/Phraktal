#include <iostream>
#include "Bar.h"

Bar::Bar(int x, int y, int w, int h) :
        x(x),
        y(y),
        w(w),
        h(h)
{
    this->barOutline = std::unique_ptr< SDL_Rect >(new SDL_Rect{x, y, w, h});
    this->filled = std::unique_ptr< SDL_Rect > (new SDL_Rect{x + 1, y + 1, w - 2, h - 2});
}

void Bar::setPos(int x, int y)
{
    this->barOutline->x = x;
    this->barOutline->y = y;
    this->filled->x = x + 1;
    this->filled->y = y + 1;
}

void Bar::setPercentage(int percentage)
{
    if (this->w != 0)
    {
        this->filled->w = (int) ((float) percentage / (100.f / (float) this->w));
    }
}
