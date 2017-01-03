#ifndef ENEMY_H
#define ENEMY_H

#include <string>

#include "SDL2/SDL.h"

#include "mimic.h"

class Enemy : public Mimic
{
public:
    Type type;
    Enemy(std::shared_ptr< Camera > camera) : Mimic(camera), type(Type::ENEMY) {};
    void setPos(int x, int y);
    Type getType();
};
#endif