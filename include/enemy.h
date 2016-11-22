#ifndef ENEMY_H
#define ENEMY_H

#include <string>

#include "SDL2/SDL.h"

#include "mimic.h"

const std::string ENEMY_PNG = "assets/enemy.png";

class Enemy : public Mimic
{
public:
    Type type;
    Enemy() : Mimic(), type(Type::ENEMY) {};
    void setPos(int x, int y);
};
#endif