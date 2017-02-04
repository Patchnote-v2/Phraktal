#ifndef TOPDOWN_BULLET_H
#define TOPDOWN_BULLET_H

#include "MoveableEntity.h"

class Bullet : public MoveableEntity
{
    const float MINIMUM_SPEED = 100;
public:
    Bullet(Camera &camera, int x, int y, float maxSpeed, Entity::Type type);

    ~Bullet()
    {};
};

#endif //TOPDOWN_BULLET_H
