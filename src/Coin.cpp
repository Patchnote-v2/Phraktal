#include "Coin.h"

Coin::Coin(Camera &camera, int x, int y) :
        Entity(camera, x, y)
{
    this->type = Entity::Type::COIN;
}