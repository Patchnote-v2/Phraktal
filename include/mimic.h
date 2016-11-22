#ifndef MIMIC_H
#define MIMIC_H

#include <stdio.h>
#include <stdlib.h>
#include <cfloat>

#include "SDL2/SDL.h"
#include "texturew.h"
#include "vector2.h"

enum class Type {PLAYER, ENEMY};

class Mimic
{
public:
    Mimic();
    ~Mimic() {};
    bool almostEqual(float A, float B, float maxDiff, float maxRelDiff = FLT_EPSILON)
    {
        // Check if the numbers are really close -- needed
        // when comparing numbers near zero.
        float diff = fabs(A - B);
        if (diff <= maxDiff)
            return true;

        A = fabs(A);
        B = fabs(B);
        float largest = (B > A) ? B : A;

        if (diff <= largest * maxRelDiff)
        {
            return true;
        }

        return false;
    }

    virtual void setTexture(std::string name);
    void setRenderer(SDL_Renderer* renderer);
    SDL_Rect* getRect();
    Vector2* getOldPos();
    Vector2* getPos();
    bool hasMoved();
    virtual bool checkCollision(Mimic* m);

    virtual void handleEvents(SDL_Event&) {};
    virtual void update(float);
    virtual void render();

protected:
    TextureW* texture;
    Vector2 oldPos;
    Vector2 pos;
    Vector2 center;
    double angle;
    Vector2 aim;

    Vector2 velocity;
    Vector2 destination;

    Type type;
};
#endif