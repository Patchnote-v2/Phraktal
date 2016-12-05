#ifndef MIMIC_H
#define MIMIC_H

#include <stdio.h>
#include <stdlib.h>
#include <cfloat>
#include <memory>

#include "SDL2/SDL.h"
#include "texturew.h"
#include "vector2.h"
#include "utils.h"

enum class Type {PLAYER, ENEMY};

class Mimic
{
public:
    Mimic();
    ~Mimic() {};

    virtual void setTexture(std::string name);
    void setRenderer(std::shared_ptr< SDL_Renderer > renderer);
    std::unique_ptr< SDL_Rect > getRect();
    std::shared_ptr< Vector2 > getOldPos();
    std::shared_ptr< Vector2 > getPos();
    bool hasMoved();
    virtual bool checkCollision(std::shared_ptr< Mimic > m2);

    virtual void handleEvents(SDL_Event&) {};
    virtual void update(float);
    virtual void render();

protected:
    std::unique_ptr< TextureW > texture;
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