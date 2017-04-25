#ifndef PHRAKTAL_COIN_H
#define PHRAKTAL_COIN_H

#include "Entity.h"

class Coin : public Entity
{
public:
    Coin(Camera &camera, int x, int y);
    ~Coin(){};
};

#endif //PHRAKTAL_COIN_H
