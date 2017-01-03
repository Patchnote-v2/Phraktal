#ifndef MIMIC_H
#define MIMIC_H

#include <stdio.h>
#include <stdlib.h>
#include <cfloat>
#include <memory>

#include "SDL2/SDL.h"
#include "base/texturew.h"
#include "base/vector2.h"
#include "base/utils.h"
#include "level.h"
#include "camera.h"

class Level;

enum class Type {NONE, PLAYER, ENEMY, BULLET};

class Mimic
{
public:
    Mimic(std::shared_ptr< Camera > camera);
    ~Mimic() {};

    virtual void setTexture(std::string name);
    void setRenderer(std::shared_ptr< SDL_Renderer > renderer);
    void setLevel(std::shared_ptr< Level > level);
    std::unique_ptr< SDL_Rect > getRect();
    std::shared_ptr< Vector2 > getOldPos();
    std::shared_ptr< Vector2 > getPos();
    std::shared_ptr< Vector2 > getCenter();
    virtual Type getType();
    bool hasMoved();
    virtual bool checkCollision(std::shared_ptr< Mimic > m2);

    virtual void handleEvents(SDL_Event&) {};
    virtual void update(float);
    virtual void render();

protected:
    std::shared_ptr< Camera > camera;

    std::unique_ptr< TextureW > texture;
    Vector2 oldPos;
    Vector2 pos;
    Vector2 center;
    double angle;
    Vector2 aim;

    Vector2 velocity;
    Vector2 destination;

    std::shared_ptr< Level > level;

    Type type;
};
#endif