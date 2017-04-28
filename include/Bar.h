#ifndef PHRAKTAL_BAR_H_H
#define PHRAKTAL_BAR_H_H

#include <SDL2/SDL.h>
#include <memory>

class Bar
{
public:
    Bar(int x, int y, int w, int h);
    ~Bar(){};

    void setPos(int x, int y);
    void setPercentage(int percentage);

    friend class Engine;

private:
    int x, y;
    int w, h;
    std::unique_ptr< SDL_Rect > filled;
    std::unique_ptr< SDL_Rect > barOutline;
};

#endif //PHRAKTAL_BAR_H_H