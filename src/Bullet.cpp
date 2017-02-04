#include "Bullet.h"

Bullet::Bullet(Camera &camera, int x, int y, float maxSpeed, Entity::Type type) :
        MoveableEntity(camera, x, y)
{
    if (maxSpeed < MINIMUM_SPEED)
    {
        this->maxSpeed = MINIMUM_SPEED;
    }
    else
    {
        this->maxSpeed = maxSpeed;
    }
    this->type = type;
}