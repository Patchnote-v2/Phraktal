#ifndef BULLET_H
#define BULLET_H

#include <memory>

#include "MoveableEntity.h"

namespace phraktal
{
    class Bullet : public MoveableEntity
    {
    public:
        Bullet(Camera& camera, int x, int y, float maxSpeed, Entity::Type type);
        ~Bullet(){};
    };
}
#endif